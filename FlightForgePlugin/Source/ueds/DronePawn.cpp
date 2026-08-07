/* includes //{ */

#include "DronePawn.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DronePawn)

#include "ImageUtils.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/TextureRenderTarget2D.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Core/Public/Async/ParallelFor.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include <cereal/details/helpers.hpp>

#include "NaniteSceneProxy.h"


// Sets default values
ADronePawn::ADronePawn() {

  // UE_LOG(LogTemp, Warning, TEXT("ADronePawn constructor"));

  PrimaryActorTick.bCanEverTick = true;
  
  PrimaryActorTick.TickGroup    = TG_PrePhysics;

  InstructionQueue = std::make_unique<TQueue<std::shared_ptr<FInstruction<ADronePawn>>>>();

  SensorsCriticalSection = std::make_unique<FCriticalSection>();

  RootMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMeshComponent"));
  RootComponent = RootMeshComponent;
  
  nextSensorID = 0;

  
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

  droneServer = std::make_shared<DroneServer>(*this);
}

//}

/* beginPlay() //{ */

// Called when the game starts or when spawned
void ADronePawn::BeginPlay() {

  //startTime = FPlatformTime::Seconds();
  
  // sensors are created lazily on first request (or explicitly via add_sensor)

  Super::BeginPlay();
}

// void ADronePawn::PostInitializeComponents() {
//     Super::PostInitializeComponents();
//     
//     // Add default sensors that every drone should have
//     // AddSensor(0);
//     // AddSensor(1); //Lidar
//     // AddSensor(2); //RangeFinder
//     // AddSensor(3); //Lidar Livox
//     // AddSensor(4); //RGB Seg camera
//     // AddSensor(5); //Stereo camera
// }


void ADronePawn::StartServer() {
  // UE_LOG(LogTemp, Warning, TEXT("Starting drone server"));
  droneServer->Run();
}


void ADronePawn::EndPlay(const EEndPlayReason::Type EndPlayReason) {
  droneServer->Stop();

  for (auto& SensorPair : Sensors)
  {
    if (SensorPair.Value)
    {
      SensorPair.Value->DestroyComponent();
    }
  }
  Sensors.Empty();
}

// Called to bind functionality to input
void ADronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool ADronePawn::GetCrashState(void) {

  // UE_LOG(LogTemp, Warning, TEXT("ADronePawn::GetCrashState"));
  return uav_crashed_;
}

// void ADronePawn::UpdateCamera(int ID, bool isExternallyLocked, double stamp = 0.0)
// {
//   if (TObjectPtr<USensor>* SensorPtr = Sensors.Find(ID))
//   {
//     USensor* Sensor = *SensorPtr;
//     if (UCamera* CamSensor = Cast<UCamera>(Sensor))
//     {
//       CamSensor->UpdateCamera(isExternallyLocked, stamp);
//     }
//   }
// }

void ADronePawn::UpdateCameraSensorsMutualVisibility(TArray<AActor*>& DronesToBeHidden)
{
  for (const TPair<int32, TObjectPtr<USensor>>& Pair : Sensors)
  {
    if (URgbCamera* CamSensor = Cast<URgbCamera>(Pair.Value)) //used to be only for RGB camera
    {
      if (CamSensor->SceneCaptureComponent2D)
      {
        CamSensor->SceneCaptureComponent2D->HiddenActors.Empty();
        CamSensor->SceneCaptureComponent2D->HiddenActors.Append(DronesToBeHidden);
      }
    }
  }
}

