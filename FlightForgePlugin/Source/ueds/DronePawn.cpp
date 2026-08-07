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
#include "PhysicalMaterials/PhysicalMaterial.h"
#include <cereal/details/helpers.hpp>

#include "NaniteSceneProxy.h"
#include "Kismet/BlueprintTypeConversions.h"
#include "Misc/FileHelper.h"

//}


/* ADronePawn() //{ */

// Sets default values
ADronePawn::ADronePawn() {

  // UE_LOG(LogTemp, Warning, TEXT("ADronePawn constructor"));

  PrimaryActorTick.bCanEverTick = true;
  PrimaryActorTick.TickGroup    = TG_PrePhysics;

#if PLATFORM_WINDOWS
  RgbCameraBufferCriticalSection    = std::make_unique<FWindowsCriticalSection>();
  StereoCameraBufferCriticalSection = std::make_unique<FWindowsCriticalSection>();
  RgbSegCameraBufferCriticalSection = std::make_unique<FWindowsCriticalSection>();
#else
  RgbCameraBufferCriticalSection    = std::make_unique<FPThreadsCriticalSection>();
  StereoCameraBufferCriticalSection = std::make_unique<FPThreadsCriticalSection>();
  RgbSegCameraBufferCriticalSection = std::make_unique<FPThreadsCriticalSection>();
#endif

  InstructionQueue = std::make_unique<TQueue<std::shared_ptr<FInstruction<ADronePawn>>>>();

  RootMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMeshComponent"));

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

  SceneCaptureMeshHolderRgb = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SceneCaptureMeshHolderRgb"));
  SceneCaptureMeshHolderRgb->SetupAttachment(RootMeshComponent);
  
  SceneCaptureMeshHolderStereoLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SceneCaptureMeshHolderStereoLeft"));
  SceneCaptureMeshHolderStereoLeft->SetupAttachment(RootMeshComponent);

  SceneCaptureMeshHolderStereoRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SceneCaptureMeshHolderStereoRight"));
  SceneCaptureMeshHolderStereoRight->SetupAttachment(RootMeshComponent);

  SceneCaptureComponent2DRgb = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2DRgb"));
  SceneCaptureComponent2DRgb->SetupAttachment(SceneCaptureMeshHolderRgb);

  SceneCaptureComponent2DRgbSeg = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2DRgbSeg"));
  SceneCaptureComponent2DRgbSeg->SetupAttachment(SceneCaptureMeshHolderRgb);

  SceneCaptureComponent2DStereoLeft = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2DStereoLeft"));
  SceneCaptureComponent2DStereoLeft->SetupAttachment(SceneCaptureMeshHolderStereoLeft);

  SceneCaptureComponent2DStereoRight = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2DStereoRight"));
  SceneCaptureComponent2DStereoRight->SetupAttachment(SceneCaptureMeshHolderStereoRight);

  ThirdPersonCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("ThirdPersonCameraSpringArm"));
  ThirdPersonCameraSpringArm->SetupAttachment(RootMeshComponent);
  ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
  ThirdPersonCamera->SetupAttachment(ThirdPersonCameraSpringArm);

  LidarConfig.ShowBeams = DEFAULT_LIDAR_SHOW_BEAMS;

  LidarConfig.BeamHorRays  = DEFAULT_LIDAR_BEAM_HOR;
  LidarConfig.BeamVertRays = DEFAULT_LIDAR_BEAM_VER;
  LidarConfig.Frequency    = 10;

  LidarConfig.BeamLength  = DEFAULT_LIDAR_BEAM_LENGTH;
  LidarConfig.Offset      = FVector(0, 0, 0);
  LidarConfig.Orientation = FRotator(0, 0, 0);
  LidarConfig.FOVVertUp   = 52.0;
  LidarConfig.FOVVertDown = 7.0;
  LidarConfig.FOVHorLeft  = 180.0;
  LidarConfig.FOVHorRight = 180.0;
  LidarConfig.vertRayDiff = (double)(LidarConfig.FOVVertUp + LidarConfig.FOVVertDown) / (double)(LidarConfig.BeamVertRays - 1.0);
  LidarConfig.horRayDif   = (double)(LidarConfig.FOVHorLeft + LidarConfig.FOVHorRight) / (double)LidarConfig.BeamHorRays;

  LidarConfig.Livox       = false;
  LidarConfig.LivoxSensor = ELivoxSensor::Mid360;

  FTimerHandle OusterTimerHandle;

  RangefinderConfig.BeamLength = DEFAULT_RANGEFINDER_BEAM_LENGTH;
  RangefinderConfig.Offset     = FVector(0, 0, -10);

#if PLATFORM_WINDOWS
  LidarHitsCriticalSection       = std::make_unique<FWindowsCriticalSection>();
  LidarSegHitsCriticalSection    = std::make_unique<FWindowsCriticalSection>();
  LidarIntHitsCriticalSection    = std::make_unique<FWindowsCriticalSection>();
  RangefinderHitsCriticalSection = std::make_unique<FWindowsCriticalSection>();
#else
  LidarHitsCriticalSection          = std::make_unique<FPThreadsCriticalSection>();
  LidarSegHitsCriticalSection       = std::make_unique<FPThreadsCriticalSection>();
  LidarIntHitsCriticalSection       = std::make_unique<FPThreadsCriticalSection>();
  RangefinderHitsCriticalSection    = std::make_unique<FPThreadsCriticalSection>();
#endif

  LidarHits     = std::make_unique<std::vector<std::tuple<double, double, double, double>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
  LidarSegHits  = std::make_unique<std::vector<std::tuple<double, double, double, double, int>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
  LidarIntHits  = std::make_unique<std::vector<std::tuple<double, double, double, double, int>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
  LidarHitStart = std::make_unique<FVector>();
  LidarHitStart = std::make_unique<FVector>();

  droneServer = std::make_shared<DroneServer>(*this);
}

//}

/* beginPlay() //{ */

