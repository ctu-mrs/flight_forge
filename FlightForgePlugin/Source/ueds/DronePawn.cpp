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
  FramePropellersTransforms.Add(FramePropellersTransform(
  FString(TEXT("x500")),
  FString(TEXT("x500")),
  FTransform(FRotator(0, 0, 0), FVector(-18.8, -18.8, 6.5), FVector(-0.85, 0.85, -0.8)),
  FTransform(FRotator(0, 0, 0), FVector(-18.8, 18.8, 6.5), FVector(-0.85, -0.85, -0.8)),
  FTransform(FRotator(0, 0, 0), FVector(18.8, -18.8, 6.5), FVector(-0.85, -0.85, -0.8)),
  FTransform(FRotator(0, 0, 0), FVector(18.8, 18.8, 6.5), FVector(-0.85, 0.85, -0.8))));

  // T650
  FramePropellersTransforms.Add(FramePropellersTransform(
  FString(TEXT("t650")),
  FString(TEXT("x500")),
    FTransform(FRotator(0, 0, 0), FVector(-26.3, -26.3, 4.6), FVector(-1, 1, -1)),
    FTransform(FRotator(0, 0, 0), FVector(-26.3, 26.3, 4.6), FVector(-1, -1, -1)),
    FTransform(FRotator(0, 0, 0), FVector(26.3, -26.3, 4.6), FVector(-1, -1, -1)),
    FTransform(FRotator(0, 0, 0), FVector(26.3, 26.3, 4.6), FVector(-1, 1, -1))));

  // A300
  FramePropellersTransforms.Add(FramePropellersTransform(
  FString(TEXT("a300")),
  FString(TEXT("robofly")),
    FTransform(FRotator(0, 0, 0), FVector(-9.45, -11.7, 2.4), FVector(1, -1, 1)),
    FTransform(FRotator(0, 0, 0), FVector(-9.45, 11.7, 2.4), FVector(1, 1, 1)),
    FTransform(FRotator(0, 0, 0), FVector(9.45, -11.7, 2.4), FVector(1, 1, 1)),
    FTransform(FRotator(0, 0, 0), FVector(9.45, 11.7, 2.4), FVector(1, -1, 1))));

  // RoboFly
  FramePropellersTransforms.Add(FramePropellersTransform(
  FString(TEXT("robofly")),
  FString(TEXT("robofly")),
    FTransform(FRotator(0, 0, 0), FVector(-9.55, -9.55,1.7), FVector(1, -1, 1)),
    FTransform(FRotator(0, 0, 0), FVector(-9.55, 9.55,1.7), FVector(1, 1, 1)),
    FTransform(FRotator(0, 0, 0), FVector(9.55, -9.55, 1.7), FVector(1, 1, 1)),
    FTransform(FRotator(0, 0, 0), FVector(9.55, 9.55, 1.7), FVector(1, -1, 1))));

  //Wings
  FramePropellersTransforms.Add(FramePropellersTransform(
  FString(TEXT("wing")),
  FString(TEXT("robofly")),
    FTransform(FRotator(0, 0, 0), FVector(-9.55, -9.55,1.7), FVector(1, -1, 1)),
    FTransform(FRotator(0, 0, 0), FVector(-9.55, 9.55,1.7), FVector(1, 1, 1)),
    FTransform(FRotator(0, 0, 0), FVector(9.55, -9.55, 1.7), FVector(1, 1, 1)),
    FTransform(FRotator(0, 0, 0), FVector(9.55, 9.55, 1.7), FVector(1, -1, 1))));

  FramePropellersTransforms.Add(FramePropellersTransform(
  FString(TEXT("wing_2")),
  FString(TEXT("robofly")),
    FTransform(FRotator(0, 0, 0), FVector(-9.55, -9.55,1.7), FVector(1, -1, 1)),
    FTransform(FRotator(0, 0, 0), FVector(-9.55, 9.55,1.7), FVector(1, 1, 1)),
    FTransform(FRotator(0, 0, 0), FVector(9.55, -9.55, 1.7), FVector(1, 1, 1)),
    FTransform(FRotator(0, 0, 0), FVector(9.55, 9.55, 1.7), FVector(1, -1, 1))));

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
  LidarConfig.Offset      = FVector(0, 0, 6);
  LidarConfig.Orientation = FRotator(0, 90, 0);
  /* LidarConfig.FOVVert     = 45.0; */
  LidarConfig.FOVVertUp  = 52.0;
  LidarConfig.FOVVertDown  = 7.0;
  /* LidarConfig.FOVHor      = 360.0; */
  LidarConfig.FOVHorLeft  = 180.0;
  LidarConfig.FOVHorRight = 180.0;
  LidarConfig.vertRayDiff = (double)(LidarConfig.FOVVertUp + LidarConfig.FOVVertDown) / (double)(LidarConfig.BeamVertRays - 1.0);
  LidarConfig.horRayDif   = (double)(LidarConfig.FOVHorLeft + LidarConfig.FOVHorRight) / (double)LidarConfig.BeamHorRays;
  FTimerHandle OusterTimerHandle;

  RangefinderConfig.BeamLength = DEFAULT_RANGEFINDER_BEAM_LENGTH;
  RangefinderConfig.Offset    = FVector(0, 0, -10);

