#include "SimServerInternal.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"

#include "FlightForgeCore/Devices/SensorDeviceLibrary.h"
#include "FlightForgeCore/Segmentation/SegmentationTagger.h"

namespace
{

/* helpers //{ */

const TCHAR* BaseLink = TEXT("base_link");

bool OnBaseLink(const flatbuffers::String* Link, const flatbuffers::String* Mount, FWireReply& Reply)
{
	const FString LinkName  = WireString(Link);
	const FString MountName = WireString(Mount);
	if (!MountName.IsEmpty())
	{
		Reply.Fail(FString::Printf(TEXT("the robot has no mount '%s'"), *MountName));
		return false;
	}
	if (!LinkName.IsEmpty() && LinkName != BaseLink)
	{
		Reply.Fail(FString::Printf(TEXT("sensors attach to %s only for now, not '%s'"), BaseLink, *LinkName));
		return false;
	}
	return true;
}

// wire ids match the plugin's, except that 3 (Livox) is a lidar flag on the wire
bool PluginSensorType(const ffw::SensorType Type, SensorType& Out)
{
	const int32 Id = static_cast<int32>(Type);
	if (Id < 0 || Id >= static_cast<int32>(SensorType::MAX_SENSOR_TYPE) || Id == static_cast<int32>(SensorType::LIDAR_LIVOX))
	{
		return false;
	}
	Out = static_cast<SensorType>(Id);
	return true;
}

ffw::SensorType WireSensorType(const int32 Type)
{
	return Type == static_cast<int32>(SensorType::LIDAR_LIVOX) ? ffw::SensorType::LIDAR : static_cast<ffw::SensorType>(Type);
}

flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<ffw::SensorInfo>>> SensorInfos(FWireReply& Reply, const std::vector<int>& Ids, const std::vector<int>& Types)
{
	std::vector<flatbuffers::Offset<ffw::SensorInfo>> Infos;
	for (size_t i = 0; i < Ids.size() && i < Types.size(); ++i)
	{
		Infos.push_back(ffw::CreateSensorInfo(Reply.Builder, Ids[i], WireSensorType(Types[i]), Reply.String(BaseLink)));
	}
	return Reply.Builder.CreateVector(Infos);
}

struct FBaseRequest
{
	bool       bHasBase = false;
	FTransform Base;
	bool       bHasSpeeds = false;
	std::vector<double> Speeds;
};

// link poses and joint positions need links the pawn does not have yet
bool ReadState(const ffw::RobotState* State, const ARobotPawn& Pawn, FBaseRequest& Out, FWireReply& Reply)
{
	if (!State)
	{
		Reply.Fail(TEXT("the request carries no state"));
		return false;
	}
	if (State->link_poses() && State->link_poses()->size() > 0)
	{
		Reply.Fail(TEXT("link_poses are not supported yet: this robot has one rigid link and propeller joints"));
		return false;
	}
	if (State->joint_positions() && State->joint_positions()->size() > 0)
	{
		Reply.Fail(TEXT("joint_positions are not supported yet: the propeller joints take joint_velocities"));
		return false;
	}

	if (State->base())
	{
		Out.bHasBase = true;
		Out.Base     = FWireFrame::FromGroundPose(State->base(), Pawn.GroundOriginCm());
	}

	if (State->joint_velocities())
	{
		const int32 Joints = Pawn.PropellerLinkNames().Num();
		if (static_cast<int32>(State->joint_velocities()->size()) > Joints)
		{
			Reply.Fail(FString::Printf(TEXT("%u joint velocities for a robot with %d joints"), State->joint_velocities()->size(), Joints));
			return false;
		}
		Out.bHasSpeeds = true;
		Out.Speeds.assign(State->joint_velocities()->begin(), State->joint_velocities()->end());
	}
	return true;
}

// game thread
void ApplyState(ARobotPawn& Pawn, const FBaseRequest& State, const bool bCollide, FHitResult& OutHit)
{
	if (State.bHasBase)
	{
		const FVector From = Pawn.GetActorLocation();

		Pawn.SetActorLocationAndRotation(State.Base.GetLocation(), State.Base.GetRotation(), bCollide, &OutHit);

		if (OutHit.IsValidBlockingHit())
		{
			Pawn.uav_crashed_     = true;
			Pawn.LastImpactPoint = OutHit.ImpactPoint;
		}

		FRotator Arm = Pawn.ThirdPersonCameraSpringArm->GetRelativeRotation();
		Arm.Yaw      = Pawn.GetActorRotation().Yaw;
		Pawn.ThirdPersonCameraSpringArm->SetRelativeRotation(Arm);

		if (Pawn.DrawMovementLine)
		{
			DrawDebugLine(Pawn.GetWorld(), From, Pawn.GetActorLocation(), FColor::Green, true, -1, 0, 3);
		}
	}

	if (State.bHasSpeeds)
	{
		int32 Count = 0;
		Pawn.SetPropellerSpeeds(State.Speeds, Count);
	}
}

flatbuffers::Offset<ffw::SetStateResponse> StateResult(FWireReply& Reply, const int32 RobotId, ARobotPawn& Pawn, const FHitResult& Hit)
{
	const ffw::Pose Base   = FWireFrame::ToGroundPose(Pawn.GetActorTransform(), Pawn.GroundOriginCm());
	const bool      bHit   = Hit.IsValidBlockingHit();
	const ffw::Vec3 Impact = FWireFrame::ToVec3(bHit ? FWireFrame::ToGround(Hit.ImpactPoint, Pawn.GroundOriginCm()) : FVector::ZeroVector);
	return ffw::CreateSetStateResponse(Reply.Builder, RobotId, &Base, bHit, bHit ? &Impact : nullptr);
}

/* //} */

}  // namespace