// Called when the game starts or when spawned
void ADronePawn::BeginPlay() {

  Super::BeginPlay();

  // TODO check https://github.com/TimmHess/UnrealImageCapture
  RenderTarget2DRgb = NewObject<UTextureRenderTarget2D>();
  RenderTarget2DRgb->InitCustomFormat(640, 480, PF_B8G8R8A8, false);
  RenderTarget2DRgb->RenderTargetFormat = RTF_RGBA8;
  RenderTarget2DRgb->bGPUSharedFlag     = true;

  RenderTarget2DStereoLeft = NewObject<UTextureRenderTarget2D>();
  RenderTarget2DStereoLeft->InitCustomFormat(640, 480, PF_B8G8R8A8, false);
  RenderTarget2DStereoLeft->RenderTargetFormat = RTF_RGBA8;
  RenderTarget2DStereoLeft->bGPUSharedFlag     = true;

  RenderTarget2DStereoRight = NewObject<UTextureRenderTarget2D>();
  RenderTarget2DStereoRight->InitCustomFormat(640, 480, PF_B8G8R8A8, false);
  RenderTarget2DStereoRight->RenderTargetFormat = RTF_RGBA8;
  RenderTarget2DStereoRight->bGPUSharedFlag     = true;

  RenderTarget2DRgbSeg = NewObject<UTextureRenderTarget2D>();
  RenderTarget2DRgbSeg->InitCustomFormat(640, 480, PF_B8G8R8A8, false);
  RenderTarget2DRgbSeg->RenderTargetFormat = RTF_RGBA8;
  RenderTarget2DRgbSeg->bGPUSharedFlag     = true;
  
  SceneCaptureComponent2DRgb->CaptureSource = SCS_FinalColorHDR;
  SceneCaptureComponent2DRgb->TextureTarget = RenderTarget2DRgb;
  SceneCaptureComponent2DRgb->ShowFlags.SetTemporalAA(true);
  SceneCaptureComponent2DRgb->bAlwaysPersistRenderingState = true;
  SceneCaptureComponent2DRgb->bCaptureEveryFrame           = false;
  SceneCaptureComponent2DRgb->bCaptureOnMovement           = false;
  SceneCaptureComponent2DRgb->bUseRayTracingIfEnabled      = true;

  SceneCaptureComponent2DRgbSeg->CaptureSource = SCS_FinalColorHDR;
  SceneCaptureComponent2DRgbSeg->TextureTarget = RenderTarget2DRgbSeg;
  SceneCaptureComponent2DRgbSeg->ShowFlags.SetTemporalAA(true);
  SceneCaptureComponent2DRgbSeg->bAlwaysPersistRenderingState = true;
  SceneCaptureComponent2DRgbSeg->bCaptureEveryFrame           = false;
  SceneCaptureComponent2DRgbSeg->bCaptureOnMovement           = false;
  SceneCaptureComponent2DRgbSeg->bUseRayTracingIfEnabled      = false;
  SceneCaptureComponent2DRgbSeg->AddOrUpdateBlendable(PostProcessMaterial, 1);

  SceneCaptureComponent2DStereoLeft->CaptureSource = SCS_FinalColorHDR;
  SceneCaptureComponent2DStereoLeft->TextureTarget = RenderTarget2DStereoLeft;
  SceneCaptureComponent2DStereoLeft->ShowFlags.SetTemporalAA(true);
  SceneCaptureComponent2DStereoLeft->bAlwaysPersistRenderingState = true;
  SceneCaptureComponent2DStereoLeft->bCaptureEveryFrame           = false;
  SceneCaptureComponent2DStereoLeft->bCaptureOnMovement           = false;
  SceneCaptureComponent2DStereoLeft->bUseRayTracingIfEnabled      = true;

  SceneCaptureComponent2DStereoRight->CaptureSource = SCS_FinalColorHDR;
  SceneCaptureComponent2DStereoRight->TextureTarget = RenderTarget2DStereoRight;
  SceneCaptureComponent2DStereoRight->ShowFlags.SetTemporalAA(true);
  SceneCaptureComponent2DStereoRight->bAlwaysPersistRenderingState = true;
  SceneCaptureComponent2DStereoRight->bCaptureEveryFrame           = false;
  SceneCaptureComponent2DStereoRight->bCaptureOnMovement           = false;
  SceneCaptureComponent2DStereoRight->bUseRayTracingIfEnabled      = true;

  rgb_camera_config_.ShowCameraComponent    = false;
  rgb_camera_config_.Offset                 = FVector(0, 0, 0);
  rgb_camera_config_.Orientation            = FRotator(0, 0, 0);
  rgb_camera_config_.FOVAngle               = 90;
  rgb_camera_config_.Width                  = 640;
  rgb_camera_config_.Height                 = 480;
  rgb_camera_config_.enable_motion_blur     = true;
  rgb_camera_config_.motion_blur_amount     = 1.0;
  rgb_camera_config_.motion_blur_distortion = 50.0;

  stereo_camera_config_.ShowCameraComponent = false;
  stereo_camera_config_.Offset_left              = FVector(0, 0, 0);
  stereo_camera_config_.Orientation_left         = FRotator(0, 0, 0);
  stereo_camera_config_.Offset_right             = FVector(0, 0, 0);
  stereo_camera_config_.Orientation_right        = FRotator(0, 0, 0);
  stereo_camera_config_.FOVAngle            = 90;
  stereo_camera_config_.Width               = 640;
  stereo_camera_config_.Height              = 480;

  SetRgbCameraConfig(rgb_camera_config_);
  SetStereoCameraConfig(stereo_camera_config_);

}

//}

/* startServer() //{ */

void ADronePawn::StartServer() {
  // UE_LOG(LogTemp, Warning, TEXT("Starting drone server"));
  droneServer->Run();
}

//}

/* endPlay() //{ */

void ADronePawn::EndPlay(const EEndPlayReason::Type EndPlayReason) {

  RgbCameraBuffer.Empty();
  StereoLeftCameraBuffer.Empty();
  StereoRightCameraBuffer.Empty();
  droneServer->Stop();
}

//}

/* setupPlayerInputComponent() //{ */

// Called to bind functionality to input
void ADronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);
}

//}

/* materialIntensity() //{ */

namespace
{
// Intensity doubles as a material class id here, not a radiometric return.
int MaterialIntensity(const FString& PhysMatName) {

  if (PhysMatName.Equals("PM_Grass", ESearchCase::IgnoreCase)) {
    return 1;
  } else if (PhysMatName.Equals("PM_Road", ESearchCase::IgnoreCase)) {
    return 2;
  } else if (PhysMatName.Equals("PM_Tree", ESearchCase::IgnoreCase)) {
    return 3;
  } else if (PhysMatName.Equals("PM_Building", ESearchCase::IgnoreCase)) {
    return 4;
  } else if (PhysMatName.Equals("PM_Fence", ESearchCase::IgnoreCase)) {
    return 5;
  } else if (PhysMatName.Equals("PM_DirtRoad", ESearchCase::IgnoreCase)) {
    return 6;
  }

  return 0;
}
}  // namespace

//}

/* getLivoxPattern() //{ */

const FLivoxPattern* ADronePawn::GetLivoxPattern() const {
  return FLivoxPatternLibrary::Get(LidarConfig.LivoxSensor);
}

//}

/* castLivoxRays() //{ */

// A stalled client would otherwise ask for an unbounded slice on its next poll;
// one second of Avia is already 240k points.
#define LIVOX_MAX_INTEGRATION_TIME 1.0

// Rays are traced in chunks this long, each from the drone pose interpolated to
// the middle of the chunk. 1 ms is ~240 rays on Avia, fine enough that the
// residual within a chunk is far below the range noise of the real sensor.
#define LIVOX_SUB_BATCH_TIME 0.001

int32 ADronePawn::CastLivoxRays(FLivoxCursor& Cursor, TFunctionRef<void(int32)> Prepare,
                                TFunctionRef<void(int32, double, const FVector&, const FHitResult&, bool)> Store) {

  const FLivoxPattern* Pattern = GetLivoxPattern();

  if (!Pattern || Pattern->Num() == 0) {
    return 0;
  }

  auto World = GetWorld();

  if (!World) {
    return 0;
  }

  const double Now = World->GetTimeSeconds();

  // First slice after enabling or reconfiguring: hand out one nominal period so
  // the client gets a sane message instead of an empty or enormous one.
  if (Cursor.LastTime < 0.0) {
    const double NominalPeriod = 1.0 / FMath::Max(LidarConfig.Frequency, 1.0);
    Cursor.LastTime            = Now - NominalPeriod;
  }

  double Elapsed = Now - Cursor.LastTime;

  // World time restarts on a level switch.
  if (Elapsed < 0.0) {
    Elapsed = 1.0 / FMath::Max(LidarConfig.Frequency, 1.0);
  }

  if (Elapsed > LIVOX_MAX_INTEGRATION_TIME) {
    UE_LOG(LogTemp, Warning, TEXT("Livox: %.2f s since last request, clamping integration to %.2f s"), Elapsed, LIVOX_MAX_INTEGRATION_TIME);
    Elapsed = LIVOX_MAX_INTEGRATION_TIME;
  }

  const int32 NumRays = FMath::Max(1, FMath::RoundToInt(Elapsed * Pattern->PointRate));

  Prepare(NumRays);

  const FTransform DroneTransform = GetActorTransform();

  const FQuat      LidarQuat     = DroneTransform.GetRotation() * LidarConfig.Orientation.Quaternion();
  const FVector    LidarLocation = DroneTransform.TransformPosition(LidarConfig.Offset);
  const FTransform RefTransform(LidarQuat, LidarLocation);

  // The slice covers [LastTime, Now]; the pose at each end is known, so poses
  // inside it are interpolated. Output is expressed in the pose at Now, which is
  // what the response stamp refers to.
  const FTransform& FromPose = Cursor.bPoseValid ? Cursor.LastPose : RefTransform;

  const int32 PatternStart   = (int32)FMath::Fmod(Cursor.PatternPos, (double)Pattern->Num());
  const int32 RaysPerBatch   = FMath::Max(1, FMath::RoundToInt(LIVOX_SUB_BATCH_TIME * Pattern->PointRate));
  const int32 NumBatches     = FMath::DivideAndRoundUp(NumRays, RaysPerBatch);
  const double MinRangeCm    = Pattern->MinRangeCm;
  const double BeamLength    = LidarConfig.BeamLength;

  ParallelFor(NumBatches, [&](int32 BatchIndex) {
    const double Alpha = NumBatches > 1 ? ((double)BatchIndex + 0.5) / (double)NumBatches : 1.0;

    const FTransform BatchPose(FQuat::Slerp(FromPose.GetRotation(), RefTransform.GetRotation(), Alpha),
                               FMath::Lerp(FromPose.GetLocation(), RefTransform.GetLocation(), Alpha));

    const FVector BatchOrigin = BatchPose.GetLocation();

    const int32 First = BatchIndex * RaysPerBatch;
    const int32 Last  = FMath::Min(First + RaysPerBatch, NumRays);

    FCollisionQueryParams CollisionParams = FCollisionQueryParams::DefaultQueryParam;

    if (LidarConfig.ShowBeams) {
      CollisionParams.TraceTag = FName(TEXT("LivoxTraceTag"));
    }

    for (int32 i = First; i < Last; ++i) {

      const FVector3f& Dir3f = Pattern->Directions[(PatternStart + i) % Pattern->Num()];
      const FVector    PatternDir(Dir3f.X, Dir3f.Y, Dir3f.Z);

      const FVector RayWorld = BatchPose.TransformVector(PatternDir) * BeamLength;

      // Fallback for a non-return: the nominal ray, expressed in the reference frame.
      FVector  OutDir = RefTransform.InverseTransformVector(RayWorld);
      double   OutDistance = -1.0;
      bool     bHasReturn  = false;

      FHitResult HitResult;

      if (World->LineTraceSingleByChannel(HitResult, BatchOrigin, BatchOrigin + RayWorld, ECC_Visibility, CollisionParams) && HitResult.bBlockingHit) {

        AActor* HitActor = HitResult.GetActor();

        const bool bHiddenDrone = !bCanSeeOtherDrone && HitActor && HitActor->IsA(ADronePawn::StaticClass()) && HitActor != this;

        if (!bHiddenDrone && HitResult.Distance >= MinRangeCm) {
          // Re-expressing the world hit in the reference frame is what bakes the
          // intra-scan motion distortion into the geometry, while keeping the
          // wire format (distance + direction) unchanged.
          const FVector Local = RefTransform.InverseTransformPosition(HitResult.ImpactPoint);

          OutDistance = Local.Size();
          OutDir      = Local.GetSafeNormal() * BeamLength;
          bHasReturn  = true;
        }
      }

      Store(i, OutDistance, OutDir, HitResult, bHasReturn);
    }
  });

  Cursor.PatternPos = FMath::Fmod(Cursor.PatternPos + NumRays, (double)Pattern->Num());
  Cursor.LastTime   = Now;
  Cursor.LastPose = RefTransform;
  Cursor.bPoseValid = true;

  LidarHitStart.reset(new FVector(LidarLocation));

  return NumRays;
}

