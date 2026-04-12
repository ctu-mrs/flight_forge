#include "DroneServer.h"

#include "../DronePawn.h"
#include "ImageUtils.h"

#include "Sockets.h"
#include "GameFramework/SpringArmComponent.h"
#include "MessageSerialization/Public/Serialize.h"

/* route() //{ */

bool DroneServer::Route(const FTCPClient &Client, std::shared_ptr<std::stringstream> InputStream) {

	if(TcpServer::Route(Client, InputStream)) {
		return true;
	}

	Serializable::Common::NetworkRequest Request;
	Serialization::SerializeRequest<Serializable::Common::NetworkRequest>(Request, *InputStream);

	if(Request.type == Serializable::Drone::MessageType::get_location) {
		Serializable::Drone::GetLocation::Request CustomRequest;
		Serialization::SerializeRequest<Serializable::Drone::GetLocation::Request>(CustomRequest, *InputStream);

		//testing new function
		// GetAllSensors(Client);
		// AddSensor(Client, 0);
		
		return GetLocation(Client, CustomRequest);
	}

	if(Request.type == Serializable::Drone::MessageType::get_crash_state) {
	
		Serializable::Drone::GetCrashState::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);
		
		return GetCrashState(Client, CustomRequest);
	}
	
	if(Request.type == Serializable::Drone::MessageType::set_location) {
	
		Serializable::Drone::SetLocation::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);
		
		return SetLocation(Client, CustomRequest);
	}
	
	if(Request.type == Serializable::Drone::MessageType::get_rgb_camera_data) {
		return GetSensorData(Client, 0);
	}
	
	if(Request.type == Serializable::Drone::MessageType::get_stereo_camera_data) {
		return GetSensorData(Client, 5);
	}
	
	if(Request.type == Serializable::Drone::MessageType::get_rgb_seg_camera_data) {
		return GetSensorData(Client, 4);
	}
	
	if(Request.type == Serializable::Drone::MessageType::get_rotation) {
	
		Serializable::Drone::GetRotation::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);

		//testing new function
		//RemoveSensor(Client, 1);
	
		return GetRotation(Client, CustomRequest);
	}
	
	if(Request.type == Serializable::Drone::MessageType::set_rotation) {
	
		Serializable::Drone::SetRotation::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);

		//testing new function
		//AddSensor(Client, 5);
		
		return SetRotation(Client, CustomRequest);
	}
	
	if(Request.type == Serializable::Drone::MessageType::set_location_and_rotation) {
	
		Serializable::Drone::SetLocationAndRotation::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);
	
		return SetLocationAndRotation(Client, CustomRequest);
	}
	
	if(Request.type == Serializable::Drone::MessageType::set_location_and_rotation_async) {
	
		Serializable::Drone::SetLocationAndRotationAsync::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);
	
		return SetLocationAndRotationAsync(Client, CustomRequest);
	}
	
	if(Request.type == Serializable::Drone::MessageType::get_lidar_data) {
		//1 - lidar, 3 - livox
		//TODO better mode selecting
		if (TObjectPtr<USensor>* SensorPtr = DronePawn->Sensors.Find(1))
		{
			USensor* Sensor = *SensorPtr;
			if (ULidar* Lidar = Cast<ULidar>(Sensor))
			{
				Lidar->SetLidarModeWithInt(0);
			}
		}
		
		return GetSensorData(Client, 1);
	}
	
	if(Request.type == Serializable::Drone::MessageType::get_lidar_seg) {
		//TODO better mode selecting
		if (TObjectPtr<USensor>* SensorPtr = DronePawn->Sensors.Find(1))
		{
			USensor* Sensor = *SensorPtr;
			if (ULidar* Lidar = Cast<ULidar>(Sensor))
			{
				Lidar->SetLidarModeWithInt(1);
			}
		}
		
		return GetSensorData(Client, 1);
	}
	if(Request.type == Serializable::Drone::MessageType::get_lidar_int) {
		//TODO better mode selecting
		if (TObjectPtr<USensor>* SensorPtr = DronePawn->Sensors.Find(1))
		{
			USensor* Sensor = *SensorPtr;
			if (ULidar* Lidar = Cast<ULidar>(Sensor))
			{
				Lidar->SetLidarModeWithInt(2);
			}
		}
		
		return GetSensorData(Client, 1);
	}
	
	if(Request.type == Serializable::Drone::MessageType::get_lidar_config) {
		return GetSensorConfig(Client, 1);
	}
	
	if(Request.type == Serializable::Drone::MessageType::set_lidar_config) {
		return SetSensorConfig(Client, InputStream, 1);
	}
	
	if(Request.type == Serializable::Drone::MessageType::get_rgb_camera_config) {
		return GetSensorConfig(Client, 0);
	}
	
	if(Request.type == Serializable::Drone::MessageType::set_rgb_camera_config) {
		return SetSensorConfig(Client, InputStream, 0);
	}
	
	if(Request.type == Serializable::Drone::MessageType::get_stereo_camera_config) {
		return GetSensorConfig(Client, 5);
	}
	
	if(Request.type == Serializable::Drone::MessageType::set_stereo_camera_config) {
		return SetSensorConfig(Client, InputStream, 5);
	}
	
	if(Request.type == Serializable::Drone::MessageType::get_move_line_visible) {
	
		Serializable::Drone::GetMoveLineVisible::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);
	
		return GetMoveLineVisible(Client, CustomRequest);
	}
	
	if(Request.type == Serializable::Drone::MessageType::set_move_line_visible) {
	
		Serializable::Drone::SetMoveLineVisible::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);
	
		return SetMoveLineVisible(Client, CustomRequest);
	}
	
	if(Request.type == Serializable::Drone::MessageType::get_rangefinder_data) {
		return GetSensorData(Client, 2);
	}

	return false;
}