/* robots //{ */

void FSimServer::SpawnRobot(const ffw::Request& Request, FWireReply& Reply)
{
	const ffw::SpawnRobotRequest* Ask = Request.body_as_SpawnRobotRequest();
	if (Ask->description_yaml() && Ask->description_yaml()->size() > 0)
	{
		Reply.Fail(TEXT("description_yaml is not supported yet: spawn a shipped frame or a model by name"));
		return;
	}

	const FString Name  = Ask->name() && Ask->name()->size() > 0 ? WireString(Ask->name()) : FString(TEXT("x500"));
	const FString Label = WireString(Ask->label());

	std::lock_guard<std::mutex> Lock(WorldMutex);
	const FGameModeLease        GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	const FTransform Transform = FWireFrame::FromGroundPose(Ask->pose(), GameMode->GetGroundOrigin());

	int32 RobotId = -1;
	RunOnGameThread(*GameMode.GameMode, [&](AFlightForgeGameModeBase& Mode) {
		const ARobotPawn* Pawn = Mode.SpawnRobot(Transform, Name, Label);
		RobotId                = Pawn ? Pawn->RobotId : -1;
	});

	if (RobotId < 0)
	{
		Reply.Fail(TEXT("the robot could not be spawned (is the default pawn an ARobotPawn?)"));
		return;
	}

	Reply.Set(ffw::CreateSpawnRobotResponse(Reply.Builder, RobotId));
}

void FSimServer::ListRobots(const ffw::Request& Request, FWireReply& Reply)
{
	std::vector<flatbuffers::Offset<ffw::RobotInfo>> Infos;
	{
		std::lock_guard<std::mutex> Lock(RobotsMutex);

		TArray<int32> Ids;
		Robots.GetKeys(Ids);
		Ids.Sort();
		for (const int32 Id : Ids)
		{
			const ARobotPawn* Pawn = Robots[Id].Pawn;
			Infos.push_back(ffw::CreateRobotInfo(Reply.Builder, Id, Reply.String(Pawn->RobotName), static_cast<ffw::RobotKind>(Pawn->WireKind),
			                                     Reply.String(Pawn->Label)));
		}
	}

	Reply.Set(ffw::CreateListRobotsResponse(Reply.Builder, Reply.Builder.CreateVector(Infos)));
}