//}

/* updateLidar() //{ */
void ADronePawn::UpdateLidar(bool isExternallyLocked) {
    if (!isExternallyLocked) {
        LidarHitsCriticalSection->Lock();
    }

    auto World = GetWorld();
    const auto droneTransform = GetActorTransform();

    // --- Invert Roll ---
    FRotator correctedLidarOrientation = LidarConfig.Orientation;
    if(!LidarConfig.Livox){
    correctedLidarOrientation.Roll = -correctedLidarOrientation.Roll;
    }

    // Lidar's global transform
    FQuat lidarQuat = droneTransform.GetRotation() * correctedLidarOrientation.Quaternion();
    FVector lidarLocation = droneTransform.TransformPosition(LidarConfig.Offset);
    FTransform lidarGlobalTransform(lidarQuat, lidarLocation);


    if (LidarConfig.Livox) {

        LidarHits = std::make_unique<std::vector<std::tuple<double, double, double, double>>>();

        CastLivoxRays(
            LivoxCursor, [&](int32 NumRays) { LidarHits->resize(NumRays); },
            [&](int32 i, double Distance, const FVector& Dir, const FHitResult& Hit, bool bHasReturn) {
                (*LidarHits)[i] = std::make_tuple(Distance, Dir.X, Dir.Y, Dir.Z);
            });

    } else {

        LidarHits = std::make_unique<std::vector<std::tuple<double, double, double, double>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);

        FVector forwardVec = lidarQuat.RotateVector(FVector::ForwardVector);
        FVector rightVector   = lidarQuat.RotateVector(FVector::RightVector);
        FVector upVec        = lidarQuat.RotateVector(FVector::UpVector);

        double totalHorFov = LidarConfig.FOVHorLeft + LidarConfig.FOVHorRight;
        double totalVertFov = LidarConfig.FOVVertUp + LidarConfig.FOVVertDown;

        ParallelFor(LidarConfig.BeamHorRays, [&](int32 row) {
            double horAngle = -LidarConfig.FOVHorLeft + (totalHorFov * (double)row / (double)(LidarConfig.BeamHorRays - 1.0));
            FVector rotatedForward = forwardVec.RotateAngleAxis(horAngle, upVec);
            FVector rotatedRight = rightVector.RotateAngleAxis(horAngle, upVec);


            ParallelFor(LidarConfig.BeamVertRays, [&](int32 col) {
                auto CollisionParams = FCollisionQueryParams::DefaultQueryParam;
                if (LidarConfig.ShowBeams) {
                    const FName TraceTag(FString::Printf(TEXT("LidarTraceTag_%d"), 0));
                    CollisionParams.TraceTag = TraceTag;
                }

                double vertAngle = -LidarConfig.FOVVertDown + (totalVertFov * (double)col / (double)(LidarConfig.BeamVertRays - 1.0));

                // Ray direction in *world* coordinates.
                FVector raycastAngle_world = rotatedForward.RotateAngleAxis(-vertAngle, rotatedRight);
                raycastAngle_world *= LidarConfig.BeamLength;

                FHitResult HitResult;
                int i = row * LidarConfig.BeamVertRays + col;

                if (World->LineTraceSingleByChannel(HitResult, lidarLocation, lidarLocation + raycastAngle_world, ECC_Visibility, CollisionParams))
                {
                    FVector ray_in_lidar_coord = lidarGlobalTransform.InverseTransformVector(raycastAngle_world);

                     if (HitResult.bBlockingHit) {
                        AActor* HitActor = HitResult.GetActor();
                        if (!bCanSeeOtherDrone && HitActor && HitActor->IsA(ADronePawn::StaticClass()) && HitActor != this)
                        {
                           (*LidarHits)[i] = std::make_tuple(HitResult.Distance, ray_in_lidar_coord.X, ray_in_lidar_coord.Y, ray_in_lidar_coord.Z);
                        } else {
                            (*LidarHits)[i] = std::make_tuple(HitResult.Distance, ray_in_lidar_coord.X, ray_in_lidar_coord.Y, ray_in_lidar_coord.Z);
                        }
                    } else {
                        (*LidarHits)[i] = std::make_tuple(-1.0, ray_in_lidar_coord.X, ray_in_lidar_coord.Y, ray_in_lidar_coord.Z);
                    }
                }
                else {
                    FVector ray_in_lidar_coord = lidarGlobalTransform.InverseTransformVector(raycastAngle_world);
                    (*LidarHits)[i] = std::make_tuple(-1.0, ray_in_lidar_coord.X, ray_in_lidar_coord.Y, ray_in_lidar_coord.Z); 
                }
            });
        });
        LidarHitStart.reset(new FVector(lidarLocation));
    }

    if (!isExternallyLocked) {
        LidarHitsCriticalSection->Unlock();
    }
}
//}

/* updateSegLidar() //{ */

