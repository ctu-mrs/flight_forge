// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "ueds/Server/DroneServer.h"
#include "Server/DroneServer.h"
#include "Instruction.h"
#include "Sensors/Sensor.h"
#include "Sensors/Lidar.h"
#include "Sensors/Camera.h"
#include "Sensors/RgbCamera.h"
#include "Sensors/RgbSegCamera.h"
#include "Sensors/StereoCamera.h"
#include "Sensors/RangeFinder.h"
#include "Sensors/LidarLivox.h"

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

  UPROPERTY(VisibleAnywhere, Category = "Sensors")
  TMap<int, TObjectPtr<USensor>> Sensors;
  
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite ,Category = "Components")
  USpringArmComponent* ThirdPersonCameraSpringArm;
  
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

  void SetCameraCaptureMode(CameraCaptureModeEnum CaptureMode);

  CameraCaptureModeEnum GetCameraCaptureMode();
  
  bool SetRgbCameraConfig(const FRgbCameraConfig& Config, int ID);
  
  bool SetStereoCameraConfig(const FStereoCameraConfig& Config, int ID);

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
  
  void UpdateCamera(int ID, bool isExternallyLocked, double stamp);

  void UpdateCameraSensorsMutualVisibility(TArray<AActor*>& DronesToBeHidden);

  int AddSensor(int SensorTypeNum);

  bool RemoveSensor(int sensorID);
  
private:
  
  int nextSensorID;
  
  bool bCanSeeOtherDrone = true;
  
  void Tick(float DeltaSeconds) override;

  void SetPropellersTransform(const int &frame_id);
  
  void DisabledPhysics_StartRotatePropellers();

  CameraCaptureModeEnum CameraCaptureMode = CameraCaptureModeEnum::CAPTURE_ON_DEMAND;

  TArray<FramePropellersTransform> FramePropellersTransforms;
};
