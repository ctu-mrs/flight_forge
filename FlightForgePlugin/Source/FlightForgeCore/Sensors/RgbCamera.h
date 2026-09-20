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
	bool ShowCameraComponent = false;

	FVector  Offset             = DEFAULT_CAMERA_MOUNT;
	FRotator Orientation        = FRotator::ZeroRotator;
	double   FOVAngle           = 90.0;
	int      Width              = 640;
	int      Height             = 480;
	bool     enable_temporal_aa = true;
	bool     enable_hdr         = true;
	bool     enable_raytracing  = true;
	bool     enable_motion_blur     = true;
	double   motion_blur_amount     = 1.0;
	double   motion_blur_distortion = 50.0;

	FCameraExposure       Exposure;
	FCameraLensEffects    Lens;
	FCameraIntrinsics     Intrinsics;
	FCameraDistortion     Distortion;
	FCameraNoise          Noise;
	FCameraRollingShutter RollingShutter;
	FCameraCompression    Compression;
};


UCLASS()
class FLIGHTFORGECORE_API URgbCamera : public UCamera
{
	GENERATED_BODY()

public:
	URgbCamera();

	std::unique_ptr<TQueue<std::shared_ptr<FInstruction<URgbCamera>>>> InstructionQueue;
	
protected:
	virtual void BeginPlay() override;

	bool GetRgbCameraDataFromServerThread(TArray<uint8>& OutArray, double &stamp);
	
	FRgbCameraConfig rgb_camera_config_;

	// consecutive-capture pose delta, for the rolling-shutter angular velocity
	FQuat  LastCaptureQuat = FQuat::Identity;
	double LastCaptureTime = 0.0;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	//FRgbCameraConfig GetRgbCameraConfig();
	bool SetRgbCameraConfig(const FRgbCameraConfig& Config);

	FRgbCameraConfig GetRgbCameraConfig() const { return rgb_camera_config_; }

	virtual void DescribeCameras(std::vector<Serializable::Drone::CameraInfo>& Out) override
	{
		const FRgbCameraConfig& C = rgb_camera_config_;
		auto Info = PinholeInfo(GetSensorID(), GetSensorType(), C.Intrinsics, C.Distortion, C.FOVAngle, C.Width, C.Height, C.Offset, C.Orientation);
		Info.readout_time_ = C.RollingShutter.Enable ? C.RollingShutter.ReadoutTime : 0.0;
		Out.push_back(Info);
	}

	void ApplyExposureAndLens(const FCameraExposure& Exposure, const FCameraLensEffects& Lens, FPostProcessSettings& PP);

	virtual void UpdateCamera(bool isExternallyLocked, double stamp) override;
	
	virtual void GetConfig(std::stringstream& OutputStream) override;
	virtual void SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream) override;
	virtual void GetData(std::stringstream& OutputStream) override;
	
};
