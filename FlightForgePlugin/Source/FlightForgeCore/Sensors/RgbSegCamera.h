#pragma once

#include "CoreMinimal.h"
#include "Camera.h"
#include "RgbCamera.h"
#include "RgbSegCamera.generated.h"


UCLASS()
class FLIGHTFORGECORE_API URgbSegCamera : public UCamera
{
	GENERATED_BODY()

public:
	URgbSegCamera();

	std::unique_ptr<TQueue<std::shared_ptr<FInstruction<URgbSegCamera>>>> InstructionQueue;

protected:
	virtual void BeginPlay() override;

	bool GetRgbSegCameraFromServerThread(FCameraFrame& OutFrame);

	FRgbCameraConfig rgb_camera_config_;

	virtual void DescribeCameras(std::vector<flightforge::wire::CameraInfoT>& Out) override
	{
		const FRgbCameraConfig& C = rgb_camera_config_;
		Out.push_back(PinholeInfo(GetSensorID(), GetSensorType(), C.Intrinsics, C.Distortion, C.FOVAngle, C.Width, C.Height, C.Offset, C.Orientation));
	}

	// set from the render thread once the blendable's shaders are actually usable there;
	// shared so an in-flight probe outliving the camera writes into live memory
	TSharedPtr<std::atomic<bool>, ESPMode::ThreadSafe> BlendableLive = MakeShared<std::atomic<bool>, ESPMode::ThreadSafe>(false);

	int32 WarmupCapturesRemaining = 2;

public:
	virtual void DrainInstructions() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	bool SetRgbSegCameraConfig(const FRgbCameraConfig& Config);

	virtual void UpdateCamera(bool isExternallyLocked, double stamp) override;
	
	virtual void GetConfig(FWireReply& Reply) override;
	virtual void SetConfig(const flightforge::wire::Request& Request, FWireReply& Reply) override;
	virtual void GetData(const flightforge::wire::Request& Request, FWireReply& Reply) override;
	virtual void SetMount(const FVector& OffsetCm, const FRotator& Rotation) override;
};