void ADronePawn::SetPropellersTransform(const std::string& frame_name) {
  
  FString frame_name_ = FString(frame_name.c_str());
  const FramePropellersTransform* Transforms = FramePropellersTransforms.GetData();
  int frame_id = 0;
  const int32 TransformCount = FramePropellersTransforms.Num();
  for (int32 idx = 0; idx < TransformCount; ++idx) {
    if (FramePropellersTransforms[idx].FrameName.Equals(frame_name_, ESearchCase::IgnoreCase)) {
      frame_id = idx;
      break;
    }
  } 
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

void ADronePawn::SetStaticMesh(const std::string &frame_name) {
  FString mesh_path = "/FlightForgePlugin/Meshes/_Drones_/";

  /* int predefined_frame_count = FramePropellersTransforms.Num();

  // last "empty" frame is not included in "FramePropellersTransforms"
  if (predefined_frame_count == frame_id) {
    return;
  }

  FString frame_name = FramePropellersTransforms.GetData()[frame_id].FrameName;
 */

  FString frame_name_ = FString(frame_name.c_str()); 
  mesh_path += frame_name_ + "/" + frame_name_ + "." + frame_name_;

  if (UStaticMesh* FrameMesh = LoadObject<UStaticMesh>(nullptr, *mesh_path)) {
    RootMeshComponent->SetStaticMesh(FrameMesh);
  } else if(frame_name_.Contains("empty")){
    UE_LOG(LogTemp, Warning, TEXT("Setted empty frame"));
  } else {  
    UE_LOG(LogTemp, Error, TEXT("The Frame was not loaded! Setted \"x500\" as default frame"));
    frame_name_ = "x500";
    mesh_path = "/FlightForgePlugin/Meshes/_Drones_/" + frame_name_ + "/" + frame_name_ + "." + frame_name_;
  }

  // wing has not propellers
  if (frame_name_.Contains("wing") || frame_name_.Contains("empty")) {
    return;
  }

  SetPropellersTransform(frame_name);
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

void ADronePawn::SetCameraCaptureMode(CameraCaptureModeEnum CaptureMode) {
  CameraCaptureMode = CaptureMode;
  UE_LOG(LogTemp, Log, TEXT("drone Pawm setting all camera's capture mode"));
  
  for (const TPair<int32, TObjectPtr<USensor>>& Pair : Sensors)
  {
    if (UCamera* CamSensor = Cast<UCamera>(Pair.Value))
    {
      CamSensor->SetCameraCaptureMode(CameraCaptureMode);
    }
  }
}

CameraCaptureModeEnum ADronePawn::GetCameraCaptureMode()
{
  return CameraCaptureMode;
}

// bool ADronePawn::SetRgbCameraConfig(const FRgbCameraConfig& Config, int ID)
// {
//   bool ret = false;
//   if (TObjectPtr<USensor>* SensorPtr = Sensors.Find(ID))
//   {
//     USensor* Sensor = *SensorPtr;
//     if (URgbCamera* CamSensor = Cast<URgbCamera>(Sensor))
//     {
//       ret = CamSensor->SetRgbCameraConfig(Config);
//     }
//   }
//
//   return ret;
// }
//
// bool ADronePawn::SetStereoCameraConfig(const FStereoCameraConfig& Config, int ID)
// {
//   bool ret = false;
//   if (TObjectPtr<USensor>* SensorPtr = Sensors.Find(ID))
//   {
//     USensor* Sensor = *SensorPtr;
//     if (UStereoCamera* CamSensor = Cast<UStereoCamera>(Sensor))
//     {
//       ret = CamSensor->SetStereoCameraConfig(Config);
//     }
//   }
//
//   return ret;
// }

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
  
  std::shared_ptr<FInstruction<ADronePawn>> Instruction;

  while (InstructionQueue->Dequeue(Instruction)) {
    Instruction->Function(*this);
    Instruction->Finished = true;
  }
  
  Super::Tick(DeltaSeconds);

  // UE_LOG(LogTemp, Warning, TEXT("Tick(), delta %f, took %f s"), DeltaSeconds, FPlatformTime::Seconds() - start);
}

int ADronePawn::AddSensor(int SensorTypeNum){
  if (SensorTypeNum < 0 || SensorTypeNum >= static_cast<int>(SensorType::MAX_SENSOR_TYPE))
  {
    UE_LOG(LogTemp, Error, TEXT("Invalid sensor type index"));
    return -1;
  }

  int ret = nextSensorID;
  

  UClass* SensorClass = nullptr;
  SensorType Type = static_cast<SensorType>(SensorTypeNum);

  switch (Type)
  {
    case SensorType::RGB_CAMERA:     SensorClass = URgbCamera::StaticClass();   break;
    case SensorType::LIDAR:          SensorClass = ULidar::StaticClass();       break;
    case SensorType::RANGEFINDER:    SensorClass = URangeFinder::StaticClass(); break;
    case SensorType::LIDAR_LIVOX:    SensorClass = ULidarLivox::StaticClass();  break;
    case SensorType::RGB_SEG_CAMERA: SensorClass = URgbSegCamera::StaticClass();break;
    case SensorType::STEREO_CAMERA:  SensorClass = UStereoCamera::StaticClass();break;
    case SensorType::EVENT_CAMERA:   SensorClass = UEventCamera::StaticClass(); break;
    case SensorType::FISHEYE_CAMERA: SensorClass = UFisheyeCamera::StaticClass();break;
    default: return -2;
  }
  
  USensor* NewSensor = NewObject<USensor>(this, SensorClass);

  if (NewSensor)
  {
    NewSensor->SetMobility(EComponentMobility::Movable);
    NewSensor->Initialize(nextSensorID);
    
    NewSensor->AttachToComponent(RootMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
    NewSensor->RegisterComponent();
    
    AddInstanceComponent(NewSensor);

    SensorsCriticalSection->Lock();
    Sensors.Add(nextSensorID, NewSensor);
    if (!DefaultSensorOfType.Contains(Type))
    {
      DefaultSensorOfType.Add(Type, nextSensorID);
    }
    SensorsCriticalSection->Unlock();

    UE_LOG(LogTemp, Log, TEXT("Successfully attached %s (ID: %d) at %s"), 
        *NewSensor->GetName(), nextSensorID, *NewSensor->GetComponentLocation().ToString());

    return nextSensorID++;
  }

  return ret;
}

bool ADronePawn::RemoveSensor(int32 sensorID)
{
  USensor* Sensor = nullptr;

  SensorsCriticalSection->Lock();
  if (TObjectPtr<USensor>* SensorPtr = Sensors.Find(sensorID))
  {
    Sensor = *SensorPtr;
    Sensors.Remove(sensorID);

    if (Sensor)
    {
      if (int* DefaultID = DefaultSensorOfType.Find(Sensor->GetSensorType()); DefaultID && *DefaultID == sensorID)
      {
        DefaultSensorOfType.Remove(Sensor->GetSensorType());
      }
    }
  }
  SensorsCriticalSection->Unlock();

  if (Sensor)
  {
    Sensor->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
    Sensor->UnregisterComponent();
    RemoveOwnedComponent(Sensor);
    Sensor->DestroyComponent();

    UE_LOG(LogTemp, Log, TEXT("Removed sensor with ID %d (%s)"), sensorID, *Sensor->GetName());
    return true;
  }

  UE_LOG(LogTemp, Warning, TEXT("Sensor with ID %d not found"), sensorID);
  return false;
}

USensor* ADronePawn::FindSensorById(int SensorID)
{
  SensorsCriticalSection->Lock();
  TObjectPtr<USensor>* Found = Sensors.Find(SensorID);
  USensor* Sensor = Found ? Found->Get() : nullptr;
  SensorsCriticalSection->Unlock();

  return Sensor;
}

int ADronePawn::ResolveSensorId(SensorType Type, int RequestedID)
{
  // the generic lidar messages also drive a dedicated livox instance
  auto TypeCompatible = [Type](SensorType Actual) {
    return Actual == Type || (Type == SensorType::LIDAR && Actual == SensorType::LIDAR_LIVOX);
  };

  SensorsCriticalSection->Lock();

  if (RequestedID >= 0)
  {
    TObjectPtr<USensor>* Found = Sensors.Find(RequestedID);
    const bool bOk = Found && *Found && TypeCompatible((*Found)->GetSensorType());
    SensorsCriticalSection->Unlock();

    if (!bOk)
    {
      UE_LOG(LogTemp, Warning, TEXT("ResolveSensorId: sensor %d missing or wrong type"), RequestedID);
    }
    return bOk ? RequestedID : -1;
  }

  if (int* DefaultID = DefaultSensorOfType.Find(Type))
  {
    const int Id = *DefaultID;
    SensorsCriticalSection->Unlock();
    return Id;
  }

  SensorsCriticalSection->Unlock();

  return AddSensorFromServerThread(static_cast<int>(Type));
}

void ADronePawn::GetSensorList(std::vector<int>& OutIds, std::vector<int>& OutTypes)
{
  SensorsCriticalSection->Lock();
  for (const TPair<int32, TObjectPtr<USensor>>& Pair : Sensors)
  {
    if (Pair.Value)
    {
      OutIds.push_back(Pair.Key);
      OutTypes.push_back(static_cast<int>(Pair.Value->GetSensorType()));
    }
  }
  SensorsCriticalSection->Unlock();
}

int ADronePawn::AddSensorFromServerThread(int SensorTypeNum)
{
  int NewID = -1;

  auto Instruction      = std::make_shared<FInstruction<ADronePawn>>();
  Instruction->Function = [SensorTypeNum, &NewID](ADronePawn& Pawn) { NewID = Pawn.AddSensor(SensorTypeNum); };
  InstructionQueue->Enqueue(Instruction);

  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  return NewID;
}

bool ADronePawn::RemoveSensorFromServerThread(int SensorID)
{
  bool bRemoved = false;

  auto Instruction      = std::make_shared<FInstruction<ADronePawn>>();
  Instruction->Function = [SensorID, &bRemoved](ADronePawn& Pawn) { bRemoved = Pawn.RemoveSensor(SensorID); };
  InstructionQueue->Enqueue(Instruction);

  FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

  return bRemoved;
}