void FSimServer::RemoveRobot(const ffw::Request& Request, FWireReply& Reply)
{
	if (!Request.robot_id().has_value())
	{
		Reply.Fail(TEXT("remove_robot needs a robot_id"));
		return;
	}
	const int32 RobotId = *Request.robot_id();

	std::lock_guard<std::mutex> Lock(WorldMutex);
	const FGameModeLease        GameMode = LeaseGameMode(Reply);
	if (!GameMode)
	{
		return;
	}

	if (!CloseRobot(RobotId))
	{
		Reply.Fail(FString::Printf(TEXT("no robot %d"), RobotId));
		return;
	}

	RunOnGameThread(*GameMode.GameMode, [RobotId](AFlightForgeGameModeBase& Mode) { Mode.RemoveRobot(RobotId); });
}

void FSimServer::GetDescription(const ffw::Request& Request, FWireReply& Reply)
{
	const FRobotLease Robot = LeaseRobot(Request, Reply);
	if (!Robot)
	{
		return;
	}

	struct FLink
	{
		FString    Name;
		FTransform Origin;
		FString    Mesh;
	};

	FString        BaseMesh;
	FString        Class;
	TArray<FLink>  Props;
	RunOnGameThread(*Robot.Pawn, [&](ARobotPawn& Pawn) {
		if (const UStaticMesh* Mesh = Pawn.RootMeshComponent->GetStaticMesh())
		{
			BaseMesh = Mesh->GetPathName();
		}

		const TArray<UStaticMeshComponent*> Propellers = Pawn.AllPropellers();
		const TArray<FString>               Names      = Pawn.PropellerLinkNames();
		for (int32 i = 0; i < Propellers.Num() && i < Names.Num(); ++i)
		{
			const UStaticMesh* Mesh = Propellers[i]->GetStaticMesh();
			Props.Add({Names[i], Propellers[i]->GetRelativeTransform(), Mesh ? Mesh->GetPathName() : FString()});
		}

		if (const USegmentationTagger* Tagger = Pawn.GetWorld() ? Pawn.GetWorld()->GetSubsystem<USegmentationTagger>() : nullptr)
		{
			const int32 ClassId = Tagger->ClassOf(&Pawn, Pawn.RootMeshComponent);
			Class               = ClassId > 0 ? Tagger->ClassPathOf(ClassId) : FString();
		}
	});

	const ffw::RobotKind Kind = static_cast<ffw::RobotKind>(Robot->WireKind);

	FString Yaml = FString::Printf(TEXT("name: %s\nkind: %s\nclass: %s\nlinks:\n  - name: %s\n    mesh: %s\n"), *Robot->RobotName,
	                               UTF8_TO_TCHAR(ffw::EnumNameRobotKind(Kind)), *Class, BaseLink, *BaseMesh);

	std::vector<flatbuffers::Offset<ffw::LinkInfo>>        Links;
	std::vector<flatbuffers::Offset<flatbuffers::String>> Joints;

	Links.push_back(ffw::CreateLinkInfo(Reply.Builder, Reply.String(BaseLink), Reply.String(FString()), ffw::JointType::FIXED, nullptr, nullptr,
	                                    Reply.String(BaseMesh)));

	const ffw::Vec3 Axis(0.0, 0.0, 1.0);
	for (const FLink& Link : Props)
	{
		const ffw::Pose Origin = FWireFrame::ToGroundPose(Link.Origin.GetLocation(), Link.Origin.GetRotation(), FVector::ZeroVector);
		Links.push_back(ffw::CreateLinkInfo(Reply.Builder, Reply.String(Link.Name), Reply.String(BaseLink), ffw::JointType::CONTINUOUS, &Axis, &Origin,
		                                    Reply.String(Link.Mesh)));
		Joints.push_back(Reply.String(Link.Name));

		Yaml += FString::Printf(TEXT("  - name: %s\n    parent: %s\n    mesh: %s\n    pose: {position: [%g, %g, %g]}\n    joint: {type: continuous, axis: [0, 0, 1]}\n"),
		                        *Link.Name, BaseLink, *Link.Mesh, Origin.position().x(), Origin.position().y(), Origin.position().z());
	}

	const auto LinksV  = Reply.Builder.CreateVector(Links);
	const auto JointsV = Reply.Builder.CreateVector(Joints);
	const auto Mounts  = Reply.Builder.CreateVector(std::vector<flatbuffers::Offset<ffw::MountInfo>>());
	Reply.Set(ffw::CreateGetDescriptionResponse(Reply.Builder, Reply.String(Robot->RobotName), Kind, Reply.String(Class), LinksV, JointsV, Mounts,
	                                            Reply.String(Yaml)));
}

