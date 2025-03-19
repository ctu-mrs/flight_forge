#include "UedsGameModeServer.h"

#include "Serialize.h"
#include "../uedsGameModeBase.h"

/* route //{ */

bool UedsGameModeServer::Route(const FTCPClient& Client, std::shared_ptr<std::stringstream> InputStream) {

  if (TcpServer::Route(Client, InputStream)) {
    return true;
  }

  Serializable::Common::NetworkRequest Request;
  Serialization::SerializeRequest<Serializable::Common::NetworkRequest>(Request, *InputStream);

  if (Request.type == Serializable::GameMode::MessageType::get_drones) {
    Serializable::GameMode::GetDrones::Request CustomRequest;
    Serialization::SerializeRequest<Serializable::GameMode::GetDrones::Request>(CustomRequest, *InputStream);
    return GetDrones(Client, CustomRequest);
  }

  if (Request.type == Serializable::GameMode::MessageType::spawn_drone) {
    Serializable::GameMode::SpawnDrone::Request CustomRequest;
    Serialization::SerializeRequest<Serializable::GameMode::SpawnDrone::Request>(CustomRequest, *InputStream);
    return SpawnDrone(Client, CustomRequest);
  }
  
  if (Request.type == Serializable::GameMode::MessageType::remove_drone) {
    Serializable::GameMode::RemoveDrone::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return RemoveDrone(Client, CustomRequest);
  }
  
  if (Request.type == Serializable::GameMode::MessageType::get_camera_capture_mode) {
    Serializable::GameMode::GetCameraCaptureMode::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return GetCameraCaptureMode(Client, CustomRequest);
  }
  
  if (Request.type == Serializable::GameMode::MessageType::set_camera_capture_mode) {
    Serializable::GameMode::SetCameraCaptureMode::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return SetCameraCaptureMode(Client, CustomRequest);
  }
  
  if (Request.type == Serializable::GameMode::MessageType::get_fps) {
    Serializable::GameMode::GetFps::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return GetFPS(Client, CustomRequest);
  }
  
  if (Request.type == Serializable::GameMode::MessageType::get_api_version) {
    Serializable::GameMode::GetApiVersion::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return GetApiVersion(Client, CustomRequest);
  }
  
  if (Request.type == Serializable::GameMode::MessageType::get_time) {
    Serializable::GameMode::GetTime::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return GetTime(Client, CustomRequest);
  }
  
  if (Request.type == Serializable::GameMode::MessageType::set_graphics_settings)
  {
    Serializable::GameMode::SetGraphicsSettings::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return SetGraphicsSettings(Client, CustomRequest);
  }
  
  if (Request.type == Serializable::GameMode::MessageType::switch_world_level)
  {
    Serializable::GameMode::SwitchWorldLevel::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return SwitchWorldLevel(Client, CustomRequest);
  }
  
  if (Request.type == Serializable::GameMode::MessageType::set_forest_density)
  {
    Serializable::GameMode::SetForestDensity::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return SetForestDensity(Client, CustomRequest);
  }
  
  if (Request.type == Serializable::GameMode::MessageType::set_forest_hilly_level)
  {
    Serializable::GameMode::SetForestHillyLevel::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return SetForestHillyLevel(Client, CustomRequest);
  }
  
  if (Request.type == Serializable::GameMode::MessageType::get_world_origin)
  {
    Serializable::GameMode::GetWorldOrigin::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return GetWorldOrigin(Client, CustomRequest);
  }
  
  if (Request.type == Serializable::GameMode::MessageType::spawn_drone_at_location)
  {
    Serializable::GameMode::SpawnDroneAtLocation::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return SpawnDroneAtLocation(Client, CustomRequest);
  }
  
  if (Request.type == Serializable::GameMode::MessageType::set_weather)
  {
    Serializable::GameMode::SetWeather::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return SetWeather(Client, CustomRequest);
  }
  
  if (Request.type == Serializable::GameMode::MessageType::set_daytime)
  {
    Serializable::GameMode::SetDaytime::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return SetDatetime(Client, CustomRequest);
  }

  if(Request.type == Serializable::GameMode::MessageType::set_mutual_visibility)
  {
    Serializable::GameMode::SetMutualVisibility::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return SetMutualDroneVisibility(Client, CustomRequest);
  }

  return false;
}

//}

/* getDrones() //{ */

