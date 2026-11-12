/* includes //{ */

#include "RobotPawn.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RobotPawn)

#include "ImageUtils.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/TextureRenderTarget2D.h"
#include "TimerManager.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Core/Public/Async/ParallelFor.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include "NaniteSceneProxy.h"
#include "FlightForgeCore/Devices/SensorDeviceLibrary.h"
#include "FlightForgeCore/FlightForgeGameInstance.h"
#include "FlightForgeCore/Server/SimServer.h"


// Sets default values
ARobotPawn::ARobotPawn() {

  // UE_LOG(LogTemp, Warning, TEXT("ARobotPawn constructor"));

  PrimaryActorTick.bCanEverTick = true;
  PrimaryActorTick.bTickEvenWhenPaused = true;  // pose requests must land while lockstep holds the world
  
  PrimaryActorTick.TickGroup    = TG_PrePhysics;

  InstructionQueue = std::make_unique<TInstructionQueue<ARobotPawn>>();

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

  // Ray-traced sensors query the Visibility channel, and nothing here answered on it, so
  // the airframe was invisible to every lidar including the ones bolted to it. Propellers
  // keep their collision off: their meshes carry no cooked collision, and switching them
  // on tears down physics on spawn. Drone-to-drone masking is separate, on ECC_Pawn.
  RootMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

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
}

//}

/* beginPlay() //{ */

// Called when the game starts or when spawned
void ARobotPawn::BeginPlay() {

  //startTime = FPlatformTime::Seconds();
  
  // sensors are created lazily on first request (or explicitly via add_sensor)

  Super::BeginPlay();
}

// void ARobotPawn::PostInitializeComponents() {
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


void ARobotPawn::DrainInstructions() {
  std::shared_ptr<FInstruction<ARobotPawn>> Instruction;
  while (InstructionQueue->Dequeue(Instruction)) {
    Instruction->Run(*this);
  }

  SensorsCriticalSection->Lock();
  for (auto& SensorPair : Sensors) {
    if (SensorPair.Value) {
      SensorPair.Value->DrainInstructions();
    }
  }
  SensorsCriticalSection->Unlock();
}

void ARobotPawn::EndPlay(const EEndPlayReason::Type EndPlayReason) {
  // a connection may be mid-request, spinning on an instruction only this thread can run
  UFlightForgeGameInstance* Instance = Cast<UFlightForgeGameInstance>(GetGameInstance());
  if (Instance && Instance->Server) {
    Instance->Server->ReleaseRobot(this, [this]() { DrainInstructions(); });
  }

  for (auto& SensorPair : Sensors)
  {
    if (SensorPair.Value)
    {
      SensorPair.Value->DestroyComponent();
    }
  }
  Sensors.Empty();

  Super::EndPlay(EndPlayReason);
}

// Called to bind functionality to input
void ARobotPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool ARobotPawn::GetCrashState(void) {

  // UE_LOG(LogTemp, Warning, TEXT("ARobotPawn::GetCrashState"));
  return uav_crashed_;
}

// void ARobotPawn::UpdateCamera(int ID, bool isExternallyLocked, double stamp = 0.0)
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

void ARobotPawn::UpdateCameraSensorsMutualVisibility(TArray<AActor*>& DronesToBeHidden)
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

