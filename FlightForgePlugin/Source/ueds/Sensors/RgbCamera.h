#pragma once

#include "CoreMinimal.h"
#include "Camera.h"

#include "RgbCamera.generated.h"


struct FRgbCameraConfig
{
	bool ShowCameraComponent;

	FVector  Offset;
	FRotator Orientation;
	double   FOVAngle;
	int      Width;
	int      Height;
	bool     enable_temporal_aa;
	bool     enable_hdr;
	bool     enable_raytracing;
	bool     enable_motion_blur;
	double   motion_blur_amount;
	double   motion_blur_distortion;
};


UCLASS()
class UEDS_API URgbCamera : public UCamera
{
	GENERATED_BODY()

public:
	URgbCamera();
	
protected:
	virtual void BeginPlay() override;

	bool GetRgbCameraDataFromServerThread(TArray<uint8>& OutArray, double &stamp);
	
	FRgbCameraConfig rgb_camera_config_;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	//FRgbCameraConfig GetRgbCameraConfig();
	bool SetRgbCameraConfig(const FRgbCameraConfig& Config);

	virtual void UpdateCamera(bool isExternallyLocked, double stamp) override;
	
	virtual void GetConfig(std::stringstream& OutputStream) override;
	virtual void SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream) override;
	virtual void GetData(std::stringstream& OutputStream) override;
	
};
