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
#include "MessageSerialization/Public/FlightForgeWire.h"

#include "Camera.generated.h"


// the wire's CaptureMode ids
enum CameraCaptureModeEnum : unsigned short
{
	CAPTURE_ALL_FRAMES  = 0x0,
	CAPTURE_ON_MOVEMENT = 0x1,
	CAPTURE_ON_DEMAND   = 0x2,
};

/* DEFAULT_CAMERA_MOUNT //{ */

// clear of the airframe: at the origin a camera stares into the Mid-360's dome
inline const FVector DEFAULT_CAMERA_MOUNT = FVector(25.0, 0.0, 0.0);

/* //} */

/* FCameraIntrinsics //{ */

// pinhole intrinsics in pixels, image origin top-left; when UseCustom the capture's
// projection matrix is built from these (off-axis when cx/cy are off-center) instead
// of from the FOV angle. Engine convention: pixel u's centre sits at u + 0.5, so the
// image centre is Cx = W/2; the wire uses OpenCV's integer-centre convention, hence
// the WirePixelShift at every Get/Set.
struct FCameraIntrinsics
{
	bool   UseCustom = false;
	double Fx        = 0.0;
	double Fy        = 0.0;
	double Cx        = 0.0;
	double Cy        = 0.0;
};

constexpr double WirePixelShift = 0.5;

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

enum class ECameraCodec : uint8
{
	Raw,
	Jpeg,
	Png,
};

// JPEG by default: PNG costs tens of milliseconds and several times the bytes per frame, for detail no vision pipeline sees
struct FCameraCompression
{
	ECameraCodec Codec   = ECameraCodec::Jpeg;
	int32        Quality = 90;  // 1-100, JPEG only
};

// a served frame: Bytes hold the codec's output, or BGRA8 rows when raw
struct FCameraFrame
{
	TArray<uint8> Bytes;
	FIntPoint     Size  = FIntPoint::ZeroValue;
	ECameraCodec  Codec = ECameraCodec::Raw;
	double        Stamp = 0.0;
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

	TArray<FFloat16> CameraBufferF16;

	FIntPoint CameraBufferSize = FIntPoint::ZeroValue;

	std::atomic<double> camera_last_request_time_{0};

	double stamp_ = 0;

	virtual void BeginDestroy() override;

protected:
	CameraCaptureModeEnum CameraCaptureMode;
	bool CameraNeedsRefresh = false;

	bool CameraRendered                  = false;

	// in-order copies on the GPU
	TArray<TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>> InflightReadbacks;
	int32  MaxInflight  = 3;
	uint32 FramesFolded = 0;

	// on demand the capture renders inside the next engine frame like a continuous one; the copy is enqueued the tick after, once the target holds it
	bool    bReadbackDue  = false;
	bool    bEverRendered = false;  // a component's first deferred capture can precede its registration; the first frame is captured immediately
	double  DeferredStamp = 0.0;
	FVector DeferredOmega = FVector::ZeroVector;

	std::atomic<uint64> FrameCounter{0};
	FEvent* FrameReadyEvent = nullptr;

	void PushStreamingViewHint();

	// the pausing tick still advanced time, so its frame belongs to the step that ran it
	bool ShouldCapture()
	{
		const UWorld* World = GetWorld();
		const double  Now   = World ? World->GetTimeSeconds() : 0.0;
		if (World && World->IsPaused() && Now <= LastCaptureSimTime)
		{
			return false;
		}
		LastCaptureSimTime = Now;
		return true;
	}

	double LastCaptureSimTime = -1.0;

	// captures only on the first tick that reaches the next due time; 0 = every tick
	double CaptureRate    = 0.0;
	double NextCaptureDue = -1.0;
	bool   CaptureDue();

	bool bStreamOn = false;

	void ApplyLevelPostProcessBaseline(USceneCaptureComponent2D* Capture);

public:

	static FMatrix IntrinsicsProjectionMatrix(const FCameraIntrinsics& Intrinsics, int32 Width, int32 Height);

	static FCameraIntrinsics EffectiveIntrinsics(const FCameraIntrinsics& Intrinsics, double FovDeg, int32 Width, int32 Height);

	// a garbled config (a client reading a stale reply) must fail the request, not fatal the engine in a LUT allocation
	static bool ValidDimensions(int32 Width, int32 Height);

	// one CameraInfo per eye, for get_camera_info
	virtual void DescribeCameras(std::vector<flightforge::wire::CameraInfoT>& Out) {}