void ADronePawn::UpdateSegLidar(bool isExternallyLocked) {

  if (!isExternallyLocked) {
    LidarSegHitsCriticalSection->Lock();
  }

  auto       World          = GetWorld();
  const auto droneTransform = GetActorTransform();

    // --- Invert Roll to match ROS conventions ---
    FRotator correctedLidarOrientation = LidarConfig.Orientation;
    if(!LidarConfig.Livox){
        correctedLidarOrientation.Roll = -correctedLidarOrientation.Roll;
    }

    // Compose the lidar's global transform
    FQuat lidarQuat = droneTransform.GetRotation() * correctedLidarOrientation.Quaternion();
    FVector lidarLocation = droneTransform.TransformPosition(LidarConfig.Offset);
    FTransform lidarGlobalTransform(lidarQuat, lidarLocation);



  if (LidarConfig.Livox) {

        LidarSegHits = std::make_unique<std::vector<std::tuple<double, double, double, double, int>>>();

        CastLivoxRays(
            LivoxSegCursor, [&](int32 NumRays) { LidarSegHits->resize(NumRays); },
            [&](int32 i, double Distance, const FVector& Dir, const FHitResult& Hit, bool bHasReturn) {
                int segmentation = -1;

                if (bHasReturn && Hit.GetComponent()) {
                    if (Hit.GetComponent()->GetName().Contains("Landscape")) {
                        segmentation = 1;
                    } else if (auto component = Cast<UStaticMeshComponent>(Hit.GetComponent())) {
                        segmentation = component->CustomDepthStencilValue;
                    }
                }

                (*LidarSegHits)[i] = std::make_tuple(Distance, Dir.X, Dir.Y, Dir.Z, segmentation);
            });

    } else {

        LidarSegHits = std::make_unique<std::vector<std::tuple<double, double, double, double, int>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);

        // Derive the lidar's world-space axes (these are now consistent)
        FVector forwardVec = lidarQuat.RotateVector(FVector::ForwardVector);
        FVector rightVector   = lidarQuat.RotateVector(FVector::RightVector);
        FVector upVec        = lidarQuat.RotateVector(FVector::UpVector);


        // Calculate total horizontal FOV and vertical FOV
        double totalHorFov  = LidarConfig.FOVHorLeft + LidarConfig.FOVHorRight;
        double totalVertFov = LidarConfig.FOVVertUp + LidarConfig.FOVVertDown;

        ParallelFor(LidarConfig.BeamHorRays, [&](int32 row) {
            // Calculate the horizontal angle for the current ray
            double  horAngle       = -LidarConfig.FOVHorLeft + (totalHorFov * (double)row / (double)(LidarConfig.BeamHorRays - 1.0));
            FVector rotatedForward = forwardVec.RotateAngleAxis(horAngle, upVec);
            FVector rotatedRight   = rightVector.RotateAngleAxis(horAngle, upVec);

            ParallelFor(LidarConfig.BeamVertRays, [&](int32 col) {
            auto CollisionParams = FCollisionQueryParams::DefaultQueryParam;

            // Calculate the vertical angle for the current ray
            double vertAngle = -LidarConfig.FOVVertDown + (totalVertFov * (double)col / (double)(LidarConfig.BeamVertRays - 1.0));

            // Calculate the direction of the ray
            FVector raycastAngle = rotatedForward.RotateAngleAxis(-vertAngle, rotatedRight); // Note the -vertAngle
            raycastAngle *= LidarConfig.BeamLength;

            FHitResult HitResult;
            int i = row * LidarConfig.BeamVertRays + col;

            if (World->LineTraceSingleByChannel(HitResult, lidarLocation, lidarLocation + raycastAngle, ECollisionChannel::ECC_Visibility, CollisionParams)) {

                if (HitResult.bBlockingHit) {

                AActor* HitActor = HitResult.GetActor();
                if (!bCanSeeOtherDrone && HitActor && HitActor->IsA(ADronePawn::StaticClass()) && HitActor != this)
                {
                    // skip
                    std::get<0>((*LidarSegHits)[i]) = -1;
                    // UE_LOG(LogTemp, Warning, TEXT("Ignoring actor: %s"), *HitActor->GetName());
                } else {
                    std::get<0>((*LidarSegHits)[i]) = HitResult.IsValidBlockingHit() ? HitResult.Distance : LidarConfig.BeamLength;
                }

                // Transform ray into Lidar coordinates
                const auto ray_in_lidar_coord   = lidarGlobalTransform.InverseTransformVector(raycastAngle);

                std::get<1>((*LidarSegHits)[i]) = ray_in_lidar_coord.X;
                std::get<2>((*LidarSegHits)[i]) = ray_in_lidar_coord.Y;
                std::get<3>((*LidarSegHits)[i]) = ray_in_lidar_coord.Z;

                auto component = Cast<UStaticMeshComponent>(HitResult.GetComponent());

                if (HitResult.GetComponent()->GetName().Contains("Landscape")) {
                    std::get<4>((*LidarSegHits)[i]) = 1;
                } else {
                    std::get<4>((*LidarSegHits)[i]) = component->CustomDepthStencilValue;
                }

                } else {

                std::get<0>((*LidarSegHits)[i]) = -1;
                    // Transform ray into Lidar coordinates
                const auto ray_in_lidar_coord = lidarGlobalTransform.InverseTransformVector(raycastAngle);

                std::get<1>((*LidarSegHits)[i]) = ray_in_lidar_coord.X;
                std::get<2>((*LidarSegHits)[i]) = ray_in_lidar_coord.Y;
                std::get<3>((*LidarSegHits)[i]) = ray_in_lidar_coord.Z;
                std::get<4>((*LidarSegHits)[i]) = -1;
                }
            }
                //added for consistency, copied from updatelidar
                else {
                    std::get<0>((*LidarSegHits)[i]) = -1;

                    // Transform the ray direction into the lidar's local coordinate frame.
                    FVector ray_in_lidar_coord = lidarGlobalTransform.InverseTransformVector(raycastAngle);
                    std::get<1>((*LidarSegHits)[i]) = ray_in_lidar_coord.X;
                    std::get<2>((*LidarSegHits)[i]) = ray_in_lidar_coord.Y;
                    std::get<3>((*LidarSegHits)[i]) = ray_in_lidar_coord.Z;
                    std::get<4>((*LidarSegHits)[i]) = -1;  // Important: Set segmentation to -1 for no hit
                }
            });
        });
    }
  LidarHitStart.reset(new FVector(lidarLocation));

  if (!isExternallyLocked) {
    LidarSegHitsCriticalSection->Unlock();
  }
}

//}

/* updateIntLidar() //{ */

