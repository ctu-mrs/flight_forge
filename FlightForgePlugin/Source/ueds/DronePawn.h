// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Server/DroneServer.h"
#include "Instruction.h"
#include "Sensor.h"
#include "Lidar.h"
#include "Camera.h"
#include "RangeFinder.h"

#include "GameFramework/Pawn.h"

#include "GameFramework/Actor.h"

#include "DronePawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USceneCaptureComponent2D;
class UTextureRenderTarget2D;

enum DroneFrame
{
  X500,
  T650,
  Agile,
  Robofly
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


using Serializable::GameMode::CameraCaptureModeEnum;

UCLASS()
class UEDS_API ADronePawn : public APawn {
  GENERATED_BODY()

public:
  std::shared_ptr<DroneServer> droneServer;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite ,Category = "Components")
  USpringArmComponent* ThirdPersonCameraSpringArm;

  UPROPERTY(VisibleAnywhere, Category = "Components")
  ULidar* Lidar;

  UPROPERTY(VisibleAnywhere, Category = "Components")
  URangeFinder* RangeFinder;

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

  UPROPERTY(VisibleAnywhere, Category = "Rigid Body", BlueprintReadWrite)
  UStaticMeshComponent* PropellerRearLeft;

  UPROPERTY(VisibleAnywhere, Category = "Rigid Body", BlueprintReadWrite)
  UStaticMeshComponent* PropellerRearRight;

  UPROPERTY(VisibleAnywhere, Category = "Rigid Body", BlueprintReadWrite)
  UStaticMeshComponent* PropellerFrontLeft;

  UPROPERTY(VisibleAnywhere, Category = "Rigid Body", BlueprintReadWrite)
  UStaticMeshComponent* PropellerFrontRight;

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

  void SetStaticMesh(const int &frame_id);

  void Simulate_UE_Physics(const float &stop_simulation_delay);

  void SetVisibilityOtherDrones(bool bEnable);
  
  bool GetVisibilityOtherDrones();
  
  FString CSVFilePath;
  
private:
  bool bCanSeeOtherDrone = true;
  
  void Tick(float DeltaSeconds) override;

  void UpdateCamera(bool isExternallyLocked, int type, double stamp);

  void SetPropellersTransform(const int &frame_id);
  
  void DisabledPhysics_StartRotatePropellers();

  CameraCaptureModeEnum CameraCaptureMode = CameraCaptureModeEnum::CAPTURE_ALL_FRAMES;
  
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

  TArray<FramePropellersTransform> FramePropellersTransforms;
};
