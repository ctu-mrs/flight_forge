// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Server/DroneServer.h"
#include "Instruction.h"

#include "GameFramework/Pawn.h"

#include "GameFramework/Actor.h"


#include "CoreMinimal.h" 
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"

class USpringArmComponent;
class UCameraComponent;
class USceneCaptureComponent2D;
class UTextureRenderTarget2D;
class UStaticMeshComponent;
class UglTFRuntimeAsset;
class UglTFRuntimeFunctionLibrary;
class AGLBtoSM; 

#include "GLBtoSM.h"

#include "DronePawn.generated.h"

#define DEFAULT_LIDAR_BEAM_HOR 256  // 100
#define DEFAULT_LIDAR_BEAM_VER 32   // 15

#define DEFAULT_LIDAR_BEAM_LENGTH 1000
/* #define DEFAULT_LIDAR_BEAM_HOR 2048 */
/* #define DEFAULT_LIDAR_BEAM_VER 128 */
#define DEFAULT_LIDAR_SHOW_BEAMS false 

#define DEFAULT_RANGEFINDER_BEAM_LENGTH 3000

enum DroneFrame
{
  X500,
  T650,
  Agile,
  Robofly
};


USTRUCT(BlueprintType)
struct FExternalPropellerConfig
{
  GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "External Model")
    FTransform RelativeTransform;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "External Model")
    FString Name; // Optional name

  FExternalPropellerConfig() : RelativeTransform(FTransform::Identity), Name(TEXT("Propeller")) {}
  FExternalPropellerConfig(const FTransform& Transform, const FString& InName = TEXT("Propeller"))
    : RelativeTransform(Transform), Name(InName) {}
};


class FramePropellersTransform
{
public:
  FramePropellersTransform(const FString& FrameName, const FString& PropellerType, const FTransform& RearLeft, const FTransform& RearRight, const FTransform& FrontLeft,
    const FTransform& FrontRight)
    : FrameName(FrameName),
      PropellerType(PropellerType),
      RearLeft(RearLeft),
      RearRight(RearRight),
      FrontLeft(FrontLeft),
      FrontRight(FrontRight)
  {
  }

  FString FrameName;
  FString PropellerType;
  FTransform RearLeft;
  FTransform RearRight;
  FTransform FrontLeft;
  FTransform FrontRight;
};

/* Enum for the camera capture type */
enum CameraMode
{
  CAMERA_MODE_NONE         = 0,
  CAMERA_MODE_RGB          = 1,
  CAMERA_MODE_STEREO       = 2,
  CAMERA_MODE_RGB_SEG      = 3,
};

struct FLidarConfig
{
  bool   Enable;
  bool Livox; 
  bool   ShowBeams;
  double BeamLength;

  int BeamHorRays;
  int BeamVertRays;

  double   Frequency;
  FVector  Offset;
  FRotator Orientation;
  /* double   FOVHor; */
  double  FOVHorLeft;
  double  FOVHorRight;
  /* double   FOVVert; */
  double   FOVVertUp;
  double   FOVVertDown;
  double   vertRayDiff;
  double   horRayDif;

  bool SemanticSegmentation;
};

struct FRangefinderConfig
{
  double BeamLength;
  FVector Offset;
};

struct FRgbCameraConfig
{
  bool ShowCameraComponent;

  FVector  Offset;
  FRotator Orientation;
  double   FOVAngle;
  int      Width;
  int      Height;
  bool     enable_temporal_aa;
  bool     enable_hdr;
  bool     enable_raytracing;
  bool     enable_motion_blur;
  double   motion_blur_amount;
  double   motion_blur_distortion;
};

struct FStereoCameraConfig
{
  bool ShowCameraComponent;

  FVector  Offset;
  FRotator Orientation;
  double   FOVAngle;
  int      Width;
  int      Height;
  double   baseline;
  bool     enable_temporal_aa;
  bool     enable_hdr;
  bool     enable_raytracing;
};

struct FLivoxDataPoint {
    double Time;
    double Azimuth; // in radians
    double Zenith;  // in radians
};


using Serializable::GameMode::CameraCaptureModeEnum;