void ADronePawn::UpdateIntLidar(bool isExternallyLocked) {

  if (!isExternallyLocked) {
    LidarIntHitsCriticalSection->Lock();
  }

  auto       World          = GetWorld();
  const auto droneTransform = GetActorTransform();

     // --- Invert Roll to match ROS conventions ---
    FRotator correctedLidarOrientation = LidarConfig.Orientation;
     if(!LidarConfig.Livox){
        correctedLidarOrientation.Roll = -correctedLidarOrientation.Roll;
    }
    // Compose the lidar's global transform: first apply the drone's rotation, then the corrected lidar orientation,
    // and finally translate by the lidar offset.
    FQuat lidarQuat = droneTransform.GetRotation() * correctedLidarOrientation.Quaternion();
    FVector lidarLocation = droneTransform.TransformPosition(LidarConfig.Offset);
    FTransform lidarGlobalTransform(lidarQuat, lidarLocation);


    if (LidarConfig.Livox) {

        LidarIntHits = std::make_unique<std::vector<std::tuple<double, double, double, double, int>>>();

        CastLivoxRays(
            LivoxIntCursor, [&](int32 NumRays) { LidarIntHits->resize(NumRays); },
            [&](int32 i, double Distance, const FVector& Dir, const FHitResult& Hit, bool bHasReturn) {
                int intensity = -1;

                if (bHasReturn) {
                    if (UPhysicalMaterial* PhysMat = Hit.PhysMaterial.Get()) {
                        intensity = MaterialIntensity(PhysMat->GetName());
                    }
                }

                (*LidarIntHits)[i] = std::make_tuple(Distance, Dir.X, Dir.Y, Dir.Z, intensity);
            });

    }else{

        LidarIntHits = std::make_unique<std::vector<std::tuple<double, double, double, double, int>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);

        // Derive the lidar's world-space axes
        FVector forwardVec = lidarQuat.RotateVector(FVector::ForwardVector);
        FVector rightVector   = lidarQuat.RotateVector(FVector::RightVector);
        FVector upVec        = lidarQuat.RotateVector(FVector::UpVector);



        // Calculate total horizontal FOV and vertical FOV
        double totalHorFov  = LidarConfig.FOVHorLeft + LidarConfig.FOVHorRight;
        double totalVertFov = LidarConfig.FOVVertUp + LidarConfig.FOVVertDown;

        ParallelFor(LidarConfig.BeamHorRays, [&](int32 row) {
            // Calculate the horizontal angle for the current ray
            double  horAngle       = -LidarConfig.FOVHorLeft + (totalHorFov * (double)row / (double)(LidarConfig.BeamHorRays - 1.0));
            FVector rotatedForward = forwardVec.RotateAngleAxis(horAngle, upVec);
            FVector rotatedRight   = rightVector.RotateAngleAxis(horAngle, upVec);

            ParallelFor(LidarConfig.BeamVertRays, [&](int32 col) {
            auto CollisionParams                    = FCollisionQueryParams::DefaultQueryParam;
            CollisionParams.bReturnPhysicalMaterial = true;

            // Calculate the vertical angle for the current ray
            double vertAngle = -LidarConfig.FOVVertDown + (totalVertFov * (double)col / (double)(LidarConfig.BeamVertRays - 1.0));

            // Calculate the direction of the ray
            FVector raycastAngle = rotatedForward.RotateAngleAxis(-vertAngle, rotatedRight); // Note the -vertAngle
            raycastAngle *= LidarConfig.BeamLength;


            FHitResult HitResult;
                int i = row * LidarConfig.BeamVertRays + col;

            if (World->LineTraceSingleByChannel(HitResult, lidarLocation, lidarLocation + raycastAngle, ECollisionChannel::ECC_Visibility, CollisionParams)) {
                if (HitResult.bBlockingHit) {

                AActor* HitActor = HitResult.GetActor();
                if (!bCanSeeOtherDrone && HitActor && HitActor->IsA(ADronePawn::StaticClass()) && HitActor != this)
                {
                    // skip
                    std::get<0>((*LidarHits)[i]) = -1;
                    // UE_LOG(LogTemp, Warning, TEXT("Ignoring actor: %s"), *HitActor->GetName());
                } else {
                    std::get<0>((*LidarIntHits)[i]) = HitResult.IsValidBlockingHit() ? HitResult.Distance : LidarConfig.BeamLength;
                }
                
                // Transform ray into Lidar coordinates
                const auto ray_in_lidar_coord   = lidarGlobalTransform.InverseTransformVector(raycastAngle);

                std::get<1>((*LidarIntHits)[i]) = ray_in_lidar_coord.X;
                std::get<2>((*LidarIntHits)[i]) = ray_in_lidar_coord.Y;
                std::get<3>((*LidarIntHits)[i]) = ray_in_lidar_coord.Z;

                UPhysicalMaterial* PhysMat   = HitResult.PhysMaterial.Get();
                double             intensity = PhysMat ? MaterialIntensity(PhysMat->GetName()) : -1;

                std::get<4>((*LidarIntHits)[i]) = intensity;
                } else {

                std::get<0>((*LidarIntHits)[i]) = -1;
                    // Transform ray into Lidar coordinates
                const auto ray_in_lidar_coord = lidarGlobalTransform.InverseTransformVector(raycastAngle);

                std::get<1>((*LidarIntHits)[i]) = ray_in_lidar_coord.X;
                std::get<2>((*LidarIntHits)[i]) = ray_in_lidar_coord.Y;
                std::get<3>((*LidarIntHits)[i]) = ray_in_lidar_coord.Z;
                std::get<4>((*LidarIntHits)[i]) = -1;
                }
            }
                //added for consistency, copied from updatelidar
                else {
                    std::get<0>((*LidarIntHits)[i]) = -1;

                    // Transform the ray direction into the lidar's local coordinate frame.
                    FVector ray_in_lidar_coord = lidarGlobalTransform.InverseTransformVector(raycastAngle);
                    std::get<1>((*LidarIntHits)[i]) = ray_in_lidar_coord.X;
                    std::get<2>((*LidarIntHits)[i]) = ray_in_lidar_coord.Y;
                    std::get<3>((*LidarIntHits)[i]) = ray_in_lidar_coord.Z;
                    std::get<4>((*LidarIntHits)[i]) = -1;  // Set intensity -1 for no hit.
                }
            });
        });
    }
  LidarHitStart.reset(new FVector(lidarLocation));

  if (!isExternallyLocked) {
    LidarIntHitsCriticalSection->Unlock();
  }
}

//}

/* getCrashedState() //{ */

bool ADronePawn::GetCrashState(void) {

  // UE_LOG(LogTemp, Warning, TEXT("ADronePawn::GetCrashState"));

  return uav_crashed_;
}

//}

/*GetRangefinderData()//{*/
void ADronePawn::GetRangefinderData(double& range) {
  RangefinderHitsCriticalSection->Lock();

  FVector Start                = GetActorLocation() + GetActorRotation().RotateVector(RangefinderConfig.Offset);
  FVector RangefinderDirection = -GetActorUpVector();

  if (UWorld* World = GetWorld()) {
    FHitResult HitResult;
    if (World->LineTraceSingleByChannel(HitResult, Start, Start + RangefinderConfig.BeamLength * RangefinderDirection, ECollisionChannel::ECC_Visibility)) {
      if (HitResult.bBlockingHit) {
        range = HitResult.Distance;
      } else {
        range = -1;
      }
      // UE_LOG(LogTemp, Warning, TEXT("Rangefinder range = %lf"), range);
    }
  }

  RangefinderHitsCriticalSection->Unlock();
}
/*//}*/

/* getLidarHits() //{ */

void ADronePawn::GetLidarHits(std::vector<Serializable::Drone::GetLidarData::LidarData>& OutLidarData, FVector& OutStart, double& OutStamp) {

  // UE_LOG(LogTemp, Warning, TEXT("DronePawn::GetLidarHits"));

  LidarHitsCriticalSection->Lock();

  auto stamp = FPlatformTime::Seconds();
  
  UpdateLidar(true);
  
  // Livox slices vary in length with the elapsed time they cover, so the
  // buffer size is authoritative, not the configured beam grid.
  OutLidarData.resize(LidarHits->size());

  for (size_t i = 0; i < LidarHits->size(); i++) {
    OutLidarData[i].distance   = std::get<0>((*LidarHits)[i]);
    OutLidarData[i].directionX = std::get<1>((*LidarHits)[i]);
    OutLidarData[i].directionY = std::get<2>((*LidarHits)[i]);
    OutLidarData[i].directionZ = std::get<3>((*LidarHits)[i]);
  }

  OutStart.X = LidarConfig.Offset.X;
  OutStart.Y = LidarConfig.Offset.Y;
  OutStart.Z = LidarConfig.Offset.Z;

  OutStamp = stamp;

  LidarHitsCriticalSection->Unlock();
}

//}

/* getSegLidarHits() //{ */

void ADronePawn::GetSegLidarHits(std::vector<Serializable::Drone::GetLidarSegData::LidarSegData>& OutLidarSegData, FVector& OutStart, double& OutStamp) {

  LidarSegHitsCriticalSection->Lock();

  auto stamp = FPlatformTime::Seconds();

  UpdateSegLidar(true);

  OutLidarSegData.resize(LidarSegHits->size());

  for (size_t i = 0; i < LidarSegHits->size(); i++) {
    OutLidarSegData[i].distance     = std::get<0>((*LidarSegHits)[i]);
    OutLidarSegData[i].directionX   = std::get<1>((*LidarSegHits)[i]);
    OutLidarSegData[i].directionY   = std::get<2>((*LidarSegHits)[i]);
    OutLidarSegData[i].directionZ   = std::get<3>((*LidarSegHits)[i]);
    OutLidarSegData[i].segmentation = std::get<4>((*LidarSegHits)[i]);
  }

  OutStart.X = LidarConfig.Offset.X;
  OutStart.Y = LidarConfig.Offset.Y;
  OutStart.Z = LidarConfig.Offset.Z;

  OutStamp = stamp;

  LidarSegHitsCriticalSection->Unlock();
}

//}

/* getIntLidarHits() //{ */

