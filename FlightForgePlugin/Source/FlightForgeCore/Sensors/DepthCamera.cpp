#include "DepthCamera.h"

#include "TextureResource.h"

#include "FlightForgeCore/RobotPawn.h"
#include "SensorWire.h"

UDepthCamera::UDepthCamera() : UCamera()
{
	SensorType = SensorType::DEPTH_CAMERA;
	Name       = TEXT("DEPTH_CAMERA");

	InstructionQueue = std::make_unique<TQueue<std::shared_ptr<FInstruction<UDepthCamera>>>>();
}

/* BeginPlay() //{ */

void UDepthCamera::BeginPlay()
{
	Super::BeginPlay();

	RenderTarget2D = NewObject<UTextureRenderTarget2D>();
	RenderTarget2D->InitCustomFormat(640, 480, PF_R16F, true);
	RenderTarget2D->RenderTargetFormat = RTF_R16f;
	RenderTarget2D->bGPUSharedFlag     = true;

	SceneCaptureComponent2D->CaptureSource = SCS_SceneDepth;
	SceneCaptureComponent2D->TextureTarget = RenderTarget2D;
	// no AA of any kind: depth samples must not be blended across edges
	SceneCaptureComponent2D->ShowFlags.SetTemporalAA(false);
	SceneCaptureComponent2D->ShowFlags.SetAntiAliasing(false);
	SceneCaptureComponent2D->bAlwaysPersistRenderingState = true;
	SceneCaptureComponent2D->bCaptureEveryFrame           = false;
	SceneCaptureComponent2D->bCaptureOnMovement           = false;
	SceneCaptureComponent2D->bUseRayTracingIfEnabled      = false;

	depth_camera_config_.ShowCameraComponent = false;
	depth_camera_config_.Offset              = FVector(0, 0, 0);
	depth_camera_config_.Orientation         = FRotator(0, 0, 0);
	depth_camera_config_.FOVAngle            = 90;
	depth_camera_config_.Width               = 640;
	depth_camera_config_.Height              = 480;

	SetDepthCameraConfig(depth_camera_config_);
	SetCameraCaptureMode(Owner->GetCameraCaptureMode());
}

/* //} */

/* TickComponent() //{ */

void UDepthCamera::DrainInstructions()
{
	std::shared_ptr<FInstruction<UDepthCamera>> Instruction;
	while (InstructionQueue->Dequeue(Instruction))
	{
		Instruction->Function(*this);
		Instruction->Finished = true;
	}
}

void UDepthCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	const double Start = FPlatformTime::Seconds();

	DrainInstructions();

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES && (Start - camera_last_request_time_) < 1.0)
	{
		if (ShouldCapture() && CaptureDue())
		{
			UpdateCamera(true, Start);
		}
	}

	if (bReadbackDue)
	{
		bReadbackDue = false;
		PushReadback(EnqueueReadback(RenderTarget2D, DeferredStamp, FVector::ZeroVector, true));
	}

	PumpReadback();

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/* //} */

/* SetDepthCameraConfig() //{ */

bool UDepthCamera::SetDepthCameraConfig(const FRgbCameraConfig& Config)
{
	if (!ValidDimensions(Config.Width, Config.Height))
	{
		return false;
	}

	depth_camera_config_ = Config;
	CaptureRate    = Config.CaptureRate;
	NextCaptureDue = -1.0;

	CameraBufferCriticalSection->Lock();

	SceneCaptureMeshHolder->SetVisibility(Config.ShowCameraComponent);
	SceneCaptureMeshHolder->SetRelativeLocation(Config.Offset);
	SceneCaptureMeshHolder->SetRelativeRotation(FRotator(0, 0, 0));
	SceneCaptureMeshHolder->SetRelativeRotation(Config.Orientation);

	// intrinsics yes, distortion not yet: the uint16 warp path does not exist,
	// so a distorted config would silently desync the depth from its RGB pair
	FCameraDistortion NoDistortion;
	SetupCaptureProjection(Config.Intrinsics, NoDistortion, Config.FOVAngle, Config.Width, Config.Height, {SceneCaptureComponent2D}, {RenderTarget2D});

	CameraBufferCriticalSection->Unlock();

	return true;
}

/* //} */

/* UpdateCamera() //{ */

void UDepthCamera::UpdateCamera(bool isExternallyLocked, double stamp)
{
	if (bReadbackDue || !TakeRingSlot())
	{
		return;
	}

	if (!SceneCaptureComponent2D->bCaptureEveryFrame && !bEverRendered)
	{
		SceneCaptureComponent2D->CaptureScene();
	}
	else if (!SceneCaptureComponent2D->bCaptureEveryFrame)
	{
		SceneCaptureComponent2D->CaptureSceneDeferred();
		DeferredStamp = stamp;
		bReadbackDue  = true;
		return;
	}

	PushReadback(EnqueueReadback(RenderTarget2D, stamp, FVector::ZeroVector, true));
}

