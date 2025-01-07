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
	
		Serializable::Drone::GetRgbCameraData::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);
	
		return GetRgbCameraData(Client, CustomRequest);
	}
	
	if(Request.type == Serializable::Drone::MessageType::get_stereo_camera_data) {
	
		Serializable::Drone::GetStereoCameraData::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);
	
		return GetStereoCameraData(Client, CustomRequest);
	}
	
	if(Request.type == Serializable::Drone::MessageType::get_rgb_seg_camera_data) {
	
		Serializable::Drone::GetRgbSegCameraData::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);
	
		return GetRgbSegCameraData(Client, CustomRequest);
	}
	
	if(Request.type == Serializable::Drone::MessageType::get_rotation) {
	
		Serializable::Drone::GetRotation::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);
	
		return GetRotation(Client, CustomRequest);
	}
	
	if(Request.type == Serializable::Drone::MessageType::set_rotation) {
	
		Serializable::Drone::SetRotation::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);
	
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
	
		Serializable::Drone::GetLidarData::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);
	
		return GetLidarData(Client, CustomRequest);
	}
	
	if(Request.type == Serializable::Drone::MessageType::get_lidar_seg) {
	
		Serializable::Drone::GetLidarSegData::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);
	
		return GetLidarSegData(Client, CustomRequest);
	}
	if(Request.type == Serializable::Drone::MessageType::get_lidar_int) {
	
		Serializable::Drone::GetLidarIntData::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);
	
		return GetLidarIntData(Client, CustomRequest);
	}
	
	if(Request.type == Serializable::Drone::MessageType::get_lidar_config) {
	
		Serializable::Drone::GetLidarConfig::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);
	
		return GetLidarConfig(Client, CustomRequest);
	}
	
	if(Request.type == Serializable::Drone::MessageType::set_lidar_config) {
	
		Serializable::Drone::SetLidarConfig::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);
	
		return SetLidarConfig(Client, CustomRequest);
	}
	
	if(Request.type == Serializable::Drone::MessageType::get_rgb_camera_config) {
	
		Serializable::Drone::GetRgbCameraConfig::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);
	
		return GetRgbCameraConfig(Client, CustomRequest);
	}
	
	if(Request.type == Serializable::Drone::MessageType::set_rgb_camera_config) {
	
		Serializable::Drone::SetRgbCameraConfig::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);
	
		return SetRgbCameraConfig(Client, CustomRequest);
	}
	
	if(Request.type == Serializable::Drone::MessageType::get_stereo_camera_config) {
	
		Serializable::Drone::GetStereoCameraConfig::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);
	
		return GetStereoCameraConfig(Client, CustomRequest);
	}
	
	if(Request.type == Serializable::Drone::MessageType::set_stereo_camera_config) {
	
		Serializable::Drone::SetStereoCameraConfig::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);
	
		return SetStereoCameraConfig(Client, CustomRequest);
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
	
		Serializable::Drone::GetRangefinderData::Request CustomRequest;
		Serialization::SerializeRequest(CustomRequest, *InputStream);
	
		return GetRangeFinderData(Client, CustomRequest);
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

/* getRgbCameraData() //{ */

bool DroneServer::GetRgbCameraData(const FTCPClient& Client, Serializable::Drone::GetRgbCameraData::Request& Request) {

	// UE_LOG(LogTemp, Warning, TEXT("DroneServer::GetRgbCameraData"));

	TArray<uint8> CompressedBitmap;
	double stamp;
	bool res = DronePawn->GetRgbCameraDataFromServerThread(CompressedBitmap, stamp);

	if (!res) {
		Serializable::Drone::GetRgbCameraData::Response Response(false);

		std::stringstream OutputStream;

		Serialization::DeserializeResponse(Response, OutputStream);

		return Respond(Client, OutputStream);
	}

	Serializable::Drone::GetRgbCameraData::Response Response(true);
	Response.image_ = std::vector<unsigned char>(CompressedBitmap.Num());
	
	// TODO
	// std::copy(CompressedBitmap.begin(), CompressedBitmap.end(), std::back_inserter(Response->imageData));
	for(int i = 0; i < CompressedBitmap.Num(); i++) {
		Response.image_[i] = CompressedBitmap[i];
	}

	std::stringstream OutputStream;

	Response.stamp_ = stamp;

	Serialization::DeserializeResponse(Response, OutputStream);

	return Respond(Client, OutputStream);
}