void ADronePawn::GetIntLidarHits(std::vector<Serializable::Drone::GetLidarIntData::LidarIntData>& OutLidarIntData, FVector& OutStart, double& OutStamp) {

  LidarIntHitsCriticalSection->Lock();

  auto stamp = FPlatformTime::Seconds();

  UpdateIntLidar(true);
  OutLidarIntData.resize(LidarIntHits->size());

  for (size_t i = 0; i < LidarIntHits->size(); i++) {
    OutLidarIntData[i].distance   = std::get<0>((*LidarIntHits)[i]);
    OutLidarIntData[i].directionX = std::get<1>((*LidarIntHits)[i]);
    OutLidarIntData[i].directionY = std::get<2>((*LidarIntHits)[i]);
    OutLidarIntData[i].directionZ = std::get<3>((*LidarIntHits)[i]);
    OutLidarIntData[i].intensity  = std::get<4>((*LidarIntHits)[i]);
  }

  OutStart.X = LidarConfig.Offset.X;
  OutStart.Y = LidarConfig.Offset.Y;
  OutStart.Z = LidarConfig.Offset.Z;

  OutStamp = stamp;

  LidarIntHitsCriticalSection->Unlock();
}

//}

/* updateCamera() //{ */

void ADronePawn::UpdateCamera(bool isExternallyLocked, int type = 1, double stamp = 0.0) {

  switch (type) {

    case CAMERA_MODE_NONE: {
      break;
    }

    case CAMERA_MODE_RGB: {

      if (!isExternallyLocked) {
        RgbCameraBufferCriticalSection->Lock();
      }

      const auto Resource = RenderTarget2DRgb->GameThread_GetRenderTargetResource();

      Resource->ReadPixels(RgbCameraBuffer);

      RgbCameraDataNeedsCompress = true;

      rgb_stamp_ = stamp;

      RgbCameraRendered = true;

      if (!isExternallyLocked) {
        RgbCameraBufferCriticalSection->Unlock();
      }

      break;
    }

    case CAMERA_MODE_STEREO: {

      if (!isExternallyLocked) {
        StereoCameraBufferCriticalSection->Lock();
      }

      const auto ResourceLeft = RenderTarget2DStereoLeft->GameThread_GetRenderTargetResource();

      const auto ResourceRight = RenderTarget2DStereoRight->GameThread_GetRenderTargetResource();

      ResourceLeft->ReadPixels(StereoLeftCameraBuffer);
      ResourceRight->ReadPixels(StereoRightCameraBuffer);

      StereoCameraDataNeedsCompress = true;

      StereoCameraRendered = true;

      stereo_stamp_ = stamp;

      if (!isExternallyLocked) {
        StereoCameraBufferCriticalSection->Unlock();
      }

      break;
    }

    case CAMERA_MODE_RGB_SEG: {

      SceneCaptureComponent2DRgbSeg->CaptureSource = SCS_FinalColorLDR;
      SceneCaptureComponent2DRgbSeg->TextureTarget = RenderTarget2DRgbSeg;
      // SceneCaptureComponent2DRgb->CaptureScene();

      if (!isExternallyLocked) {
        RgbSegCameraBufferCriticalSection->Lock();
      }

      const auto ResourceSeg = RenderTarget2DRgbSeg->GameThread_GetRenderTargetResource();

      ResourceSeg->ReadPixels(RgbSegCameraBuffer);

      RgbSegCameraDataNeedsCompress = false;

      RgbSegCameraRendered = true;

      rgb_seg_stamp_ = stamp;

      if (!isExternallyLocked) {
        RgbSegCameraBufferCriticalSection->Unlock();
      }

      break;
    }

    default: {
      SceneCaptureComponent2DRgb->CaptureSource = SCS_FinalColorLDR;
      SceneCaptureComponent2DRgb->TextureTarget = RenderTarget2DRgb;
      break;
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

//}

/* GetRgbCameraDataFromServerThread() //{ */

bool ADronePawn::GetRgbCameraDataFromServerThread(TArray<uint8>& OutArray, double& stamp) {

  RgbCameraBufferCriticalSection->Lock();

  rgb_camera_last_request_time_ = FPlatformTime::Seconds();

  if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND) {
    auto Instruction      = std::make_shared<FInstruction<ADronePawn>>();
    Instruction->Function = [](ADronePawn& _DronePawn) { _DronePawn.UpdateCamera(true, CAMERA_MODE_RGB); };
    InstructionQueue->Enqueue(Instruction);

    FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });
  }

  if (!RgbCameraRendered) {
    UE_LOG(LogTemp, Warning, TEXT("DronePawn(): rgb camera is not rendered, returning"));
    RgbCameraBufferCriticalSection->Unlock();
    return false;
  }

  if (RgbCameraDataNeedsCompress) {
    FImageUtils::ThumbnailCompressImageArray(RenderTarget2DRgb->SizeX, RenderTarget2DRgb->SizeY, RgbCameraBuffer, *CompressedRgbCameraData);
    RgbCameraDataNeedsCompress = false;
  }

  const auto Size = (*CompressedRgbCameraData).Num();
  OutArray.SetNumUninitialized(Size);
  FMemory::Memcpy(OutArray.GetData(), (*CompressedRgbCameraData).GetData(), Size * sizeof(uint8));

  stamp = rgb_stamp_;

  RgbCameraBufferCriticalSection->Unlock();

  return true;
}

bool ADronePawn::GetStereoCameraDataFromServerThread(TArray<uint8>& image_left, TArray<uint8>& image_right, double& stamp) {

  {
    StereoCameraBufferCriticalSection->Lock();

    stereo_camera_last_request_time_ = FPlatformTime::Seconds();

    if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND) {
      auto Instruction      = std::make_shared<FInstruction<ADronePawn>>();
      Instruction->Function = [](ADronePawn& _DronePawn) { _DronePawn.UpdateCamera(true, CAMERA_MODE_STEREO); };
      InstructionQueue->Enqueue(Instruction);
      FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });
    }

    if (!StereoCameraRendered) {
      UE_LOG(LogTemp, Warning, TEXT("DronePawn(): stereo camera is not rendered, returning"));
      StereoCameraBufferCriticalSection->Unlock();
      return false;
    }

    if (StereoCameraDataNeedsCompress) {

      FImageUtils::ThumbnailCompressImageArray(RenderTarget2DStereoLeft->SizeX, RenderTarget2DStereoLeft->SizeY, StereoLeftCameraBuffer,
                                               *CompressedStereoLeftCameraData);
      FImageUtils::ThumbnailCompressImageArray(RenderTarget2DStereoRight->SizeX, RenderTarget2DStereoRight->SizeY, StereoRightCameraBuffer,
                                               *CompressedStereoRightCameraData);
      StereoCameraDataNeedsCompress = false;
    }

    {
      const auto Size = (*CompressedStereoLeftCameraData).Num();
      image_left.SetNumUninitialized(Size);
      FMemory::Memcpy(image_left.GetData(), (*CompressedStereoLeftCameraData).GetData(), Size * sizeof(uint8));
    }

    {
      const auto Size = (*CompressedStereoRightCameraData).Num();
      image_right.SetNumUninitialized(Size);
      FMemory::Memcpy(image_right.GetData(), (*CompressedStereoRightCameraData).GetData(), Size * sizeof(uint8));
    }

    stamp = stereo_stamp_;

    StereoCameraBufferCriticalSection->Unlock();

    return true;
  }
}

//}

/* getCameraSegFromServerThread() //{ */

void ADronePawn::TransformImageArray(int32 ImageWidth, int32 ImageHeight, const TArray<FColor>& SrcData, TArray<uint8>& DstData) {
  // Calculate the memory size required for the image data
  int32 MemorySize = ImageWidth * ImageHeight * 4;  // 4 bytes per pixel (RGBA)

  // Resize the destination array to hold the transformed data
  DstData.SetNumUninitialized(MemorySize);

  // Pointer to the source data (BGRA format)
  const FColor* SrcDataPtr = SrcData.GetData();

  // Pointer to the destination data (RGBA format)
  uint8* DstDataPtr = DstData.GetData();

  // Iterate over each pixel and transform BGRA to RGBA
  for (int32 Index = 0; Index < ImageWidth * ImageHeight; Index++) {
    // Copy and swap channels
    DstDataPtr[Index * 3 + 0] = SrcDataPtr[Index].B;  // Red channel
    DstDataPtr[Index * 3 + 1] = SrcDataPtr[Index].G;  // Green channel
    DstDataPtr[Index * 3 + 2] = SrcDataPtr[Index].R;  // Blue channel
  }
}