	static flightforge::wire::CameraInfoT PinholeInfo(int SensorId, ::SensorType Type, const FCameraIntrinsics& Intrinsics, const FCameraDistortion& Distortion,
	                                                  double FovDeg, int32 Width, int32 Height, const FVector& Offset, const FRotator& Orientation);

	static void ApplyProjection(USceneCaptureComponent2D* Capture, const FCameraIntrinsics& Intrinsics, double FovDeg, int32 Width, int32 Height);

	static TSharedPtr<const FDistortionLUT, ESPMode::ThreadSafe> BuildDistortionLUT(const FCameraDistortion& Distortion, const FCameraIntrinsics& EffectiveK,
	                                                                                int32 Width, int32 Height);

	static void ResampleImage(const FDistortionLUT& LUT, const TArray<FColor>& Source, TArray<FColor>& OutResampled, bool bNearestNeighbor);

	static void WarpFrame(const TArray<FColor>& Source, FIntPoint SourceSize, TArray<FColor>& OutFrame, FIntPoint OutputSize, const FDistortionLUT* LUT,
	                      const FCameraIntrinsics& EffectiveK, const FVector& OmegaCamera, double ReadoutTime);

	static void ApplyNoise(TArray<FColor>& Pixels, FIntPoint Size, const FCameraNoise& Noise, uint32 Seed);

	// bLossless turns JPEG into PNG; raw stays raw
	static void CompressFrame(const TArray<FColor>& Pixels, FIntPoint Size, const FCameraCompression& Compression, bool bLossless, FCameraFrame& OutFrame);

protected:
	void SetupCaptureProjection(const FCameraIntrinsics& Intrinsics, const FCameraDistortion& Distortion, double FovDeg, int32 Width, int32 Height,
	                            std::initializer_list<USceneCaptureComponent2D*> Captures, std::initializer_list<UTextureRenderTarget2D*> Targets);

	TSharedPtr<const FDistortionLUT, ESPMode::ThreadSafe> DistortionLUT;

	FVector capture_omega_ = FVector::ZeroVector;

	TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> EnqueueReadback(UTextureRenderTarget2D* RT, double Stamp, const FVector& Omega = FVector::ZeroVector,
	                                                                 bool bFloat16 = false, bool bEncodeSrgb = false, bool bHalfRgba = false, bool bLuma = false);

	bool PumpReadback();

	void PushReadback(const TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>& Slot);
	bool RingFull();
	void ClearRing();
	TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> NewestReadback();

	// lockstep waits for a slot, free-running folds the frame
	bool TakeRingSlot();

	virtual bool PumpFrames() { return PumpReadback(); }

	void NotifyFrameReady();

	bool WaitForNewFrame(uint64 BaselineFrame, double TimeoutSeconds);

	// server thread: waits for the capture instruction to run, then for its readback, and publishes it itself
	bool WaitForRequestedFrame(const std::atomic<bool>& InstructionFinished, uint64 BaselineFrame, double TimeoutSeconds);

	// server thread, on demand: registers the copy with the frame that will render the capture and
	// asks for that capture, with no game tick in the loop; polls the fence and publishes itself
	bool RequestFrameFromServerThread(UTextureRenderTarget2D* RT, double Stamp, const FVector& Omega, bool bFloat16, bool bEncodeSrgb, bool bHalfRgba,
	                                  double TimeoutSeconds);

	TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> MakeReadbackSlot(UTextureRenderTarget2D* RT, double Stamp, const FVector& Omega, bool bFloat16,
	                                                                  bool bEncodeSrgb, bool bHalfRgba, bool bLuma);

	// polls the slot's fence from the calling thread and resolves it there; Copied covers the render-thread race
	bool WaitForSlot(const TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>& Slot, double Deadline);

	// the target's render resource, refreshed on the game thread for the server thread's use
	std::atomic<const FRenderTarget*> CachedTargetResource{nullptr};

	void TransformImageArray(int32 ImageWidth, int32 ImageHeight, const TArray<FColor> &SrcData, TArray<uint8> &DstData);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void SetCameraCaptureMode(CameraCaptureModeEnum CaptureMode);
	
	virtual void UpdateCamera(bool isExternallyLocked, double stamp);

	// game thread: blocks until the frame in flight has landed
	virtual void DrainReadback();

protected:
	virtual void BeginPlay() override;
	virtual void Initialize(int InSensorID) override;
};
