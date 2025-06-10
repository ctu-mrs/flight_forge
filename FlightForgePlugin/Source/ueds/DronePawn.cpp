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
#include "SWarningOrErrorBox.h"
#include "Kismet/BlueprintTypeConversions.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "WorldPartition/ContentBundle/ContentBundleLog.h"

//}

/*LoadCSVData()//{*/
bool ADronePawn::LoadCSVData(const FString& FilePath)
{
    // Convert FString to std::string for CSV parsing
    std::string filePathStr = std::string(TCHAR_TO_UTF8(*FilePath));
    std::ifstream file(filePathStr);

    if (!file.is_open()) {
        UE_LOG(LogTemp, Error, TEXT("Could not open CSV file: %s"), *FilePath);
        return false;
    }

    std::string line;
    std::getline(file, line); // Skip the header line

    LivoxData.Empty(); // Clear any existing data

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;
        FLivoxDataPoint dataPoint;
        
        // Read values from the CSV line
        std::getline(ss, value, ','); // Read Time/s
        try {
            dataPoint.Time = std::stod(value);
        }
        catch (const std::invalid_argument& e) {
            UE_LOG(LogTemp, Error, TEXT("Invalid data in CSV (Time): %s, %s"), UTF8_TO_TCHAR(value.c_str()), UTF8_TO_TCHAR(e.what()));
            file.close(); // Close file on read error
            return false;
        }

        std::getline(ss, value, ','); // Read Azimuth/deg
         try {
            dataPoint.Azimuth = FMath::DegreesToRadians(std::stod(value));  // Convert to radians
        }
        catch (const std::invalid_argument& e) {
           UE_LOG(LogTemp, Error, TEXT("Invalid data in CSV (Azimuth): %s, %s"), UTF8_TO_TCHAR(value.c_str()), UTF8_TO_TCHAR(e.what()));
           file.close();
           return false;
        }

        std::getline(ss, value, ','); // Read Zenith/deg
        try {
            dataPoint.Zenith = FMath::DegreesToRadians(std::stod(value));  // Convert to radians
        } catch (const std::invalid_argument& e) {
            UE_LOG(LogTemp, Error, TEXT("Invalid data in CSV (Zenith): %s, %s"), UTF8_TO_TCHAR(value.c_str()), UTF8_TO_TCHAR(e.what()));
            file.close();
            return false;
        }
        
        LivoxData.Add(dataPoint); // Add the data point to the array
    }

    file.close();

    if (LivoxData.Num() == 0) {
        UE_LOG(LogTemp, Warning, TEXT("CSV file is empty or contains no valid data."));
        return false;  // Return false if no data was loaded
    }

    UE_LOG(LogTemp, Log, TEXT("Loaded %d data points from CSV."), LivoxData.Num());
    return true;
}
/*//}*/

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
  DepthCameraBufferCriticalSection = std::make_unique<FWindowsCriticalSection>();
