#pragma once

#include <atomic>

#include "CoreMinimal.h"
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

/* FCameraReadback //{ */

// One in-flight GPU->CPU copy of a render target. The game thread enqueues the
// copy right after the capture and keeps ticking; render-thread commands fill
// Pixels and flip bCopied when the transfer lands, typically a frame later.
// Shared ownership: the render commands hold a reference, so the slot outlives
// the component even if it is torn down mid-flight.
struct FCameraReadback
{
	TUniquePtr<FRHIGPUTextureReadback> Readback;
	TArray<FColor>    Pixels;
	FIntPoint         Size = FIntPoint::ZeroValue;
	double            Stamp = 0.0;
	std::atomic<bool> bResolveIssued{false};
	std::atomic<bool> bCopied{false};
};

//}

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

protected:

	TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> EnqueueReadback(UTextureRenderTarget2D* RT, double Stamp);
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
