#pragma once

#include "CoreMinimal.h"
#include "Camera.h"
#include "RgbCamera.h"
#include "FlightForgeCore/Segmentation/SegmentationTagger.h"
#include "SegIdCamera.generated.h"

/* USegIdCamera //{ */

/**
 * Instance-segmentation camera backed by the plugin's SegId mesh pass
 */
UCLASS()
class FLIGHTFORGECORE_API USegIdCamera : public UCamera
{
	GENERATED_BODY()

public:
	USegIdCamera();

	std::unique_ptr<TQueue<std::shared_ptr<FInstruction<USegIdCamera>>>> InstructionQueue;

protected:
	virtual void BeginPlay() override;

	bool GetInstanceSegFromServerThread(FCameraFrame& OutFrame);

	FRgbCameraConfig seg_camera_config_;

	TArray<FSegInstanceEntry> InstanceMap;

	double LastSubmitWall = 0.0;

public:
	virtual void DrainInstructions() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool SetSegIdCameraConfig(const FRgbCameraConfig& Config);

	virtual void DescribeCameras(std::vector<flightforge::wire::CameraInfoT>& Out) override
	{
		const FRgbCameraConfig& C = seg_camera_config_;
		Out.push_back(PinholeInfo(GetSensorID(), GetSensorType(), C.Intrinsics, C.Distortion, C.FOVAngle, C.Width, C.Height, C.Offset, C.Orientation));
	}

	const FRgbCameraConfig& GetSegIdCameraConfig() const
	{
		return seg_camera_config_;
	}

	// raw ids of the last frame at the render target size, with the id -> actor map it was rendered against
	bool CopyLastFrame(TArray<FColor>& OutIds, FIntPoint& OutSize, TArray<FSegInstanceEntry>& OutMap);

	virtual void UpdateCamera(bool isExternallyLocked, double stamp) override;

	virtual void GetConfig(FWireReply& Reply) override;
	virtual void SetConfig(const flightforge::wire::Request& Request, FWireReply& Reply) override;
	virtual void GetData(const flightforge::wire::Request& Request, FWireReply& Reply) override;
	virtual void SetMount(const FVector& OffsetCm, const FRotator& Rotation) override;

	void GetMapData(FWireReply& Reply);
};

/* //} */