void ARobotPawn::SetPropellersTransform(const std::string& frame_name) {
  
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

/* SetStaticMesh() //{ */

namespace {
FString PredefinedFramePath(const FString &FrameName) {
  return FString::Printf(TEXT("/FlightForgePlugin/Meshes/_Drones_/%s/%s.%s"), *FrameName, *FrameName, *FrameName);
}
}  // namespace

void ARobotPawn::SetStaticMesh(const std::string &frame_name) {

  const FString FrameName = FString(UTF8_TO_TCHAR(frame_name.c_str()));

  ClearDynamicPropellers();

  if (FrameName.Contains("empty")) {
    RootMeshComponent->SetStaticMesh(nullptr);
    return;
  }

  const auto SetPredefinedPropellersVisible = [this](const bool bVisible) {
    for (UStaticMeshComponent *Propeller : {PropellerFrontLeft, PropellerFrontRight, PropellerRearLeft, PropellerRearRight}) {
      Propeller->SetVisibility(bVisible);
    }
  };

  if (UStaticMesh *FrameMesh = LoadObject<UStaticMesh>(nullptr, *PredefinedFramePath(FrameName), nullptr, LOAD_NoWarn | LOAD_Quiet)) {
    RootMeshComponent->SetStaticMesh(FrameMesh);
    SetPredefinedPropellersVisible(true);

    // wing has not propellers
    if (!FrameName.Contains("wing")) {
      SetPropellersTransform(frame_name);
    }

    return;
  }

  // Not cooked into the build: the name may refer to a user model sitting on disk, which
  // is how a packaged binary gets a frame it was never built with.
  if (LoadExternalModel(FrameName)) {
    SetPredefinedPropellersVisible(false);
    return;
  }

  UE_LOG(LogTemp, Error, TEXT("The frame \"%s\" is neither packed nor on disk, falling back to \"x500\""), *FrameName);

  if (UStaticMesh *DefaultMesh = LoadObject<UStaticMesh>(nullptr, *PredefinedFramePath(TEXT("x500")))) {
    RootMeshComponent->SetStaticMesh(DefaultMesh);
    SetPredefinedPropellersVisible(true);
    SetPropellersTransform(std::string("x500"));
  }
}

/* //} */

/* LoadExternalModel() //{ */

bool ARobotPawn::LoadExternalModel(const FString &ModelName) {

  if (!ExternalModelResolver) {
    ExternalModelResolver = NewObject<UAssetResolver>(this);
  }

  TArray<FString> ModelPaths = ExternalModelPaths;

  if (ModelPaths.IsEmpty()) {
    ModelPaths.Add(FPaths::Combine(FPaths::ProjectDir(), TEXT("FlightForge/Models")));

    if (const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("FlightForgePlugin"))) {
      ModelPaths.Add(FPaths::Combine(Plugin->GetContentDir(), TEXT("ExternalModels")));
    }
  }

  FString ModelFilePath;
  if (!UAssetResolver::FindModelFile(ModelName, ModelPaths, ModelFilePath)) {
    return false;
  }

  UStaticMesh *Mesh = ExternalModelResolver->ImportGltf(ModelFilePath, this);
  if (!Mesh) {
    return false;
  }

  RootMeshComponent->SetStaticMesh(Mesh);

  // The propeller sidecar sits next to the model and shares its name.
  FExternalModelConfig Config;
  FString              Error;

  if (ExternalModelConfig::Load(FPaths::ChangeExtension(ModelFilePath, TEXT("yaml")), Config, Error)) {
    ApplyExternalPropellers(Config);
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[RobotPawn] \"%s\" has no usable propeller sidecar: %s"), *ModelName, *Error);
  }

  return true;
}

/* //} */

/* ApplyExternalPropellers() //{ */

void ARobotPawn::ApplyExternalPropellers(const FExternalModelConfig &Config) {

  ClearDynamicPropellers();

  if (Config.Propellers.IsEmpty()) {
    return;
  }

  const FString PropellerMeshPath =
      Config.PropellerMeshPath.IsEmpty() ? TEXT("/FlightForgePlugin/Meshes/Propellers/propeller_X500.propeller_X500") : Config.PropellerMeshPath;

  UStaticMesh *PropellerMesh = LoadObject<UStaticMesh>(nullptr, *PropellerMeshPath);
  if (!PropellerMesh) {
    UE_LOG(LogTemp, Error, TEXT("[RobotPawn] the propeller mesh \"%s\" could not be loaded"), *PropellerMeshPath);
    return;
  }

  for (const FExternalPropellerConfig &PropellerConfig : Config.Propellers) {

    const FName ComponentName = MakeUniqueObjectName(this, UStaticMeshComponent::StaticClass(), FName(*PropellerConfig.Name));

    UStaticMeshComponent *Propeller = NewObject<UStaticMeshComponent>(this, ComponentName);
    Propeller->SetupAttachment(RootMeshComponent);
    Propeller->RegisterComponent();
    Propeller->SetRelativeTransform(PropellerConfig.RelativeTransform);
    Propeller->SetStaticMesh(PropellerMesh);
    Propeller->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Propeller->SetCollisionResponseToChannel(ECC_Pawn, bCanSeeOtherDrone ? ECR_Block : ECR_Ignore);

    DynamicPropellers.Add(Propeller);
  }
}

/* //} */

/* ClearDynamicPropellers() //{ */

