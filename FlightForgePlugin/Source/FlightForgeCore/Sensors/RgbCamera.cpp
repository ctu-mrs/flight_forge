#include "RgbCamera.h"

#include "Engine/World.h"
#include "TextureResource.h"

#include "FlightForgeCore/RobotPawn.h"
#include "SensorWire.h"
#include "MultiViewCapture.h"

URgbCamera::URgbCamera() : UCamera()
{
	SensorType = SensorType::RGB_CAMERA;
	Name = TEXT("RGB_CAMERA");

	InstructionQueue = std::make_unique<TInstructionQueue<URgbCamera>>();
}


void URgbCamera::BeginPlay()
{
	Super::BeginPlay();

	RenderTarget2D = NewObject<UTextureRenderTarget2D>();
	RenderTarget2D->InitCustomFormat(640, 480, PF_B8G8R8A8, false);
	RenderTarget2D->RenderTargetFormat = RTF_RGBA8;
	RenderTarget2D->bGPUSharedFlag     = true;

	SceneCaptureComponent2D->TextureTarget = RenderTarget2D;
	SceneCaptureComponent2D->bAlwaysPersistRenderingState = true;
	SceneCaptureComponent2D->bCaptureEveryFrame           = false;
	SceneCaptureComponent2D->bCaptureOnMovement           = false;

	SetRgbCameraConfig(rgb_camera_config_);
	SetCameraCaptureMode(Owner->GetCameraCaptureMode());
}


void URgbCamera::DrainInstructions()
{
	std::shared_ptr<FInstruction<URgbCamera>> Instruction;
	while (InstructionQueue->Dequeue(Instruction))
	{
		Instruction->Run(*this);
	}
}

void URgbCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	DrainInstructions();

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES)
	{
		const bool bWanted = StreamWanted();

		if (bWanted && !bStreamOn)
		{
			bStreamOn = true;
			bEveryFrameWanted = CaptureRate <= 0.0;
			UE_LOG(LogTemp, Warning, TEXT("Turning on RGB capture"));
		}
		else if (!bWanted && bStreamOn)
		{
			bStreamOn = false;
			bEveryFrameWanted = false;
			CameraBufferCriticalSection->Lock();
			CameraRendered = false;
			CameraBufferCriticalSection->Unlock();
			UE_LOG(LogTemp, Warning, TEXT("Turning off RGB capture, %u frames folded"), FramesFolded);
		}
		else if (bWanted && ShouldCapture() && CaptureDue())
		{
			UpdateCamera(true, SimNow());
		}
	}

	// a batched camera is captured by its robot's batch, never by the engine on its own
	SceneCaptureComponent2D->bCaptureEveryFrame = bEveryFrameWanted && !UMultiViewCapture::BatchEnabled();

	PumpReadback();

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/* applyExposureAndLens() //{ */

void URgbCamera::ApplyExposureAndLens(const FCameraExposure& Exposure, const FCameraLensEffects& Lens, FPostProcessSettings& PP)
{
  if (Exposure.Manual) {
    PP.bOverride_AutoExposureMethod = true;
    PP.AutoExposureMethod           = AEM_Manual;

    PP.bOverride_CameraShutterSpeed = true;
    PP.CameraShutterSpeed           = 1.0f / FMath::Max((float)Exposure.ShutterTime, 1e-5f);

    PP.bOverride_CameraISO = true;
    PP.CameraISO           = FMath::Max((float)Exposure.ISO, 1.0f);

    PP.bOverride_AutoExposureBias = true;
    PP.AutoExposureBias           = Exposure.EvCompensation;

    PP.bOverride_DepthOfFieldFstop = true;
    PP.DepthOfFieldFstop           = FMath::Max((float)Lens.Fstop, 0.1f);

    if (Lens.MotionBlurFromShutter) {
      // blur length physically tied to exposure: shutter time over frame time
      const float FrameTime = GetWorld() ? FMath::Max(GetWorld()->GetDeltaSeconds(), 1e-4f) : 1.0f / 60.0f;
      PP.bOverride_MotionBlurAmount = true;
      PP.MotionBlurAmount           = FMath::Clamp((float)Exposure.ShutterTime / FrameTime, 0.0f, 1.0f);
    }
  }

  if (Lens.FocalDistance > 0.0) {
    PP.bOverride_DepthOfFieldFocalDistance = true;
    PP.DepthOfFieldFocalDistance           = Lens.FocalDistance * 100.0f;  // m -> cm

    PP.bOverride_DepthOfFieldSensorWidth = true;
    PP.DepthOfFieldSensorWidth           = Lens.SensorWidthMm;
  }

  PP.bOverride_VignetteIntensity = Lens.VignetteIntensity >= 0.0;
  PP.VignetteIntensity           = Lens.VignetteIntensity;

  PP.bOverride_SceneFringeIntensity = Lens.ChromaticAberration >= 0.0;
  PP.SceneFringeIntensity           = Lens.ChromaticAberration;

  PP.bOverride_BloomIntensity = Lens.BloomIntensity >= 0.0;
  PP.BloomIntensity           = Lens.BloomIntensity;

  PP.bOverride_LensFlareIntensity = Lens.LensFlareIntensity >= 0.0;
  PP.LensFlareIntensity           = Lens.LensFlareIntensity;

  if (Lens.WhiteTemp > 0.0) {
    PP.bOverride_WhiteTemp = true;
    PP.WhiteTemp           = Lens.WhiteTemp;

    PP.bOverride_WhiteTint = true;
    PP.WhiteTint           = Lens.WhiteTint;
  }
}