#else
  RgbCameraBufferCriticalSection    = std::make_unique<FPThreadsCriticalSection>();
  StereoCameraBufferCriticalSection = std::make_unique<FPThreadsCriticalSection>();
  RgbSegCameraBufferCriticalSection = std::make_unique<FPThreadsCriticalSection>();
  DepthCameraBufferCriticalSection = std::make_unique<FPThreadsCriticalSection>();
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

  SceneCaptureComponent2DDepth = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2DDepth"));
  SceneCaptureComponent2DDepth->SetupAttachment(SceneCaptureMeshHolderRgb);

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

  LidarConfig.Livox = false;

  StartIndex = 0;


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

  RenderTarget2DDepth = NewObject<UTextureRenderTarget2D>();
  // RenderTarget2DDepth->InitCustomFormat(640, 480, EPixelFormat::PF_R16F, false);
	RenderTarget2DDepth->InitCustomFormat(640, 480, EPixelFormat::PF_FloatRGBA, false);
  RenderTarget2DDepth->RenderTargetFormat = RTF_RGBA16f; //RTF_RGBA8; //RTF_R16f;
  RenderTarget2DDepth->bGPUSharedFlag = true;
	// RenderTarget2DDepth->InitCustomFormat(640, 480, PF_B8G8R8A8, false);
	// RenderTarget2DDepth->InitAutoFormat(16, 16);
  // RenderTarget2DDepth->InitCustomFormat(640, 480, EPixelFormat::PF_FloatRGB, false);
  // RenderTarget2DDepth->ClearColor = FLinearColor::Black;
  // RenderTarget2DDepth->UpdateResourceImmediate(true);
	
  
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
  SceneCaptureComponent2DRgbSeg->AddOrUpdateBlendable(PostProcessMaterialSegmentation, 1);

  SceneCaptureComponent2DDepth->CaptureSource = SCS_SceneDepth;
  SceneCaptureComponent2DDepth->TextureTarget = RenderTarget2DDepth;
  SceneCaptureComponent2DDepth->ShowFlags.SetTemporalAA(true);
  SceneCaptureComponent2DDepth->bAlwaysPersistRenderingState = true;
  SceneCaptureComponent2DDepth->bCaptureEveryFrame           = true;
  SceneCaptureComponent2DDepth->bCaptureOnMovement           = false;
  SceneCaptureComponent2DDepth->bUseRayTracingIfEnabled      = false;
  //SceneCaptureComponent2DDepth->AddOrUpdateBlendable(PostProcessMaterialDepth, 1);

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
  stereo_camera_config_.Offset              = FVector(0, 0, 0);
  stereo_camera_config_.Orientation         = FRotator(0, 0, 0);
  stereo_camera_config_.FOVAngle            = 90;
  stereo_camera_config_.Width               = 640;
  stereo_camera_config_.Height              = 480;
  stereo_camera_config_.baseline            = 0.1;

  SetRgbCameraConfig(rgb_camera_config_);
  SetStereoCameraConfig(stereo_camera_config_);

  if (!LoadCSVData(CSVFilePath)) {
        UE_LOG(LogTemp, Error, TEXT("Failed to load CSV data in BeginPlay."));
    }

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

    LidarHits = std::make_unique<std::vector<std::tuple<double, double, double, double>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);

    if (LidarConfig.Livox) {
        // Livox Mode (CSV Data)
        int pointsPerFrame = 20000;

        ParallelFor(LidarConfig.BeamHorRays, [&](int32 row) {
            for (int32 col = 0; col < LidarConfig.BeamVertRays; ++col) {
                int i = row * LidarConfig.BeamVertRays + col;

                int dataIndex = (StartIndex + i) % LivoxData.Num();
                const FLivoxDataPoint& dataPoint = LivoxData[dataIndex];

                double azimuth = dataPoint.Azimuth;
                double zenith = dataPoint.Zenith; 

                // Ray direction in *lidar's local frame
                FVector rayDirection_local(
                    FMath::Cos(zenith) * FMath::Cos(azimuth),
                    FMath::Cos(zenith) * FMath::Sin(azimuth),
                    FMath::Sin(zenith)
                );

                // Transform to *world* coordinates
                FVector rayDirection_world = lidarGlobalTransform.TransformVector(rayDirection_local);
                rayDirection_world *= LidarConfig.BeamLength;


                FHitResult HitResult;
                auto CollisionParams = FCollisionQueryParams::DefaultQueryParam;
                rayDirection_local = lidarGlobalTransform.InverseTransformVector(rayDirection_world);
                if (World->LineTraceSingleByChannel(HitResult, lidarLocation, lidarLocation + rayDirection_world, ECC_Visibility, CollisionParams))
                {
                    if (HitResult.bBlockingHit) {
                        AActor* HitActor = HitResult.GetActor();
                        if (!bCanSeeOtherDrone && HitActor && HitActor->IsA(ADronePawn::StaticClass()) && HitActor != this) {
                             // Store distance and *local* ray direction.
                            (*LidarHits)[i] = std::make_tuple(-1.0, rayDirection_local.X, rayDirection_local.Y, rayDirection_local.Z);
                        }
                        else{
                            (*LidarHits)[i] = std::make_tuple(HitResult.Distance, rayDirection_local.X, rayDirection_local.Y, rayDirection_local.Z); // CORRECTED
                        }
                    } else {
                        // No hit. Store *local* ray direction.
                        (*LidarHits)[i] = std::make_tuple(-1.0, rayDirection_local.X, rayDirection_local.Y, rayDirection_local.Z);
                    }
                }
                else{
                    (*LidarHits)[i] = std::make_tuple(-1.0, rayDirection_local.X, rayDirection_local.Y, rayDirection_local.Z);  // No hit
                }
            }
        });

        StartIndex = (StartIndex + pointsPerFrame) % LivoxData.Num();
        LidarHitStart.reset(new FVector(lidarLocation));

    } else {

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


    LidarSegHits = std::make_unique<std::vector<std::tuple<double, double, double, double, int>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);


  if (LidarConfig.Livox) {
        // Livox Mode (CSV Data)
        int pointsPerFrame = 20000;

        ParallelFor(LidarConfig.BeamHorRays, [&](int32 row) {
            for (int32 col = 0; col < LidarConfig.BeamVertRays; ++col) {
                int i = row * LidarConfig.BeamVertRays + col;

                int dataIndex = (StartIndex + i) % LivoxData.Num();
                const FLivoxDataPoint& dataPoint = LivoxData[dataIndex];

                double azimuth = dataPoint.Azimuth;
                double zenith = dataPoint.Zenith; 

                // Ray direction in *lidar's local frame
                FVector rayDirection_local(
                    FMath::Cos(zenith) * FMath::Cos(azimuth),
                    FMath::Cos(zenith) * FMath::Sin(azimuth),
                    FMath::Sin(zenith)
                );

                // Transform to *world* coordinates
                FVector rayDirection_world = lidarGlobalTransform.TransformVector(rayDirection_local);
                rayDirection_world *= LidarConfig.BeamLength;


                FHitResult HitResult;
                auto CollisionParams = FCollisionQueryParams::DefaultQueryParam;
                rayDirection_local = lidarGlobalTransform.InverseTransformVector(rayDirection_world);
                if (World->LineTraceSingleByChannel(HitResult, lidarLocation, lidarLocation + rayDirection_world, ECC_Visibility, CollisionParams))
                {
                    if (HitResult.bBlockingHit) {
                        AActor* HitActor = HitResult.GetActor();
                        if (!bCanSeeOtherDrone && HitActor && HitActor->IsA(ADronePawn::StaticClass()) && HitActor != this) {
                            std::get<0>((*LidarSegHits)[i]) = -1.0;
                            std::get<1>((*LidarSegHits)[i]) = rayDirection_local.X;
                            std::get<2>((*LidarSegHits)[i]) = rayDirection_local.Y;
                            std::get<3>((*LidarSegHits)[i]) = rayDirection_local.Z;
                            std::get<4>((*LidarSegHits)[i]) = -1;
                        }
                        else{
                            std::get<0>((*LidarSegHits)[i]) = HitResult.Distance;
                            std::get<1>((*LidarSegHits)[i]) = rayDirection_local.X;
                            std::get<2>((*LidarSegHits)[i]) = rayDirection_local.Y;
                            std::get<3>((*LidarSegHits)[i]) = rayDirection_local.Z;
                             auto component = Cast<UStaticMeshComponent>(HitResult.GetComponent());

                            if (HitResult.GetComponent()->GetName().Contains("Landscape")) {
                                std::get<4>((*LidarSegHits)[i]) = 1;
                            } else {
                                std::get<4>((*LidarSegHits)[i]) = component->CustomDepthStencilValue;
                            }
                        }
                    } else {
                        // No hit. 
                        std::get<0>((*LidarSegHits)[i]) = -1.0;
                        std::get<1>((*LidarSegHits)[i]) = rayDirection_local.X;
                        std::get<2>((*LidarSegHits)[i]) = rayDirection_local.Y;
                        std::get<3>((*LidarSegHits)[i]) = rayDirection_local.Z;
                        std::get<4>((*LidarSegHits)[i]) = -1;
                    }
                }
                else{
                    std::get<0>((*LidarSegHits)[i]) = -1.0;
                    std::get<1>((*LidarSegHits)[i]) = rayDirection_local.X;
                    std::get<2>((*LidarSegHits)[i]) = rayDirection_local.Y;
                    std::get<3>((*LidarSegHits)[i]) = rayDirection_local.Z;
                    std::get<4>((*LidarSegHits)[i]) = -1;  // No hit
                }
            }
        });

        StartIndex = (StartIndex + pointsPerFrame) % LivoxData.Num();

    } else {

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

  LidarIntHits = std::make_unique<std::vector<std::tuple<double, double, double, double, int>>>(LidarConfig.BeamHorRays * LidarConfig.BeamVertRays);

    if (LidarConfig.Livox) {
        // Livox Mode (CSV Data)
        int pointsPerFrame = 20000;

        ParallelFor(LidarConfig.BeamHorRays, [&](int32 row) {
            for (int32 col = 0; col < LidarConfig.BeamVertRays; ++col) {
                int i = row * LidarConfig.BeamVertRays + col;

                int dataIndex = (StartIndex + i) % LivoxData.Num();
                const FLivoxDataPoint& dataPoint = LivoxData[dataIndex];

                double azimuth = dataPoint.Azimuth;
                double zenith = dataPoint.Zenith; 

                // Ray direction in *lidar's local frame
                FVector rayDirection_local(
                    FMath::Cos(zenith) * FMath::Cos(azimuth),
                    FMath::Cos(zenith) * FMath::Sin(azimuth),
                    FMath::Sin(zenith)
                );

                // Transform to *world* coordinates
                FVector rayDirection_world = lidarGlobalTransform.TransformVector(rayDirection_local);
                rayDirection_world *= LidarConfig.BeamLength;


                FHitResult HitResult;
                auto CollisionParams = FCollisionQueryParams::DefaultQueryParam;
                CollisionParams.bReturnPhysicalMaterial = true;
                rayDirection_local = lidarGlobalTransform.InverseTransformVector(rayDirection_world);
                if (World->LineTraceSingleByChannel(HitResult, lidarLocation, lidarLocation + rayDirection_world, ECC_Visibility, CollisionParams))
                {
                    if (HitResult.bBlockingHit) {
                        AActor* HitActor = HitResult.GetActor();
                        if (!bCanSeeOtherDrone && HitActor && HitActor->IsA(ADronePawn::StaticClass()) && HitActor != this) {
                            std::get<0>((*LidarIntHits)[i]) = -1.0;
                            std::get<1>((*LidarIntHits)[i]) = rayDirection_local.X;
                            std::get<2>((*LidarIntHits)[i]) = rayDirection_local.Y;
                            std::get<3>((*LidarIntHits)[i]) = rayDirection_local.Z;
                            std::get<4>((*LidarIntHits)[i]) = -1;
                        }
                        else{
                            std::get<0>((*LidarIntHits)[i]) = HitResult.Distance;
                            std::get<1>((*LidarIntHits)[i]) = rayDirection_local.X;
                            std::get<2>((*LidarIntHits)[i]) = rayDirection_local.Y;
                            std::get<3>((*LidarIntHits)[i]) = rayDirection_local.Z;
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
                        }
                    } else {
                        // No hit. 
                        std::get<0>((*LidarIntHits)[i]) = -1.0;
                        std::get<1>((*LidarIntHits)[i]) = rayDirection_local.X;
                        std::get<2>((*LidarIntHits)[i]) = rayDirection_local.Y;
                        std::get<3>((*LidarIntHits)[i]) = rayDirection_local.Z;
                        std::get<4>((*LidarIntHits)[i]) = -1;
                    }
                }
                else{
                    std::get<0>((*LidarIntHits)[i]) = -1.0;
                    std::get<1>((*LidarIntHits)[i]) = rayDirection_local.X;
                    std::get<2>((*LidarIntHits)[i]) = rayDirection_local.Y;
                    std::get<3>((*LidarIntHits)[i]) = rayDirection_local.Z;
                    std::get<4>((*LidarIntHits)[i]) = -1;  // No hit
                }
            }
        });

        StartIndex = (StartIndex + pointsPerFrame) % LivoxData.Num();

    }else{

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

  	case CAMERA_MODE_DEPTH: {

      if (!isExternallyLocked) {
        DepthCameraBufferCriticalSection->Lock();
      }

      const auto ResourceDepth = RenderTarget2DDepth->GameThread_GetRenderTargetResource();

      // ResourceDepth->ReadPixels(DepthCameraBuffer);
      ResourceDepth->ReadFloat16Pixels(DepthCameraBuffer);
    		

    		// FFloat16Color* ptr = DepthCameraBuffer.GetData();
      // for (int i = 0; i < 100; i++)
      // {
	     //  for (int j = 0; j < 100; j++)
	     //  {
	     //  	UE_LOG(LogTemp, Warning, TEXT("Depth[%i][%i] = %f"), i, j, ptr[i*100+j].R.GetFloat());
	     //  }
      // 	UE_LOG(LogTemp, Warning, TEXT("\n"));
      // }		
      // int i = 0;
      // FFloat16Color* ptr = DepthCameraBuffer.GetData();
      // while (i < 10)
      // {
      // 	UE_LOG(LogTemp, Warning, TEXT("Depth[%i] = %f"), i, ptr[i].R.GetFloat());
      // 	i++;
      // }

      // DepthCameraDataNeedsCompress = false; TO BE IMPLEMENTED

      DepthCameraRendered = true;

      depth_stamp_ = stamp;

      if (!isExternallyLocked) {
        DepthCameraBufferCriticalSection->Unlock();
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

void TransformImageArrayToUint16(int32 Width, int32 Height, const TArray<FFloat16Color>& DepthCameraBuffer, std::vector<uint16_t>& OutArray)
{
	const int32 PixelCount = Width * Height;
	OutArray.resize(PixelCount);

	for (int32 i = 0; i < PixelCount; ++i)
	{
		OutArray[i] = DepthCameraBuffer[i].R.Encoded;  // Copy 16-bit depth
	}
}

bool ADronePawn::GetDepthCameraDataFromServerThread(std::vector<uint16_t>& OutArray, double& stamp)
{
	DepthCameraBufferCriticalSection->Lock();

	depth_camera_last_request_time_ = FPlatformTime::Seconds();

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND) {
		auto Instruction      = std::make_shared<FInstruction<ADronePawn>>();
		Instruction->Function = [](ADronePawn& _DronePawn) { _DronePawn.UpdateCamera(true, CAMERA_MODE_DEPTH); };
		InstructionQueue->Enqueue(Instruction);
		FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });
	}

	if (!DepthCameraRendered) {
		UE_LOG(LogTemp, Warning, TEXT("DronePawn(): depth camera is not rendered, returning"));
		DepthCameraBufferCriticalSection->Unlock();
		return false;
	}

	/* if (RgbSegCameraDataNeedsCompress) { */
	/*   FImageUtils::ThumbnailCompressImageArray(RenderTarget2DRgbSeg->SizeX, RenderTarget2DRgbSeg->SizeY, RgbSegCameraBuffer, *CompressedRgbSegCameraData); */
	/*   RgbSegCameraDataNeedsCompress = false; */
	/* } */

	TransformImageArrayToUint16(RenderTarget2DDepth->SizeX, RenderTarget2DDepth->SizeY, DepthCameraBuffer, OutArray);

	// FFloat16Color Color;
	// Color.R.Encoded (uint16)
	//
	// auto byteArray = MakeUnique<TArray<uint16>>();
	// ADronePawn::TransformImageArray(RenderTarget2DDepth->SizeX, RenderTarget2DDepth->SizeY, DepthCameraBuffer, *byteArray);
	//
	// const auto Size = (*byteArray).Num();
	// OutArray.SetNumUninitialized(Size);
	// FMemory::Memcpy(OutArray.GetData(), (*byteArray).GetData(), Size * sizeof(uint8));

	DepthCameraBufferCriticalSection->Unlock();

	stamp = depth_stamp_;

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

  LidarConfig.Enable     = Config.Enable;
  LidarConfig.Livox     = Config.Livox;
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

	SceneCaptureComponent2DDepth->FOVAngle = Config.FOVAngle;

  if (Config.enable_hdr) {
    SceneCaptureComponent2DRgb->CaptureSource = SCS_FinalColorHDR;
  } else {
    SceneCaptureComponent2DRgb->CaptureSource = SCS_FinalColorLDR;
  }

  SceneCaptureComponent2DRgb->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);
	SceneCaptureComponent2DRgbSeg->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);
	SceneCaptureComponent2DDepth->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);
	
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

  if (Config.Width > 0 && Config.Height > 0) {
    RenderTarget2DRgb->ResizeTarget(Config.Width, Config.Height);
    RenderTarget2DRgbSeg->ResizeTarget(Config.Width, Config.Height);
  	RenderTarget2DDepth->ResizeTarget(Config.Width, Config.Height);
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
  	DepthCameraBufferCriticalSection->Lock();
	
  	if ((FPlatformTime::Seconds() - depth_camera_last_request_time_) > 1.0) {
  		if (SceneCaptureComponent2DDepth->bCaptureEveryFrame) {
  			SceneCaptureComponent2DDepth->bCaptureEveryFrame = false;
  			DepthCameraRendered                              = false;
  			UE_LOG(LogTemp, Warning, TEXT("Turning off DEPTH Camera capture"));
  		}
  	} else {
  		if (!SceneCaptureComponent2DDepth->bCaptureEveryFrame) {
  			SceneCaptureComponent2DDepth->bCaptureEveryFrame = true;
  			UE_LOG(LogTemp, Warning, TEXT("Turning on DEPTH Camera capture"));
  		} else {
  			UpdateCamera(true, CAMERA_MODE_DEPTH, start);
  		}
  	}
	
  	DepthCameraBufferCriticalSection->Unlock();
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