//}

/* getLocation() //{ */

bool DroneServer::GetLocation(const FTCPClient& Client, Serializable::Drone::GetLocation::Request& Request)
{

	if(!DronePawn) {
		return false;
	}

	auto Location = DronePawn->GetActorLocation();

	auto Response = Serializable::Drone::GetLocation::FVectorToResponse(Location);

	std::stringstream OutputStream;
	Serialization::DeserializeResponse<Serializable::Drone::GetLocation::Response>(*Response, OutputStream);

	return Respond(Client, OutputStream);
}

//}

/* getCrashState() //{ */

bool DroneServer::GetCrashState(const FTCPClient& Client, Serializable::Drone::GetCrashState::Request& Request)
{

	// UE_LOG(LogTemp, Warning, TEXT("DroneServer::GetCrashState"));

	if(!DronePawn) {
		return false;
	}

	auto crash_state = DronePawn->GetCrashState();

	Serializable::Drone::GetCrashState::Response Response(true);

	Response.crashed = crash_state;

	// UE_LOG(LogTemp, Warning, TEXT("DroneServer::GetCrashState %d %d"), Response.status, Response.crashed);

	std::stringstream OutputStream;
	Serialization::DeserializeResponse(Response, OutputStream);

	return Respond(Client, OutputStream);
}

//}

/* setLocation() //{ */

bool DroneServer::SetLocation(const FTCPClient& Client, Serializable::Drone::SetLocation::Request& Request)
{

	if(!DronePawn) {
		return false;
	}

	auto Location = FVector(Request.x, Request.y, Request.z);
	auto TeleportedToLocation = FVector();
	bool CheckCollisions = Request.checkCollisions;

	FHitResult HitResult;

	auto Instruction = std::make_shared<FInstruction<ADronePawn>>();

	Instruction->Function = [&Location, &TeleportedToLocation, &HitResult, CheckCollisions](ADronePawn& _DronePawn) {
		_DronePawn.SetLocation(Location, TeleportedToLocation, CheckCollisions, HitResult);
	};

	DronePawn->InstructionQueue->Enqueue(Instruction);
	FGenericPlatformProcess::ConditionalSleep([Instruction](){return Instruction->Finished;});

	Serializable::Drone::SetLocation::Response Response(true);

	Response.teleportedToX = TeleportedToLocation.X;
	Response.teleportedToY = TeleportedToLocation.Y;
	Response.teleportedToZ = TeleportedToLocation.Z;
	Response.isHit = HitResult.IsValidBlockingHit();
	Response.impactPointX = HitResult.ImpactPoint.X;
	Response.impactPointY = HitResult.ImpactPoint.Y;
	Response.impactPointZ = HitResult.ImpactPoint.Z;

	std::stringstream OutputStream;
	Serialization::DeserializeResponse(Response, OutputStream);

	return Respond(Client, OutputStream);
}

