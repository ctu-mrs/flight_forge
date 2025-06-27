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

enum SensorType
{
  CAMERA,
  LIDAR,
  RANGEFINDER,

  MAX_SENSOR_TYPE
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
  UCamera* CameraSensor;

  UPROPERTY(VisibleAnywhere, Category = "Sensors")
  TArray<TObjectPtr<USensor>> Sensors;

  UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
  UCameraComponent* ThirdPersonCamera;

  UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
  UCameraComponent* DroneCamera;

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
  
  void UpdateCamera(bool isExternallyLocked, int type, double stamp);

  void UpdateCameraSensorsMutualVisibility(TArray<AActor*>& DronesToBeHidden);

  USensor* AddSensor(int SensorTypeNum);

  USensor* RemoveSensor(int sensorID);
  
private:
  // double startTime; //debugging
  // bool added = false;
  // bool removed = false;
  
  int nextSensorID;
  
  bool bCanSeeOtherDrone = true;
  
  void Tick(float DeltaSeconds) override;

  void SetPropellersTransform(const int &frame_id);
  
  void DisabledPhysics_StartRotatePropellers();

//  CameraCaptureModeEnum CameraCaptureMode = CameraCaptureModeEnum::CAPTURE_ALL_FRAMES;

  TArray<FramePropellersTransform> FramePropellersTransforms;
};