UCLASS()
class UEDS_API ADronePawn : public APawn {
  GENERATED_BODY()

public:
  std::shared_ptr<DroneServer> droneServer;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite ,Category = "Components")
  USpringArmComponent* ThirdPersonCameraSpringArm;

  UPROPERTY(VisibleAnywhere, Category = "Components")
  UTextureRenderTarget2D* RenderTarget2DRgb;

  UPROPERTY(VisibleAnywhere, Category = "Components")
  UTextureRenderTarget2D* RenderTarget2DStereoLeft;

  UPROPERTY(VisibleAnywhere, Category = "Components")
  UTextureRenderTarget2D* RenderTarget2DStereoRight;

  UPROPERTY(VisibleAnywhere, Category = "Components")
  UTextureRenderTarget2D* RenderTarget2DRgbSeg;

  UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
  UCameraComponent* ThirdPersonCamera;

  UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
  UCameraComponent* DroneCamera;

  UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
  UStaticMeshComponent* SceneCaptureMeshHolderRgb;

  UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
  UStaticMeshComponent* SceneCaptureMeshHolderRgbSeg;

  UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
  UStaticMeshComponent* SceneCaptureMeshHolderStereoLeft;

  UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
  UStaticMeshComponent* SceneCaptureMeshHolderStereoRight;

  UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
  USceneCaptureComponent2D* SceneCaptureComponent2DRgb;

  UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
  USceneCaptureComponent2D* SceneCaptureComponent2DRgbSeg;

  UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
  USceneCaptureComponent2D* SceneCaptureComponent2DStereoLeft;

  UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
  USceneCaptureComponent2D* SceneCaptureComponent2DStereoRight;

  // PostProcessMaterial used for segmentation
  UPROPERTY(EditAnywhere, Category = "Segmentation PostProcess Setup")
  UMaterial* PostProcessMaterial = nullptr;

  UPROPERTY(VisibleAnywhere, Category = "Rigid Body", BlueprintReadWrite)
  UStaticMeshComponent* RootMeshComponent;

  UPROPERTY(VisibleAnywhere, Category = "Rigid Body")
  TArray<UStaticMeshComponent*> DynamicPropellers;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "External Model", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<AGLBtoSM> GltfLoaderActor; 

  // Sets default values for this character's properties
  ADronePawn();

#if PLATFORM_WINDOWS
  std::unique_ptr<FWindowsCriticalSection> RgbCameraBufferCriticalSection;
  std::unique_ptr<FWindowsCriticalSection> StereoCameraBufferCriticalSection;
  std::unique_ptr<FWindowsCriticalSection> RgbSegCameraBufferCriticalSection;
#else
  std::unique_ptr<FPThreadsCriticalSection> RgbCameraBufferCriticalSection;
  std::unique_ptr<FPThreadsCriticalSection> StereoCameraBufferCriticalSection;
  std::unique_ptr<FPThreadsCriticalSection> RgbSegCameraBufferCriticalSection;
#endif

  TArray<FColor>                                                     RgbCameraBuffer;
  TArray<FColor>                                                     StereoLeftCameraBuffer;
  TArray<FColor>                                                     StereoRightCameraBuffer;
  TArray<FColor>                                                     SemanticBuffer;
  TArray<FColor>                                                     RgbSegCameraBuffer;

  double rgb_camera_last_request_time_ = 0;
  double rgb_seg_camera_last_request_time_ = 0;
  double stereo_camera_last_request_time_ = 0;

  double rgb_stamp_ = 0;
  double rgb_seg_stamp_ = 0;
  double stereo_stamp_ = 0;

  std::unique_ptr<TQueue<std::shared_ptr<FInstruction<ADronePawn>>>> InstructionQueue;

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  void StartServer();

  UFUNCTION(BlueprintCallable)
  void GetRangefinderData(double& range);

  void GetLidarHits(std::vector<Serializable::Drone::GetLidarData::LidarData>& OutLidarData, FVector& OutStart);

  void GetSegLidarHits(std::vector<Serializable::Drone::GetLidarSegData::LidarSegData>& OutLidarSegData, FVector& OutStart);

  void GetIntLidarHits(std::vector<Serializable::Drone::GetLidarIntData::LidarIntData>& OutLidarIntData, FVector& OutStart);

  bool GetRgbCameraDataFromServerThread(TArray<uint8>& OutArray, double &stamp);

  bool GetStereoCameraDataFromServerThread(TArray<uint8>& image_left, TArray<uint8>& image_right, double &stamp);

  void TransformImageArray(int32 ImageWidth, int32 ImageHeight, const TArray<FColor> &SrcData, TArray<uint8> &DstData);

  bool GetRgbSegCameraFromServerThread(TArray<uint8>& OutArray, double &stamp);

  void SetCameraCaptureMode(CameraCaptureModeEnum CaptureMode);

  FLidarConfig GetLidarConfig();
  bool         SetLidarConfig(const FLidarConfig& Config);

  FRgbCameraConfig GetRgbCameraConfig();
  bool             SetRgbCameraConfig(const FRgbCameraConfig& Config);

  FStereoCameraConfig GetStereoCameraConfig();
  bool                SetStereoCameraConfig(const FStereoCameraConfig& Config);

  void SetLocation(FVector& Location, FVector& TeleportedToLocation, bool CheckCollisions, FHitResult& HitResult);

  bool GetCrashState(void);

  bool DrawMovementLine = false;

  bool uav_crashed_ = false;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  bool propellers_rotate = false;

  FTimerHandle TimerHandle_Disabled_Physics;
  
  UFUNCTION(BlueprintCallable, Category = "DroneSetup")
  void SetStaticMeshByName(const FString& ModelName);

  /* void SetStaticMesh(const int &frame_id); */

  void Simulate_UE_Physics(const float &stop_simulation_delay);

  void SetVisibilityOtherDrones(bool bEnable);

  bool LoadCSVData(const FString& FilePath);
  TArray<FLivoxDataPoint> LivoxData;
  bool bLivox;
  int StartIndex;

  
  FString CSVFilePath;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "External Model")
  FString ExternalModelBasePath;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "External Model")
  TSoftObjectPtr<UStaticMesh> DefaultExternalPropellerMesh;

  FString DefaultExternalPropellerMeshPath;

  const TArray<FramePropellersTransform>& GetPredefinedFrameTransforms() const; 