//}

bool URgbCamera::SetRgbCameraConfig(const FRgbCameraConfig& Config)
{
  UE_LOG(LogTemp, Log, TEXT("UCamera::SetRgbCameraConfig"));

  if (!ValidDimensions(Config.Width, Config.Height)) {
    return false;
  }

  rgb_camera_config_ = Config;
  CaptureRate    = Config.CaptureRate;
  NextCaptureDue = -1.0;
  if (bStreamOn) {
    bEveryFrameWanted = CaptureRate <= 0.0;
  }

  CameraBufferCriticalSection->Lock();

  SceneCaptureMeshHolder->SetVisibility(Config.ShowCameraComponent);
  SceneCaptureMeshHolder->SetRelativeLocation(Config.Offset);
  SceneCaptureMeshHolder->SetRelativeRotation(FRotator(0, 0, 0));
  SceneCaptureMeshHolder->SetRelativeRotation(Config.Orientation);

  ApplyViewportLook(SceneCaptureComponent2D);
  ApplyColorOutput(SceneCaptureComponent2D, RenderTarget2D, Config.enable_hdr);

  SetupCaptureProjection(Config.Intrinsics, Config.Distortion, Config.FOVAngle, Config.Width, Config.Height, {SceneCaptureComponent2D}, {RenderTarget2D});

  SceneCaptureComponent2D->bUseRayTracingIfEnabled = Config.enable_raytracing;

  SceneCaptureComponent2D->TextureTarget                        = RenderTarget2D;
  SceneCaptureComponent2D->bAlwaysPersistRenderingState         = true;
  bEveryFrameWanted = false;
  SceneCaptureComponent2D->bCaptureOnMovement                   = false;

  SceneCaptureComponent2D->ShowFlags.SetMotionBlur(Config.enable_motion_blur);
  SceneCaptureComponent2D->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);

  ApplyExposureAndLens(Config.Exposure, Config.Lens, SceneCaptureComponent2D->PostProcessSettings);


  CameraBufferCriticalSection->Unlock();

  return true;
}

// game thread; starts the capture and hands the pixel transfer to the GPU
// readback pipeline instead of stalling on ReadPixels()
void URgbCamera::UpdateCamera(bool isExternallyLocked, double stamp)
{
	if (!TakeRingSlot()) {
		return;
	}

	const FVector OmegaCamera = CaptureOmega(stamp);

	if (bEveryFrameWanted || bEverRendered)
	{
		if (UMultiViewCapture* Batch = UMultiViewCapture::BatchEnabled() ? UMultiViewCapture::RobotBatch(Owner) : nullptr)
		{
			Batch->Defer(SceneCaptureComponent2D);
		}
		else
		{
			SceneCaptureComponent2D->CaptureSceneDeferred();
		}
		PushReadback(SubmitCaptureJob(RenderTarget2D, OmegaCamera, false, rgb_camera_config_.enable_hdr, rgb_camera_config_.enable_hdr));
		return;
	}

	SceneCaptureComponent2D->CaptureScene();
	PushReadback(EnqueueReadback(RenderTarget2D, stamp, OmegaCamera, false, rgb_camera_config_.enable_hdr, rgb_camera_config_.enable_hdr));
}

// camera-frame angular velocity between captures, remapped from UE axes
// (X fwd, Y right, Z up) to the image convention (x right, y down, z forward)
FVector URgbCamera::CaptureOmega(const double stamp)
{
	FVector     OmegaCamera = FVector::ZeroVector;
	const FQuat CurrentQuat = SceneCaptureComponent2D->GetComponentQuat();
	const double DeltaTime  = stamp - LastCaptureTime;

	if (LastCaptureTime > 0.0 && DeltaTime > 1.0e-4 && DeltaTime < 1.0)
	{
		FVector Axis;
		double  Angle;
		(LastCaptureQuat.Inverse() * CurrentQuat).ToAxisAndAngle(Axis, Angle);

		const FVector OmegaUE = Axis * (Angle / DeltaTime);
		OmegaCamera           = FVector(OmegaUE.Y, -OmegaUE.Z, OmegaUE.X);
	}

	LastCaptureQuat = CurrentQuat;
	LastCaptureTime = stamp;

	return OmegaCamera;
}

