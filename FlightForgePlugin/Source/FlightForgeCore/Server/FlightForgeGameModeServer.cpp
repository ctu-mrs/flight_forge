#include "FlightForgeGameModeServer.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/WorldSettings.h"

#include "Serialize.h"
#include "../FlightForgeGameModeBase.h"
#include "../Utils/DistanceFieldQuery.h"
#include "../Utils/WorldMeshExtractor.h"
#include "../Segmentation/SegmentationTagger.h"

/* route //{ */

bool FlightForgeGameModeServer::Route(const FTCPClient& Client, std::shared_ptr<std::stringstream> InputStream) {

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

  if(Request.type == Serializable::GameMode::MessageType::spawn_objects_from_yaml)
  {
    Serializable::GameMode::SpawnObjectsFromYaml::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return SpawnObjectsFromYaml(Client, CustomRequest);
  }

  if(Request.type == Serializable::GameMode::MessageType::remove_spawned_object)
  {
    Serializable::GameMode::RemoveSpawnedObject::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return RemoveSpawnedObject(Client, CustomRequest);
  }

  if(Request.type == Serializable::GameMode::MessageType::move_spawned_object)
  {
    Serializable::GameMode::MoveSpawnedObject::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return MoveSpawnedObject(Client, CustomRequest);
  }

  if(Request.type == Serializable::GameMode::MessageType::list_spawned_objects)
  {
    Serializable::GameMode::ListSpawnedObjects::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return ListSpawnedObjects(Client, CustomRequest);
  }

  if(Request.type == Serializable::GameMode::MessageType::export_scene)
  {
    Serializable::GameMode::ExportScene::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return ExportScene(Client, CustomRequest);
  }

  if(Request.type == Serializable::GameMode::MessageType::list_assets)
  {
    Serializable::GameMode::ListAssets::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return ListAssets(Client, CustomRequest);
  }

  if(Request.type == Serializable::GameMode::MessageType::get_world_mesh)
  {
    Serializable::GameMode::GetWorldMesh::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return GetWorldMesh(Client, CustomRequest);
  }

  if(Request.type == Serializable::GameMode::MessageType::set_time_dilation)
  {
    Serializable::GameMode::SetTimeDilation::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return SetTimeDilation(Client, CustomRequest);
  }

  if(Request.type == Serializable::GameMode::MessageType::set_fixed_timestep)
  {
    Serializable::GameMode::SetFixedTimestep::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return SetFixedTimestep(Client, CustomRequest);
  }

  if(Request.type == Serializable::GameMode::MessageType::set_lockstep)
  {
    Serializable::GameMode::SetLockstep::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return SetLockstep(Client, CustomRequest);
  }

  if(Request.type == Serializable::GameMode::MessageType::step)
  {
    Serializable::GameMode::Step::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return Step(Client, CustomRequest);
  }

  if(Request.type == Serializable::GameMode::MessageType::get_class_map)
  {
    Serializable::GameMode::GetClassMap::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return GetClassMap(Client, CustomRequest);
  }

  if(Request.type == Serializable::GameMode::MessageType::get_distance_batch)
  {
    Serializable::GameMode::GetDistanceBatch::Request CustomRequest;
    Serialization::SerializeRequest(CustomRequest, *InputStream);
    return GetDistanceBatch(Client, CustomRequest);
  }

  return false;
}


void FlightForgeGameModeServer::SetCurrentGameMode(AFlightForgeGameModeBase* NewGameMode)
{
  std::lock_guard<std::mutex> Lock(GameModeMutex);
  GameMode = NewGameMode;
}