void FSimServer::GetState(const ffw::Request& Request, FWireReply& Reply)
{
	const FRobotLease Robot = LeaseRobot(Request, Reply);
	if (!Robot)
	{
		return;
	}

	ARobotPawn* Pawn = Robot.Pawn;

	std::vector<double> Positions;
	std::vector<double> Velocities;
	FVector             Linear;
	FVector             Angular;
	{
		std::lock_guard<std::mutex> Lock(Robot.Sync->StateMutex);
		for (const UStaticMeshComponent* Propeller : Pawn->AllPropellers())
		{
			// the wire turns counterclockwise, Unreal's yaw clockwise
			Positions.push_back(FMath::DegreesToRadians(-Propeller->GetRelativeRotation().Yaw));
		}
		Velocities.assign(Pawn->PropellerSpeeds.GetData(), Pawn->PropellerSpeeds.GetData() + Pawn->PropellerSpeeds.Num());
		Linear  = Pawn->BaseLinearVelocity;
		Angular = Pawn->BaseAngularVelocity;
	}

	const UWorld*   World    = Pawn->GetWorld();
	const ffw::Pose Base     = FWireFrame::ToGroundPose(Pawn->GetActorTransform(), Pawn->GroundOriginCm());
	const ffw::Vec3 LinearV  = FWireFrame::ToVec3(Linear);
	const ffw::Vec3 AngularV = FWireFrame::ToVec3(Angular);

	const auto PositionsV  = Reply.Builder.CreateVector(Positions);
	const auto VelocitiesV = Reply.Builder.CreateVector(Velocities);
	const auto State       = ffw::CreateRobotState(Reply.Builder, &Base, PositionsV, VelocitiesV, 0, &LinearV, &AngularV);
	Reply.Set(ffw::CreateGetStateResponse(Reply.Builder, World ? World->GetTimeSeconds() : 0.0, State));
}

void FSimServer::SetState(const ffw::Request& Request, FWireReply& Reply)
{
	const ffw::SetStateRequest* Ask   = Request.body_as_SetStateRequest();
	const FRobotLease           Robot = LeaseRobot(Request, Reply);
	if (!Robot)
	{
		return;
	}

	FBaseRequest State;
	if (!ReadState(Ask->state(), *Robot.Pawn, State, Reply))
	{
		return;
	}

	const bool bCollide = Ask->collide();

	std::lock_guard<std::mutex> Lock(Robot.Sync->StateMutex);

	if (Ask->state()->base_linear_velocity())
	{
		Robot->BaseLinearVelocity = FWireFrame::FromVec3(*Ask->state()->base_linear_velocity());
	}
	if (Ask->state()->base_angular_velocity())
	{
		Robot->BaseAngularVelocity = FWireFrame::FromVec3(*Ask->state()->base_angular_velocity());
	}

	// a pose stream must not wait for the tick it lands in
	if (!Ask->wait_applied())
	{
		PostToGameThread(*Robot.Pawn, [State, bCollide](ARobotPawn& Pawn) {
			FHitResult Hit;
			ApplyState(Pawn, State, bCollide, Hit);
		});
		return;
	}

	FHitResult Hit;
	RunOnGameThread(*Robot.Pawn, [&](ARobotPawn& Pawn) { ApplyState(Pawn, State, bCollide, Hit); });

	Reply.Set(StateResult(Reply, Robot.Id, *Robot.Pawn, Hit));
}

