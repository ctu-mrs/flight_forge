#pragma once

#include "CoreMinimal.h"
#include "Camera.h"

#include "RgbCamera.generated.h"


/* FCameraExposure / FCameraLensEffects //{ */

// physically-based exposure; Manual replaces auto-exposure with the EV100
// model driven by shutter time, ISO and f-stop
struct FCameraExposure
{
	bool   Manual         = false;
	double ShutterTime    = 1.0 / 120.0;  // seconds
	double ISO            = 100.0;
	double EvCompensation = 0.0;
};

struct FCameraLensEffects
{
	double Fstop               = 4.0;
	double FocalDistance       = 0.0;  // meters; <= 0 disables depth of field
	double SensorWidthMm       = 23.76;
	double VignetteIntensity   = 0.0;
	double ChromaticAberration = 0.0;
	double BloomIntensity      = 0.675;
	double LensFlareIntensity  = 0.0;
	double WhiteTemp           = 0.0;  // Kelvin; <= 0 keeps engine default
	double WhiteTint           = 0.0;

	// derive motion blur length from shutter/frame time instead of the
	// free-standing amount
	bool MotionBlurFromShutter = false;
};

//}

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

	FCameraExposure    Exposure;
	FCameraLensEffects Lens;
};


UCLASS()
class UEDS_API URgbCamera : public UCamera
{
	GENERATED_BODY()

public:
	URgbCamera();

	std::unique_ptr<TQueue<std::shared_ptr<FInstruction<URgbCamera>>>> InstructionQueue;
	
protected:
	virtual void BeginPlay() override;

	bool GetRgbCameraDataFromServerThread(TArray<uint8>& OutArray, double &stamp);
	
	FRgbCameraConfig rgb_camera_config_;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	//FRgbCameraConfig GetRgbCameraConfig();
	bool SetRgbCameraConfig(const FRgbCameraConfig& Config);

	void ApplyExposureAndLens(const FCameraExposure& Exposure, const FCameraLensEffects& Lens, FPostProcessSettings& PP);

	virtual void UpdateCamera(bool isExternallyLocked, double stamp) override;
	
	virtual void GetConfig(std::stringstream& OutputStream) override;
	virtual void SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream) override;
	virtual void GetData(std::stringstream& OutputStream) override;
	
};