protected:

  UFUNCTION(BlueprintImplementableEvent)
  UStaticMesh* LoadGLBtoSMs_BlueprintEvent(const FString& AbsPathToFile);

  bool LoadExternalModel(const FString& ModelName);

  bool ParsePropellerYAML(const FString& YamlPath, TArray<FExternalPropellerConfig>& OutPropConfigs, FString& OutPropellerMeshPath);

  void ApplyExternalPropellerConfig(const TArray<FExternalPropellerConfig>& PropConfigs, const FString& PropellerMeshPath);

  void ClearDynamicPropellers();
  
private:
  bool bCanSeeOtherDrone = true;
  
  void Tick(float DeltaSeconds) override;

  void UpdateLidar(bool isExternallyLocked);

  void UpdateSegLidar(bool isExternallyLocked);

  void UpdateIntLidar(bool isExternallyLocked);

  void UpdateCamera(bool isExternallyLocked, int type, double stamp);
  
  void SetPredefinedPropellersTransform(const int &frame_id);
  
  TArray<FramePropellersTransform> FramePropellersTransforms;

  /* UFUNCTION(BlueprintPure, Category = "DroneSetup") */
  /* void SetPropellersTransform(const int &frame_id); */
  
  void DisabledPhysics_StartRotatePropellers();

#if PLATFORM_WINDOWS
  std::unique_ptr<FWindowsCriticalSection> LidarHitsCriticalSection;
  std::unique_ptr<FWindowsCriticalSection> LidarSegHitsCriticalSection;
  std::unique_ptr<FWindowsCriticalSection> LidarIntHitsCriticalSection;
  std::unique_ptr<FWindowsCriticalSection> RangefinderHitsCriticalSection;


#else
  std::unique_ptr<FPThreadsCriticalSection> LidarHitsCriticalSection;
  std::unique_ptr<FPThreadsCriticalSection> LidarSegHitsCriticalSection;
  std::unique_ptr<FPThreadsCriticalSection> LidarIntHitsCriticalSection;
  std::unique_ptr<FPThreadsCriticalSection> RangefinderHitsCriticalSection;
#endif
  std::unique_ptr<std::vector<std::tuple<double, double, double, double>>>      LidarHits;
  std::unique_ptr<std::vector<std::tuple<double, double, double, double, int>>> LidarSegHits;
  std::unique_ptr<std::vector<std::tuple<double, double, double, double, int>>> LidarIntHits;
  std::unique_ptr<FVector>                                                      LidarHitStart;

  CameraCaptureModeEnum CameraCaptureMode = CameraCaptureModeEnum::CAPTURE_ALL_FRAMES;

  FRangefinderConfig RangefinderConfig;
  FLidarConfig LidarConfig;
  bool         CameraNeedsRefresh = false;

  FRgbCameraConfig rgb_camera_config_;
  FStereoCameraConfig stereo_camera_config_;

  std::unique_ptr<TArray<uint8>> CompressedRgbCameraData         = std::make_unique<TArray<uint8>>();
  std::unique_ptr<TArray<uint8>> CompressedStereoLeftCameraData  = std::make_unique<TArray<uint8>>();
  std::unique_ptr<TArray<uint8>> CompressedStereoRightCameraData = std::make_unique<TArray<uint8>>();
  std::unique_ptr<TArray<uint8>> CompressedRgbSegCameraData      = std::make_unique<TArray<uint8>>();

  bool RgbCameraDataNeedsCompress         = false;
  bool StereoCameraDataNeedsCompress      = false;
  bool RgbSegCameraDataNeedsCompress      = false;

  bool RgbCameraRendered                  = false;
  bool RgbSegCameraRendered               = false;
  bool StereoCameraRendered               = false;

  bool bIsExternalModelLoaded = false;

};
