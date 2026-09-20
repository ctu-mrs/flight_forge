#pragma once

#include "CoreMinimal.h"
#include "Camera.h"
#include "RgbCamera.h"
#include "DepthCamera.generated.h"

/* UDepthCamera //{ */

/**
 * Depth camera: SCS_SceneDepth into an R16F target, published as uint16
 * millimetres (0 invalid, 65535 clamp - the Kinect/RealSense convention) over
 * the async GPU readback pipeline.
 *
 * Rewritten from PR #11 by Aidar (abdzhaid@cvut.cz) onto the multi-sensor
 * architecture. Speaks the RGB camera config messages (resolution, mount pose,
 * FOV, custom intrinsics), so it can share the calibration of the camera it is
 * depth for; distortion warping of depth is not applied yet.
 */
UCLASS()
class FLIGHTFORGECORE_API UDepthCamera : public UCamera
{
	GENERATED_BODY()

public:
	UDepthCamera();

	std::unique_ptr<TQueue<std::shared_ptr<FInstruction<UDepthCamera>>>> InstructionQueue;

protected:
	virtual void BeginPlay() override;

	bool GetDepthCameraDataFromServerThread(TArray<uint16>& OutDepthMm, FIntPoint& OutSize, double& stamp);

	FRgbCameraConfig depth_camera_config_;

public:
	virtual void DrainInstructions() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool SetDepthCameraConfig(const FRgbCameraConfig& Config);

	virtual void DescribeCameras(std::vector<Serializable::Drone::CameraInfo>& Out) override
	{
		const FRgbCameraConfig& C = depth_camera_config_;
		Out.push_back(PinholeInfo(GetSensorID(), GetSensorType(), C.Intrinsics, FCameraDistortion{}, C.FOVAngle, C.Width, C.Height, C.Offset, C.Orientation));
	}

	FRgbCameraConfig GetDepthCameraConfig() const
	{
		return depth_camera_config_;
	}

	virtual void UpdateCamera(bool isExternallyLocked, double stamp) override;

	virtual void GetConfig(std::stringstream& OutputStream) override;
	virtual void SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream) override;
	virtual void GetData(std::stringstream& OutputStream) override;
};

/* //} */