/* //} */

/* GetDepthCameraDataFromServerThread() //{ */

bool UDepthCamera::GetDepthCameraDataFromServerThread(TArray<uint16>& OutDepthMm, FIntPoint& OutSize, double& stamp)
{
	camera_last_request_time_ = FPlatformTime::Seconds();

	CameraBufferCriticalSection->Lock();
	const bool bHaveFrame = CameraRendered;
	CameraBufferCriticalSection->Unlock();

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND && !SceneCaptureComponent2D->bCaptureEveryFrame && CachedTargetResource.load() && bEverRendered)
	{
		if (!RequestFrameFromServerThread(RenderTarget2D, FPlatformTime::Seconds(), FVector::ZeroVector, true, false, false, 2.0))
		{
			UE_LOG(LogTemp, Warning, TEXT("UDepthCamera: timed out waiting for a frame"));
		}
	}
	else if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND || !bHaveFrame)
	{
		// a new camera's first capture is a warm-up that publishes nothing
		bool bRendered = false;
		for (int32 Attempt = 0; Attempt < 3 && !bRendered; ++Attempt)
		{
			const uint64 Baseline = FrameCounter.load();

			auto Instruction      = std::make_shared<FInstruction<UDepthCamera>>();
			Instruction->Function = [](UDepthCamera& _Camera) { _Camera.UpdateCamera(true, FPlatformTime::Seconds()); };
			InstructionQueue->Enqueue(Instruction);

			if (!WaitForRequestedFrame(Instruction->Finished, Baseline, 2.0))
			{
				UE_LOG(LogTemp, Warning, TEXT("UDepthCamera: timed out waiting for a frame"));
			}

			CameraBufferCriticalSection->Lock();
			bRendered = CameraRendered;
			CameraBufferCriticalSection->Unlock();
		}
	}

	TArray<FFloat16> RawCopy;
	{
		CameraBufferCriticalSection->Lock();

		if (!CameraRendered || CameraBufferF16.Num() == 0)
		{
			CameraBufferCriticalSection->Unlock();
			UE_LOG(LogTemp, Warning, TEXT("UDepthCamera: camera is not rendered, returning"));
			return false;
		}

		RawCopy = CameraBufferF16;
		OutSize = CameraBufferSize;
		stamp   = stamp_;

		CameraBufferCriticalSection->Unlock();
	}

	// scene depth arrives in centimetres; publish uint16 millimetres, clamped
	OutDepthMm.SetNumUninitialized(RawCopy.Num());

	for (int32 Index = 0; Index < RawCopy.Num(); ++Index)
	{
		const float Millimetres = RawCopy[Index].GetFloat() * 10.0f;
		OutDepthMm[Index]       = static_cast<uint16>(FMath::Clamp(Millimetres, 0.0f, 65535.0f));
	}

	return true;
}

/* //} */

/* wire //{ */

void UDepthCamera::GetData(const flightforge::wire::Request& Request, FWireReply& Reply)
{
	TArray<uint16> DepthMm;
	FIntPoint      Size;
	double         Stamp = 0.0;

	if (!GetDepthCameraDataFromServerThread(DepthMm, Size, Stamp))
	{
		Reply.Fail(TEXT("the depth camera has no frame yet"));
		return;
	}

	const auto Data  = Reply.CopyVector(reinterpret_cast<const uint8*>(DepthMm.GetData()), DepthMm.Num() * sizeof(uint16));
	const auto Image = flightforge::wire::CreateImage(Reply.Builder, Size.X, Size.Y, flightforge::wire::PixelFormat::MONO16,
	                                                  flightforge::wire::Compression::NONE, Data);
	Reply.Set(flightforge::wire::CreateDepthCameraDataResponse(Reply.Builder, Stamp, Image));
}

void UDepthCamera::GetConfig(FWireReply& Reply)
{
	FSensorWire::ReplyConfig(depth_camera_config_, Reply);
}

void UDepthCamera::SetConfig(const flightforge::wire::Request& Request, FWireReply& Reply)
{
	FRgbCameraConfig Config;
	if (!FSensorWire::FromWire(Request, Config, Reply))
	{
		return;
	}

	bool bApplied = false;
	RunOnSensorQueue(*this, [&Config, &bApplied](UDepthCamera& Camera) { bApplied = Camera.SetDepthCameraConfig(Config); });

	if (!bApplied)
	{
		Reply.Fail(TEXT("the depth camera rejected the config"));
	}
}

void UDepthCamera::SetMount(const FVector& OffsetCm, const FRotator& Rotation)
{
	RunOnSensorQueue(*this, [&](UDepthCamera& Camera) {
		FRgbCameraConfig Config = Camera.depth_camera_config_;
		Config.Offset           = OffsetCm;
		Config.Orientation      = Rotation;
		Camera.SetDepthCameraConfig(Config);
	});
}

/* //} */