bool ADronePawn::GetRgbSegCameraFromServerThread(TArray<uint8>& OutArray, double& stamp) {

  RgbSegCameraBufferCriticalSection->Lock();

  rgb_seg_camera_last_request_time_ = FPlatformTime::Seconds();

  if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND) {
    auto Instruction      = std::make_shared<FInstruction<ADronePawn>>();
    Instruction->Function = [](ADronePawn& _DronePawn) { _DronePawn.UpdateCamera(true, CAMERA_MODE_RGB_SEG); };
    InstructionQueue->Enqueue(Instruction);
    FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });
  }

  if (!RgbSegCameraRendered) {
    UE_LOG(LogTemp, Warning, TEXT("DronePawn(): rgb seg camera is not rendered, returning"));
    RgbSegCameraBufferCriticalSection->Unlock();
    return false;
  }

  /* if (RgbSegCameraDataNeedsCompress) { */
  /*   FImageUtils::ThumbnailCompressImageArray(RenderTarget2DRgbSeg->SizeX, RenderTarget2DRgbSeg->SizeY, RgbSegCameraBuffer, *CompressedRgbSegCameraData); */
  /*   RgbSegCameraDataNeedsCompress = false; */
  /* } */

  auto byteArray = MakeUnique<TArray<uint8>>();
  ADronePawn::TransformImageArray(RenderTarget2DRgbSeg->SizeX, RenderTarget2DRgbSeg->SizeY, RgbSegCameraBuffer, *byteArray);

  const auto Size = (*byteArray).Num();
  OutArray.SetNumUninitialized(Size);
  FMemory::Memcpy(OutArray.GetData(), (*byteArray).GetData(), Size * sizeof(uint8));

  RgbSegCameraBufferCriticalSection->Unlock();

  stamp = rgb_seg_stamp_;

  return true;
}

//}

/* setCameraCaptureMode() //{ */

void ADronePawn::SetCameraCaptureMode(CameraCaptureModeEnum CaptureMode) {

  // CameraCaptureMode = CaptureMode;

  // if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES || CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_MOVEMENT) {
  //   UpdateCamera(true, CAMERA_MODE_RGB);
  // }
}

//}

/* getLidarConfig() //{ */

FLidarConfig ADronePawn::GetLidarConfig() {
  return LidarConfig;
}

//}

/* setLidarConfig() //{ */

bool ADronePawn::SetLidarConfig(const FLidarConfig& Config) {

  LidarHitsCriticalSection->Lock();
  LidarSegHitsCriticalSection->Lock();

  const bool         LivoxWas       = LidarConfig.Livox;
  const ELivoxSensor LivoxSensorWas = LidarConfig.LivoxSensor;

  LidarConfig.Enable      = Config.Enable;
  LidarConfig.Livox       = Config.Livox;
  LidarConfig.LivoxSensor = Config.LivoxSensor;
  LidarConfig.ShowBeams   = Config.ShowBeams;
  LidarConfig.BeamLength  = Config.BeamLength;

  // Switching sensor mid-flight would otherwise resume at a pattern index that
  // belongs to the previous sensor's table.
  if (LidarConfig.Livox != LivoxWas || LidarConfig.LivoxSensor != LivoxSensorWas) {
    LivoxCursor    = FLivoxCursor();
    LivoxSegCursor = FLivoxCursor();
    LivoxIntCursor = FLivoxCursor();

    if (LidarConfig.Livox) {
      // Fail loudly at configuration time rather than returning empty clouds.
      if (const FLivoxPattern* Pattern = GetLivoxPattern()) {
        UE_LOG(LogTemp, Log, TEXT("Livox: '%s' active, %.0f kHz -> %d points per message at %.1f Hz"), FLivoxPatternLibrary::Name(LidarConfig.LivoxSensor),
               Pattern->PointRate / 1000.0, FMath::RoundToInt(Pattern->PointRate / FMath::Max(LidarConfig.Frequency, 1.0)), LidarConfig.Frequency);
      } else {
        UE_LOG(LogTemp, Error, TEXT("Livox: no scan pattern for sensor %d, lidar will return nothing"), (int32)LidarConfig.LivoxSensor);
      }
    }
  }

  if (Config.BeamHorRays > 0 && Config.BeamVertRays > 0) {
    LidarConfig.BeamHorRays  = Config.BeamHorRays;
    LidarConfig.BeamVertRays = Config.BeamVertRays;
    UE_LOG(LogTemp, Warning, TEXT("Setting LiDAR with width = %d, height %d"), LidarConfig.BeamHorRays, LidarConfig.BeamVertRays);
  } else {
    UE_LOG(LogTemp, Error, TEXT("Invalid dimensions for Lidar. BeamHorRays and BeamVertRays should be greater than 0."));
  }

  LidarConfig.Frequency = Config.Frequency;

  LidarConfig.Offset.X = Config.Offset.X;
  LidarConfig.Offset.Y = Config.Offset.Y;
  LidarConfig.Offset.Z = Config.Offset.Z;

  LidarConfig.Orientation.Pitch = Config.Orientation.Pitch;
  LidarConfig.Orientation.Yaw   = -Config.Orientation.Yaw;
  LidarConfig.Orientation.Roll  = -Config.Orientation.Roll;

  /* LidarConfig.FOVHor  = Config.FOVHor; */
  /* LidarConfig.FOVVert = Config.FOVVert; */
  LidarConfig.FOVVertUp   = Config.FOVVertUp;
  LidarConfig.FOVVertDown = Config.FOVVertDown;
  LidarConfig.FOVHorLeft  = Config.FOVHorLeft;
  LidarConfig.FOVHorRight = Config.FOVHorRight;
  LidarConfig.vertRayDiff = (double)(LidarConfig.FOVVertUp + LidarConfig.FOVVertDown) / (double)(LidarConfig.BeamVertRays - 1.0);
  LidarConfig.horRayDif   = (double)(LidarConfig.FOVHorLeft + LidarConfig.FOVHorRight) / (double)(LidarConfig.BeamHorRays);

  LidarHits->resize(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
  LidarSegHits->resize(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);
  LidarHitsCriticalSection->Unlock();
  LidarSegHitsCriticalSection->Unlock();

  return true;
}

//}

/* getRgbCameraConfig() //{ */

FRgbCameraConfig ADronePawn::GetRgbCameraConfig() {

  return rgb_camera_config_;
}

//}

/* getStereoCameraConfig() //{ */

FStereoCameraConfig ADronePawn::GetStereoCameraConfig() {

  return stereo_camera_config_;
}

//}

/* setRgbCameraConfig() //{ */

bool ADronePawn::SetRgbCameraConfig(const FRgbCameraConfig& Config) {

  UE_LOG(LogTemp, Warning, TEXT("ADronePawn::SetRgbCameraConfig"));

  rgb_camera_config_ = Config;

  RgbCameraBufferCriticalSection->Lock();
  RgbSegCameraBufferCriticalSection->Lock();

  SceneCaptureMeshHolderRgb->SetVisibility(Config.ShowCameraComponent);
  SceneCaptureMeshHolderRgb->SetRelativeLocation(Config.Offset);
  SceneCaptureMeshHolderRgb->SetRelativeRotation(FRotator(0, 0, 0));
  SceneCaptureMeshHolderRgb->SetRelativeRotation(Config.Orientation);

  SceneCaptureComponent2DRgb->FOVAngle = Config.FOVAngle;

  SceneCaptureComponent2DRgbSeg->FOVAngle = Config.FOVAngle;

  if (Config.enable_hdr) {
    SceneCaptureComponent2DRgb->CaptureSource = SCS_FinalColorHDR;
  } else {
    SceneCaptureComponent2DRgb->CaptureSource = SCS_FinalColorLDR;
  }

  SceneCaptureComponent2DRgb->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);

  SceneCaptureComponent2DRgb->bUseRayTracingIfEnabled = Config.enable_raytracing;

  SceneCaptureComponent2DRgb->TextureTarget                        = RenderTarget2DRgb;
  SceneCaptureComponent2DRgb->bAlwaysPersistRenderingState         = true;
  SceneCaptureComponent2DRgb->bCaptureEveryFrame                   = false;
  SceneCaptureComponent2DRgb->bCaptureOnMovement                   = false;
  SceneCaptureComponent2DRgb->PostProcessSettings.MotionBlurAmount = Config.motion_blur_amount;
  SceneCaptureComponent2DRgb->PostProcessSettings.MotionBlurMax    = Config.motion_blur_distortion;
  SceneCaptureComponent2DRgb->ShowFlags.SetMotionBlur(Config.enable_motion_blur);

  SceneCaptureComponent2DRgbSeg->TextureTarget                = RenderTarget2DRgbSeg;
  SceneCaptureComponent2DRgbSeg->bAlwaysPersistRenderingState = true;
  SceneCaptureComponent2DRgbSeg->bCaptureEveryFrame           = false;
  SceneCaptureComponent2DRgbSeg->bCaptureOnMovement           = false;
  SceneCaptureComponent2DRgbSeg->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);


  if (Config.Width > 0 && Config.Height > 0) {
    RenderTarget2DRgb->ResizeTarget(Config.Width, Config.Height);
    RenderTarget2DRgbSeg->ResizeTarget(Config.Width, Config.Height);
  } else {
    UE_LOG(LogTemp, Warning, TEXT("Invalid dimensions for RenderTarget2D. Width and Height should be greater than 0."));
  }

  RgbCameraBufferCriticalSection->Unlock();
  RgbSegCameraBufferCriticalSection->Unlock();

  return true;
}

