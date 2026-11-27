#pragma once

#include "CoreMinimal.h"
#include "Camera.h"
#include "FisheyeCamera.generated.h"


enum class EFisheyeLensModel : int
{
	EQUIDISTANT    = 0,  // r = f * theta
	EQUISOLID      = 1,  // r = 2f * sin(theta/2)
	STEREOGRAPHIC  = 2,  // r = 2f * tan(theta/2)
	KANNALA_BRANDT = 3,  // r = f * theta * (1 + k1 theta^2 + k2 theta^4 + k3 theta^6 + k4 theta^8), OpenCV fisheye
};

struct FFisheyeCameraConfig
{
	bool ShowCameraComponent = false;

	FVector  Offset      = DEFAULT_CAMERA_MOUNT;
	FRotator Orientation = FRotator::ZeroRotator;

	double FOVAngle;  // rendered angle cap even with custom intrinsics

	int Width;
	int Height;

	EFisheyeLensModel LensModel;

	bool FullFrame = false;
	double CaptureRate = DEFAULT_CAPTURE_RATE;  // 0 = every tick

	// with UseCustom, Fx/Fy are pixels per radian and Cx/Cy the principal point (engine convention) instead of the fov-fitted focal
	FCameraIntrinsics Intrinsics;

	// Kannala-Brandt theta-polynomial, used by KANNALA_BRANDT only
	double K1 = 0.0;
	double K2 = 0.0;
	double K3 = 0.0;
	double K4 = 0.0;

	FCameraCompression Compression;
};

/* FFisheyeLUTEntry //{ */

// one output pixel's source: a face and bilinear coordinates within it;
// face < 0 marks pixels outside the image circle
struct FFisheyeLUTEntry
{
	int8  Face;
	float FX;
	float FY;
};

//}

UCLASS()
class FLIGHTFORGECORE_API UFisheyeCamera : public UCamera
{
	GENERATED_BODY()

public:
	UFisheyeCamera();

	static constexpr int32 NumFaces = 5;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TArray<UTextureRenderTarget2D*> FaceRenderTargets;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TArray<USceneCaptureComponent2D*> FaceCaptures;

	std::unique_ptr<TInstructionQueue<UFisheyeCamera>> InstructionQueue;

	bool SetFisheyeCameraConfig(const FFisheyeCameraConfig& Config);

	FFisheyeCameraConfig GetFisheyeCameraConfig() const { return fisheye_camera_config_; }

	virtual void DescribeCameras(std::vector<flightforge::wire::CameraInfoT>& Out) override;

	virtual USceneCaptureComponent2D* ViewCapture(int32 View) const override { return FaceCaptures.IsValidIndex(View) ? FaceCaptures[View] : nullptr; }

	// pixels per radian of theta such that theta = fov/2 lands on the image circle, matching RebuildLUT
	static double FocalPx(EFisheyeLensModel Model, double FovDeg, int32 Width, int32 Height, bool FullFrame);

	// the intrinsics the LUT renders with (fov-fitted or custom), engine pixel convention
	static FCameraIntrinsics EffectiveFisheyeIntrinsics(const FFisheyeCameraConfig& Config);

	// theta (from the optical axis) and phi (image angle, +u right, +v down) of pixel centre (px, py);
	// false outside the rendered angle. This is the model the LUT is built from, so it is also what get_camera_info must invert.
	static bool PixelToAngles(const FFisheyeCameraConfig& Config, const FCameraIntrinsics& K, double px, double py, double& Theta, double& Phi);

	// Kannala-Brandt theta-series of the closed-form lenses, for reporting them through the equidistant distortion model
	static void KannalaBrandtCoefficients(const FFisheyeCameraConfig& Config, double& K1, double& K2, double& K3, double& K4);

	// the calibration get_camera_info reports for this config (sensor id/type left for the caller), wire pixel convention
	static flightforge::wire::CameraInfoT FisheyeInfo(const FFisheyeCameraConfig& Config);

	virtual void DrainInstructions() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void UpdateCamera(bool isExternallyLocked, double stamp) override;

	virtual void GetConfig(FWireReply& Reply) override;
	virtual void SetConfig(const flightforge::wire::Request& Request, FWireReply& Reply) override;
	virtual void GetData(const flightforge::wire::Request& Request, FWireReply& Reply) override;
	virtual void SetMount(const FVector& OffsetCm, const FRotator& Rotation) override;

	virtual void BeginDestroy() override;

	bool GetFisheyeCameraDataFromServerThread(FCameraFrame& OutFrame);

protected:
	virtual void BeginPlay() override;
	virtual void Initialize(int InSensorID) override;

	FFisheyeCameraConfig fisheye_camera_config_;

	TArray<TArray<FColor>> FaceBuffers;
	int32 PublishedFaceSize = 0;

	TSharedPtr<const TArray<FFisheyeLUTEntry>, ESPMode::ThreadSafe> LUT;
	FIntPoint LUTSize = FIntPoint::ZeroValue;

	// one face set per frame in flight, published only when every face of the head set has landed
	TArray<TArray<TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>>> InflightFaceSets;

	int32 FaceSize() const;

	void RebuildLUT();

	bool PumpFisheyeReadback();

	virtual void DrainReadback() override;

	void SetCaptureEveryFrame(bool bEnable);
};
