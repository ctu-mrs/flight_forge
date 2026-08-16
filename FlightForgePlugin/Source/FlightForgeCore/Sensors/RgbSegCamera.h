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

	bool GetRgbSegCameraFromServerThread(TArray<uint8>& OutArray, double &stamp);

	FRgbCameraConfig rgb_camera_config_;

	// set from the render thread once the blendable's shaders are actually usable there;
	// shared so an in-flight probe outliving the camera writes into live memory
	TSharedPtr<std::atomic<bool>, ESPMode::ThreadSafe> BlendableLive = MakeShared<std::atomic<bool>, ESPMode::ThreadSafe>(false);

	int32 WarmupCapturesRemaining = 2;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	bool SetRgbSegCameraConfig(const FRgbCameraConfig& Config);

	virtual void UpdateCamera(bool isExternallyLocked, double stamp) override;
	
	virtual void GetConfig(std::stringstream& OutputStream) override;
	virtual void SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream) override;
	
	virtual void GetData(std::stringstream& OutputStream) override;
};
