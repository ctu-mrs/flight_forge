#pragma once

#include "CoreMinimal.h"
#include "Camera.h"
#include "RgbCamera.h"
#include "SegIdCamera.generated.h"

/* USegIdCamera //{ */

/**
 * Instance-segmentation camera backed by the plugin's SegId mesh pass (see
 * Segmentation/SegIdPass.h): every actor renders as its 24-bit instance id with
 * correct occlusion, unlimited by the 255-value stencil cap. The scene capture
 * only drives the view; the ID image comes from the pass's own render target and
 * readback, so no tonemapping or gamma ever touches the values.
 *
 * Speaks the RGB camera config messages (resolution, mount pose, FOV, custom
 * intrinsics, distortion), so it can be given the exact calibration of the RGB
 * camera it pairs with.
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

	bool GetInstanceSegFromServerThread(TArray<uint8>& OutArray, double& stamp);

	FRgbCameraConfig seg_camera_config_;

	// id -> actor paths snapshotted with the last submitted frame; guarded by CameraBufferCriticalSection
	TArray<TPair<int32, FString>> InstanceMap;

	double LastSubmitTime = 0.0;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool SetSegIdCameraConfig(const FRgbCameraConfig& Config);

	virtual void UpdateCamera(bool isExternallyLocked, double stamp) override;

	virtual void GetConfig(std::stringstream& OutputStream) override;
	virtual void SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream) override;
	virtual void GetData(std::stringstream& OutputStream) override;

	// serves get_instance_seg_map from the snapshot paired with the last frame
	void GetMapData(std::stringstream& OutputStream);
};

/* //} */
