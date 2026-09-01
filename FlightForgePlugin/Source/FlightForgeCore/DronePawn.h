// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "FlightForgeCore/Server/DroneServer.h"
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
#include "Sensors/EventCamera.h"
#include "Sensors/FisheyeCamera.h"
#include "Sensors/SegIdCamera.h"
#include "Sensors/DepthCamera.h"

#include "GameFramework/Pawn.h"

#include "GameFramework/Actor.h"

#include "Utils/AssetResolver.h"
#include "Utils/ExternalModelConfig.h"

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
class FLIGHTFORGECORE_API ADronePawn : public APawn {
  GENERATED_BODY()

public:
  ADronePawn();
  
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

  std::unique_ptr<TQueue<std::shared_ptr<FInstruction<ADronePawn>>>> InstructionQueue;

protected:
  virtual void BeginPlay() override;
  //virtual void PostInitializeComponents() override;
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  void StartServer();

  void SetCameraCaptureMode(CameraCaptureModeEnum CaptureMode);

  CameraCaptureModeEnum GetCameraCaptureMode();
  
  // bool SetRgbCameraConfig(const FRgbCameraConfig& Config, int ID);
  //
  // bool SetStereoCameraConfig(const FStereoCameraConfig& Config, int ID);

  void SetLocation(FVector& Location, FVector& TeleportedToLocation, bool CheckCollisions, FHitResult& HitResult);

  /* pose trajectory //{ */

  struct FPoseSample
  {
    double  T;
    FVector Location;
    FQuat   Rotation;
  };

  // any thread; empty detaches the pawn again
  void SetPoseTrajectory(TArray<FPoseSample>&& Samples);

  //}

  bool GetCrashState(void);

  bool DrawMovementLine = false;

  bool uav_crashed_ = false;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlightForge|Drone")
  bool propellers_rotate = false;

  FTimerHandle TimerHandle_Disabled_Physics;

  void SetStaticMesh(const std::string &frame_name);

  /** Directories searched for user models that are not cooked into the build. */
  UPROPERTY(EditAnywhere, Category = "FlightForge|External Model")
  TArray<FString> ExternalModelPaths;

  void Simulate_UE_Physics(const float &stop_simulation_delay);

  void SetVisibilityOtherDrones(bool bEnable);
  
  bool GetVisibilityOtherDrones();

  //void UpdateCamera(int ID, bool isExternallyLocked, double stamp);

  void UpdateCameraSensorsMutualVisibility(TArray<AActor*>& DronesToBeHidden);

  //should be called from game thread
  int AddSensor(int SensorTypeNum);
  bool RemoveSensor(int sensorID);

  USensor* FindSensorById(int SensorID);

  // server thread: id of the sensor a request addresses; RequestedID < 0 means
  // the default instance of Type, created lazily on first use. Returns -1 when
  // RequestedID does not exist or is of an incompatible type.
  int ResolveSensorId(SensorType Type, int RequestedID);

  void GetSensorList(std::vector<int>& OutIds, std::vector<int>& OutTypes);

  // server-thread wrappers; the actual component work hops to the game thread
  int  AddSensorFromServerThread(int SensorTypeNum);
  bool RemoveSensorFromServerThread(int SensorID);

  // game thread: instantiates a device preset at a mount pose; returns the
  // created sensor ids/types, optionally attaching a debug mesh
  bool AddDevice(const FString& DeviceName, const FVector& MountOffset, const FRotator& MountRotation, bool bShowMesh, std::vector<int>& OutIds,
                 std::vector<int>& OutTypes);

  bool AddDeviceFromServerThread(const FString& DeviceName, const FVector& MountOffset, const FRotator& MountRotation, bool bShowMesh, std::vector<int>& OutIds,
                                 std::vector<int>& OutTypes);
  
private:

  int nextSensorID;

  // guards Sensors and DefaultSensorOfType; sensors are created/destroyed on
  // the game thread but resolved from server threads
  std::unique_ptr<FCriticalSection> SensorsCriticalSection;

  TMap<SensorType, int> DefaultSensorOfType;

  bool bCanSeeOtherDrone = true;

  std::unique_ptr<FCriticalSection> TrajectoryCriticalSection;
  TArray<FPoseSample>               Trajectory;

  void ApplyTrajectory(double SimTime);

  void Tick(float DeltaSeconds) override;

  void SetPropellersTransform(const std::string &frame_name);

  /* external models //{ */

  /** Imports a user model from disk and rebuilds the propellers from its sidecar. */
  bool LoadExternalModel(const FString &ModelName);

  void ApplyExternalPropellers(const FExternalModelConfig &Config);

  void ClearDynamicPropellers();

  /** Propellers built at runtime for an external model, which has an arbitrary number of them. */
  UPROPERTY()
  TArray<TObjectPtr<UStaticMeshComponent>> DynamicPropellers;

  UPROPERTY()
  TObjectPtr<UAssetResolver> ExternalModelResolver;

  /* //} */
  
  void DisabledPhysics_StartRotatePropellers();

  CameraCaptureModeEnum CameraCaptureMode = CameraCaptureModeEnum::CAPTURE_ON_DEMAND; //initialized by GameModeBase

  TArray<FramePropellersTransform> FramePropellersTransforms;
};