bool UedsGameModeServer::GetDrones(const FTCPClient& Client, Serializable::GameMode::GetDrones::Request& Request) {

  if (!GameMode) {
    return false;
  }

  auto Response  = Serializable::GameMode::GetDrones::Response(true);
  Response.ports = std::vector<int>();
  GameMode->GetDronePorts(Response.ports);

  std::stringstream OutputStream;
  Serialization::DeserializeResponse<Serializable::GameMode::GetDrones::Response>(Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}

/* SetForestDensity() //{ */

bool UedsGameModeServer::SetForestDensity(const FTCPClient& Client, Serializable::GameMode::SetForestDensity::Request& Request) {

  if (!GameMode) {
    return false;
  }

  auto Response  = Serializable::GameMode::SetForestDensity::Response(true);

  //UE_LOG(LogTemp, Error, TEXT("Want to set DensityLevel to %d"), Request.Density_Level);
  GameMode->ForestDensityLevel = Request.Density_Level;

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

bool UedsGameModeServer::SetForestHillyLevel(const FTCPClient& Client, Serializable::GameMode::SetForestHillyLevel::Request& Request) {

  if (!GameMode) {
    return false;
  }

  auto Response  = Serializable::GameMode::SetForestHillyLevel::Response(true);
  
  GameMode->ForestHillyLevel = Request.Hilly_Level;

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

bool UedsGameModeServer::GetWorldOrigin(const FTCPClient& Client,
  Serializable::GameMode::GetWorldOrigin::Request& Request)
{
  if(!GameMode) {
    return false;
  }

  FVector Location = FVector(0, 0, 0);
  
  auto Instruction      = std::make_shared<FInstruction<AuedsGameModeBase>>();
  Instruction->Function = [&Location](AuedsGameModeBase& _GameMode) { Location = _GameMode.GetWorldOrigin(); };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  auto Response = Serializable::GameMode::GetWorldOrigin::FVectorToResponse(Location);
  
  std::stringstream OutputStream;
  Serialization::DeserializeResponse(*Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}

/* spawnDrone() //{ */

bool UedsGameModeServer::SpawnDrone(const FTCPClient& Client, Serializable::GameMode::SpawnDrone::Request& Request) {

  if (!GameMode) {
    return false;
  }

  int PawnPort = -1;

  auto Instruction      = std::make_shared<FInstruction<AuedsGameModeBase>>();
  Instruction->Function = [&PawnPort](AuedsGameModeBase& _GameMode) { PawnPort = _GameMode.SpawnDrone(); };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  auto Response = Serializable::GameMode::SpawnDrone::Response(PawnPort >= 0);
  Response.port = PawnPort;

  std::stringstream OutputStream;
  Serialization::DeserializeResponse<Serializable::GameMode::SpawnDrone::Response>(Response, OutputStream);

  return Respond(Client, OutputStream);
}

bool UedsGameModeServer::SpawnDroneAtLocation(const FTCPClient& Client,
  Serializable::GameMode::SpawnDroneAtLocation::Request& Request)
{
  if (!GameMode) {
    return false;
  }

  int PawnPort = -1;

  auto Instruction      = std::make_shared<FInstruction<AuedsGameModeBase>>();
  Instruction->Function = [&PawnPort, &Request](AuedsGameModeBase& _GameMode) { PawnPort = _GameMode.SpawnDroneAtLocation(FVector(Request.x,Request.y, Request.z), Request.idMesh); };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  auto Response = Serializable::GameMode::SpawnDrone::Response(PawnPort >= 0);
  Response.port = PawnPort;

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);  
}

//}

/* removeDrone() //{ */

bool UedsGameModeServer::RemoveDrone(const FTCPClient& Client, Serializable::GameMode::RemoveDrone::Request& Request) {

  if (!GameMode) {
    return false;
  }

  auto Response    = Serializable::GameMode::RemoveDrone::Response();
  auto Instruction = std::make_shared<FInstruction<AuedsGameModeBase>>();

  Instruction->Function = [&Request, &Response](AuedsGameModeBase& _GameMode) { Response.status = _GameMode.RemoveDrone(Request.port); };
  GameMode->InstructionQueue->Enqueue(Instruction);

  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}

/* getCameraCaptureMode() //{ */

bool UedsGameModeServer::GetCameraCaptureMode(const FTCPClient& Client, Serializable::GameMode::GetCameraCaptureMode::Request& Request) {

  if (!GameMode) {
    return false;
  }

  auto Response              = Serializable::GameMode::GetCameraCaptureMode::Response(true);
  Response.cameraCaptureMode = GameMode->GetCaptureMode();

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}

/* setCameraCaptureMode() //{ */

bool UedsGameModeServer::SetCameraCaptureMode(const FTCPClient& Client, Serializable::GameMode::SetCameraCaptureMode::Request& Request) {

  if (!GameMode) {
    return false;
  }

  auto Response    = Serializable::GameMode::SetCameraCaptureMode::Response();
  auto Instruction = std::make_shared<FInstruction<AuedsGameModeBase>>();

  Instruction->Function = [&Request, &Response](AuedsGameModeBase& _GameMode) { Response.status = _GameMode.SetCameraCaptureMode(Request.cameraCaptureMode); };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}

/* getFPS() //{ */

bool UedsGameModeServer::GetFPS(const FTCPClient& Client, Serializable::GameMode::GetFps::Request& Request) {

  if (!GameMode) {
    return false;
  }

  auto Response = Serializable::GameMode::GetFps::Response(true);
  Response.fps  = GameMode->GetFPS();

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}

/* getApiVersion() //{ */

bool UedsGameModeServer::GetApiVersion(const FTCPClient& Client, Serializable::GameMode::GetApiVersion::Request& Request) {

  if (!GameMode) {
    return false;
  }

  auto Response = Serializable::GameMode::GetApiVersion::Response(true);
  Response.api_version_major  = API_VERSION_MAJOR;
  Response.api_version_minor  = API_VERSION_MINOR;

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
  
}

//}

/* getTime() //{ */

bool UedsGameModeServer::GetTime(const FTCPClient& Client, Serializable::GameMode::GetTime::Request& Request) {

  if (!GameMode) {
    return false;
  }

	// UE_LOG(LogTemp, Warning, TEXT("UedsGameModeServer::GetTime"));

  auto Response = Serializable::GameMode::GetTime::Response(true);
  Response.time = FPlatformTime::Seconds();

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

bool UedsGameModeServer::SetGraphicsSettings(const FTCPClient& Client,
  Serializable::GameMode::SetGraphicsSettings::Request& Request)
{
  if (!GameMode) {
    return false;
  }

  auto Response    = Serializable::GameMode::SetGraphicsSettings::Response();
  auto Instruction = std::make_shared<FInstruction<AuedsGameModeBase>>();

  Instruction->Function = [&Request, &Response](AuedsGameModeBase& _GameMode) { Response.status = _GameMode.SetGraphicsSettings(Request.graphicsSettings); };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream); 
}

bool UedsGameModeServer::SwitchWorldLevel(const FTCPClient& Client,
  Serializable::GameMode::SwitchWorldLevel::Request& Request)
{
  if (!GameMode) {
    return false;
  }

  auto Response    = Serializable::GameMode::SwitchWorldLevel::Response();
  auto Instruction = std::make_shared<FInstruction<AuedsGameModeBase>>();
  
  Response.status = true;
  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);
  const bool result = Respond(Client, OutputStream);

  Instruction->Function = [&Request, &Response](AuedsGameModeBase& _GameMode) { _GameMode.SwitchWorldLevel(Request.worldLevelEnum); };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });


  //return Respond(Client, OutputStream);
  return result;
}

