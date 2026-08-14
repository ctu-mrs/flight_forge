#pragma once

#include "CoreMinimal.h"
#include "Camera.h"
#include "RgbCamera.h"
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

	bool GetInstanceSegFromServerThread(TArray<uint8>& OutArray, double& stamp);

	FRgbCameraConfig seg_camera_config_;

	TArray<TPair<int32, FString>> InstanceMap;

	double LastSubmitTime = 0.0;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool SetSegIdCameraConfig(const FRgbCameraConfig& Config);

	virtual void UpdateCamera(bool isExternallyLocked, double stamp) override;

	virtual void GetConfig(std::stringstream& OutputStream) override;

	virtual void SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream) override;

	virtual void GetData(std::stringstream& OutputStream) override;

	void GetMapData(std::stringstream& OutputStream);
};

/* //} */