#if PLATFORM_WINDOWS
  LidarHitsCriticalSection    = std::make_unique<FWindowsCriticalSection>();
  LidarSegHitsCriticalSection = std::make_unique<FWindowsCriticalSection>();
  LidarIntHitsCriticalSection = std::make_unique<FWindowsCriticalSection>();
  RangefinderHitsCriticalSection = std::make_unique<FWindowsCriticalSection>();
#else
  LidarHitsCriticalSection          = std::make_unique<FPThreadsCriticalSection>();
  LidarSegHitsCriticalSection       = std::make_unique<FPThreadsCriticalSection>();
  LidarIntHitsCriticalSection       = std::make_unique<FPThreadsCriticalSection>();
  RangefinderHitsCriticalSection = std::make_unique<FPThreadsCriticalSection>();
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

  rgb_camera_config_.ShowCameraComponent = false;
  rgb_camera_config_.Offset              = FVector(0, 0, 0);
  rgb_camera_config_.Orientation         = FRotator(0, 0, 0);
  rgb_camera_config_.FOVAngle            = 90;
  rgb_camera_config_.Width               = 640;
  rgb_camera_config_.Height              = 480;
  rgb_camera_config_.enable_motion_blur  = true;
  rgb_camera_config_.motion_blur_amount  = 1.0;
  rgb_camera_config_.motion_blur_distortion = 50.0;

  stereo_camera_config_.ShowCameraComponent = false;
  stereo_camera_config_.Offset              = FVector(0, 0, 0);
  stereo_camera_config_.Orientation         = FRotator(0, 0, 0);
  stereo_camera_config_.FOVAngle            = 90;
  stereo_camera_config_.Width               = 640;
  stereo_camera_config_.Height              = 480;
  stereo_camera_config_.baseline            = 0.1;

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

