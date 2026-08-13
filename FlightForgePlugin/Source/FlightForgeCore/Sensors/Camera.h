#pragma once

#include <atomic>

#include "CoreMinimal.h"
#include "CameraReadback.h"
#include "FlightForgeCore/Instruction.h"
#include "ImageUtils.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/Material.h"
#include "RHIGPUReadback.h"
#include "Sensor.h"

#include "Camera.generated.h"


using Serializable::GameMode::CameraCaptureModeEnum;

/* FCameraIntrinsics //{ */

// pinhole intrinsics in pixels, image origin top-left; when UseCustom the capture's
// projection matrix is built from these (off-axis when cx/cy are off-center) instead
// of from the FOV angle
struct FCameraIntrinsics
{
	bool   UseCustom = false;
	double Fx        = 0.0;
	double Fy        = 0.0;
	double Cx        = 0.0;
	double Cy        = 0.0;
};

//}

/* FCameraDistortion / FDistortionLUT //{ */

// Brown-Conrady lens distortion (OpenCV convention: radial k1..k3, tangential p1, p2)
// applied to the same normalized coordinates the intrinsics K describes
struct FCameraDistortion
{
	bool   Enable = false;
	double K1     = 0.0;
	double K2     = 0.0;
	double K3     = 0.0;
	double P1     = 0.0;
	double P2     = 0.0;
};

// Heteroscedastic sensor noise applied in the linearized domain:
// sigma = sqrt(ShotScale * signal + ReadSigma^2) per channel, plus an optional
// per-row offset (CMOS banding)
struct FCameraNoise
{
	bool   Enable    = false;
	double ShotScale = 2.0e-4;
	double ReadSigma = 3.0e-3;
	double RowSigma  = 0.0;
};

// First-order (rotation-only) rolling shutter: rows are shifted by the
// rotational image flow accumulated over their readout delay
struct FCameraRollingShutter
{
	bool   Enable      = false;
	double ReadoutTime = 0.03;  // seconds, top-to-bottom
};

// Wire encoding of a served frame. JPEG by default: PNG costs tens of milliseconds and
// several times the bytes per frame, for detail no vision pipeline sees.
struct FCameraCompression
{
	bool  bJpeg   = true;
	int32 Quality = 90;  // 1-100, JPEG only
};

// Precomputed output-pixel -> source-pixel map. The source image is an ideal pinhole
// render padded by Pad on each side (overscan), so barrel distortion can pull in
// content from outside the nominal frustum. Rebuilt only on config changes; the serve
// threads resample through it per frame.
struct FDistortionLUT
{
	FIntPoint OutputSize = FIntPoint::ZeroValue;
	FIntPoint SourceSize = FIntPoint::ZeroValue;
	FIntPoint Pad        = FIntPoint::ZeroValue;

	// per output pixel, source coords in array-index space (0 = first pixel center)
	TArray<FVector2f> Source;
};

//}

UCLASS()
class FLIGHTFORGECORE_API UCamera : public USensor
{
	GENERATED_BODY()

public:
	UCamera();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTextureRenderTarget2D* RenderTarget2D;

	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
	UStaticMeshComponent* SceneCaptureMeshHolder;

	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
	USceneCaptureComponent2D* SceneCaptureComponent2D;
	
	UPROPERTY(EditAnywhere, Category = "Segmentation PostProcess Setup")
	UMaterial* PostProcessMaterial = nullptr;


	std::unique_ptr<FCriticalSection> CameraBufferCriticalSection;

	TArray<FColor> CameraBuffer;

	// published instead of CameraBuffer for float16 (depth) captures
	TArray<FFloat16> CameraBufferF16;

	// dimensions of the published CameraBuffer, not of the (possibly resized) render target
	FIntPoint CameraBufferSize = FIntPoint::ZeroValue;

	std::atomic<double> camera_last_request_time_{0};

	double stamp_ = 0;

	virtual void BeginDestroy() override;

protected:
	CameraCaptureModeEnum CameraCaptureMode;
	bool CameraNeedsRefresh = false;

	bool CameraRendered                  = false;

	TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> PendingReadback;

	// bumped each time a frame is published to CameraBuffer; server threads wait on it
	std::atomic<uint64> FrameCounter{0};
	FEvent* FrameReadyEvent = nullptr;

	// registers this camera as a streaming view each tick while active, so
	// textures near it keep full mips even when the player viewport is
	// elsewhere or absent; without it off-pawn cameras render low-mip blur
	void PushStreamingViewHint();