//}

/* getRotation() //{ */

bool DroneServer::GetRotation(const FTCPClient& Client, Serializable::Drone::GetRotation::Request& Request) {

	if(!DronePawn) {
		return false;
	}

	auto Rotation = DronePawn->GetActorRotation();

	auto Response = Serializable::Drone::GetRotation::FRotatorToResponse(Rotation);
	std::stringstream OutputStream;
	Serialization::DeserializeResponse(*Response, OutputStream);

	return Respond(Client, OutputStream);
}

//}

/* postRotation() //{ */

void _PostRotation(ADronePawn& DronePawn)
{
	// Also rotate third person camera along Yaw axis
	auto SpringArmRotator = DronePawn.ThirdPersonCameraSpringArm->GetRelativeRotation();
	SpringArmRotator.Yaw = DronePawn.GetActorRotation().Yaw;
	DronePawn.ThirdPersonCameraSpringArm->SetRelativeRotation(SpringArmRotator);
}

//}

/* _setRotation() //{ */

void _SetRotation(ADronePawn& DronePawn, FRotator& Rotation, FHitResult& HitResult)
{
	DronePawn.SetActorRelativeRotation(Rotation, true, &HitResult);
	_PostRotation(DronePawn);
}

//}

/* _setLocationAndRotation() //{ */

void _SetLocationAndRotation(ADronePawn& DronePawn, FVector& Location, FRotator& Rotation, bool& should_collide, FHitResult& HitResult)
{
	DronePawn.SetActorLocationAndRotation(Location, Rotation, should_collide, &HitResult);

	_PostRotation(DronePawn);
}

//}

/* _setLocationAndRotationAsync() //{ */

void _SetLocationAndRotationAsync(ADronePawn& DronePawn, const FVector Location, const FRotator Rotation, const bool should_collide, const FHitResult HitResult)
{
	FVector location = Location;
	FRotator rotation = Rotation;
	FHitResult hit_result = HitResult;

	// UE_LOG(LogTemp, Warning, TEXT("_SetLocationAndRotation %d"), should_collide);

	DronePawn.SetActorLocationAndRotation(location, rotation, should_collide, &hit_result);

	if (!DronePawn.uav_crashed_) {
		DronePawn.uav_crashed_ = hit_result.IsValidBlockingHit();
	}

	_PostRotation(DronePawn);
}

//}

/* setRotation() //{ */

bool DroneServer::SetRotation(const FTCPClient& Client, Serializable::Drone::SetRotation::Request& Request)
{
	//UE_LOG(LogTemp, Warning, TEXT("DroneServer::SetRotation"));
	if(!DronePawn)
	{
		return false;
	}

	auto Rotation = FRotator(Request.pitch, Request.yaw, Request.roll);
	FHitResult HitResult;

	auto Instruction = std::make_shared<FInstruction<ADronePawn>>();
	Instruction->Function = [&Rotation, &HitResult](ADronePawn& _DronePawn)
	{
		_SetRotation(_DronePawn, Rotation, HitResult);
	};
	DronePawn->InstructionQueue->Enqueue(Instruction);
	FGenericPlatformProcess::ConditionalSleep([Instruction](){return Instruction->Finished;});

	Rotation = DronePawn->GetActorRotation();
	//UE_LOG(LogTemp, Warning, TEXT("Rotated: %s"), *Rotation.ToString());

	Serializable::Drone::SetRotation::Response Response(true);
	Response.rotatedToPitch = Rotation.Pitch;
	Response.rotatedToYaw = Rotation.Yaw;
	Response.rotatedToRoll = Rotation.Roll;
	Response.isHit = HitResult.IsValidBlockingHit();
	Response.impactPointX = HitResult.ImpactPoint.X;
	Response.impactPointY = HitResult.ImpactPoint.Y;
	Response.impactPointZ = HitResult.ImpactPoint.Z;

	std::stringstream OutputStream;
	Serialization::DeserializeResponse(Response, OutputStream);
	return Respond(Client, OutputStream);
}