void FSimServer::SetStates(const ffw::Request& Request, FWireReply& Reply)
{
	const ffw::SetStatesRequest* Ask = Request.body_as_SetStatesRequest();
	const bool                   bCollide = Ask->collide();

	std::vector<FRobotLease>  Leases;
	std::vector<FBaseRequest> States;
	if (Ask->states())
	{
		for (const ffw::RobotStateEntry* Entry : *Ask->states())
		{
			FRobotLease Robot = LeaseRobot(Entry->robot_id(), Reply);
			if (!Robot)
			{
				return;
			}

			FBaseRequest State;
			if (!ReadState(Entry->state(), *Robot.Pawn, State, Reply))
			{
				return;
			}

			Leases.push_back(MoveTemp(Robot));
			States.push_back(MoveTemp(State));
		}
	}

	std::vector<std::shared_ptr<FInstruction<ARobotPawn>>> Pending;
	std::vector<FHitResult>                                Hits(Leases.size());

	for (size_t i = 0; i < Leases.size(); ++i)
	{
		const ffw::RobotState* Wire = Ask->states()->Get(static_cast<flatbuffers::uoffset_t>(i))->state();

		std::lock_guard<std::mutex> Lock(Leases[i].Sync->StateMutex);
		if (Wire->base_linear_velocity())
		{
			Leases[i]->BaseLinearVelocity = FWireFrame::FromVec3(*Wire->base_linear_velocity());
		}
		if (Wire->base_angular_velocity())
		{
			Leases[i]->BaseAngularVelocity = FWireFrame::FromVec3(*Wire->base_angular_velocity());
		}

		auto Instruction = std::make_shared<FInstruction<ARobotPawn>>();
		if (Ask->wait_applied())
		{
			FHitResult* Hit       = &Hits[i];
			const FBaseRequest* S = &States[i];
			Instruction->Function = [S, bCollide, Hit](ARobotPawn& Pawn) { ApplyState(Pawn, *S, bCollide, *Hit); };
			Pending.push_back(Instruction);
		}
		else
		{
			Instruction->Function = [S = States[i], bCollide](ARobotPawn& Pawn) {
				FHitResult Hit;
				ApplyState(Pawn, S, bCollide, Hit);
			};
		}
		Leases[i]->InstructionQueue->Enqueue(Instruction);
	}

	if (!Ask->wait_applied())
	{
		return;
	}

	for (const auto& Instruction : Pending)
	{
		FGenericPlatformProcess::ConditionalSleep([&Instruction]() { return Instruction->Finished.load(); });
	}

	std::vector<flatbuffers::Offset<ffw::SetStateResponse>> Results;
	for (size_t i = 0; i < Leases.size(); ++i)
	{
		Results.push_back(StateResult(Reply, Leases[i].Id, *Leases[i].Pawn, Hits[i]));
	}
	Reply.Set(ffw::CreateSetStatesResponse(Reply.Builder, Reply.Builder.CreateVector(Results)));
}

void FSimServer::SetTrajectory(const ffw::Request& Request, FWireReply& Reply)
{
	const ffw::SetTrajectoryRequest* Ask   = Request.body_as_SetTrajectoryRequest();
	const FRobotLease                Robot = LeaseRobot(Request, Reply);
	if (!Robot)
	{
		return;
	}

	TArray<ARobotPawn::FPoseSample> Samples;
	if (Ask->samples())
	{
		Samples.Reserve(Ask->samples()->size());
		for (const ffw::TrajectorySample* Sample : *Ask->samples())
		{
			const ffw::RobotState* State = Sample->state();
			if (!State || !State->base())
			{
				Reply.Fail(TEXT("every trajectory sample needs a base pose"));
				return;
			}
			if ((State->joint_positions() && State->joint_positions()->size() > 0) || (State->joint_velocities() && State->joint_velocities()->size() > 0) ||
			    (State->link_poses() && State->link_poses()->size() > 0))
			{
				Reply.Fail(TEXT("trajectory samples carry base poses only for now"));
				return;
			}
			if (Samples.Num() > 0 && Sample->t() < Samples.Last().T)
			{
				Reply.Fail(TEXT("trajectory samples must be in ascending t"));
				return;
			}

			const FTransform Base = FWireFrame::FromGroundPose(State->base(), Robot->GroundOriginCm());
			Samples.Add({Sample->t(), Base.GetLocation(), Base.GetRotation()});
		}
	}

	std::lock_guard<std::mutex> Lock(Robot.Sync->StateMutex);
	Robot->SetPoseTrajectory(MoveTemp(Samples));
}