/* updateLidar() //{ */
void ADronePawn::UpdateLidar(bool isExternallyLocked) {

    if (!isExternallyLocked) {
        LidarHitsCriticalSection->Lock();
    }

    auto World = GetWorld();
    const auto Start = GetActorLocation() + GetActorRotation().RotateVector(LidarConfig.Offset);
    const auto droneTransform = GetActorTransform();
    auto forwardVec = GetActorForwardVector();
    auto rightVector = GetActorRightVector();
    auto upVec = GetActorUpVector();

	// Apply Lidar orientation
    auto lidarRotation = GetActorRotation() + LidarConfig.Orientation;
	forwardVec = lidarRotation.Vector();
    rightVector = lidarRotation.RotateVector(FVector::RightVector);
	upVec = lidarRotation.RotateVector(FVector::UpVector);
    
    LidarHits = std::make_unique<std::vector<std::tuple<double, double, double, double>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);

    // Calculate total horizontal FOV and vertical FOV
    double totalHorFov = LidarConfig.FOVHorLeft + LidarConfig.FOVHorRight;
    double totalVertFov = LidarConfig.FOVVertUp + LidarConfig.FOVVertDown;

    ParallelFor(LidarConfig.BeamHorRays, [&](int32 row) {

        // Calculate the horizontal angle for the current ray
        double horAngle = -LidarConfig.FOVHorLeft + (totalHorFov * (double)row / (double)(LidarConfig.BeamHorRays - 1.0));
		
		FVector rotatedForward = forwardVec.RotateAngleAxis(horAngle, upVec);
        FVector rotatedRight = rightVector.RotateAngleAxis(horAngle, upVec);


        ParallelFor(LidarConfig.BeamVertRays, [&](int32 col) {

            auto CollisionParams = FCollisionQueryParams::DefaultQueryParam;

            if (LidarConfig.ShowBeams) {
              const FName TraceTag(FString::Printf(TEXT("LidarTraceTag_%d"), 0));
              CollisionParams.TraceTag = TraceTag;
              // World->DebugDrawTraceTag = TraceTag;
            }

            // Calculate the vertical angle for the current ray
            double vertAngle = -LidarConfig.FOVVertDown + (totalVertFov * (double)col / (double)(LidarConfig.BeamVertRays - 1.0));

            // Calculate the direction of the ray
            FVector raycastAngle = rotatedForward.RotateAngleAxis(vertAngle, rotatedRight);
            raycastAngle *= LidarConfig.BeamLength;


            FHitResult HitResult;
            
            if (World->LineTraceSingleByChannel(HitResult, Start, Start + raycastAngle, ECollisionChannel::ECC_Visibility, CollisionParams)) {

                int i = row * LidarConfig.BeamVertRays + col;
                
                if (HitResult.bBlockingHit) {
                  std::get<0>((*LidarHits)[i])  = HitResult.IsValidBlockingHit() ? HitResult.Distance : LidarConfig.BeamLength;
                  const auto ray_in_drone_coord = droneTransform.InverseTransformVector(raycastAngle);
                  std::get<1>((*LidarHits)[i])  = ray_in_drone_coord.X;
                  std::get<2>((*LidarHits)[i])  = ray_in_drone_coord.Y;
                  std::get<3>((*LidarHits)[i])  = ray_in_drone_coord.Z;
                } else {
                  std::get<0>((*LidarHits)[i])  = -1;
                  const auto ray_in_drone_coord = droneTransform.InverseTransformVector(raycastAngle);
                  std::get<1>((*LidarHits)[i])  = ray_in_drone_coord.X;
                  std::get<2>((*LidarHits)[i])  = ray_in_drone_coord.Y;
                  std::get<3>((*LidarHits)[i])  = ray_in_drone_coord.Z;
                }
            }
        });
    });
  
    LidarHitStart.reset(new FVector(Start));

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

    auto World = GetWorld();
    const auto Start = GetActorLocation() + GetActorRotation().RotateVector(LidarConfig.Offset);
    const auto droneTransform = GetActorTransform();
    auto forwardVec = GetActorForwardVector();
    auto rightVector = GetActorRightVector();
    auto upVec = GetActorUpVector();
    
	// Apply Lidar orientation
    auto lidarRotation = GetActorRotation() + LidarConfig.Orientation;
    forwardVec = lidarRotation.Vector();
    rightVector = lidarRotation.RotateVector(FVector::RightVector);
    upVec = lidarRotation.RotateVector(FVector::UpVector);
    

    LidarSegHits = std::make_unique<std::vector<std::tuple<double, double, double, double, int>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);

    // Calculate total horizontal FOV and vertical FOV
    double totalHorFov = LidarConfig.FOVHorLeft + LidarConfig.FOVHorRight;
    double totalVertFov = LidarConfig.FOVVertUp + LidarConfig.FOVVertDown;

    ParallelFor(LidarConfig.BeamHorRays, [&](int32 row) {

        // Calculate the horizontal angle for the current ray
        double horAngle = -LidarConfig.FOVHorLeft + (totalHorFov * (double)row / (double)(LidarConfig.BeamHorRays - 1.0));
		FVector rotatedForward = forwardVec.RotateAngleAxis(horAngle, upVec);
		FVector rotatedRight = rightVector.RotateAngleAxis(horAngle, upVec);

        ParallelFor(LidarConfig.BeamVertRays, [&](int32 col) {
            auto    CollisionParams = FCollisionQueryParams::DefaultQueryParam;

            // Calculate the vertical angle for the current ray
            double vertAngle = -LidarConfig.FOVVertDown + (totalVertFov * (double)col / (double)(LidarConfig.BeamVertRays - 1.0));

            // Calculate the direction of the ray
            FVector raycastAngle    = rotatedForward.RotateAngleAxis(vertAngle, rotatedRight);
            raycastAngle *= LidarConfig.BeamLength;

            FHitResult HitResult;

            if (World->LineTraceSingleByChannel(HitResult, Start, Start + raycastAngle, ECollisionChannel::ECC_Visibility, CollisionParams)) {

              int i = row * LidarConfig.BeamVertRays + col;

              if (HitResult.bBlockingHit) {

                std::get<0>((*LidarSegHits)[i]) = HitResult.IsValidBlockingHit() ? HitResult.Distance : LidarConfig.BeamLength;
                const auto ray_in_drone_coord   = droneTransform.InverseTransformVector(raycastAngle);

                std::get<1>((*LidarSegHits)[i]) = ray_in_drone_coord.X;
                std::get<2>((*LidarSegHits)[i]) = ray_in_drone_coord.Y;
                std::get<3>((*LidarSegHits)[i]) = ray_in_drone_coord.Z;

                auto component = Cast<UStaticMeshComponent>(HitResult.GetComponent());

                if (HitResult.GetComponent()->GetName().Contains("Landscape")) {
                  std::get<4>((*LidarSegHits)[i]) = 1;
                } else {
                  std::get<4>((*LidarSegHits)[i]) = component->CustomDepthStencilValue;
                }

              } else {

                std::get<0>((*LidarSegHits)[i]) = -1;

                const auto ray_in_drone_coord = droneTransform.InverseTransformVector(raycastAngle);

                std::get<1>((*LidarSegHits)[i]) = ray_in_drone_coord.X;
                std::get<2>((*LidarSegHits)[i]) = ray_in_drone_coord.Y;
                std::get<3>((*LidarSegHits)[i]) = ray_in_drone_coord.Z;
                std::get<4>((*LidarSegHits)[i]) = -1;
              }
            }
        });
    });

    LidarHitStart.reset(new FVector(Start));

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

    auto World = GetWorld();
    const auto Start = GetActorLocation() + GetActorRotation().RotateVector(LidarConfig.Offset);
    const auto droneTransform = GetActorTransform();
    auto forwardVec = GetActorForwardVector();
    auto rightVector = GetActorRightVector();
    auto upVec = GetActorUpVector();
	
	// Apply Lidar orientation
    auto lidarRotation = GetActorRotation() + LidarConfig.Orientation;
    forwardVec = lidarRotation.Vector();
    rightVector = lidarRotation.RotateVector(FVector::RightVector);
    upVec = lidarRotation.RotateVector(FVector::UpVector);

    LidarIntHits = std::make_unique<std::vector<std::tuple<double, double, double, double, int>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);

    // Calculate total horizontal FOV and vertical FOV
    double totalHorFov = LidarConfig.FOVHorLeft + LidarConfig.FOVHorRight;
    double totalVertFov = LidarConfig.FOVVertUp + LidarConfig.FOVVertDown;

    ParallelFor(LidarConfig.BeamHorRays, [&](int32 row) {

        // Calculate the horizontal angle for the current ray
        double horAngle = -LidarConfig.FOVHorLeft + (totalHorFov * (double)row / (double)(LidarConfig.BeamHorRays - 1.0));
		FVector rotatedForward = forwardVec.RotateAngleAxis(horAngle, upVec);
		FVector rotatedRight = rightVector.RotateAngleAxis(horAngle, upVec);

        ParallelFor(LidarConfig.BeamVertRays, [&](int32 col) {
            auto CollisionParams                    = FCollisionQueryParams::DefaultQueryParam;
            CollisionParams.bReturnPhysicalMaterial = true;

            // Calculate the vertical angle for the current ray
             double vertAngle = -LidarConfig.FOVVertDown + (totalVertFov * (double)col / (double)(LidarConfig.BeamVertRays - 1.0));

            // Calculate the direction of the ray
            FVector raycastAngle = rotatedForward.RotateAngleAxis(vertAngle, rotatedRight);
            raycastAngle *= LidarConfig.BeamLength;


            FHitResult HitResult;

            if (World->LineTraceSingleByChannel(HitResult, Start, Start + raycastAngle, ECollisionChannel::ECC_Visibility, CollisionParams)) {

                int i = row * LidarConfig.BeamVertRays + col;
                if (HitResult.bBlockingHit) {

                  std::get<0>((*LidarIntHits)[i]) = HitResult.IsValidBlockingHit() ? HitResult.Distance : LidarConfig.BeamLength;
                  const auto ray_in_drone_coord   = droneTransform.InverseTransformVector(raycastAngle);

                  std::get<1>((*LidarIntHits)[i]) = ray_in_drone_coord.X;
                  std::get<2>((*LidarIntHits)[i]) = ray_in_drone_coord.Y;
                  std::get<3>((*LidarIntHits)[i]) = ray_in_drone_coord.Z;

                  FVector            surfaceNormal = HitResult.ImpactNormal;
                  UPhysicalMaterial* PhysMat       = HitResult.PhysMaterial.Get();
                  /* UE_LOG(LogTemp, Warning, TEXT("HitResult.PhysMaterial.Get() %s"), *HitResult.PhysMaterial->GetName()); */
                  double intensity = -1;
                  /* double intensity = 1.0 - FMath::Abs(FVector::DotProduct(surfaceNormal, raycastAngle.GetSafeNormal())); */
                  if (PhysMat) {
                    FString PhysMatName = PhysMat->GetName();
                    if (PhysMatName.Equals("PM_Grass", ESearchCase::IgnoreCase)) {  
                      intensity = 1;
                    } else if (PhysMatName.Equals("PM_Road", ESearchCase::IgnoreCase)) {
                      intensity = 2;
                    } else if (PhysMatName.Equals("PM_Tree", ESearchCase::IgnoreCase)) {
                      intensity = 3;
                    } else if (PhysMatName.Equals("PM_Building", ESearchCase::IgnoreCase)) { 
                      intensity = 4;
                    } else if (PhysMatName.Equals("PM_Fence", ESearchCase::IgnoreCase)) {
                      intensity = 5;
                    } else if (PhysMatName.Equals("PM_DirtRoad", ESearchCase::IgnoreCase)) {
                      intensity = 6;
                    } else {
                      intensity = 0;
                    }
                  }
                  std::get<4>((*LidarIntHits)[i]) = intensity;
                } else {

                  std::get<0>((*LidarIntHits)[i]) = -1;

                  const auto ray_in_drone_coord = droneTransform.InverseTransformVector(raycastAngle);

                  std::get<1>((*LidarIntHits)[i]) = ray_in_drone_coord.X;
                  std::get<2>((*LidarIntHits)[i]) = ray_in_drone_coord.Y;
                  std::get<3>((*LidarIntHits)[i]) = ray_in_drone_coord.Z;
                  std::get<4>((*LidarIntHits)[i]) = -1;
                }
            }
        });
    });

    LidarHitStart.reset(new FVector(Start));

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
void ADronePawn::GetRangefinderData(double& range)
{
  RangefinderHitsCriticalSection->Lock();

  FVector Start = GetActorLocation() + GetActorRotation().RotateVector(RangefinderConfig.Offset);
  FVector RangefinderDirection = - GetActorUpVector();

  if(UWorld* World = GetWorld())
  {
    FHitResult HitResult;
    if(World->LineTraceSingleByChannel(HitResult, Start, Start + RangefinderConfig.BeamLength * RangefinderDirection, ECollisionChannel::ECC_Visibility))
    {
      if(HitResult.bBlockingHit)
      {
        range = HitResult.Distance;
      }
      else
      {
        range = -1;
      }
      //UE_LOG(LogTemp, Warning, TEXT("Rangefinder range = %lf"), range);
    }
  }

  RangefinderHitsCriticalSection->Unlock();
}
/*//}*/