//}

/* getStereoCameraData() //{ */

bool DroneServer::GetStereoCameraData(const FTCPClient& Client, Serializable::Drone::GetStereoCameraData::Request& Request) {

	TArray<uint8> image_left_;
	TArray<uint8> image_right_;
	double stamp;
	bool res = DronePawn->GetStereoCameraDataFromServerThread(image_left_, image_right_, stamp);

	if (!res) {
		Serializable::Drone::GetStereoCameraData::Response Response(false);

		std::stringstream OutputStream;

		Serialization::DeserializeResponse(Response, OutputStream);

		return Respond(Client, OutputStream);
	}

	Serializable::Drone::GetStereoCameraData::Response Response(true);

	Response.image_left_ = std::vector<unsigned char>(image_left_.Num());
	Response.image_right_ = std::vector<unsigned char>(image_right_.Num());

	// TODO
	// std::copy(CompressedBitmap.begin(), CompressedBitmap.end(), std::back_inserter(Response->imageData));
	for(int i = 0; i < image_left_.Num(); i++) {
		Response.image_left_[i] = image_left_[i];
	}

	for(int i = 0; i < image_right_.Num(); i++) {
		Response.image_right_[i] = image_right_[i];
	}

	std::stringstream OutputStream;

	Response.stamp_ = stamp;

	Serialization::DeserializeResponse(Response, OutputStream);

	return Respond(Client, OutputStream);
}

//}

/* getRgbSegCameraData() //{ */