//}

/* setLocationAndRotation() //{ */

bool DroneServer::SetLocationAndRotation(const FTCPClient& Client, Serializable::Drone::SetLocationAndRotation::Request& Request)
{
	if(!DronePawn) {
		return false;
	}

    double start = FPlatformTime::Seconds();

	auto Location = FVector(Request.x, Request.y, Request.z);
	FHitResult HitResult;
	auto Rotation = FRotator(Request.pitch, Request.yaw, Request.roll);
	bool should_collide = Request.should_collide;

	auto Instruction = std::make_shared<FInstruction<ADronePawn>>();
        Instruction->Function = [&Location, &Rotation, &should_collide,
                                 &HitResult](ADronePawn &_DronePawn) {
          _SetLocationAndRotation(_DronePawn, Location, Rotation,
                                  should_collide, HitResult);
        };

        DronePawn->InstructionQueue->Enqueue(Instruction);
	FGenericPlatformProcess::ConditionalSleep([Instruction](){return Instruction->Finished;});

	Location = DronePawn->GetActorLocation();
	Rotation = DronePawn->GetActorRotation();

	Serializable::Drone::SetLocationAndRotation::Response Response(true);
	Response.teleportedToX = Location.X;
	Response.teleportedToY = Location.Y;
	Response.teleportedToZ = Location.Z;
	Response.rotatedToPitch = Rotation.Pitch;
	Response.rotatedToYaw = Rotation.Yaw;
	Response.rotatedToRoll = Rotation.Roll;
	Response.isHit = HitResult.IsValidBlockingHit();
	Response.impactPointX = HitResult.ImpactPoint.X;
	Response.impactPointY = HitResult.ImpactPoint.Y;
	Response.impactPointZ = HitResult.ImpactPoint.Z;

	std::stringstream OutputStream;
	Serialization::DeserializeResponse(Response, OutputStream);

    double end = FPlatformTime::Seconds();

	return Respond(Client, OutputStream);
}

//}

/* setLocationAndRotationAsync() //{ */

bool DroneServer::SetLocationAndRotationAsync(const FTCPClient& Client, Serializable::Drone::SetLocationAndRotationAsync::Request& Request)
{
	if(!DronePawn) {
		return false;
	}

    double start = FPlatformTime::Seconds();

	auto Location = FVector(Request.x, Request.y, Request.z);
	FHitResult HitResult;
	auto Rotation = FRotator(Request.pitch, Request.yaw, Request.roll);
	bool should_collide = Request.should_collide;

	// UE_LOG(LogTemp, Warning, TEXT("DroneServer::SetLocationAndRotation %d"), should_collide);

	auto Instruction = std::make_shared<FInstruction<ADronePawn>>();
	Instruction->Function = [Location, Rotation, should_collide, HitResult](ADronePawn& _DronePawn){
		_SetLocationAndRotationAsync(_DronePawn, Location, Rotation, should_collide, HitResult);
	};

	DronePawn->InstructionQueue->Enqueue(Instruction);
	// FGenericPlatformProcess::ConditionalSleep([Instruction](){return Instruction->Finished;});

	Serializable::Drone::SetLocationAndRotation::Response Response(true);

	std::stringstream OutputStream;
	Serialization::DeserializeResponse(Response, OutputStream);

    double end = FPlatformTime::Seconds();

	return Respond(Client, OutputStream);
}

/* getMoveLineVisible() //{ */

bool DroneServer::GetMoveLineVisible(const FTCPClient& Client, Serializable::Drone::GetMoveLineVisible::Request& Request)
{
	//UE_LOG(LogTemp, Warning, TEXT("DroneServer::GetMoveLineVisible"));
	if(!DronePawn)
	{
		return false;
	}

	bool Value = DronePawn->DrawMovementLine;

	std::stringstream OutputStream;
	Serializable::Drone::GetMoveLineVisible::Response Response(true);
	Response.visible = Value;
	Serialization::DeserializeResponse(Response, OutputStream);
	return Respond(Client, OutputStream);
}

//}

/* setMoveLineVisible() //{ */