void ARobotPawn::ClearDynamicPropellers() {

  for (const TObjectPtr<UStaticMeshComponent> &Propeller : DynamicPropellers) {
    if (IsValid(Propeller)) {
      Propeller->DestroyComponent();
    }
  }

  DynamicPropellers.Empty();
}

/* //} */

void ARobotPawn::Simulate_UE_Physics(const float& stop_simulation_delay) {
  // RootMeshComponent->SetSimulatePhysics(true);
  GetWorldTimerManager().SetTimer(TimerHandle_Disabled_Physics, this, &ARobotPawn::DisabledPhysics_StartRotatePropellers, stop_simulation_delay, false);
}


/**
 * Visibility settings
 * @param bEnable
 * 
 * - true -- UAVs see each others
 *
 * - false -- RL purpose, UAVs do not see each others
 */
void ARobotPawn::SetVisibilityOtherDrones(bool bEnable)
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

    for (const TObjectPtr<UStaticMeshComponent>& Propeller : DynamicPropellers)
    {
      if (IsValid(Propeller))
      {
        Propeller->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
        Propeller->MarkRenderStateDirty();
      }
    }
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

    for (const TObjectPtr<UStaticMeshComponent>& Propeller : DynamicPropellers)
    {
      if (IsValid(Propeller))
      {
        Propeller->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
        Propeller->MarkRenderStateDirty();
      }
    }
  }
}

void ARobotPawn::DisabledPhysics_StartRotatePropellers()
{
  UE_LOG(LogTemp, Warning, TEXT("Disabled physics after a 3 second delay"));

  RootMeshComponent->SetSimulatePhysics(false);
  this->propellers_rotate = true;
}