bool DroneServer::GetRgbSegCameraData(const FTCPClient& Client, Serializable::Drone::GetRgbSegCameraData::Request& Request)
{
	//UE_LOG(LogTemp, Warning, TEXT("DroneServer::GetCameraData"));

	TArray<uint8> CompressedBitmap;
	double stamp;
	bool res = DronePawn->GetRgbSegCameraFromServerThread(CompressedBitmap, stamp);

	if (!res) {
		Serializable::Drone::GetRgbSegCameraData::Response Response(false);

		std::stringstream OutputStream;

		Serialization::DeserializeResponse(Response, OutputStream);

		return Respond(Client, OutputStream);
	}

	Serializable::Drone::GetRgbSegCameraData::Response Response(true);
	Response.image_ = std::vector<unsigned char>(CompressedBitmap.Num());

	for(int i = 0; i < CompressedBitmap.Num(); i++) {
		Response.image_[i] = CompressedBitmap[i];
	}

	Response.stamp_ = stamp;

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

bool DroneServer::GetRangeFinderData(const FTCPClient& Client,
	Serializable::Drone::GetRangefinderData::Request& Request)
{
	if(!DronePawn) {
		return false;
	}

	double range;
	DronePawn->GetRangefinderData(range);
	
	std::stringstream OutputStream;
	Serializable::Drone::GetRangefinderData::Response Response(true);
	Response.range = range;

	Serialization::DeserializeResponse(Response, OutputStream);

	return Respond(Client, OutputStream);	
}

//}

/* getLidarData() //{ */

bool DroneServer::GetLidarData(const FTCPClient& Client, Serializable::Drone::GetLidarData::Request& Request) {

	// UE_LOG(LogTemp, Warning, TEXT("DroneServer::GetLidarData"));

	if(!DronePawn) {
		return false;
	}

	std::vector<Serializable::Drone::GetLidarData::LidarData> LidarData;
	FVector Start;
	DronePawn->GetLidarHits(LidarData, Start);

	std::stringstream OutputStream;
	Serializable::Drone::GetLidarData::Response Response(true);
	Response.lidarData = LidarData;
	Response.startX = Start.X;
	Response.startY = Start.Y;
	Response.startZ = Start.Z;

	Serialization::DeserializeResponse(Response, OutputStream);

	return Respond(Client, OutputStream);
}

//}

/* getLidarSegData() //{ */

bool DroneServer::GetLidarSegData(const FTCPClient& Client, Serializable::Drone::GetLidarSegData::Request& Request)
{
	//UE_LOG(LogTemp, Warning, TEXT("DroneServer::GetLidarData"));
	if(!DronePawn) {
		return false;
	}

	std::vector<Serializable::Drone::GetLidarSegData::LidarSegData> LidarSegData;
	FVector Start;
	DronePawn->GetSegLidarHits(LidarSegData, Start);

	std::stringstream OutputStream;
	Serializable::Drone::GetLidarSegData::Response Response(true);
	Response.lidarSegData = LidarSegData;
	Response.startX = Start.X;
	Response.startY = Start.Y;
	Response.startZ = Start.Z;

	Serialization::DeserializeResponse(Response, OutputStream);
	return Respond(Client, OutputStream);
}

//}

/* getLidarIntData() //{ */

bool DroneServer::GetLidarIntData(const FTCPClient& Client, Serializable::Drone::GetLidarIntData::Request& Request)
{
	//UE_LOG(LogTemp, Warning, TEXT("DroneServer::GetLidarData"));
	if(!DronePawn) {
		return false;
	}

	std::vector<Serializable::Drone::GetLidarIntData::LidarIntData> LidarIntData;
	FVector Start;
	DronePawn->GetIntLidarHits(LidarIntData, Start);

	std::stringstream OutputStream;
	Serializable::Drone::GetLidarIntData::Response Response(true);
	Response.lidarIntData = LidarIntData;
	Response.startX = Start.X;
	Response.startY = Start.Y;
	Response.startZ = Start.Z;

	Serialization::DeserializeResponse(Response, OutputStream);
	return Respond(Client, OutputStream);
}

//}

/* getLidarConfig() //{ */

bool DroneServer::GetLidarConfig(const FTCPClient& Client, Serializable::Drone::GetLidarConfig::Request& Request)
{
	//UE_LOG(LogTemp, Warning, TEXT("DroneServer::GetLidarConfig"));
	if(!DronePawn)
	{
		return false;
	}

	const auto LidarConfig = DronePawn->GetLidarConfig();

	std::stringstream OutputStream;
	Serializable::Drone::GetLidarConfig::Response Response(true);
	Response.config = Serializable::Drone::LidarConfig{};

	Response.config.Enable = LidarConfig.Enable;
	Response.config.ShowBeams = LidarConfig.ShowBeams;
	Response.config.BeamLength = LidarConfig.BeamLength;
	Response.config.BeamHorRays = LidarConfig.BeamHorRays;
	Response.config.BeamVertRays = LidarConfig.BeamVertRays;
	Response.config.Frequency= LidarConfig.Frequency;
	Response.config.OffsetX = LidarConfig.Offset.X;
	Response.config.OffsetY = LidarConfig.Offset.Y;
	Response.config.OffsetZ = LidarConfig.Offset.Z;
	Response.config.OrientationPitch = LidarConfig.Orientation.Pitch;
	Response.config.OrientationYaw = LidarConfig.Orientation.Yaw;
	Response.config.OrientationRoll = LidarConfig.Orientation.Roll;
	Response.config.FOVHor = LidarConfig.FOVHor;
	Response.config.FOVVert = LidarConfig.FOVVert;

	Serialization::DeserializeResponse(Response, OutputStream);
	
	return Respond(Client, OutputStream);
}

//}

/* setLidarConfig() //{ */

bool DroneServer::SetLidarConfig(const FTCPClient& Client, Serializable::Drone::SetLidarConfig::Request& Request)
{
	if(!DronePawn) {
		return false;
	}

	FLidarConfig Config;
	Config.Enable = Request.config.Enable;	
	Config.ShowBeams = Request.config.ShowBeams;
	Config.BeamLength = Request.config.BeamLength;
	Config.BeamHorRays = Request.config.BeamHorRays;
	Config.BeamVertRays = Request.config.BeamVertRays;
	Config.Frequency = Request.config.Frequency;

	Config.Offset = FVector(Request.config.OffsetX, Request.config.OffsetY, Request.config.OffsetZ);
	
	Config.Orientation = FRotator(Request.config.OrientationPitch, Request.config.OrientationYaw, Request.config.OrientationRoll);
	Config.FOVHor = Request.config.FOVHor;
	Config.FOVVert = Request.config.FOVVert;

	const auto Status = DronePawn->SetLidarConfig(Config);

	std::stringstream OutputStream;
	Serializable::Drone::SetLidarConfig::Response Response(Status);
	Serialization::DeserializeResponse(Response, OutputStream);
	return Respond(Client, OutputStream);
}

//}

/* getRgbCameraConfig() //{ */

bool DroneServer::GetRgbCameraConfig(const FTCPClient& Client, Serializable::Drone::GetRgbCameraConfig::Request& Request)
{
	if(!DronePawn) {
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("DroneServer::GetRgbCameraConfig()"));

	const auto CameraConfig = DronePawn->GetRgbCameraConfig();

	std::stringstream OutputStream;
	Serializable::Drone::GetRgbCameraConfig::Response Response(true);
	Response.config = Serializable::Drone::RgbCameraConfig{};

	Response.config.show_debug_camera_= CameraConfig.ShowCameraComponent;

	Response.config.offset_x_ = CameraConfig.Offset.X;
	Response.config.offset_y_ = CameraConfig.Offset.Y;
	Response.config.offset_z_ = CameraConfig.Offset.Z;

	Response.config.rotation_pitch_ = CameraConfig.Orientation.Pitch;
	Response.config.rotation_yaw_ = CameraConfig.Orientation.Yaw;
	Response.config.rotation_roll_ = CameraConfig.Orientation.Roll;

	Response.config.fov_ = CameraConfig.FOVAngle;

	Response.config.width_ = CameraConfig.Width;
	Response.config.height_ = CameraConfig.Height;

	Response.config.enable_temporal_aa_ = CameraConfig.enable_temporal_aa;
	Response.config.enable_hdr_ = CameraConfig.enable_hdr;
	Response.config.enable_raytracing_ = CameraConfig.enable_raytracing;

	Serialization::DeserializeResponse(Response, OutputStream);

	return Respond(Client, OutputStream);
}

//}

/* getStereoCameraConfig() //{ */

bool DroneServer::GetStereoCameraConfig(const FTCPClient& Client, Serializable::Drone::GetStereoCameraConfig::Request& Request)
{
	if(!DronePawn) {
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("DroneServer::GetStereoCameraConfig()"));

	const auto CameraConfig = DronePawn->GetStereoCameraConfig();

	std::stringstream OutputStream;
	Serializable::Drone::GetStereoCameraConfig::Response Response(true);
	Response.config = Serializable::Drone::StereoCameraConfig{};

	Response.config.show_debug_camera_= CameraConfig.ShowCameraComponent;

	Response.config.offset_x_ = CameraConfig.Offset.X;
	Response.config.offset_y_ = CameraConfig.Offset.Y;
	Response.config.offset_z_ = CameraConfig.Offset.Z;

	Response.config.rotation_pitch_ = CameraConfig.Orientation.Pitch;
	Response.config.rotation_yaw_ = CameraConfig.Orientation.Yaw;
	Response.config.rotation_roll_ = CameraConfig.Orientation.Roll;

	Response.config.fov_ = CameraConfig.FOVAngle;

	Response.config.width_ = CameraConfig.Width;
	Response.config.height_ = CameraConfig.Height;

	Response.config.baseline_ = CameraConfig.baseline;

	Response.config.enable_temporal_aa_ = CameraConfig.enable_temporal_aa;
	Response.config.enable_hdr_ = CameraConfig.enable_hdr;
	Response.config.enable_raytracing_ = CameraConfig.enable_raytracing;

	Serialization::DeserializeResponse(Response, OutputStream);

	return Respond(Client, OutputStream);
}

//}

/* setRgbCameraConfig() //{ */

bool DroneServer::SetRgbCameraConfig(const FTCPClient& Client, Serializable::Drone::SetRgbCameraConfig::Request& Request)
{
	UE_LOG(LogTemp, Warning, TEXT("DroneServer::SetRgbCameraConfig"));

	if(!DronePawn) {
		return false;
	}
	
	FRgbCameraConfig Config;
	Config.ShowCameraComponent = Request.config.show_debug_camera_;

	Config.Offset = FVector(Request.config.offset_x_, Request.config.offset_y_, Request.config.offset_z_);
	Config.Orientation = FRotator(Request.config.rotation_pitch_, Request.config.rotation_yaw_, Request.config.rotation_roll_);

	Config.FOVAngle = Request.config.fov_;

	Config.Width = Request.config.width_;
	Config.Height = Request.config.height_;

	Config.enable_hdr = Request.config.enable_hdr_;
	Config.enable_temporal_aa = Request.config.enable_temporal_aa_;
	Config.enable_raytracing = Request.config.enable_raytracing_;

	Config.enable_motion_blur = Request.config.enable_motion_blur_;
	Config.motion_blur_amount = Request.config.motion_blur_amount_;
	Config.motion_blur_distortion = Request.config.motion_blur_distortion_;

	bool Status = false;

	auto Instruction = std::make_shared<FInstruction<ADronePawn>>();

	Instruction->Function = [&Config, &Status](ADronePawn& _DronePawn) {
		Status = _DronePawn.SetRgbCameraConfig(Config);
	};

	DronePawn->InstructionQueue->Enqueue(Instruction);
	FGenericPlatformProcess::ConditionalSleep([Instruction](){return Instruction->Finished;});

	std::stringstream OutputStream;
	Serializable::Drone::SetRgbCameraConfig::Response Response(Status);
	Serialization::DeserializeResponse(Response, OutputStream);
	return Respond(Client, OutputStream);
}

//}

/* setStereoCameraConfig() //{ */

bool DroneServer::SetStereoCameraConfig(const FTCPClient& Client, Serializable::Drone::SetStereoCameraConfig::Request& Request)
{
	UE_LOG(LogTemp, Warning, TEXT("DroneServer::SetStereoCameraConfig"));

	if(!DronePawn) {
		return false;
	}

	FStereoCameraConfig Config;
	Config.ShowCameraComponent = Request.config.show_debug_camera_;

	Config.Offset = FVector(Request.config.offset_x_, Request.config.offset_y_, Request.config.offset_z_);
	Config.Orientation = FRotator(Request.config.rotation_pitch_, Request.config.rotation_yaw_, Request.config.rotation_roll_);

	Config.FOVAngle = Request.config.fov_;

	Config.Width = Request.config.width_;
	Config.Height = Request.config.height_;

	Config.baseline = Request.config.baseline_;

	Config.enable_hdr = Request.config.enable_hdr_;
	Config.enable_temporal_aa = Request.config.enable_temporal_aa_;
	Config.enable_raytracing = Request.config.enable_raytracing_;

	bool Status = false;

	auto Instruction = std::make_shared<FInstruction<ADronePawn>>();

	Instruction->Function = [&Config, &Status](ADronePawn& _DronePawn) {
		Status = _DronePawn.SetStereoCameraConfig(Config);
	};

	DronePawn->InstructionQueue->Enqueue(Instruction);
	FGenericPlatformProcess::ConditionalSleep([Instruction](){return Instruction->Finished;});

	std::stringstream OutputStream;
	Serializable::Drone::SetStereoCameraConfig::Response Response(Status);
	Serialization::DeserializeResponse(Response, OutputStream);
	return Respond(Client, OutputStream);
}

//}

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
