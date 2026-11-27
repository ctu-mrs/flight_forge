#pragma once

#include "CoreMinimal.h"
#include "MessageSerialization/Public/FlightForgeWire.h"

// ground frame: x = Unreal +X, y = Unreal -Y, z up, m; link frames (REP-103) flip the same way with a zero origin
struct FWireFrame
{
	// the ROS node's offset between the "UAV" PlayerStart and the floor
	static constexpr double PlayerStartHeightCm = 92.12;

	/* vectors //{ */

	static FVector ToRightHanded(const FVector& Cm)
	{
		return FVector(Cm.X / 100.0, -Cm.Y / 100.0, Cm.Z / 100.0);
	}

	static FVector FromRightHanded(const FVector& M)
	{
		return FVector(M.X * 100.0, -M.Y * 100.0, M.Z * 100.0);
	}

	// mirroring y reverses the sense of rotations about x and z
	static FQuat ToRightHanded(const FQuat& Q)
	{
		return FQuat(-Q.X, Q.Y, -Q.Z, Q.W);
	}

	static FQuat FromRightHanded(const FQuat& Q)
	{
		return FQuat(-Q.X, Q.Y, -Q.Z, Q.W);
	}

	static FVector ToGround(const FVector& UnrealCm, const FVector& OriginCm)
	{
		return ToRightHanded(UnrealCm - OriginCm);
	}

	static FVector FromGround(const FVector& GroundM, const FVector& OriginCm)
	{
		return OriginCm + FromRightHanded(GroundM);
	}

	/* //} */

	/* wire structs //{ */

	static flightforge::wire::Vec3 ToVec3(const FVector& V)
	{
		return flightforge::wire::Vec3(V.X, V.Y, V.Z);
	}

	static FVector FromVec3(const flightforge::wire::Vec3& V)
	{
		return FVector(V.x(), V.y(), V.z());
	}

	static flightforge::wire::Vec3f ToVec3f(const FVector& V)
	{
		return flightforge::wire::Vec3f(static_cast<float>(V.X), static_cast<float>(V.Y), static_cast<float>(V.Z));
	}

	static FVector FromVec3f(const flightforge::wire::Vec3f& V)
	{
		return FVector(V.x(), V.y(), V.z());
	}

	static flightforge::wire::Quat ToWireQuat(const FQuat& Q)
	{
		return flightforge::wire::Quat(Q.X, Q.Y, Q.Z, Q.W);
	}

	// a zeroed quaternion is what a client gets by leaving the orientation unset
	static FQuat FromWireQuat(const flightforge::wire::Quat& Q)
	{
		FQuat Out(Q.x(), Q.y(), Q.z(), Q.w());
		if (Out.SizeSquared() < UE_SMALL_NUMBER)
		{
			return FQuat::Identity;
		}
		Out.Normalize();
		return Out;
	}

	/* //} */

	/* poses //{ */

	static flightforge::wire::Pose ToGroundPose(const FVector& LocationCm, const FQuat& Rotation, const FVector& OriginCm)
	{
		return flightforge::wire::Pose(ToVec3(ToGround(LocationCm, OriginCm)), ToWireQuat(ToRightHanded(Rotation)));
	}

	static flightforge::wire::Pose ToGroundPose(const FTransform& Transform, const FVector& OriginCm)
	{
		return ToGroundPose(Transform.GetLocation(), Transform.GetRotation(), OriginCm);
	}

	// an absent pose is the frame origin
	static FTransform FromGroundPose(const flightforge::wire::Pose* Pose, const FVector& OriginCm)
	{
		if (!Pose)
		{
			return FTransform(FQuat::Identity, OriginCm);
		}
		return FTransform(FromRightHanded(FromWireQuat(Pose->orientation())), FromGround(FromVec3(Pose->position()), OriginCm));
	}

	static flightforge::wire::Pose ToMountPose(const FVector& OffsetCm, const FRotator& Rotation)
	{
		return flightforge::wire::Pose(ToVec3(ToRightHanded(OffsetCm)), ToWireQuat(ToRightHanded(Rotation.Quaternion())));
	}

	static void FromMountPose(const flightforge::wire::Pose* Pose, FVector& OutOffsetCm, FRotator& OutRotation)
	{
		const FTransform Mount = FromGroundPose(Pose, FVector::ZeroVector);
		OutOffsetCm            = Mount.GetLocation();
		OutRotation            = Mount.Rotator();
	}

	/* //} */
};
