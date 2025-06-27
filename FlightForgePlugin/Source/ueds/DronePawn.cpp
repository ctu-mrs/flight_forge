/* includes //{ */

#include "DronePawn.h"

#include "ImageUtils.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Core/Public/Async/ParallelFor.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include <fstream>
#include "PhysicalMaterials/PhysicalMaterial.h"
#include <cereal/details/helpers.hpp>

#include "NaniteSceneProxy.h"
#include "Kismet/BlueprintTypeConversions.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"


// Sets default values
ADronePawn::ADronePawn() {

  // UE_LOG(LogTemp, Warning, TEXT("ADronePawn constructor"));

  PrimaryActorTick.bCanEverTick = true;
  PrimaryActorTick.TickGroup    = TG_PrePhysics;

  InstructionQueue = std::make_unique<TQueue<std::shared_ptr<FInstruction<ADronePawn>>>>();

  RootMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMeshComponent"));

  
  nextSensorID = 0;
  Lidar = CreateDefaultSubobject<ULidar>(TEXT("Lidar"));
  Lidar->Initialize(nextSensorID++);
  Lidar->SetupAttachment(RootMeshComponent);
  
  RangeFinder = CreateDefaultSubobject<URangeFinder>(TEXT("RangeFinder"));
  RangeFinder->Initialize(nextSensorID++);
  RangeFinder->SetupAttachment(RootMeshComponent);
  
  CameraSensor = CreateDefaultSubobject<UCamera>(TEXT("CameraSensor"));
  CameraSensor->Initialize(nextSensorID++);
  CameraSensor->SetupAttachment(RootMeshComponent);
  
  Sensors.Add(Lidar);
  Sensors.Add(RangeFinder);
  Sensors.Add(CameraSensor);

  
  PropellerFrontLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PropellerFrontLeft"));
  PropellerFrontLeft->SetupAttachment(RootMeshComponent);

  PropellerFrontRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PropellerFrontRight"));
  PropellerFrontRight->SetupAttachment(RootMeshComponent);

  PropellerRearLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PropellerRearLeft"));
  PropellerRearLeft->SetupAttachment(RootMeshComponent);

  PropellerRearRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PropellerRearRight"));
  PropellerRearRight->SetupAttachment(RootMeshComponent);

  // X500
  FramePropellersTransforms.Add(FramePropellersTransform(FString(TEXT("x500")), FString(TEXT("x500")),
                                                         FTransform(FRotator(0, 0, 0), FVector(-18.8, -18.8, 6.5), FVector(-0.85, 0.85, -0.8)),
                                                         FTransform(FRotator(0, 0, 0), FVector(-18.8, 18.8, 6.5), FVector(-0.85, -0.85, -0.8)),
                                                         FTransform(FRotator(0, 0, 0), FVector(18.8, -18.8, 6.5), FVector(-0.85, -0.85, -0.8)),
                                                         FTransform(FRotator(0, 0, 0), FVector(18.8, 18.8, 6.5), FVector(-0.85, 0.85, -0.8))));

  // T650
  FramePropellersTransforms.Add(FramePropellersTransform(FString(TEXT("t650")), FString(TEXT("x500")),
                                                         FTransform(FRotator(0, 0, 0), FVector(-26.3, -26.3, 4.6), FVector(-1, 1, -1)),
                                                         FTransform(FRotator(0, 0, 0), FVector(-26.3, 26.3, 4.6), FVector(-1, -1, -1)),
                                                         FTransform(FRotator(0, 0, 0), FVector(26.3, -26.3, 4.6), FVector(-1, -1, -1)),
                                                         FTransform(FRotator(0, 0, 0), FVector(26.3, 26.3, 4.6), FVector(-1, 1, -1))));

  // A300
  FramePropellersTransforms.Add(FramePropellersTransform(
      FString(TEXT("a300")), FString(TEXT("robofly")), FTransform(FRotator(0, 0, 0), FVector(-9.45, -11.7, 2.4), FVector(1, -1, 1)),
      FTransform(FRotator(0, 0, 0), FVector(-9.45, 11.7, 2.4), FVector(1, 1, 1)), FTransform(FRotator(0, 0, 0), FVector(9.45, -11.7, 2.4), FVector(1, 1, 1)),
      FTransform(FRotator(0, 0, 0), FVector(9.45, 11.7, 2.4), FVector(1, -1, 1))));

  // RoboFly
  FramePropellersTransforms.Add(FramePropellersTransform(
      FString(TEXT("robofly")), FString(TEXT("robofly")), FTransform(FRotator(0, 0, 0), FVector(-9.55, -9.55, 1.7), FVector(1, -1, 1)),
      FTransform(FRotator(0, 0, 0), FVector(-9.55, 9.55, 1.7), FVector(1, 1, 1)), FTransform(FRotator(0, 0, 0), FVector(9.55, -9.55, 1.7), FVector(1, 1, 1)),
      FTransform(FRotator(0, 0, 0), FVector(9.55, 9.55, 1.7), FVector(1, -1, 1))));

  // Wings
  FramePropellersTransforms.Add(FramePropellersTransform(
      FString(TEXT("wing")), FString(TEXT("robofly")), FTransform(FRotator(0, 0, 0), FVector(-9.55, -9.55, 1.7), FVector(1, -1, 1)),
      FTransform(FRotator(0, 0, 0), FVector(-9.55, 9.55, 1.7), FVector(1, 1, 1)), FTransform(FRotator(0, 0, 0), FVector(9.55, -9.55, 1.7), FVector(1, 1, 1)),
      FTransform(FRotator(0, 0, 0), FVector(9.55, 9.55, 1.7), FVector(1, -1, 1))));

  FramePropellersTransforms.Add(FramePropellersTransform(
      FString(TEXT("wing_2")), FString(TEXT("robofly")), FTransform(FRotator(0, 0, 0), FVector(-9.55, -9.55, 1.7), FVector(1, -1, 1)),
      FTransform(FRotator(0, 0, 0), FVector(-9.55, 9.55, 1.7), FVector(1, 1, 1)), FTransform(FRotator(0, 0, 0), FVector(9.55, -9.55, 1.7), FVector(1, 1, 1)),
      FTransform(FRotator(0, 0, 0), FVector(9.55, 9.55, 1.7), FVector(1, -1, 1))));

  //Gimbal UAV
  FramePropellersTransforms.Add(FramePropellersTransform(
 FString(TEXT("gimbal")),
 FString(TEXT("robofly")),
   FTransform(FRotator(0, 0, 0), FVector(-13.65, -16.58, -4), FVector(2, -2, 2)),
   FTransform(FRotator(0, 0, 0), FVector(-13.65, 16.58, -4), FVector(2, 2, 2)),
   FTransform(FRotator(0, 0, 0), FVector(13.65, -16.58, -4), FVector(2, 2, 2)),
   FTransform(FRotator(0, 0, 0), FVector(13.65, 16.58, -4), FVector(2, -2, 2))));

  ThirdPersonCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("ThirdPersonCameraSpringArm"));
  ThirdPersonCameraSpringArm->SetupAttachment(RootMeshComponent);
  ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
  ThirdPersonCamera->SetupAttachment(ThirdPersonCameraSpringArm);


  /* CSVFilePath = FString(TEXT("/home/david/mid360.csv")); */
  /* CSVFilePath = FPaths::ProjectContentDir() + TEXT("mid360.csv"); */ 
  /* CSVFilePath = FPaths::ProjectSavedDir() + TEXT("mid360.csv"); */
  /* FString PluginContentDir = FString(TEXT("/Game/../Plugins/flight_forge/FlightForgePlugin/Content/")); */

  /* CSVFilePath = PluginContentDir + TEXT("mid360.csv"); */

    FString PluginName = TEXT("FlightForgePlugin");
    TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(PluginName);
    if (Plugin.IsValid())
    {
        FString PluginContentDir = Plugin->GetContentDir();
        FString CSVRelativePath = TEXT("Lidar/mid360.csv");
        CSVFilePath = FPaths::Combine(PluginContentDir, CSVRelativePath);

    }
    else
    {
      UE_LOG(LogTemp, Error, TEXT("Plugin not found"));
    }

  FTimerHandle OusterTimerHandle;

  droneServer = std::make_shared<DroneServer>(*this);
}