bool FlightForgeGameModeServer::GetDrones(const FTCPClient& Client, Serializable::GameMode::GetDrones::Request& Request) {

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

bool FlightForgeGameModeServer::SetForestDensity(const FTCPClient& Client, Serializable::GameMode::SetForestDensity::Request& Request) {

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

bool FlightForgeGameModeServer::SetForestHillyLevel(const FTCPClient& Client, Serializable::GameMode::SetForestHillyLevel::Request& Request) {

  if (!GameMode) {
    return false;
  }

  auto Response  = Serializable::GameMode::SetForestHillyLevel::Response(true);
  
  GameMode->ForestHillyLevel = Request.Hilly_Level;

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

bool FlightForgeGameModeServer::GetWorldOrigin(const FTCPClient& Client,
  Serializable::GameMode::GetWorldOrigin::Request& Request)
{
  if(!GameMode) {
    return false;
  }

  FVector Location = FVector(0, 0, 0);
  
  auto Instruction      = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();
  Instruction->Function = [&Location](AFlightForgeGameModeBase& _GameMode) { Location = _GameMode.GetWorldOrigin(); };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  auto Response = Serializable::GameMode::GetWorldOrigin::FVectorToResponse(Location);
  
  std::stringstream OutputStream;
  Serialization::DeserializeResponse(*Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}

/* spawnDrone() //{ */

bool FlightForgeGameModeServer::SpawnDrone(const FTCPClient& Client, Serializable::GameMode::SpawnDrone::Request& Request) {

  if (!GameMode) {
    return false;
  }

  int PawnPort = -1;

  auto Instruction      = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();
  Instruction->Function = [&PawnPort](AFlightForgeGameModeBase& _GameMode) { PawnPort = _GameMode.SpawnDrone(); };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  auto Response = Serializable::GameMode::SpawnDrone::Response(PawnPort >= 0);
  Response.port = PawnPort;
  UE_LOG(LogTemp, Warning, TEXT("Pawn port: %d"), PawnPort);
  
  std::stringstream OutputStream;
  Serialization::DeserializeResponse<Serializable::GameMode::SpawnDrone::Response>(Response, OutputStream);

  return Respond(Client, OutputStream);
}

bool FlightForgeGameModeServer::SpawnDroneAtLocation(const FTCPClient& Client,
  Serializable::GameMode::SpawnDroneAtLocation::Request& Request)
{
  if (!GameMode) {
    return false;
  }

  int PawnPort = -1;

  auto Instruction      = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();
  Instruction->Function = [&PawnPort, &Request](AFlightForgeGameModeBase& _GameMode) { PawnPort = _GameMode.SpawnDroneAtLocation(FVector(Request.x,Request.y, Request.z), Request.MeshName); };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  auto Response = Serializable::GameMode::SpawnDroneAtLocation::Response(PawnPort >= 0);
  Response.port = PawnPort;

  std::stringstream OutputStream;
  Serialization::DeserializeResponse<Serializable::GameMode::SpawnDroneAtLocation::Response>(Response, OutputStream);

  return Respond(Client, OutputStream);  
}

//}

/* removeDrone() //{ */

bool FlightForgeGameModeServer::RemoveDrone(const FTCPClient& Client, Serializable::GameMode::RemoveDrone::Request& Request) {

  if (!GameMode) {
    return false;
  }

  auto Response    = Serializable::GameMode::RemoveDrone::Response();
  auto Instruction = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();

  Instruction->Function = [&Request, &Response](AFlightForgeGameModeBase& _GameMode) { Response.status = _GameMode.RemoveDrone(Request.port); };
  GameMode->InstructionQueue->Enqueue(Instruction);

  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}

/* getCameraCaptureMode() //{ */

bool FlightForgeGameModeServer::GetCameraCaptureMode(const FTCPClient& Client, Serializable::GameMode::GetCameraCaptureMode::Request& Request) {

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

bool FlightForgeGameModeServer::SetCameraCaptureMode(const FTCPClient& Client, Serializable::GameMode::SetCameraCaptureMode::Request& Request) {

  if (!GameMode) {
    return false;
  }

  auto Response    = Serializable::GameMode::SetCameraCaptureMode::Response();
  auto Instruction = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();

  Instruction->Function = [&Request, &Response](AFlightForgeGameModeBase& _GameMode) { Response.status = _GameMode.SetCameraCaptureMode(Request.cameraCaptureMode); };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}

/* getFPS() //{ */

/* getClassMap() //{ */

bool FlightForgeGameModeServer::GetClassMap(const FTCPClient& Client, Serializable::GameMode::GetClassMap::Request& Request) {

  if (!GameMode) {
    return false;
  }

  auto Response = Serializable::GameMode::GetClassMap::Response(true);

  const UWorld*              World  = GameMode->GetWorld();
  const USegmentationTagger* Tagger = World ? World->GetSubsystem<USegmentationTagger>() : nullptr;
  if (Tagger) {
    TArray<int32> Ids;
    Tagger->Classes().GetKeys(Ids);
    Ids.Sort();
    for (const int32 Id : Ids) {
      const FSegClass&                                Class = Tagger->Classes()[Id];
      Serializable::GameMode::GetClassMap::ClassEntry Entry;
      Entry.id        = Class.Id;
      Entry.parent_id = Class.ParentId;
      Entry.path      = TCHAR_TO_UTF8(*Class.Path);
      Response.classes_.push_back(Entry);
    }
    Response.hash_ = Tagger->ClassMapHash();
  }

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}

bool FlightForgeGameModeServer::GetFPS(const FTCPClient& Client, Serializable::GameMode::GetFps::Request& Request) {

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

bool FlightForgeGameModeServer::GetApiVersion(const FTCPClient& Client, Serializable::GameMode::GetApiVersion::Request& Request) {

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

bool FlightForgeGameModeServer::GetTime(const FTCPClient& Client, Serializable::GameMode::GetTime::Request& Request) {

  if (!GameMode) {
    return false;
  }

	// UE_LOG(LogTemp, Warning, TEXT("FlightForgeGameModeServer::GetTime"));

  auto Response = Serializable::GameMode::GetTime::Response(true);
  Response.time = FPlatformTime::Seconds();

  if (const UWorld* World = GameMode->GetWorld()) {
    Response.sim_time      = World->GetTimeSeconds();
    Response.time_dilation = World->GetWorldSettings() ? World->GetWorldSettings()->GetEffectiveTimeDilation() : 1.0;
  }

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

/* setTimeDilation() //{ */

bool FlightForgeGameModeServer::SetTimeDilation(const FTCPClient& Client, Serializable::GameMode::SetTimeDilation::Request& Request) {

  if (!GameMode) {
    return false;
  }

  double Applied = 1.0;
  bool   bOk     = false;

  auto Instruction = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();

  Instruction->Function = [&Applied, &bOk, &Request](AFlightForgeGameModeBase& _GameMode) {
    UWorld* World = _GameMode.GetWorld();
    if (!World || Request.dilation <= 0.0) {
      return;
    }

    UGameplayStatics::SetGlobalTimeDilation(World, static_cast<float>(Request.dilation));

    // the engine clamps to the world's Min/MaxGlobalTimeDilation, so the caller is told
    // what it actually got rather than what it asked for
    Applied = World->GetWorldSettings() ? World->GetWorldSettings()->GetEffectiveTimeDilation() : 1.0;
    bOk     = true;

    UE_LOG(LogTemp, Log, TEXT("FlightForge: time dilation requested %.4f, applied %.4f"), Request.dilation, Applied);
  };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  auto Response    = Serializable::GameMode::SetTimeDilation::Response(bOk);
  Response.applied = Applied;

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}

bool FlightForgeGameModeServer::SetGraphicsSettings(const FTCPClient& Client,
  Serializable::GameMode::SetGraphicsSettings::Request& Request)
{
  if (!GameMode) {
    return false;
  }

  auto Response    = Serializable::GameMode::SetGraphicsSettings::Response();
  auto Instruction = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();

  Instruction->Function = [&Request, &Response](AFlightForgeGameModeBase& _GameMode) { Response.status = _GameMode.SetGraphicsSettings(Request.graphicsSettings); };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream); 
}

bool FlightForgeGameModeServer::SwitchWorldLevel(const FTCPClient& Client,
  Serializable::GameMode::SwitchWorldLevel::Request& Request)
{
  if (!GameMode) {
    return false;
  }

  auto Response    = Serializable::GameMode::SwitchWorldLevel::Response();
  auto Instruction = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();
  
  Response.status = true;
  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);
  const bool result = Respond(Client, OutputStream);

  Instruction->Function = [&Request, &Response](AFlightForgeGameModeBase& _GameMode) { _GameMode.SwitchWorldLevel(Request.worldLevelName); };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });


  //return Respond(Client, OutputStream);
  return result;
}

bool FlightForgeGameModeServer::SetWeather(const FTCPClient& Client, Serializable::GameMode::SetWeather::Request& Request)
{
  if (!GameMode) {
    return false;
  }

  auto Response    = Serializable::GameMode::SetWeather::Response();
  auto Instruction = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();

  Instruction->Function = [&Request, &Response](AFlightForgeGameModeBase& _GameMode) { Response.status = _GameMode.SetWeather(Request.weather_id); };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream); 
}

bool FlightForgeGameModeServer::SetDatetime(const FTCPClient& Client, Serializable::GameMode::SetDaytime::Request& Request)
{
  if (!GameMode) {
    return false;
  }

  auto Response    = Serializable::GameMode::SetWeather::Response();
  auto Instruction = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();

  Instruction->Function = [&Request, &Response](AFlightForgeGameModeBase& _GameMode) { Response.status = _GameMode.SetDaytime(Request.hour, Request.minute); };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream); 
}

bool FlightForgeGameModeServer::SetMutualDroneVisibility(const FTCPClient& Client,
  Serializable::GameMode::SetMutualVisibility::Request& Request)
{
  if (!GameMode) {
    return false;
  }

  auto Response    = Serializable::GameMode::SetMutualVisibility::Response(true);
  auto Instruction = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();

  Instruction->Function = [&Request, &Response](AFlightForgeGameModeBase& _GameMode) { _GameMode.SetMutualVisibility(Request.mutual_visibiliti_enabled); };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}

/* spawnObjectsFromYaml() //{ */

bool FlightForgeGameModeServer::SpawnObjectsFromYaml(const FTCPClient& Client, Serializable::GameMode::SpawnObjectsFromYaml::Request& Request)
{
  if (!GameMode) {
    return false;
  }

  TArray<int32> SpawnedIds;
  FString       Error;

  auto Instruction = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();

  Instruction->Function = [&Request, &SpawnedIds, &Error](AFlightForgeGameModeBase& _GameMode) {
    SpawnedIds = _GameMode.SpawnObjectsFromYaml(Request.yaml, Error);
  };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  // An empty result is only a failure when the configuration itself could not be read;
  // a config that placed nothing still succeeded.
  auto Response = Serializable::GameMode::SpawnObjectsFromYaml::Response(Error.IsEmpty());

  for (const int32 SpawnedId : SpawnedIds) {
    Response.object_ids.push_back(SpawnedId);
  }
  Response.error = TCHAR_TO_UTF8(*Error);

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}

/* removeSpawnedObject() //{ */

bool FlightForgeGameModeServer::RemoveSpawnedObject(const FTCPClient& Client, Serializable::GameMode::RemoveSpawnedObject::Request& Request)
{
  if (!GameMode) {
    return false;
  }

  int32 RemovedCount = 0;

  auto Instruction = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();

  Instruction->Function = [&Request, &RemovedCount](AFlightForgeGameModeBase& _GameMode) {
    RemovedCount = Request.object_id < 0 ? _GameMode.RemoveAllSpawnedObjects() : (_GameMode.RemoveSpawnedObject(Request.object_id) ? 1 : 0);
  };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  // Clearing everything is idempotent: an already-empty scene is the requested state, not a
  // failure. Removing one specific id still fails when that id is not there.
  const bool bRemoved = Request.object_id < 0 ? true : RemovedCount > 0;

  auto Response          = Serializable::GameMode::RemoveSpawnedObject::Response(bRemoved);
  Response.removed_count = RemovedCount;

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}

/* moveSpawnedObject() //{ */

bool FlightForgeGameModeServer::MoveSpawnedObject(const FTCPClient& Client, Serializable::GameMode::MoveSpawnedObject::Request& Request)
{
  if (!GameMode) {
    return false;
  }

  bool bMoved = false;

  auto Instruction = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();

  Instruction->Function = [&Request, &bMoved](AFlightForgeGameModeBase& _GameMode) {
    const FVector Position(Request.position[0], Request.position[1], Request.position[2]);
    const FVector Orientation(Request.orientation[0], Request.orientation[1], Request.orientation[2]);
    const FVector Scale(Request.scale[0], Request.scale[1], Request.scale[2]);

    bMoved = _GameMode.MoveSpawnedObject(Request.object_id, Position, Orientation, Scale);
  };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  auto Response = Serializable::GameMode::MoveSpawnedObject::Response(bMoved);

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}

/* listSpawnedObjects() //{ */

bool FlightForgeGameModeServer::ListSpawnedObjects(const FTCPClient& Client, Serializable::GameMode::ListSpawnedObjects::Request& Request)
{
  if (!GameMode) {
    return false;
  }

  TArray<FSpawnedObjectState> States;

  auto Instruction = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();

  Instruction->Function = [&States](AFlightForgeGameModeBase& _GameMode) { States = _GameMode.ListSpawnedObjects(); };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  auto Response = Serializable::GameMode::ListSpawnedObjects::Response(true);

  for (const FSpawnedObjectState& State : States) {
    Serializable::GameMode::SpawnedObjectInfo Info;
    Info.id          = State.Id;
    Info.asset       = TCHAR_TO_UTF8(*State.Asset);
    Info.stencil     = State.Stencil;
    Info.position    = {State.Position.X, State.Position.Y, State.Position.Z};
    Info.orientation = {State.Orientation.X, State.Orientation.Y, State.Orientation.Z};
    Info.scale       = {State.Scale.X, State.Scale.Y, State.Scale.Z};

    Response.objects.push_back(Info);
  }

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}

/* exportScene() //{ */

bool FlightForgeGameModeServer::ExportScene(const FTCPClient& Client, Serializable::GameMode::ExportScene::Request& Request)
{
  if (!GameMode) {
    return false;
  }

  FString Yaml;

  auto Instruction = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();

  Instruction->Function = [&Yaml](AFlightForgeGameModeBase& _GameMode) { Yaml = _GameMode.ExportSceneYaml(); };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  auto Response = Serializable::GameMode::ExportScene::Response(true);
  Response.yaml = TCHAR_TO_UTF8(*Yaml);

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}

/* getWorldMesh() //{ */

bool FlightForgeGameModeServer::GetWorldMesh(const FTCPClient& Client, Serializable::GameMode::GetWorldMesh::Request& Request)
{
  if (!GameMode) {
    return false;
  }

  constexpr int32 DefaultMaxTriangles = 2000000;

  FBox SceneBounds(EForceInit::ForceInit);
  if (Request.extent_x_ > 0.0 && Request.extent_y_ > 0.0 && Request.extent_z_ > 0.0) {
    const FVector Center(Request.center_x_, Request.center_y_, Request.center_z_);
    const FVector Extent(Request.extent_x_, Request.extent_y_, Request.extent_z_);
    SceneBounds += Center - Extent;
    SceneBounds += Center + Extent;
  }

  const int32 MaxTriangles = Request.max_triangles_ > 0 ? static_cast<int32>(Request.max_triangles_) : DefaultMaxTriangles;

  FWorldMeshResult Mesh;
  bool             bOk = false;

  auto Instruction = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();

  Instruction->Function = [&Mesh, &bOk, &SceneBounds, MaxTriangles](AFlightForgeGameModeBase& _GameMode) {
    bOk = FWorldMeshExtractor::Extract(_GameMode.GetWorld(), _GameMode.GetWorldOrigin(), SceneBounds, MaxTriangles, Mesh);
  };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  auto Response = Serializable::GameMode::GetWorldMesh::Response(bOk);

  if (bOk) {
    Response.vertices_.assign(Mesh.Vertices.GetData(), Mesh.Vertices.GetData() + Mesh.Vertices.Num());
    Response.indices_.assign(Mesh.Indices.GetData(), Mesh.Indices.GetData() + Mesh.Indices.Num());
    Response.truncated_    = Mesh.bTruncated;
    Response.source_count_ = Mesh.SourceCount;
  }

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}

/* getDistanceBatch() //{ */

bool FlightForgeGameModeServer::GetDistanceBatch(const FTCPClient& Client, Serializable::GameMode::GetDistanceBatch::Request& Request)
{
  if (!GameMode) {
    return false;
  }

  TArray<float> Points;
  Points.SetNumUninitialized(Request.query_points_.size());
  FMemory::Memcpy(Points.GetData(), Request.query_points_.data(), Request.query_points_.size() * sizeof(float));

  TArray<float> Distances;
  bool          bOk = false;

  auto Instruction = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();

  Instruction->Function = [&Points, &Distances, &bOk, &Request](AFlightForgeGameModeBase& _GameMode) {
    bOk = FDistanceFieldQuery::Batch(_GameMode.GetWorld(), _GameMode.GetWorldOrigin(), Points, Request.max_radius_, Request.tolerance_,
                                     Request.terrain_only_, Distances);
  };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  auto Response = Serializable::GameMode::GetDistanceBatch::Response(bOk);

  if (bOk) {
    Response.distances_.assign(Distances.GetData(), Distances.GetData() + Distances.Num());
  }

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}

/* listAssets() //{ */

bool FlightForgeGameModeServer::ListAssets(const FTCPClient& Client, Serializable::GameMode::ListAssets::Request& Request)
{
  if (!GameMode) {
    return false;
  }

  constexpr int32 MaxResults = 5000;

  TArray<FString> Names;
  bool            bTruncated = false;

  auto Instruction = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();

  Instruction->Function = [&Request, &Names, &bTruncated](AFlightForgeGameModeBase& _GameMode) {
    Names = _GameMode.ListSpawnableAssets(FString(UTF8_TO_TCHAR(Request.path_prefix.c_str())), FString(UTF8_TO_TCHAR(Request.name_filter.c_str())),
                                          MaxResults, bTruncated);
  };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  auto Response = Serializable::GameMode::ListAssets::Response(true);
  Response.truncated = bTruncated;

  for (const FString& Name : Names) {
    Response.assets.push_back(TCHAR_TO_UTF8(*Name));
  }

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}

/* setFixedTimestep() //{ */

bool FlightForgeGameModeServer::SetFixedTimestep(const FTCPClient& Client, Serializable::GameMode::SetFixedTimestep::Request& Request) {

  if (!GameMode) {
    return false;
  }

  bool   bOk     = false;
  double Applied = 0.0;

  auto Instruction = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();

  Instruction->Function = [&bOk, &Applied, &Request](AFlightForgeGameModeBase& _GameMode) {
    bOk     = _GameMode.SetFixedTimestep(Request.dt, Request.adaptive_min_dt);
    Applied = _GameMode.GetFixedTimestep();
  };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  auto Response    = Serializable::GameMode::SetFixedTimestep::Response(bOk);
  Response.applied = Applied;

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}

/* setLockstep() //{ */

bool FlightForgeGameModeServer::SetLockstep(const FTCPClient& Client, Serializable::GameMode::SetLockstep::Request& Request) {

  if (!GameMode) {
    return false;
  }

  bool bOk = false;

  auto Instruction = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();

  Instruction->Function = [&bOk, &Request](AFlightForgeGameModeBase& _GameMode) { bOk = _GameMode.SetLockstep(Request.enabled); };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  auto Response = Serializable::GameMode::SetLockstep::Response(bOk);

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}

/* step() //{ */

bool FlightForgeGameModeServer::Step(const FTCPClient& Client, Serializable::GameMode::Step::Request& Request) {

  if (!GameMode) {
    return false;
  }

  bool bOk = false;

  auto Instruction = std::make_shared<FInstruction<AFlightForgeGameModeBase>>();

  Instruction->Function = [&bOk, &Request](AFlightForgeGameModeBase& _GameMode) { bOk = _GameMode.RequestStep(Request.ticks); };
  GameMode->InstructionQueue->Enqueue(Instruction);
  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  // a step not done within a minute means the world went away underneath it
  if (bOk) {
    const double Deadline = FPlatformTime::Seconds() + 60.0;
    while (!GameMode->IsStepDone() && FPlatformTime::Seconds() < Deadline) {
      FPlatformProcess::SleepNoStats(0.0002f);
    }
    bOk = GameMode->IsStepDone();
  }

  auto Response       = Serializable::GameMode::Step::Response(bOk);
  Response.sim_time   = GameMode->GetSimTime();
  Response.tick_count = GameMode->GetLockstepTicks();

  std::stringstream OutputStream;
  Serialization::DeserializeResponse(Response, OutputStream);

  return Respond(Client, OutputStream);
}

//}
