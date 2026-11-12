#include "RgbSegCamera.h"

#include "HAL/IConsoleManager.h"
#include "Materials/Material.h"
#include "Materials/MaterialRenderProxy.h"
#include "TextureResource.h"

#include "SensorWire.h"
#include "FlightForgeCore/RobotPawn.h"

URgbSegCamera::URgbSegCamera() : UCamera()
{
	SensorType = SensorType::RGB_SEG_CAMERA;
	Name = TEXT("RGB_SEG_CAMERA");

	InstructionQueue = std::make_unique<TInstructionQueue<URgbSegCamera>>();
}


void URgbSegCamera::BeginPlay()
{
	Super::BeginPlay();
	

	RenderTarget2D = NewObject<UTextureRenderTarget2D>();
	RenderTarget2D->InitCustomFormat(640, 480, PF_B8G8R8A8, false);
	RenderTarget2D->RenderTargetFormat = RTF_RGBA8;
	RenderTarget2D->bGPUSharedFlag     = true;
	
	SceneCaptureComponent2D->CaptureSource = SCS_FinalColorHDR;
	SceneCaptureComponent2D->TextureTarget = RenderTarget2D;
	SceneCaptureComponent2D->ShowFlags.SetTemporalAA(true);
	SceneCaptureComponent2D->bAlwaysPersistRenderingState = true;
	SceneCaptureComponent2D->bCaptureEveryFrame           = false;
	SceneCaptureComponent2D->bCaptureOnMovement           = false;
	SceneCaptureComponent2D->bUseRayTracingIfEnabled      = false;
	// PostProcessMaterial is an EditAnywhere property, which only a Blueprint-placed
	// component would fill in; sensors are built at runtime, so load the plugin's own
	// material here. Without it the capture is an ordinary render, not a mask.
	if (!PostProcessMaterial)
	{
		PostProcessMaterial = LoadObject<UMaterial>(nullptr, TEXT("/FlightForgePlugin/PP_segmentationMat.PP_segmentationMat"));
	}

	if (PostProcessMaterial)
	{
		SceneCaptureComponent2D->AddOrUpdateBlendable(PostProcessMaterial, 1);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("URgbSegCamera: could not load PP_segmentationMat; this camera will return an unlabelled scene render"));
	}

	// Stencil segmentation needs r.CustomDepth=3 ("with stencil"); at 1 the stencil buffer
	// is never written, SetCustomDepthStencilValue does nothing, and this camera quietly
	// returns an ordinary render instead of a mask.
	if (static IConsoleVariable* CustomDepth = IConsoleManager::Get().FindConsoleVariable(TEXT("r.CustomDepth")); CustomDepth && CustomDepth->GetInt() != 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("URgbSegCamera: r.CustomDepth is %d, needs 3 (Custom Depth-Stencil Pass = 'Enabled with Stencil'); "
		                              "segmentation output will be an unlabelled scene render"),
		       CustomDepth->GetInt());
	}

	rgb_camera_config_.ShowCameraComponent    = false;
	rgb_camera_config_.Offset                 = FVector(0, 0, 0);
	rgb_camera_config_.Orientation            = FRotator(0, 0, 0);
	rgb_camera_config_.FOVAngle               = 90;
	rgb_camera_config_.Width                  = 640;
	rgb_camera_config_.Height                 = 480;
	rgb_camera_config_.enable_motion_blur     = true;
	rgb_camera_config_.motion_blur_amount     = 1.0;
	rgb_camera_config_.motion_blur_distortion = 50.0;
	
	SetRgbSegCameraConfig(rgb_camera_config_);
	SetCameraCaptureMode(Owner->GetCameraCaptureMode());
	
}


void URgbSegCamera::DrainInstructions()
{
	std::shared_ptr<FInstruction<URgbSegCamera>> Instruction;
	while (InstructionQueue->Dequeue(Instruction))
	{
		Instruction->Run(*this);
	}
}

void URgbSegCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	DrainInstructions();
	
	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES)
	{
		const bool bWanted = StreamWanted();

		if (bWanted && !bStreamOn)
		{
			bStreamOn = true;
			SceneCaptureComponent2D->bCaptureEveryFrame = CaptureRate <= 0.0;
			UE_LOG(LogTemp, Warning, TEXT("Turning on RGB segmented capture"));
		}
		else if (!bWanted && bStreamOn)
		{
			bStreamOn = false;
			SceneCaptureComponent2D->bCaptureEveryFrame = false;
			CameraBufferCriticalSection->Lock();
			CameraRendered = false;
			CameraBufferCriticalSection->Unlock();
			UE_LOG(LogTemp, Warning, TEXT("Turning off RGB segmented capture, %u frames folded"), FramesFolded);
		}
		else if (bWanted && ShouldCapture() && CaptureDue())
		{
			UpdateCamera(true, SimNow());
		}
	}

	PumpReadback();

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool URgbSegCamera::SetRgbSegCameraConfig(const FRgbCameraConfig& Config)
{
	UE_LOG(LogTemp, Log, TEXT("URgbSegCamera::SetRgbSegCameraConfig"));

	if (!ValidDimensions(Config.Width, Config.Height))
	{
		return false;
	}

	rgb_camera_config_ = Config;
	CaptureRate    = Config.CaptureRate;
	NextCaptureDue = -1.0;
	if (bStreamOn) {
		SceneCaptureComponent2D->bCaptureEveryFrame = CaptureRate <= 0.0;
	}

	CameraBufferCriticalSection->Lock();

	SceneCaptureMeshHolder->SetVisibility(Config.ShowCameraComponent);
	SceneCaptureMeshHolder->SetRelativeLocation(Config.Offset);
	SceneCaptureMeshHolder->SetRelativeRotation(Config.Orientation);

	SetupCaptureProjection(Config.Intrinsics, Config.Distortion, Config.FOVAngle, Config.Width, Config.Height, {SceneCaptureComponent2D}, {RenderTarget2D});

	SceneCaptureComponent2D->TextureTarget                = RenderTarget2D;
	SceneCaptureComponent2D->bAlwaysPersistRenderingState = true;
	SceneCaptureComponent2D->bCaptureEveryFrame           = false;
	SceneCaptureComponent2D->bCaptureOnMovement           = false;
	SceneCaptureComponent2D->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);

	// Re-assert the segmentation material: anything that assigns PostProcessSettings
	// wholesale (a level post-process baseline, for one) replaces WeightedBlendables and
	// silently turns this back into an ordinary colour camera.
	if (PostProcessMaterial)
	{
		SceneCaptureComponent2D->AddOrUpdateBlendable(PostProcessMaterial, 1);
	}

	UE_LOG(LogTemp, Log, TEXT("URgbSegCamera: %d blendable(s) on the capture"), SceneCaptureComponent2D->PostProcessSettings.WeightedBlendables.Array.Num());

	CameraBufferCriticalSection->Unlock();
	
	return true;
}

// game thread; starts the capture and hands the pixel transfer to the GPU
// readback pipeline instead of stalling on ReadPixels()
void URgbSegCamera::UpdateCamera(bool isExternallyLocked, double stamp)
{
	if (!TakeRingSlot()) {
		return;
	}

	// While the blendable's shaders are still compiling a capture renders without it,
	// handing the client a photograph labelled as a mask. Only the render thread knows
	// when the compiled shaders are actually in use, so ask it and wait for the answer.
	if (PostProcessMaterial && !BlendableLive->load()) {
		const FMaterialRenderProxy*  Proxy        = PostProcessMaterial->GetRenderProxy();
		const ERHIFeatureLevel::Type FeatureLevel = GetWorld()->GetFeatureLevel();

		ENQUEUE_RENDER_COMMAND(FlightForgeSegBlendableProbe)(
			[Proxy, FeatureLevel, Flag = BlendableLive](FRHICommandListImmediate&)
			{
				if (Proxy && Proxy->GetMaterialNoFallback(FeatureLevel))
				{
					Flag->store(true);
				}
			});

		return;
	}

	SceneCaptureComponent2D->CaptureSource = SCS_FinalColorLDR;
	SceneCaptureComponent2D->TextureTarget = RenderTarget2D;

	// the capture's first renders see an empty custom-stencil input, and the mask material
	// passes scene colour through for unlabelled pixels - photographs; render and discard
	if (WarmupCapturesRemaining > 0)
	{
		--WarmupCapturesRemaining;
		SceneCaptureComponent2D->CaptureScene();
		return;
	}

	if (SceneCaptureComponent2D->bCaptureEveryFrame || bEverRendered)
	{
		SceneCaptureComponent2D->CaptureSceneDeferred();
		PushReadback(SubmitCaptureJob(RenderTarget2D));
		return;
	}

	SceneCaptureComponent2D->CaptureScene();
	PushReadback(EnqueueReadback(RenderTarget2D, stamp));
}