void FSimServer::GetCollision(const ffw::Request& Request, FWireReply& Reply)
{
	const FRobotLease Robot = LeaseRobot(Request, Reply);
	if (!Robot)
	{
		return;
	}

	const bool      bCollided = Robot->GetCrashState();
	const ffw::Vec3 Impact    = FWireFrame::ToVec3(FWireFrame::ToGround(Robot->LastImpactPoint, Robot->GroundOriginCm()));
	Reply.Set(ffw::CreateGetCollisionResponse(Reply.Builder, bCollided, bCollided ? &Impact : nullptr));
}

void FSimServer::GetMoveLineVisible(const ffw::Request& Request, FWireReply& Reply)
{
	const FRobotLease Robot = LeaseRobot(Request, Reply);
	if (Robot)
	{
		Reply.Set(ffw::CreateGetMoveLineVisibleResponse(Reply.Builder, Robot->DrawMovementLine));
	}
}

void FSimServer::SetMoveLineVisible(const ffw::Request& Request, FWireReply& Reply)
{
	const FRobotLease Robot = LeaseRobot(Request, Reply);
	if (Robot)
	{
		Robot->DrawMovementLine = Request.body_as_SetMoveLineVisibleRequest()->visible();
	}
}

/* //} */

/* sensors and devices //{ */

void FSimServer::AddSensor(const ffw::Request& Request, FWireReply& Reply)
{
	const ffw::AddSensorRequest* Ask   = Request.body_as_AddSensorRequest();
	const FRobotLease            Robot = LeaseRobot(Request, Reply);
	if (!Robot || !OnBaseLink(Ask->link(), Ask->mount(), Reply))
	{
		return;
	}

	SensorType Type;
	if (!PluginSensorType(Ask->type(), Type))
	{
		Reply.Fail(FString::Printf(TEXT("unknown sensor type %d"), static_cast<int32>(Ask->type())));
		return;
	}

	std::lock_guard<std::mutex> Lock(Robot.Sync->StateMutex);

	const int32 SensorId = Robot->AddSensorFromServerThread(static_cast<int32>(Type));
	USensor*    Sensor   = SensorId >= 0 ? Robot->FindSensorById(SensorId) : nullptr;
	if (!Sensor)
	{
		Reply.Fail(TEXT("the sensor could not be created"));
		return;
	}

	if (Ask->pose())
	{
		FVector  Offset;
		FRotator Rotation;
		FWireFrame::FromMountPose(Ask->pose(), Offset, Rotation);
		Sensor->SetMount(Offset, Rotation);
	}

	Reply.Set(ffw::CreateAddSensorResponse(Reply.Builder, SensorId));
}

void FSimServer::RemoveSensor(const ffw::Request& Request, FWireReply& Reply)
{
	const FRobotLease Robot = LeaseRobot(Request, Reply);
	if (!Robot)
	{
		return;
	}
	if (!Request.sensor_id().has_value())
	{
		Reply.Fail(TEXT("remove_sensor needs a sensor_id"));
		return;
	}
	const int32 SensorId = *Request.sensor_id();

	std::lock_guard<std::mutex> Lock(Robot.Sync->StateMutex);

	const std::shared_ptr<FSensorSlot> Slot = Robot->FindSensorSlot(SensorId);
	if (!Slot)
	{
		Reply.Fail(FString::Printf(TEXT("robot %d has no sensor %d"), Robot.Id, SensorId));
		return;
	}

	// a request already on the sensor finishes first; the ones queued behind it see it gone
	{
		std::lock_guard<std::mutex> SensorLock(Slot->RequestMutex);
		Slot->bRemoved = true;
	}

	Robot->RemoveSensorFromServerThread(SensorId);
}

void FSimServer::ListSensors(const ffw::Request& Request, FWireReply& Reply)
{
	const FRobotLease Robot = LeaseRobot(Request, Reply);
	if (!Robot)
	{
		return;
	}

	std::vector<int> Ids;
	std::vector<int> Types;
	Robot->GetSensorList(Ids, Types);

	Reply.Set(ffw::CreateListSensorsResponse(Reply.Builder, SensorInfos(Reply, Ids, Types)));
}