	// scene captures ignore unbound PostProcessVolumes; seed the capture with
	// the level's look so sensor output matches the viewport, with per-camera
	// config overrides applied on top afterwards
	void ApplyLevelPostProcessBaseline(USceneCaptureComponent2D* Capture);

public:
	// reversed-Z off-axis projection equivalent to the pinhole K = [fx 0 cx; 0 fy cy]
	static FMatrix IntrinsicsProjectionMatrix(const FCameraIntrinsics& Intrinsics, int32 Width, int32 Height);

	// the intrinsics the rendered image actually has: the custom ones when set and
	// valid, otherwise fx = fy derived from the horizontal FOV with a centered
	// principal point
	static FCameraIntrinsics EffectiveIntrinsics(const FCameraIntrinsics& Intrinsics, double FovDeg, int32 Width, int32 Height);

	// sets either the custom projection matrix or the plain FOV path on the capture
	static void ApplyProjection(USceneCaptureComponent2D* Capture, const FCameraIntrinsics& Intrinsics, double FovDeg, int32 Width, int32 Height);

	// LUT for undoing BuildDistortionLUT's view: output pixel -> where in the padded
	// ideal-pinhole source that pixel's ray lands; null when distortion is off
	static TSharedPtr<const FDistortionLUT, ESPMode::ThreadSafe> BuildDistortionLUT(const FCameraDistortion& Distortion, const FCameraIntrinsics& EffectiveK,
	                                                                                int32 Width, int32 Height);

	// bilinear for imagery; nearest for label images so classes never blend
	static void ResampleImage(const FDistortionLUT& LUT, const TArray<FColor>& Source, TArray<FColor>& OutResampled, bool bNearestNeighbor);

	// One combined bilinear resample: distortion LUT (optional) plus the per-pixel
	// first-order rolling-shutter displacement computed from OmegaCamera (camera
	// frame: x right, y down, z forward) and the per-row readout delay.
	static void WarpFrame(const TArray<FColor>& Source, FIntPoint SourceSize, TArray<FColor>& OutFrame, FIntPoint OutputSize, const FDistortionLUT* LUT,
	                      const FCameraIntrinsics& EffectiveK, const FVector& OmegaCamera, double ReadoutTime);

	// linearize (sRGB), add shot + read + row noise, re-encode; Seed makes a
	// served frame's noise deterministic for its stamp
	static void ApplyNoise(TArray<FColor>& Pixels, FIntPoint Size, const FCameraNoise& Noise, uint32 Seed);

	// Encodes the served frame. bLossless forces PNG whatever the config says, for the
	// label cameras whose pixel values are ids rather than colour.
	static void CompressFrame(const TArray<FColor>& Pixels, FIntPoint Size, const FCameraCompression& Compression, bool bLossless, TArray<uint8>& OutBytes);

protected:
	// One-stop projection setup shared by the RGB/seg/stereo Set paths: without
	// distortion it applies the plain (or custom-K) projection at the nominal size;
	// with it, it renders the padded overscan frustum and stores the warp LUT.
	// Caller must hold CameraBufferCriticalSection (DistortionLUT is written here,
	// serve threads read it under the same lock).
	void SetupCaptureProjection(const FCameraIntrinsics& Intrinsics, const FCameraDistortion& Distortion, double FovDeg, int32 Width, int32 Height,
	                            std::initializer_list<USceneCaptureComponent2D*> Captures, std::initializer_list<UTextureRenderTarget2D*> Targets);

	TSharedPtr<const FDistortionLUT, ESPMode::ThreadSafe> DistortionLUT;

	// published with the frame in PumpReadback, read by serve threads under the buffer lock
	FVector capture_omega_ = FVector::ZeroVector;

	TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> EnqueueReadback(UTextureRenderTarget2D* RT, double Stamp, const FVector& Omega = FVector::ZeroVector,
	                                                                 bool bFloat16 = false);
	static void IssueResolve(const TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>& Slot);
	// game thread: advances the pending readback and publishes a completed frame; true when published
	bool PumpReadback();
	void NotifyFrameReady();
	// server thread: true once FrameCounter passes BaselineFrame
	bool WaitForNewFrame(uint64 BaselineFrame, double TimeoutSeconds);

	void TransformImageArray(int32 ImageWidth, int32 ImageHeight, const TArray<FColor> &SrcData, TArray<uint8> &DstData);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void SetCameraCaptureMode(CameraCaptureModeEnum CaptureMode);
	
	virtual void UpdateCamera(bool isExternallyLocked, double stamp);
	
	virtual void GetConfig(std::stringstream& OutputStream) override;
	virtual void SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream) override;
	virtual void GetData(std::stringstream& OutputStream) override;

	
protected:
	virtual void BeginPlay() override;
	virtual void Initialize(int InSensorID) override;
};
