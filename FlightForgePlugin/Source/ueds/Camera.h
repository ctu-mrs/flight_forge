#pragma once

#include "CoreMinimal.h"
#include "Sensor.h"
#include "Server/UedsGameModeServer.h"
#include "Camera.generated.h"

enum CameraMode
{
	CAMERA_MODE_NONE         = 0,
	CAMERA_MODE_RGB          = 1,
	CAMERA_MODE_STEREO       = 2,
	CAMERA_MODE_RGB_SEG      = 3,
};

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

struct FStereoCameraConfig
{
	bool ShowCameraComponent;

	FVector  Offset;
	FRotator Orientation;
	double   FOVAngle;
	int      Width;
	int      Height;
	double   baseline;
	bool     enable_temporal_aa;
	bool     enable_hdr;
	bool     enable_raytracing;
};

using Serializable::GameMode::CameraCaptureModeEnum;

UCLASS()
class UEDS_API UCamera : public USensor
{
	GENERATED_BODY()

public:
	UCamera();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTextureRenderTarget2D* RenderTarget2DRgb;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTextureRenderTarget2D* RenderTarget2DStereoLeft;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTextureRenderTarget2D* RenderTarget2DStereoRight;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTextureRenderTarget2D* RenderTarget2DRgbSeg;

	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
	UStaticMeshComponent* SceneCaptureMeshHolderRgb;

	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
	UStaticMeshComponent* SceneCaptureMeshHolderRgbSeg;

	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
	UStaticMeshComponent* SceneCaptureMeshHolderStereoLeft;

	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
	UStaticMeshComponent* SceneCaptureMeshHolderStereoRight;

	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
	USceneCaptureComponent2D* SceneCaptureComponent2DRgb;

	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
	USceneCaptureComponent2D* SceneCaptureComponent2DRgbSeg;

	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
	USceneCaptureComponent2D* SceneCaptureComponent2DStereoLeft;

	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
	USceneCaptureComponent2D* SceneCaptureComponent2DStereoRight;

	UPROPERTY(EditAnywhere, Category = "Segmentation PostProcess Setup")
	UMaterial* PostProcessMaterial = nullptr;

#if PLATFORM_WINDOWS
	std::unique_ptr<FWindowsCriticalSection> RgbCameraBufferCriticalSection;
	std::unique_ptr<FWindowsCriticalSection> StereoCameraBufferCriticalSection;
	std::unique_ptr<FWindowsCriticalSection> RgbSegCameraBufferCriticalSection;
#else
	std::unique_ptr<FPThreadsCriticalSection> RgbCameraBufferCriticalSection;
	std::unique_ptr<FPThreadsCriticalSection> StereoCameraBufferCriticalSection;
	std::unique_ptr<FPThreadsCriticalSection> RgbSegCameraBufferCriticalSection;
#endif

	TArray<FColor> RgbCameraBuffer;
	TArray<FColor> StereoLeftCameraBuffer;
	TArray<FColor> StereoRightCameraBuffer;
	TArray<FColor> SemanticBuffer;
	TArray<FColor> RgbSegCameraBuffer;

	double rgb_camera_last_request_time_ = 0;
	double rgb_seg_camera_last_request_time_ = 0;
	double stereo_camera_last_request_time_ = 0;

	double rgb_stamp_ = 0;
	double rgb_seg_stamp_ = 0;
	double stereo_stamp_ = 0;
	
	virtual void BeginDestroy() override;
	
protected:
	virtual void BeginPlay() override;

	// Cleanup on destruction

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	bool GetRgbCameraDataFromServerThread(TArray<uint8>& OutArray, double &stamp);

	bool GetStereoCameraDataFromServerThread(TArray<uint8>& image_left, TArray<uint8>& image_right, double &stamp);

	bool GetRgbSegCameraFromServerThread(TArray<uint8>& OutArray, double &stamp);

	void SetCameraCaptureMode(CameraCaptureModeEnum CaptureMode);

	FRgbCameraConfig GetRgbCameraConfig();
	bool             SetRgbCameraConfig(const FRgbCameraConfig& Config);

	FStereoCameraConfig GetStereoCameraConfig();
	bool                SetStereoCameraConfig(const FStereoCameraConfig& Config);

	void UpdateCamera(bool isExternallyLocked, int type, double stamp);
	
private:
	CameraCaptureModeEnum CameraCaptureMode = CameraCaptureModeEnum::CAPTURE_ALL_FRAMES;
	bool CameraNeedsRefresh = false;
	FRgbCameraConfig rgb_camera_config_;
	FStereoCameraConfig stereo_camera_config_;

	std::unique_ptr<TArray<uint8>> CompressedRgbCameraData         = std::make_unique<TArray<uint8>>();
	std::unique_ptr<TArray<uint8>> CompressedStereoLeftCameraData  = std::make_unique<TArray<uint8>>();
	std::unique_ptr<TArray<uint8>> CompressedStereoRightCameraData = std::make_unique<TArray<uint8>>();
	std::unique_ptr<TArray<uint8>> CompressedRgbSegCameraData      = std::make_unique<TArray<uint8>>();

	bool RgbCameraDataNeedsCompress         = false;
	bool StereoCameraDataNeedsCompress      = false;
	bool RgbSegCameraDataNeedsCompress      = false;

	bool RgbCameraRendered                  = false;
	bool RgbSegCameraRendered               = false;
	bool StereoCameraRendered               = false;

	void TransformImageArray(int32 ImageWidth, int32 ImageHeight, const TArray<FColor> &SrcData, TArray<uint8> &DstData);
	
};