//}

/* beginPlay() //{ */

// Called when the game starts or when spawned
void ADronePawn::BeginPlay() {

  //startTime = FPlatformTime::Seconds();
  
  // CameraSensor = Cast<UCamera>(AddSensor(0));
  // Lidar = Cast<ULidar>(AddSensor(1));
  // RangeFinder = Cast<URangeFinder>(AddSensor(2));
  
  Super::BeginPlay();
}


void ADronePawn::StartServer() {
  // UE_LOG(LogTemp, Warning, TEXT("Starting drone server"));
  droneServer->Run();
}


void ADronePawn::EndPlay(const EEndPlayReason::Type EndPlayReason) {
  droneServer->Stop();
}

// Called to bind functionality to input
void ADronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool ADronePawn::GetCrashState(void) {

  // UE_LOG(LogTemp, Warning, TEXT("ADronePawn::GetCrashState"));

  return uav_crashed_;
}

void ADronePawn::GetRangefinderData(double& range) {

	RangeFinder->GetRangefinderData(range);
}

void ADronePawn::GetLidarHits(std::vector<Serializable::Drone::GetLidarData::LidarData>& OutLidarData, FVector& OutStart) {
	Lidar->GetLidarHits(OutLidarData, OutStart);
}


void ADronePawn::GetSegLidarHits(std::vector<Serializable::Drone::GetLidarSegData::LidarSegData>& OutLidarSegData, FVector& OutStart) {
	Lidar->GetSegLidarHits(OutLidarSegData, OutStart);
}