bool UedsGameModeServer::SetWeather(const FTCPClient& Client, Serializable::GameMode::SetWeather::Request& Request)
{
  if (!GameMode) {
    return false;
  }

  auto Response    = Serializable::GameMode::SetWeather::Response();
  auto Instruction = std::make_shared<FInstruction<AuedsGameModeBase>>();

  Instruction->Function = [&Request, &Response](AuedsGameModeBase& _GameMode) { Response.status = _GameMode.SetWeather(Request.weather_id); };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream); 
}

bool UedsGameModeServer::SetDatetime(const FTCPClient& Client, Serializable::GameMode::SetDaytime::Request& Request)
{
  if (!GameMode) {
    return false;
  }

  auto Response    = Serializable::GameMode::SetWeather::Response();
  auto Instruction = std::make_shared<FInstruction<AuedsGameModeBase>>();

  Instruction->Function = [&Request, &Response](AuedsGameModeBase& _GameMode) { Response.status = _GameMode.SetDaytime(Request.hour, Request.minute); };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream); 
}

bool UedsGameModeServer::SetMutualDroneVisibility(const FTCPClient& Client,
  Serializable::GameMode::SetMutualVisibility::Request& Request)
{
  if (!GameMode) {
    return false;
  }

  auto Response    = Serializable::GameMode::SetMutualVisibility::Response(true);
  auto Instruction = std::make_shared<FInstruction<AuedsGameModeBase>>();

  Instruction->Function = [&Request, &Response](AuedsGameModeBase& _GameMode) { _GameMode.SetMutualVisibility(Request.mutual_visibiliti_enabled); };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream); 
}

//}