//}

/* setStereoCameraConfig() //{ */

bool ADronePawn::SetStereoCameraConfig(const FStereoCameraConfig& Config) {

  UE_LOG(LogTemp, Warning, TEXT("ADronePawn::SetStereoCameraConfig"));

  stereo_camera_config_ = Config;

  StereoCameraBufferCriticalSection->Lock();

  SceneCaptureMeshHolderStereoLeft->SetVisibility(Config.ShowCameraComponent);
  SceneCaptureMeshHolderStereoLeft->SetRelativeLocation(Config.Offset_left);
  SceneCaptureMeshHolderStereoLeft->SetRelativeRotation(FRotator(0, 0, 0));
  SceneCaptureMeshHolderStereoLeft->SetRelativeRotation(Config.Orientation_left);

  SceneCaptureMeshHolderStereoRight->SetVisibility(Config.ShowCameraComponent);
  SceneCaptureMeshHolderStereoRight->SetRelativeLocation(Config.Offset_right);
  SceneCaptureMeshHolderStereoRight->SetRelativeRotation(FRotator(0, 0, 0));
  SceneCaptureMeshHolderStereoRight->SetRelativeRotation(Config.Orientation_right);

  SceneCaptureComponent2DStereoLeft->FOVAngle  = Config.FOVAngle;
  SceneCaptureComponent2DStereoRight->FOVAngle = Config.FOVAngle;

  if (Config.enable_hdr) {
    SceneCaptureComponent2DStereoLeft->CaptureSource  = SCS_FinalColorHDR;
    SceneCaptureComponent2DStereoRight->CaptureSource = SCS_FinalColorHDR;
  } else {
    SceneCaptureComponent2DStereoLeft->CaptureSource  = SCS_FinalColorLDR;
    SceneCaptureComponent2DStereoRight->CaptureSource = SCS_FinalColorLDR;
  }

  SceneCaptureComponent2DStereoLeft->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);
  SceneCaptureComponent2DStereoRight->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);

  SceneCaptureComponent2DStereoLeft->bUseRayTracingIfEnabled  = Config.enable_raytracing;
  SceneCaptureComponent2DStereoRight->bUseRayTracingIfEnabled = Config.enable_raytracing;

  if (Config.Width > 0 && Config.Height > 0) {
    RenderTarget2DStereoLeft->ResizeTarget(Config.Width, Config.Height);
    RenderTarget2DStereoRight->ResizeTarget(Config.Width, Config.Height);
  } else {
    UE_LOG(LogTemp, Warning, TEXT("Invalid dimensions for RenderTarget2D. Width and Height should be greater than 0."));
  }

  StereoCameraBufferCriticalSection->Unlock();

  return true;
}

//}

/* setLocation() //{ */

void ADronePawn::SetLocation(FVector& Location, FVector& TeleportedToLocation, bool CheckCollisions, FHitResult& HitResult) {

  bool DrawLine      = DrawMovementLine;
  auto StartLocation = DrawLine ? GetActorLocation() : FVector();

  SetActorLocation(Location, CheckCollisions, &HitResult);
  TeleportedToLocation = CheckCollisions ? GetActorLocation() : Location;

  if (DrawLine) {
    DrawDebugLine(GetWorld(), StartLocation, TeleportedToLocation, FColor::Green, true, -1, 0, 3);
  }
}

//}

/* tick() //{ */


void ADronePawn::Tick(float DeltaSeconds) {

  auto start = FPlatformTime::Seconds();

  std::shared_ptr<FInstruction<ADronePawn>> Instruction;

  while (InstructionQueue->Dequeue(Instruction)) {
    Instruction->Function(*this);
    Instruction->Finished = true;
  }

  {
    RgbCameraBufferCriticalSection->Lock();

    if ((FPlatformTime::Seconds() - rgb_camera_last_request_time_) > 1.0) {
      if (SceneCaptureComponent2DRgb->bCaptureEveryFrame) {
        SceneCaptureComponent2DRgb->bCaptureEveryFrame = false;
        RgbCameraRendered                              = false;
        UE_LOG(LogTemp, Warning, TEXT("Turning off RGB capture"));
      }
    } else {
      if (!SceneCaptureComponent2DRgb->bCaptureEveryFrame) {
        SceneCaptureComponent2DRgb->bCaptureEveryFrame = true;
        UE_LOG(LogTemp, Warning, TEXT("Turning on RGB capture"));
      } else {
        UpdateCamera(true, CAMERA_MODE_RGB, start);
      }
    }

    RgbCameraBufferCriticalSection->Unlock();
  }

  {
    RgbSegCameraBufferCriticalSection->Lock();

    if ((FPlatformTime::Seconds() - rgb_seg_camera_last_request_time_) > 1.0) {
      if (SceneCaptureComponent2DRgbSeg->bCaptureEveryFrame) {
        SceneCaptureComponent2DRgbSeg->bCaptureEveryFrame = false;
        RgbSegCameraRendered                              = false;
        UE_LOG(LogTemp, Warning, TEXT("Turning off RGB segmented capture"));
      }
    } else {
      if (!SceneCaptureComponent2DRgbSeg->bCaptureEveryFrame) {
        SceneCaptureComponent2DRgbSeg->bCaptureEveryFrame = true;
        UE_LOG(LogTemp, Warning, TEXT("Turning on RGB segmented capture"));
      } else {
        UpdateCamera(true, CAMERA_MODE_RGB_SEG, start);
      }
    }

    RgbSegCameraBufferCriticalSection->Unlock();
  }

  {
    StereoCameraBufferCriticalSection->Lock();

    if ((FPlatformTime::Seconds() - stereo_camera_last_request_time_) > 1.0) {
      if (SceneCaptureComponent2DStereoLeft->bCaptureEveryFrame) {
        SceneCaptureComponent2DStereoLeft->bCaptureEveryFrame  = false;
        SceneCaptureComponent2DStereoRight->bCaptureEveryFrame = false;
        StereoCameraRendered                                   = false;
        UE_LOG(LogTemp, Warning, TEXT("Turning off Stereo capture"));
      }
    } else {
      if (!SceneCaptureComponent2DStereoLeft->bCaptureEveryFrame) {
        UE_LOG(LogTemp, Warning, TEXT("Turning on Stereo capture"));
        SceneCaptureComponent2DStereoLeft->bCaptureEveryFrame  = true;
        SceneCaptureComponent2DStereoRight->bCaptureEveryFrame = true;
      } else {
        UpdateCamera(true, CAMERA_MODE_STEREO, start);
      }
    }

    StereoCameraBufferCriticalSection->Unlock();
  }

  Super::Tick(DeltaSeconds);

  // UE_LOG(LogTemp, Warning, TEXT("Tick(), delta %f, took %f s"), DeltaSeconds, FPlatformTime::Seconds() - start);
}

//}