void ADronePawn::GetIntLidarHits(std::vector<Serializable::Drone::GetLidarIntData::LidarIntData>& OutLidarIntData, FVector& OutStart) {
	Lidar->GetIntLidarHits(OutLidarIntData, OutStart);
}

void ADronePawn::UpdateCamera(bool isExternallyLocked, int type = 1, double stamp = 0.0) {

  CameraSensor->UpdateCamera(isExternallyLocked, type, stamp);
}

void ADronePawn::UpdateCameraSensorsMutualVisibility(TArray<AActor*>& DronesToBeHidden)
{
  for (TObjectPtr<USensor> Sensor : Sensors)
  {
    if (UCamera* CamSensor = Cast<UCamera>(Sensor))
    {
      CamSensor->SceneCaptureComponent2DRgb->HiddenActors.Empty();
      CamSensor->SceneCaptureComponent2DRgb->HiddenActors.Append(DronesToBeHidden);
    }
  }
  
}

void ADronePawn::SetPropellersTransform(const int& frame_id) {
  const FramePropellersTransform* Transforms = FramePropellersTransforms.GetData();

  FString mesh_path = "/FlightForgePlugin/Meshes/Propellers/propeller_" + Transforms[frame_id].PropellerType;

  if (UStaticMesh* PropellerMesh = LoadObject<UStaticMesh>(nullptr, *mesh_path)) {
    PropellerFrontLeft->SetStaticMesh(PropellerMesh);
    PropellerFrontRight->SetStaticMesh(PropellerMesh);
    PropellerRearLeft->SetStaticMesh(PropellerMesh);
    PropellerRearRight->SetStaticMesh(PropellerMesh);
  } else {
    UE_LOG(LogTemp, Error, TEXT("The Propeller was not loaded!"));
  }

  PropellerFrontLeft->SetRelativeTransform(Transforms[frame_id].FrontLeft);
  PropellerFrontRight->SetRelativeTransform(Transforms[frame_id].FrontRight);
  PropellerRearLeft->SetRelativeTransform(Transforms[frame_id].RearLeft);
  PropellerRearRight->SetRelativeTransform(Transforms[frame_id].RearRight);
}

void ADronePawn::SetStaticMesh(const int& frame_id) {
  FString mesh_path = "/FlightForgePlugin/Meshes/_Drones_/";

  int predefined_frame_count = FramePropellersTransforms.Num();

  // last "empty" frame is not included in "FramePropellersTransforms"
  if (predefined_frame_count == frame_id) {
    return;
  }

  FString frame_name = FramePropellersTransforms.GetData()[frame_id].FrameName;

  mesh_path += frame_name + "/" + frame_name + "." + frame_name;

  if (UStaticMesh* FrameMesh = LoadObject<UStaticMesh>(nullptr, *mesh_path)) {
    RootMeshComponent->SetStaticMesh(FrameMesh);
  } else {
    UE_LOG(LogTemp, Error, TEXT("The Frame was not loaded!"));
  }

  // wing has not propellers
  if (frame_name.Contains("wing")) {
    return;
  }

  SetPropellersTransform(frame_id);
}