bool URgbCamera::GetRgbCameraDataFromServerThread(FCameraFrame& OutFrame)
{
	camera_last_request_time_ = FPlatformTime::Seconds();

	CameraBufferCriticalSection->Lock();
	const bool bHaveFrame = CameraRendered;
	CameraBufferCriticalSection->Unlock();

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND && !bEveryFrameWanted && CachedTargetResource.load() && bEverRendered) {
		if (!RequestFrameFromServerThread(RenderTarget2D, CaptureOmega(SimNow()), false, rgb_camera_config_.enable_hdr, rgb_camera_config_.enable_hdr, 2.0)) {
			UE_LOG(LogTemp, Warning, TEXT("URgbCamera: timed out waiting for a frame"));
		}
	} else if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND || !bHaveFrame) {
		const uint64 Baseline = FrameCounter.load();

		auto Instruction      = std::make_shared<FInstruction<URgbCamera>>();
		Instruction->Function = [](URgbCamera& _URgbCamera) { _URgbCamera.UpdateCamera(true, _URgbCamera.SimNow()); };
		InstructionQueue->Enqueue(Instruction);

		if (!WaitForRequestedFrame(Instruction->Finished, Baseline, 2.0)) {
			UE_LOG(LogTemp, Warning, TEXT("URgbCamera: timed out waiting for a frame"));
		}
	}

	TArray<FColor>                                        RawCopy;
	FIntPoint                                             RawSize;
	TSharedPtr<const FDistortionLUT, ESPMode::ThreadSafe> LUT;
	FVector                                               Omega;
	FCameraNoise                                          NoiseConfig;
	FCameraRollingShutter                                 RollingShutterConfig;
	FCameraCompression                                    CompressionConfig;
	{
		CameraBufferCriticalSection->Lock();

		if (!CameraRendered) {
			CameraBufferCriticalSection->Unlock();
			UE_LOG(LogTemp, Warning, TEXT("URgbCamera: camera is not rendered, returning"));
			return false;
		}

		RawCopy              = CameraBuffer;
		RawSize              = CameraBufferSize;
		OutFrame.Stamp       = stamp_;
		LUT                  = DistortionLUT;
		Omega                = capture_omega_;
		NoiseConfig          = rgb_camera_config_.Noise;
		RollingShutterConfig = rgb_camera_config_.RollingShutter;
		CompressionConfig    = rgb_camera_config_.Compression;

		CameraBufferCriticalSection->Unlock();
	}

	// warp + noise + compress outside the lock so the game thread's publish path never blocks
	const bool bLutValid       = LUT && RawSize == LUT->SourceSize;
	const bool bRollingShutter = RollingShutterConfig.Enable && RollingShutterConfig.ReadoutTime > 0.0 && !Omega.IsNearlyZero();

	if (bLutValid || bRollingShutter) {
		const FCameraIntrinsics EffectiveK =
		    EffectiveIntrinsics(rgb_camera_config_.Intrinsics, rgb_camera_config_.FOVAngle, rgb_camera_config_.Width, rgb_camera_config_.Height);
		const FIntPoint OutSize = bLutValid ? LUT->OutputSize : RawSize;

		TArray<FColor> Warped;
		WarpFrame(RawCopy, RawSize, Warped, OutSize, bLutValid ? LUT.Get() : nullptr, EffectiveK, bRollingShutter ? Omega : FVector::ZeroVector,
		          bRollingShutter ? RollingShutterConfig.ReadoutTime : 0.0);
		RawCopy = MoveTemp(Warped);
		RawSize = OutSize;
	}

	if (NoiseConfig.Enable) {
		ApplyNoise(RawCopy, RawSize, NoiseConfig, static_cast<uint32>(OutFrame.Stamp * 1.0e6));
	}

	CompressFrame(RawCopy, RawSize, CompressionConfig, false, OutFrame);

	return true;
}

/* wire //{ */

void URgbCamera::GetConfig(FWireReply& Reply)
{
	FSensorWire::ReplyConfig(rgb_camera_config_, Reply);
}

void URgbCamera::SetConfig(const flightforge::wire::Request& Request, FWireReply& Reply)
{
	FRgbCameraConfig Config;
	if (!FSensorWire::FromWire(Request, Config, Reply))
	{
		return;
	}

	bool bApplied = false;
	RunOnSensorQueue(*this, [&Config, &bApplied](URgbCamera& Camera) { bApplied = Camera.SetRgbCameraConfig(Config); });

	if (!bApplied)
	{
		Reply.Fail(TEXT("the rgb camera rejected the config"));
	}
}

void URgbCamera::SetMount(const FVector& OffsetCm, const FRotator& Rotation)
{
	RunOnSensorQueue(*this, [&](URgbCamera& Camera) {
		FRgbCameraConfig Config = Camera.rgb_camera_config_;
		Config.Offset           = OffsetCm;
		Config.Orientation      = Rotation;
		Camera.SetRgbCameraConfig(Config);
	});
}

void URgbCamera::GetData(const flightforge::wire::Request& Request, FWireReply& Reply)
{
	FCameraFrame Frame;
	if (!GetRgbCameraDataFromServerThread(Frame))
	{
		Reply.Fail(TEXT("the rgb camera has no frame yet"));
		return;
	}

	const auto Image = FSensorWire::Image(Reply, Frame, FSensorWire::ColourFormat(Frame.Codec));
	Reply.Set(flightforge::wire::CreateRgbCameraDataResponse(Reply.Builder, Frame.Stamp, Image));
}

/* //} */
