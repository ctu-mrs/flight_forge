// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera.h"
#include "StereoCamera.generated.h"


struct FStereoCameraConfig
{
	bool ShowCameraComponent;

	FVector  Offset_left;
	FVector  Offset_right;
	FRotator Orientation_left;
	FRotator Orientation_right;
	double   FOVAngle;
	int      Width;
	int      Height;
	bool     enable_temporal_aa;
	bool     enable_hdr;
	bool     enable_raytracing;
	double   CaptureRate = 0.0;  // Hz in simulated time, 0 = every tick

	// shared by both eyes
	FCameraIntrinsics  Intrinsics;
	FCameraDistortion  Distortion;
	FCameraCompression Compression;
};


UCLASS()
class FLIGHTFORGECORE_API UStereoCamera : public UCamera
{
	GENERATED_BODY()

public:
	UStereoCamera();

	//base camera properties are used as stereo left
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTextureRenderTarget2D* RenderTarget2DStereoRight;
	
	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
	UStaticMeshComponent* SceneCaptureMeshHolderStereoRight;

	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
	USceneCaptureComponent2D* SceneCaptureComponent2DStereoRight;

	TArray<FColor> StereoRightCameraBuffer;
	
	//FStereoCameraConfig GetStereoCameraConfig();

	std::unique_ptr<TInstructionQueue<UStereoCamera>> InstructionQueue;
	
	virtual void BeginDestroy() override;
	
protected:
	virtual void BeginPlay() override;
	virtual void Initialize(int InSensorID) override;
	

	FStereoCameraConfig stereo_camera_config_;

	bool GetStereoCameraDataFromServerThread(FCameraFrame& OutLeft, FCameraFrame& OutRight);

	// the base ring carries the left eye; the right eye rides here in step and both publish together
	TArray<TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>> InflightRight;

	bool PumpStereoReadback();

	virtual void DrainReadback() override;

	virtual bool PumpFrames() override { return PumpStereoReadback(); }

public:
	virtual void DrainInstructions() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	bool SetStereoCameraConfig(const FStereoCameraConfig& Config);

	FStereoCameraConfig GetStereoCameraConfig() const { return stereo_camera_config_; }

	virtual void DescribeCameras(std::vector<flightforge::wire::CameraInfoT>& Out) override
	{
		const FStereoCameraConfig& C = stereo_camera_config_;
		const double BaselineM = FVector::Dist(C.Offset_left, C.Offset_right) / 100.0;
		auto Left  = PinholeInfo(GetSensorID(), GetSensorType(), C.Intrinsics, C.Distortion, C.FOVAngle, C.Width, C.Height, C.Offset_left, C.Orientation_left);
		auto Right = PinholeInfo(GetSensorID(), GetSensorType(), C.Intrinsics, C.Distortion, C.FOVAngle, C.Width, C.Height, C.Offset_right, C.Orientation_right);
		Left.baseline_m = Right.baseline_m = BaselineM;
		Right.eye = flightforge::wire::Eye::RIGHT;
		Out.push_back(std::move(Left));
		Out.push_back(std::move(Right));
	}

	virtual void UpdateCamera(bool isExternallyLocked, double stamp) override;
	
	virtual void GetConfig(FWireReply& Reply) override;
	virtual void SetConfig(const flightforge::wire::Request& Request, FWireReply& Reply) override;
	virtual void GetData(const flightforge::wire::Request& Request, FWireReply& Reply) override;
	virtual void SetMount(const FVector& OffsetCm, const FRotator& Rotation) override;
};