/* getLidarHits() //{ */

void ADronePawn::GetLidarHits(std::vector<Serializable::Drone::GetLidarData::LidarData>& OutLidarData, FVector& OutStart) {

  // UE_LOG(LogTemp, Warning, TEXT("DronePawn::GetLidarHits"));

  LidarHitsCriticalSection->Lock();

  UpdateLidar(true);

  OutLidarData.resize(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);

  for (int i = 0; i < LidarConfig.BeamHorRays * LidarConfig.BeamVertRays; i++) {
    OutLidarData[i].distance   = std::get<0>((*LidarHits)[i]);
    OutLidarData[i].directionX = std::get<1>((*LidarHits)[i]);
    OutLidarData[i].directionY = std::get<2>((*LidarHits)[i]);
    OutLidarData[i].directionZ = std::get<3>((*LidarHits)[i]);
  }

  OutStart.X = LidarConfig.Offset.X;
  OutStart.Y = LidarConfig.Offset.Y;
  OutStart.Z = LidarConfig.Offset.Z;

  LidarHitsCriticalSection->Unlock();
}

//}

/* getSegLidarHits() //{ */

void ADronePawn::GetSegLidarHits(std::vector<Serializable::Drone::GetLidarSegData::LidarSegData>& OutLidarSegData, FVector& OutStart) {

  LidarSegHitsCriticalSection->Lock();

  UpdateSegLidar(true);

  OutLidarSegData.resize(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);

  for (int i = 0; i < LidarConfig.BeamHorRays * LidarConfig.BeamVertRays; i++) {
    OutLidarSegData[i].distance     = std::get<0>((*LidarSegHits)[i]);
    OutLidarSegData[i].directionX   = std::get<1>((*LidarSegHits)[i]);
    OutLidarSegData[i].directionY   = std::get<2>((*LidarSegHits)[i]);
    OutLidarSegData[i].directionZ   = std::get<3>((*LidarSegHits)[i]);
    OutLidarSegData[i].segmentation = std::get<4>((*LidarSegHits)[i]);
  }

  OutStart.X = LidarConfig.Offset.X;
  OutStart.Y = LidarConfig.Offset.Y;
  OutStart.Z = LidarConfig.Offset.Z;

  LidarSegHitsCriticalSection->Unlock();
}

