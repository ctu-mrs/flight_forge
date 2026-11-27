#include "SurfaceHeightQuery.h"
#include "WireFrame.h"

#include "Engine/World.h"
#include "EngineUtils.h"
#include "FlightForgeCore/RobotPawn.h"

/* Batch() //{ */

void FSurfaceHeightQuery::Batch(UWorld* World, const FVector& GroundOrigin, const TArray<FVector>& PointsM, const bool bIgnoreRobots, TArray<double>& OutHeightsM,
                                const TArray<const AActor*>* IgnoredPerPoint)
{
	constexpr double ReachCm  = 1.0e6;
	constexpr int32  MaxSkips = 8;

	OutHeightsM.Init(std::numeric_limits<double>::quiet_NaN(), PointsM.Num());
	if (!World)
	{
		return;
	}

	// trace what blocks an airframe, so a landing rests where the robot's own collision sweep would stop
	ECollisionChannel        Channel = ECC_Pawn;
	FCollisionResponseParams Responses;
	bool                     bFoundBody = false;
	FCollisionQueryParams    Params(SCENE_QUERY_STAT(FlightForgeSurfaceHeight), false);
	for (TActorIterator<ARobotPawn> It(World); It; ++It)
	{
		const UStaticMeshComponent* Body = It->RootMeshComponent;
		if (!bFoundBody && Body && Body->IsCollisionEnabled())
		{
			Channel    = Body->GetCollisionObjectType();
			Responses  = FCollisionResponseParams(Body->GetCollisionResponseToChannels());
			bFoundBody = true;
		}
		if (bIgnoreRobots && !IgnoredPerPoint)
		{
			Params.AddIgnoredActor(*It);
		}
	}

	for (int32 i = 0; i < PointsM.Num(); ++i)
	{
		const FVector& Point = PointsM[i];
		const FVector  At    = FWireFrame::FromGround(FVector(Point.X, Point.Y, FMath::IsNaN(Point.Z) ? 0.0 : Point.Z), GroundOrigin);
		const FVector  Start(At.X, At.Y, FMath::IsNaN(Point.Z) ? GroundOrigin.Z + ReachCm : At.Z);
		const FVector  End(At.X, At.Y, GroundOrigin.Z - ReachCm);

		FCollisionQueryParams Query = Params;
		if (IgnoredPerPoint && IgnoredPerPoint->IsValidIndex(i) && (*IgnoredPerPoint)[i])
		{
			Query.AddIgnoredActor((*IgnoredPerPoint)[i]);
		}
		for (int32 Skip = 0; Skip <= MaxSkips; ++Skip)
		{
			FHitResult Hit;
			if (!World->LineTraceSingleByChannel(Hit, Start, End, Channel, Query, Responses))
			{
				break;
			}
			// the trace started inside this geometry or met a face from behind
			if (Hit.bStartPenetrating || Hit.ImpactNormal.Z <= 0.0)
			{
				Query.AddIgnoredComponent(Hit.GetComponent());
				continue;
			}
			OutHeightsM[i] = FWireFrame::ToGround(Hit.ImpactPoint, GroundOrigin).Z;
			break;
		}
	}
}

/* //} */