bool URgbSegCamera::GetRgbSegCameraFromServerThread(FCameraFrame& OutFrame) {

	camera_last_request_time_ = FPlatformTime::Seconds();

	CameraBufferCriticalSection->Lock();
	const bool bHaveFrame = CameraRendered;
	CameraBufferCriticalSection->Unlock();

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND && !SceneCaptureComponent2D->bCaptureEveryFrame && WarmupCapturesRemaining <= 0 && CachedTargetResource.load() && bEverRendered) {
		if (!RequestFrameFromServerThread(RenderTarget2D, FVector::ZeroVector, false, false, false, 2.0)) {
			UE_LOG(LogTemp, Warning, TEXT("URgbSegCamera: timed out waiting for a frame"));
		}
	} else if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND || !bHaveFrame) {
		// the blendable probe and the warm-up captures each take a request of their own
		bool bGot = false;
		for (int32 Attempt = 0; Attempt < 8 && !bGot; ++Attempt) {
			const uint64 Baseline = FrameCounter.load();

			auto Instruction      = std::make_shared<FInstruction<URgbSegCamera>>();
			Instruction->Function = [](URgbSegCamera& _URgbSegCamera) { _URgbSegCamera.UpdateCamera(true, _URgbSegCamera.SimNow()); };
			InstructionQueue->Enqueue(Instruction);

			bGot = WaitForRequestedFrame(Instruction->Finished, Baseline, 2.0);
		}
		if (!bGot) {
			UE_LOG(LogTemp, Warning, TEXT("URgbSegCamera: timed out waiting for a frame"));
		}
	}

	TArray<FColor>                                        RawCopy;
	FIntPoint                                             RawSize;
	TSharedPtr<const FDistortionLUT, ESPMode::ThreadSafe> LUT;
	{
		CameraBufferCriticalSection->Lock();

		if (!CameraRendered) {
			CameraBufferCriticalSection->Unlock();
			UE_LOG(LogTemp, Warning, TEXT("URgbSegCamera: camera is not rendered, returning"));
			return false;
		}

		RawCopy        = CameraBuffer;
		RawSize        = CameraBufferSize;
		OutFrame.Stamp = stamp_;
		LUT            = DistortionLUT;

		CameraBufferCriticalSection->Unlock();
	}

	// nearest-neighbor so class labels never blend across the warp
	if (LUT && RawSize == LUT->SourceSize) {
		TArray<FColor> Warped;
		ResampleImage(*LUT, RawCopy, Warped, true);
		RawCopy = MoveTemp(Warped);
		RawSize = LUT->OutputSize;
	}

	TransformImageArray(RawSize.X, RawSize.Y, RawCopy, OutFrame.Bytes);
	OutFrame.Size  = RawSize;
	OutFrame.Codec = ECameraCodec::Raw;

	return true;
}

/* wire //{ */

void URgbSegCamera::GetConfig(FWireReply& Reply)
{
	FSensorWire::ReplyConfig(rgb_camera_config_, Reply);
}

void URgbSegCamera::SetConfig(const flightforge::wire::Request& Request, FWireReply& Reply)
{
	FRgbCameraConfig Config;
	if (!FSensorWire::FromWire(Request, Config, Reply))
	{
		return;
	}

	bool bApplied = false;
	RunOnSensorQueue(*this, [&Config, &bApplied](URgbSegCamera& Camera) { bApplied = Camera.SetRgbSegCameraConfig(Config); });

	if (!bApplied)
	{
		Reply.Fail(TEXT("the segmentation camera rejected the config"));
	}
}

void URgbSegCamera::SetMount(const FVector& OffsetCm, const FRotator& Rotation)
{
	RunOnSensorQueue(*this, [&](URgbSegCamera& Camera) {
		FRgbCameraConfig Config = Camera.rgb_camera_config_;
		Config.Offset           = OffsetCm;
		Config.Orientation      = Rotation;
		Camera.SetRgbSegCameraConfig(Config);
	});
}

// labels never go through a lossy codec
void URgbSegCamera::GetData(const flightforge::wire::Request& Request, FWireReply& Reply)
{
	FCameraFrame Frame;
	if (!GetRgbSegCameraFromServerThread(Frame))
	{
		Reply.Fail(TEXT("the segmentation camera has no frame yet"));
		return;
	}

	const auto Image = FSensorWire::Image(Reply, Frame, flightforge::wire::PixelFormat::BGR8);
	Reply.Set(flightforge::wire::CreateRgbSegCameraDataResponse(Reply.Builder, Frame.Stamp, Image));
}

/* //} */