void ADronePawn::Simulate_UE_Physics(const float& stop_simulation_delay) {
  // RootMeshComponent->SetSimulatePhysics(true);
  GetWorldTimerManager().SetTimer(TimerHandle_Disabled_Physics, this, &ADronePawn::DisabledPhysics_StartRotatePropellers, stop_simulation_delay, false);
}


/**
 * Visibility settings
 * @param bEnable
 * 
 * - true -- UAVs see each others
 *
 * - false -- RL purpose, UAVs do not see each others
 */
void ADronePawn::SetVisibilityOtherDrones(bool bEnable)
{
  if (bEnable)
  {
    // Visibility settings default -- UAVs see each others
    bCanSeeOtherDrone = true;
    
    RootMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
    RootMeshComponent->MarkRenderStateDirty();
    
    PropellerFrontLeft->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
    PropellerFrontLeft->MarkRenderStateDirty();
    
    PropellerFrontRight->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
    PropellerFrontRight->MarkRenderStateDirty();
    
    PropellerRearLeft->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
    PropellerRearLeft->MarkRenderStateDirty();
    
    PropellerRearRight->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
    PropellerRearRight->MarkRenderStateDirty();
  }
  else
  {
    // Visibility settings -- RL purpose, UAVs do not see each others
    bCanSeeOtherDrone = false;
    
    RootMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
    RootMeshComponent->MarkRenderStateDirty();

    PropellerFrontLeft->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
    PropellerFrontLeft->MarkRenderStateDirty();
    
    PropellerFrontRight->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
    PropellerFrontRight->MarkRenderStateDirty();
   
    PropellerRearLeft->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
    PropellerRearLeft->MarkRenderStateDirty();
    
    PropellerRearRight->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
    PropellerRearRight->MarkRenderStateDirty();
  }
}

void ADronePawn::DisabledPhysics_StartRotatePropellers()
{
  UE_LOG(LogTemp, Warning, TEXT("Disabled physics after a 3 second delay"));

  RootMeshComponent->SetSimulatePhysics(false);
  this->propellers_rotate = true;
}

//}

/* GetLeftCameraDataFromServerThread() //{ */

bool ADronePawn::GetRgbCameraDataFromServerThread(TArray<uint8>& OutArray, double& stamp) {
  return CameraSensor->GetRgbCameraDataFromServerThread(OutArray, stamp);
}

bool ADronePawn::GetStereoCameraDataFromServerThread(TArray<uint8>& image_left, TArray<uint8>& image_right, double& stamp) {
  return CameraSensor->GetStereoCameraDataFromServerThread(image_left, image_right, stamp);
}

bool ADronePawn::GetRgbSegCameraFromServerThread(TArray<uint8>& OutArray, double& stamp) {
  return CameraSensor->GetRgbSegCameraFromServerThread(OutArray, stamp);
}

void ADronePawn::SetCameraCaptureMode(CameraCaptureModeEnum CaptureMode) {
  CameraSensor->SetCameraCaptureMode(CaptureMode);
}

FLidarConfig ADronePawn::GetLidarConfig() {
  return Lidar->GetLidarConfig();
}

bool ADronePawn::SetLidarConfig(const FLidarConfig& Config) {
  return Lidar->SetLidarConfig(Config);
}

FRgbCameraConfig ADronePawn::GetRgbCameraConfig() {
  return CameraSensor->GetRgbCameraConfig();
  //return rgb_camera_config_;
}

FStereoCameraConfig ADronePawn::GetStereoCameraConfig() {
  return CameraSensor->GetStereoCameraConfig();
  //return stereo_camera_config_;
}

bool ADronePawn::SetRgbCameraConfig(const FRgbCameraConfig& Config) {
  return CameraSensor->SetRgbCameraConfig(Config);
}

bool ADronePawn::SetStereoCameraConfig(const FStereoCameraConfig& Config) {
  return CameraSensor->SetStereoCameraConfig(Config);
}