void ARobotPawn::SetCameraCaptureMode(CameraCaptureModeEnum CaptureMode) {
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

CameraCaptureModeEnum ARobotPawn::GetCameraCaptureMode()
{
  return CameraCaptureMode;
}

// bool ARobotPawn::SetRgbCameraConfig(const FRgbCameraConfig& Config, int ID)
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
// bool ARobotPawn::SetStereoCameraConfig(const FStereoCameraConfig& Config, int ID)
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

bool ARobotPawn::GetVisibilityOtherDrones()
{
	return bCanSeeOtherDrone;
}

void ARobotPawn::SetLocation(FVector& Location, FVector& TeleportedToLocation, bool CheckCollisions, FHitResult& HitResult) {

  bool DrawLine      = DrawMovementLine;
  auto StartLocation = DrawLine ? GetActorLocation() : FVector();

  SetActorLocation(Location, CheckCollisions, &HitResult);
  TeleportedToLocation = CheckCollisions ? GetActorLocation() : Location;

  if (DrawLine) {
    DrawDebugLine(GetWorld(), StartLocation, TeleportedToLocation, FColor::Green, true, -1, 0, 3);
  }
}

/* pose trajectory //{ */

void ARobotPawn::SetPoseTrajectory(TArray<FPoseSample>&& Samples) {

  if (!TrajectoryCriticalSection) {
    TrajectoryCriticalSection = std::make_unique<FCriticalSection>();
  }

  TrajectoryCriticalSection->Lock();
  Trajectory = MoveTemp(Samples);
  TrajectoryCriticalSection->Unlock();
}

void ARobotPawn::ApplyTrajectory(double SimTime) {

  if (!TrajectoryCriticalSection) {
    return;
  }

  FVector Location;
  FQuat   Rotation;
  bool    bHavePose = false;

  TrajectoryCriticalSection->Lock();

  const int32 N = Trajectory.Num();

  if (N > 0) {
    bHavePose = true;

    int32 Next = 0;
    while (Next < N && Trajectory[Next].T <= SimTime) {
      ++Next;
    }

    if (Next == 0) {
      Location = Trajectory[0].Location;
      Rotation = Trajectory[0].Rotation;
    } else if (Next >= N) {
      Location = Trajectory[N - 1].Location;
      Rotation = Trajectory[N - 1].Rotation;
    } else {
      const FPoseSample& A     = Trajectory[Next - 1];
      const FPoseSample& B     = Trajectory[Next];
      const double       Span  = B.T - A.T;
      const double       Alpha = Span > 0.0 ? FMath::Clamp((SimTime - A.T) / Span, 0.0, 1.0) : 1.0;

      Location = FMath::Lerp(A.Location, B.Location, Alpha);
      Rotation = FQuat::Slerp(A.Rotation, B.Rotation, Alpha);
    }

    if (Next > 1) {
      Trajectory.RemoveAt(0, Next - 1, EAllowShrinking::No);
    }
  }

  TrajectoryCriticalSection->Unlock();

  if (bHavePose) {
    SetActorLocationAndRotation(Location, Rotation, false, nullptr, ETeleportType::TeleportPhysics);
  }
}

//}

void ARobotPawn::Tick(float DeltaSeconds) {

  if (const UWorld* World = GetWorld()) {
    ApplyTrajectory(World->GetTimeSeconds());
  }

  // Once a client sets speeds, every propeller is turned from here - including the four
  // the frame declares, which BP_DronePawn would otherwise spin at its own fixed rate.
  // Without taking them over there would be no way to give one rotor a different speed.
  if (PropellerSpeeds.Num() > 0) {

    const TArray<UStaticMeshComponent*> Propellers = AllPropellers();

    // Unreal's yaw turns clockwise seen from above, the wire's rate counterclockwise
    for (int32 i = 0; i < Propellers.Num() && i < PropellerSpeeds.Num(); ++i) {
      if (IsValid(Propellers[i])) {
        const double DegreesPerSecond = FMath::RadiansToDegrees(PropellerSpeeds[i]);
        Propellers[i]->AddLocalRotation(FRotator(0, -DegreesPerSecond * DeltaSeconds, 0));
      }
    }
  }
  else if (propellers_rotate && DynamicPropellers.Num() > 0) {

    // an external model's propellers are not reachable from the Blueprint
    const float RotationStep = 2000.0f * DeltaSeconds;

    for (int32 i = 0; i < DynamicPropellers.Num(); ++i) {
      if (IsValid(DynamicPropellers[i])) {
        DynamicPropellers[i]->AddLocalRotation(FRotator(0, (i % 2 == 0) ? RotationStep : -RotationStep, 0));
      }
    }
  }
  
  DrainInstructions();

  Super::Tick(DeltaSeconds);

  // UE_LOG(LogTemp, Warning, TEXT("Tick(), delta %f, took %f s"), DeltaSeconds, FPlatformTime::Seconds() - start);
}

int ARobotPawn::AddSensor(int SensorTypeNum){
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
    case SensorType::INSTANCE_SEG_CAMERA: SensorClass = USegIdCamera::StaticClass(); break;
    case SensorType::DEPTH_CAMERA:   SensorClass = UDepthCamera::StaticClass();  break;
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
    SensorSlots.Add(nextSensorID, std::make_shared<FSensorSlot>());
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

bool ARobotPawn::RemoveSensor(int32 sensorID)
{
  USensor* Sensor = nullptr;

  SensorsCriticalSection->Lock();
  if (TObjectPtr<USensor>* SensorPtr = Sensors.Find(sensorID))
  {
    Sensor = *SensorPtr;
    Sensors.Remove(sensorID);
    SensorSlots.Remove(sensorID);

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

USensor* ARobotPawn::FindSensorById(int SensorID)
{
  SensorsCriticalSection->Lock();
  TObjectPtr<USensor>* Found = Sensors.Find(SensorID);
  USensor* Sensor = Found ? Found->Get() : nullptr;
  SensorsCriticalSection->Unlock();

  return Sensor;
}

int ARobotPawn::ResolveSensorId(SensorType Type, int RequestedID)
{
  // the generic lidar messages also drive a dedicated livox instance, and the rgb
  // config messages also address seg cameras (same config struct on the wire) so
  // seg resolution/intrinsics/distortion are settable by explicit id
  auto TypeCompatible = [Type](SensorType Actual) {
    return Actual == Type || (Type == SensorType::LIDAR && Actual == SensorType::LIDAR_LIVOX) ||
           (Type == SensorType::RGB_CAMERA &&
            (Actual == SensorType::RGB_SEG_CAMERA || Actual == SensorType::INSTANCE_SEG_CAMERA || Actual == SensorType::DEPTH_CAMERA));
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

std::shared_ptr<FSensorSlot> ARobotPawn::FindSensorSlot(int SensorID)
{
  SensorsCriticalSection->Lock();
  const std::shared_ptr<FSensorSlot>* Found = SensorSlots.Find(SensorID);
  std::shared_ptr<FSensorSlot>        Slot  = Found ? *Found : nullptr;
  SensorsCriticalSection->Unlock();

  return Slot;
}

void ARobotPawn::GetSensorList(std::vector<int>& OutIds, std::vector<int>& OutTypes)
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

/* addDevice() //{ */

bool ARobotPawn::AddDevice(const FString& DeviceName, const FVector& MountOffset, const FRotator& MountRotation, bool bShowMesh, std::vector<int>& OutIds,
                           std::vector<int>& OutTypes)
{
  const FSensorDeviceSpec* Device = FSensorDeviceLibrary::Find(DeviceName);

  if (!Device)
  {
    UE_LOG(LogTemp, Error, TEXT("AddDevice: unknown device '%s'"), *DeviceName);
    return false;
  }

  for (const FDeviceSensorEntry& Entry : Device->Sensors)
  {
    const int SensorID = AddSensor(static_cast<int>(Entry.Type));

    if (SensorID < 0)
    {
      return false;
    }

    const FVector  AbsOffset   = MountOffset + MountRotation.RotateVector(Entry.RelOffset);
    const FRotator AbsRotation = (MountRotation.Quaternion() * Entry.RelRotation.Quaternion()).Rotator();

    if (Entry.Configure)
    {
      Entry.Configure(FindSensorById(SensorID), AbsOffset, AbsRotation);
    }

    OutIds.push_back(SensorID);
    OutTypes.push_back(static_cast<int>(Entry.Type));
  }

  if (bShowMesh)
  {
    const TCHAR* ShapePath = Device->MeshShape == EDeviceMeshShape::Cylinder ? TEXT("/Engine/BasicShapes/Cylinder.Cylinder") : TEXT("/Engine/BasicShapes/Cube.Cube");

    if (UStaticMesh* Shape = LoadObject<UStaticMesh>(nullptr, ShapePath))
    {
      UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(this);
      Mesh->SetStaticMesh(Shape);
      Mesh->SetMobility(EComponentMobility::Movable);
      Mesh->AttachToComponent(RootMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
      // engine basic shapes are 100 cm across and centred, and the housing base sits on the mount point
      Mesh->SetRelativeLocation(MountOffset + MountRotation.RotateVector(FVector(0, 0, Device->MeshDimensionsCm.Z * 0.5)));
      Mesh->SetRelativeRotation(MountRotation);
      Mesh->SetRelativeScale3D(Device->MeshDimensionsCm / 100.0f);
      Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
      Mesh->RegisterComponent();
      AddInstanceComponent(Mesh);

      // the datasheet box stands in until the real housing has been imported in the background
      const FString MeshPath = DeviceMeshPath(*Device);
      UFlightForgeGameInstance* Instance = MeshPath.IsEmpty() ? nullptr : Cast<UFlightForgeGameInstance>(GetGameInstance());
      if (Instance)
      {
        Instance->GetDeviceMeshes()->Request(MeshPath, Mesh, MountOffset + MountRotation.RotateVector(Device->MeshOffsetCm),
                                             (MountRotation.Quaternion() * Device->MeshRotation.Quaternion()).Rotator());
      }
    }
  }

  UE_LOG(LogTemp, Log, TEXT("AddDevice: '%s' -> %d sensors at (%s)"), *DeviceName, (int)Device->Sensors.Num(), *MountOffset.ToCompactString());

  return true;
}

/* DeviceMeshPath() //{ */

FString ARobotPawn::DeviceMeshPath(const FSensorDeviceSpec& Device) const
{
  if (Device.MeshFile.IsEmpty())
  {
    return FString();
  }

  // a project can override or add a housing without touching the plugin
  TArray<FString> Dirs = {FPaths::Combine(FPaths::ProjectDir(), TEXT("FlightForge"), TEXT("Models"), TEXT("Sensors"))};
  if (const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("FlightForgePlugin")))
  {
    Dirs.Add(FPaths::Combine(Plugin->GetContentDir(), TEXT("Meshes"), TEXT("Sensors")));
  }

  for (const FString& Dir : Dirs)
  {
    const FString Path = FPaths::Combine(Dir, Device.MeshFile + TEXT(".glb"));
    if (FPaths::FileExists(Path))
    {
      return Path;
    }
  }

  UE_LOG(LogTemp, Warning, TEXT("AddDevice: '%s' has no %s.glb under %s, keeping the %s"), *Device.Name, *Device.MeshFile, *FString::Join(Dirs, TEXT(" or ")),
         Device.MeshShape == EDeviceMeshShape::Cylinder ? TEXT("cylinder") : TEXT("box"));
  return FString();
}

//}

bool ARobotPawn::AddDeviceFromServerThread(const FString& DeviceName, const FVector& MountOffset, const FRotator& MountRotation, bool bShowMesh,
                                           std::vector<int>& OutIds, std::vector<int>& OutTypes)
{
  bool bAdded = false;

  auto Instruction      = std::make_shared<FInstruction<ARobotPawn>>();
  Instruction->Function = [&](ARobotPawn& Pawn) { bAdded = Pawn.AddDevice(DeviceName, MountOffset, MountRotation, bShowMesh, OutIds, OutTypes); };
  InstructionQueue->Enqueue(Instruction);

  Instruction->Wait();

  return bAdded;
}

//}

int ARobotPawn::AddSensorFromServerThread(int SensorTypeNum)
{
  int NewID = -1;

  auto Instruction      = std::make_shared<FInstruction<ARobotPawn>>();
  Instruction->Function = [SensorTypeNum, &NewID](ARobotPawn& Pawn) { NewID = Pawn.AddSensor(SensorTypeNum); };
  InstructionQueue->Enqueue(Instruction);

  Instruction->Wait();

  return NewID;
}

bool ARobotPawn::RemoveSensorFromServerThread(int SensorID)
{
  bool bRemoved = false;

  auto Instruction      = std::make_shared<FInstruction<ARobotPawn>>();
  Instruction->Function = [SensorID, &bRemoved](ARobotPawn& Pawn) { bRemoved = Pawn.RemoveSensor(SensorID); };
  InstructionQueue->Enqueue(Instruction);

  Instruction->Wait();

  return bRemoved;
}


/* allPropellers() //{ */

TArray<UStaticMeshComponent*> ARobotPawn::AllPropellers() const
{
  TArray<UStaticMeshComponent*> Propellers;

  for (UStaticMeshComponent* Propeller : {PropellerRearLeft, PropellerRearRight, PropellerFrontLeft, PropellerFrontRight})
  {
    // a frame that declares no propellers leaves these with no mesh; skipping them keeps
    // the index the client sees aligned with the propellers it can actually see turning
    if (IsValid(Propeller) && Propeller->GetStaticMesh())
    {
      Propellers.Add(Propeller);
    }
  }

  for (const TObjectPtr<UStaticMeshComponent>& Propeller : DynamicPropellers)
  {
    if (IsValid(Propeller))
    {
      Propellers.Add(Propeller);
    }
  }

  return Propellers;
}

/* //} */

/* propellerLinkNames() //{ */

TArray<FString> ARobotPawn::PropellerLinkNames() const
{
  TArray<FString> Names;

  const TPair<UStaticMeshComponent*, const TCHAR*> Fixed[] = {
      {PropellerRearLeft, TEXT("prop_rl")}, {PropellerRearRight, TEXT("prop_rr")}, {PropellerFrontLeft, TEXT("prop_fl")}, {PropellerFrontRight, TEXT("prop_fr")}};

  for (const auto& [Propeller, Name] : Fixed)
  {
    if (IsValid(Propeller) && Propeller->GetStaticMesh())
    {
      Names.Add(Name);
    }
  }

  for (const TObjectPtr<UStaticMeshComponent>& Propeller : DynamicPropellers)
  {
    if (IsValid(Propeller))
    {
      Names.Add(Propeller->GetName());
    }
  }

  return Names;
}

/* //} */

/* setPropellerSpeeds() //{ */

bool ARobotPawn::SetPropellerSpeeds(const std::vector<double>& Speeds, int32& OutCount)
{
  const TArray<UStaticMeshComponent*> Propellers = AllPropellers();
  OutCount                                       = Propellers.Num();

  PropellerSpeeds.Reset();

  for (const double Speed : Speeds)
  {
    PropellerSpeeds.Add(Speed);
  }

  if (PropellerSpeeds.Num() > 0)
  {
    // the Blueprint's own spin would fight the speeds set here
    propellers_rotate = false;
  }

  UE_LOG(LogTemp, Log, TEXT("SetPropellerSpeeds: %d values for %d propellers"), PropellerSpeeds.Num(), OutCount);

  return true;
}

/* //} */