//}

/* getIntLidarHits() //{ */

void ADronePawn::GetIntLidarHits(std::vector<Serializable::Drone::GetLidarIntData::LidarIntData>& OutLidarIntData, FVector& OutStart) {

  LidarIntHitsCriticalSection->Lock();

  UpdateIntLidar(true);
  OutLidarIntData.resize(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);

  for (int i = 0; i < LidarConfig.BeamHorRays * LidarConfig.BeamVertRays; i++) {
    OutLidarIntData[i].distance   = std::get<0>((*LidarIntHits)[i]);
    OutLidarIntData[i].directionX = std::get<1>((*LidarIntHits)[i]);
    OutLidarIntData[i].directionY = std::get<2>((*LidarIntHits)[i]);
    OutLidarIntData[i].directionZ = std::get<3>((*LidarIntHits)[i]);
    OutLidarIntData[i].intensity  = std::get<4>((*LidarIntHits)[i]);
  }

  OutStart.X = LidarConfig.Offset.X;
  OutStart.Y = LidarConfig.Offset.Y;
  OutStart.Z = LidarConfig.Offset.Z;

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

void ADronePawn::SetPropellersTransform(const int& frame_id)
{
  const FramePropellersTransform *Transforms = FramePropellersTransforms.GetData();

  FString mesh_path = "/FlightForgePlugin/Meshes/Propellers/propeller_" + Transforms[frame_id].PropellerType;
  
  if (UStaticMesh* PropellerMesh = LoadObject<UStaticMesh>(nullptr, *mesh_path))
  {
    PropellerFrontLeft->SetStaticMesh(PropellerMesh);
    PropellerFrontRight->SetStaticMesh(PropellerMesh);
    PropellerRearLeft->SetStaticMesh(PropellerMesh);
    PropellerRearRight->SetStaticMesh(PropellerMesh);
  }  else
  {
    UE_LOG(LogTemp, Error, TEXT("The Propeller was not loaded!"));
  }

  PropellerFrontLeft->SetRelativeTransform(Transforms[frame_id].FrontLeft);
  PropellerFrontRight->SetRelativeTransform(Transforms[frame_id].FrontRight);
  PropellerRearLeft->SetRelativeTransform(Transforms[frame_id].RearLeft);
  PropellerRearRight->SetRelativeTransform(Transforms[frame_id].RearRight);
}

void ADronePawn::SetStaticMesh(const int &frame_id)
{
  FString mesh_path = "/FlightForgePlugin/Meshes/_Drones_/";
  
  int predefined_frame_count = FramePropellersTransforms.Num();

  //last "empty" frame is not included in "FramePropellersTransforms"
  if (predefined_frame_count == frame_id)
  {
    return;
  }

  FString frame_name = FramePropellersTransforms.GetData()[frame_id].FrameName;
  
  mesh_path += frame_name + "/" + frame_name + "." + frame_name;
  
  if (UStaticMesh* FrameMesh = LoadObject<UStaticMesh>(nullptr, *mesh_path))
  {
      RootMeshComponent->SetStaticMesh(FrameMesh);
  }  else
  {
    UE_LOG(LogTemp, Error, TEXT("The Frame was not loaded!"));
  }

  // wing has not propellers
  if (frame_name.Contains("wing"))
  {
    return;
  }
  
  SetPropellersTransform(frame_id);
}

void ADronePawn::Simulate_UE_Physics(const float& stop_simulation_delay)
{
  //RootMeshComponent->SetSimulatePhysics(true);
  GetWorldTimerManager().SetTimer(TimerHandle_Disabled_Physics, this, &ADronePawn::DisabledPhysics_StartRotatePropellers, stop_simulation_delay, false);
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

void ADronePawn::TransformImageArray(int32 ImageWidth, int32 ImageHeight, const TArray<FColor> &SrcData, TArray<uint8> &DstData)
{
    // Calculate the memory size required for the image data
    int32 MemorySize = ImageWidth * ImageHeight * 4; // 4 bytes per pixel (RGBA)

    // Resize the destination array to hold the transformed data
    DstData.SetNumUninitialized(MemorySize);

    // Pointer to the source data (BGRA format)
    const FColor* SrcDataPtr = SrcData.GetData();

    // Pointer to the destination data (RGBA format)
    uint8* DstDataPtr = DstData.GetData();

    // Iterate over each pixel and transform BGRA to RGBA
    for (int32 Index = 0; Index < ImageWidth * ImageHeight; Index++)
    {
        // Copy and swap channels
        DstDataPtr[Index * 3 + 0] = SrcDataPtr[Index].B; // Red channel
        DstDataPtr[Index * 3 + 1] = SrcDataPtr[Index].G; // Green channel
        DstDataPtr[Index * 3 + 2] = SrcDataPtr[Index].R; // Blue channel
    }
}

bool ADronePawn::GetRgbSegCameraFromServerThread(TArray<uint8>& OutArray, double &stamp) {

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
  UE_LOG(LogTemp, Warning, TEXT("ADronePawn::SetLidarConfig"));
  LidarHitsCriticalSection->Lock();
  LidarSegHitsCriticalSection->Lock();

  LidarConfig.Enable     = Config.Enable;
  LidarConfig.ShowBeams  = Config.ShowBeams;
  LidarConfig.BeamLength = Config.BeamLength;

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
  LidarConfig.Orientation.Yaw   = Config.Orientation.Yaw;
  LidarConfig.Orientation.Roll  = Config.Orientation.Roll;

  LidarConfig.FOVVertUp = Config.FOVVertUp;
  LidarConfig.FOVVertDown = Config.FOVVertDown;
  LidarConfig.FOVHorLeft = Config.FOVHorLeft;
  LidarConfig.FOVHorRight = Config.FOVHorRight;
  LidarConfig.vertRayDiff = (double)(LidarConfig.FOVVertUp + LidarConfig.FOVVertDown) / (double)(LidarConfig.BeamVertRays - 1.0);
  LidarConfig.horRayDif   = (double)(LidarConfig.FOVHorRight + LidarConfig.FOVHorLeft) / (double)LidarConfig.BeamHorRays;

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

  SceneCaptureComponent2DRgb->TextureTarget                = RenderTarget2DRgb;
  SceneCaptureComponent2DRgb->bAlwaysPersistRenderingState = true;
  SceneCaptureComponent2DRgb->bCaptureEveryFrame           = false;
  SceneCaptureComponent2DRgb->bCaptureOnMovement           = false;
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
  SceneCaptureMeshHolderStereoLeft->SetRelativeLocation(Config.Offset);
  SceneCaptureMeshHolderStereoLeft->SetRelativeRotation(FRotator(0, 0, 0));
  SceneCaptureMeshHolderStereoLeft->SetRelativeRotation(Config.Orientation);

  SceneCaptureMeshHolderStereoRight->SetVisibility(Config.ShowCameraComponent);
  SceneCaptureMeshHolderStereoRight->SetRelativeLocation(Config.Offset + FVector(0.0, 100.0 * Config.baseline, 0.0));
  SceneCaptureMeshHolderStereoRight->SetRelativeRotation(FRotator(0, 0, 0));
  SceneCaptureMeshHolderStereoRight->SetRelativeRotation(Config.Orientation);

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