bool ADronePawn::GetVisibilityOtherDrones()
{
	return bCanSeeOtherDrone;
}

void ADronePawn::SetLocation(FVector& Location, FVector& TeleportedToLocation, bool CheckCollisions, FHitResult& HitResult) {

  bool DrawLine      = DrawMovementLine;
  auto StartLocation = DrawLine ? GetActorLocation() : FVector();

  SetActorLocation(Location, CheckCollisions, &HitResult);
  TeleportedToLocation = CheckCollisions ? GetActorLocation() : Location;

  if (DrawLine) {
    DrawDebugLine(GetWorld(), StartLocation, TeleportedToLocation, FColor::Green, true, -1, 0, 3);
  }
}

void ADronePawn::Tick(float DeltaSeconds) {

  //debugging
  // if (added == false && FPlatformTime::Seconds() - startTime > 5)
  // {
  //   AddSensor(2);
  //   added = true;
  // }
  // if (removed == false && FPlatformTime::Seconds() - startTime > 20)
  // {
  //   RemoveSensor(3);
  //   removed = true;
  // }

  
  std::shared_ptr<FInstruction<ADronePawn>> Instruction;

  while (InstructionQueue->Dequeue(Instruction)) {
    Instruction->Function(*this);
    Instruction->Finished = true;
  }

  
  Super::Tick(DeltaSeconds);

  // UE_LOG(LogTemp, Warning, TEXT("Tick(), delta %f, took %f s"), DeltaSeconds, FPlatformTime::Seconds() - start);
}

USensor* ADronePawn::AddSensor(int SensorTypeNum){
  if (SensorTypeNum < 0 || SensorTypeNum >= static_cast<int>(SensorType::MAX_SENSOR_TYPE))
  {
    UE_LOG(LogTemp, Error, TEXT("Invalid sensor type index"));
    return nullptr;
  }
  
  USensor* NewSensor = nullptr;
  SensorType Type = static_cast<SensorType>(SensorTypeNum);

  switch (Type)
  {
  case SensorType::CAMERA:
    {
      UCamera* NewCam = NewObject<UCamera>(this, UCamera::StaticClass(), NAME_None, RF_Transient);
      NewCam->Initialize(nextSensorID++);
      NewSensor = NewCam;
      break;
    }

  case SensorType::LIDAR:
    {
      ULidar* newLidar = NewObject<ULidar>(this, ULidar::StaticClass(), NAME_None, RF_Transient);
      newLidar->Initialize(nextSensorID++);
      NewSensor = newLidar;
      break;
    }

  case SensorType::RANGEFINDER:
    {
      URangeFinder* RF = NewObject<URangeFinder>(this, URangeFinder::StaticClass(), NAME_None, RF_Transient);
      RF->Initialize(nextSensorID++);
      NewSensor = RF;
      break;
    }

  default:
    UE_LOG(LogTemp, Error, TEXT("Unhandled sensor type: %d"), SensorTypeNum);
    return nullptr;
  }

  if (NewSensor)
  {
    NewSensor->RegisterComponent();
    NewSensor->AttachToComponent(RootMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
    AddInstanceComponent(NewSensor);

    Sensors.Add(NewSensor);

    UE_LOG(LogTemp, Log, TEXT("Added sensor %s with ID %d"),
        *NewSensor->GetName(), nextSensorID);
  }

  return NewSensor;
}

USensor* ADronePawn::RemoveSensor(int sensorID)
{
  USensor* deletedSensor = nullptr;
  
  for (int i = 0; i < Sensors.Num(); i++)
  {
    TObjectPtr<USensor> Sensor = Sensors[i];
    if (Sensor && Sensor->GetSensorID() == sensorID)
    {
      deletedSensor = Sensor;

      Sensor->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
      Sensors.RemoveAt(i);
      Sensor->DestroyComponent();

      UE_LOG(LogTemp, Log, TEXT("Removed sensor with ID %d (%s)"), 
                   sensorID, *Sensor->GetName());
      break;
    }
  }

  if (!deletedSensor)
  {
    UE_LOG(LogTemp, Warning, TEXT("Sensor with ID %d not found"), sensorID);
  }
  
  return deletedSensor;
}