void FSimServer::AddDevice(const ffw::Request& Request, FWireReply& Reply)
{
	const ffw::AddDeviceRequest* Ask   = Request.body_as_AddDeviceRequest();
	const FRobotLease            Robot = LeaseRobot(Request, Reply);
	if (!Robot || !OnBaseLink(Ask->link(), Ask->mount(), Reply))
	{
		return;
	}

	const FString Name = WireString(Ask->device_name());
	if (!FSensorDeviceLibrary::Find(Name))
	{
		Reply.Fail(FString::Printf(TEXT("unknown device '%s'"), *Name));
		return;
	}

	FVector  Offset;
	FRotator Rotation;
	FWireFrame::FromMountPose(Ask->pose(), Offset, Rotation);

	std::lock_guard<std::mutex> Lock(Robot.Sync->StateMutex);

	std::vector<int> Ids;
	std::vector<int> Types;
	if (!Robot->AddDeviceFromServerThread(Name, Offset, Rotation, Ask->show_mesh(), Ids, Types))
	{
		Reply.Fail(FString::Printf(TEXT("device '%s' could not be added"), *Name));
		return;
	}

	Reply.Set(ffw::CreateAddDeviceResponse(Reply.Builder, SensorInfos(Reply, Ids, Types)));
}

void FSimServer::ListDevices(const ffw::Request& Request, FWireReply& Reply)
{
	std::vector<flatbuffers::Offset<ffw::DeviceInfo>> Devices;
	for (const FSensorDeviceSpec& Device : FSensorDeviceLibrary::Get())
	{
		Devices.push_back(ffw::CreateDeviceInfo(Reply.Builder, Reply.String(Device.Name), Reply.String(Device.Description)));
	}
	Reply.Set(ffw::CreateListDevicesResponse(Reply.Builder, Reply.Builder.CreateVector(Devices)));
}

void FSimServer::GetCameraInfo(const ffw::Request& Request, FWireReply& Reply)
{
	const FRobotLease Robot = LeaseRobot(Request, Reply);
	if (!Robot)
	{
		return;
	}

	std::vector<int> Ids;
	std::vector<int> Types;
	if (Request.sensor_id().has_value())
	{
		Ids.push_back(*Request.sensor_id());
	}
	else
	{
		Robot->GetSensorList(Ids, Types);
	}

	std::vector<ffw::CameraInfoT> Cameras;
	for (const int Id : Ids)
	{
		const std::shared_ptr<FSensorSlot> Slot = Robot->FindSensorSlot(Id);
		if (!Slot)
		{
			continue;
		}

		std::lock_guard<std::mutex> SensorLock(Slot->RequestMutex);
		if (UCamera* Camera = Slot->bRemoved ? nullptr : Cast<UCamera>(Robot->FindSensorById(Id)))
		{
			Camera->DescribeCameras(Cameras);
		}
	}

	if (Request.sensor_id().has_value() && Cameras.empty())
	{
		Reply.Fail(FString::Printf(TEXT("robot %d has no camera %d"), Robot.Id, *Request.sensor_id()));
		return;
	}

	std::vector<flatbuffers::Offset<ffw::CameraInfo>> Infos;
	for (const ffw::CameraInfoT& Camera : Cameras)
	{
		Infos.push_back(ffw::CreateCameraInfo(Reply.Builder, &Camera));
	}
	Reply.Set(ffw::CreateGetCameraInfoResponse(Reply.Builder, Reply.Builder.CreateVector(Infos)));
}

/* //} */

/* SensorOp() //{ */