bool DroneServer::SetMoveLineVisible(const FTCPClient& Client, Serializable::Drone::SetMoveLineVisible::Request& Request)
{
	//UE_LOG(LogTemp, Warning, TEXT("DroneServer::SetMoveLineVisible"));
	if(!DronePawn) {
		return false;
	}

	DronePawn->DrawMovementLine = Request.visible;

	std::stringstream OutputStream;
	Serializable::Drone::SetMoveLineVisible::Response Response(true);
	Serialization::DeserializeResponse(Response, OutputStream);
	return Respond(Client, OutputStream);
}

//}

bool DroneServer::GetSensorConfig(const FTCPClient& Client, int SensorID)
{
	UE_LOG(LogTemp, Log, TEXT("DroneServer::GetSensorConfig"));

	if(!DronePawn) {
		return false;
	}

	std::stringstream OutputStream;

	//0 - camera, 1 - lidar, 2 - rangefinder 
	if (TObjectPtr<USensor>* SensorPtr = DronePawn->Sensors.Find(SensorID))
	{
		USensor* Sensor = *SensorPtr;
		Sensor->GetConfig(OutputStream);
		//UE_LOG(LogTemp, Warning, TEXT("Serialized Sensor Config:\n%s"), *FString(OutputStream.str().c_str()));
		UE_LOG(LogTemp, Warning, TEXT("Stream size: %d bytes"), OutputStream.str().size());
		
		return Respond(Client, OutputStream);
	}
	
	return false;
}

bool DroneServer::SetSensorConfig(const FTCPClient& Client, std::shared_ptr<std::stringstream> InputStream, int SensorID)
{
	UE_LOG(LogTemp, Log, TEXT("DroneServer::SetSensorConfig"));

	if(!DronePawn) {
		return false;
	}

	std::stringstream OutputStream;
	if (TObjectPtr<USensor>* SensorPtr = DronePawn->Sensors.Find(SensorID))
	{
		USensor* Sensor = *SensorPtr;
		Sensor->SetConfig(OutputStream, InputStream);
		
		return Respond(Client, OutputStream);
	}
	
	return false;
}


bool DroneServer::GetSensorData(const FTCPClient& Client, int SensorID)
{
	UE_LOG(LogTemp, Log, TEXT("DroneServer::GetSensorData"));

	if(!DronePawn) {
		return false;
	}

	std::stringstream OutputStream;
	if (TObjectPtr<USensor>* SensorPtr = DronePawn->Sensors.Find(SensorID))
	{
		USensor* Sensor = *SensorPtr;
		Sensor->GetData(OutputStream);
		
		return Respond(Client, OutputStream);
	}
	
	return false;
}

bool DroneServer::GetAllSensors(const FTCPClient& Client)
{
	 //ID - name
	for (const TPair<int32, TObjectPtr<USensor>>& Pair : DronePawn->Sensors)
	{
		UE_LOG(LogTemp, Log, TEXT("Sensor ID: %d, Name: %s"), Pair.Key, *Pair.Value->GetName());
	}
	return true;
}

bool DroneServer::AddSensor(const FTCPClient& Client, int SensorType)
{
	UE_LOG(LogTemp, Log, TEXT("DroneServer::AddSensor"));

	if(!DronePawn) {
		return false;
	}

	bool ret = false;
	int SensorID = -1;
	
	auto Instruction = std::make_shared<FInstruction<ADronePawn>>();
	Instruction->Function = [SensorType, &SensorID](ADronePawn& _DronePawn)
	{
		SensorID = _DronePawn.AddSensor(SensorType);
	};
	DronePawn->InstructionQueue->Enqueue(Instruction);
	FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });
	
	if(SensorID >= 0)
	{
		//probably send back
		ret = true;
	}

	return ret;
}
bool DroneServer::RemoveSensor(const FTCPClient& Client, int RemoveSensorID)
{
	UE_LOG(LogTemp, Log, TEXT("DroneServer::RemoveSensor"));

	if(!DronePawn) {
		return false;
	}

	bool status = false;
	
	auto Instruction = std::make_shared<FInstruction<ADronePawn>>();
	Instruction->Function = [&status, RemoveSensorID](ADronePawn& _DronePawn)
	{
		status = _DronePawn.RemoveSensor(RemoveSensorID);
	};
	DronePawn->InstructionQueue->Enqueue(Instruction);
	FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });
	
	return status;
}