void FSimServer::SensorOp(const ffw::Request& Request, FWireReply& Reply)
{
	using B = ffw::RequestBody;

	enum class EKind
	{
		GetConfig,
		SetConfig,
		Data
	};

	SensorType Type = SensorType::RGB_CAMERA;
	EKind      Kind = EKind::Data;

	switch (Request.body_type())
	{
		case B::GetRgbCameraConfigRequest: Kind = EKind::GetConfig; break;
		case B::SetRgbCameraConfigRequest: Kind = EKind::SetConfig; break;
		case B::GetStereoCameraConfigRequest: Type = SensorType::STEREO_CAMERA; Kind = EKind::GetConfig; break;
		case B::SetStereoCameraConfigRequest: Type = SensorType::STEREO_CAMERA; Kind = EKind::SetConfig; break;
		case B::GetFisheyeCameraConfigRequest: Type = SensorType::FISHEYE_CAMERA; Kind = EKind::GetConfig; break;
		case B::SetFisheyeCameraConfigRequest: Type = SensorType::FISHEYE_CAMERA; Kind = EKind::SetConfig; break;
		case B::GetEventCameraConfigRequest: Type = SensorType::EVENT_CAMERA; Kind = EKind::GetConfig; break;
		case B::SetEventCameraConfigRequest: Type = SensorType::EVENT_CAMERA; Kind = EKind::SetConfig; break;
		case B::GetLidarConfigRequest: Type = SensorType::LIDAR; Kind = EKind::GetConfig; break;
		case B::SetLidarConfigRequest: Type = SensorType::LIDAR; Kind = EKind::SetConfig; break;
		case B::GetRgbCameraDataRequest: break;
		case B::GetStereoCameraDataRequest: Type = SensorType::STEREO_CAMERA; break;
		case B::GetFisheyeCameraDataRequest: Type = SensorType::FISHEYE_CAMERA; break;
		case B::GetDepthCameraDataRequest: Type = SensorType::DEPTH_CAMERA; break;
		case B::GetRgbSegCameraDataRequest: Type = SensorType::RGB_SEG_CAMERA; break;
		case B::GetInstanceSegDataRequest:
		case B::GetInstanceSegMapRequest: Type = SensorType::INSTANCE_SEG_CAMERA; break;
		case B::GetEventCameraDataRequest: Type = SensorType::EVENT_CAMERA; break;
		case B::GetLidarScanRequest: Type = SensorType::LIDAR; break;
		case B::GetRangefinderDataRequest: Type = SensorType::RANGEFINDER; break;
		default: Reply.Fail(TEXT("not a sensor request")); return;
	}

	const FRobotLease Robot = LeaseRobot(Request, Reply);
	if (!Robot)
	{
		return;
	}

	const int32 Requested = Request.sensor_id().has_value() ? *Request.sensor_id() : -1;
	if (Request.sensor_id().has_value() && Requested < 0)
	{
		Reply.Fail(FString::Printf(TEXT("sensor_id %d is not a sensor"), Requested));
		return;
	}

	// the default sensor of a type is created on first use, once
	int32 SensorId = -1;
	{
		std::lock_guard<std::mutex> Lock(Robot.Sync->StateMutex);
		SensorId = Robot->ResolveSensorId(Type, Requested);
	}

	const std::shared_ptr<FSensorSlot> Slot   = SensorId >= 0 ? Robot->FindSensorSlot(SensorId) : nullptr;
	USensor*                           Sensor = Slot ? Robot->FindSensorById(SensorId) : nullptr;
	if (!Sensor)
	{
		Reply.Fail(Requested >= 0 ? FString::Printf(TEXT("robot %d has no sensor %d that serves %s"), Robot.Id, Requested, *WireName(Request.body_type()))
		                          : FString::Printf(TEXT("robot %d could not create a sensor for %s"), Robot.Id, *WireName(Request.body_type())));
		return;
	}

	// the rgb config table also configures depth and segmentation cameras; data never crosses types
	const SensorType Actual = Sensor->GetSensorType();
	const bool bLidar       = Type == SensorType::LIDAR && Actual == SensorType::LIDAR_LIVOX;
	if (Kind == EKind::Data && Actual != Type && !bLidar)
	{
		Reply.Fail(FString::Printf(TEXT("sensor %d is not the kind %s reads"), SensorId, *WireName(Request.body_type())));
		return;
	}

	std::lock_guard<std::mutex> SensorLock(Slot->RequestMutex);
	if (Slot->bRemoved)
	{
		Reply.Fail(FString::Printf(TEXT("sensor %d was removed"), SensorId));
		return;
	}

	switch (Kind)
	{
		case EKind::GetConfig: Sensor->GetConfig(Reply); break;
		case EKind::SetConfig: Sensor->SetConfig(Request, Reply); break;
		case EKind::Data: Sensor->GetData(Request, Reply); break;
	}
}

/* //} */
