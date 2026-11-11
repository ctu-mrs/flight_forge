#include "RgbCamera.h"

#include "Engine/World.h"
#include "TextureResource.h"

#include "Serialize.h"
#include "FlightForgeCore/RobotPawn.h"

static TAutoConsoleVariable<int32> CVarHdrTargetBits(TEXT("FlightForge.Camera.HdrTargetBits"), 16,
                                                     TEXT("Float bits per channel of the HDR capture target: 16 or 32."));

// 1: filmic tone curve, highlights roll off like film. 0: linear until the pixel well
// saturates, then hard clip - how a machine-vision sensor actually responds.
static TAutoConsoleVariable<int32> CVarFilmicToneCurve(TEXT("FlightForge.Camera.FilmicToneCurve"), 1,
                                                       TEXT("HDR capture transform: 1 filmic tone curve, 0 linear sensor with hard clipping."));

URgbCamera::URgbCamera() : UCamera()
{
	SensorType = SensorType::RGB_CAMERA;
	Name = TEXT("RGB_CAMERA");

	InstructionQueue = std::make_unique<TQueue<std::shared_ptr<FInstruction<URgbCamera>>>>();
}


void URgbCamera::BeginPlay()
{
	Super::BeginPlay();

	RenderTarget2D = NewObject<UTextureRenderTarget2D>();
	RenderTarget2D->InitCustomFormat(640, 480, PF_B8G8R8A8, false);
	RenderTarget2D->RenderTargetFormat = RTF_RGBA8;
	RenderTarget2D->bGPUSharedFlag     = true;

	SceneCaptureComponent2D->TextureTarget = RenderTarget2D;
	SceneCaptureComponent2D->ShowFlags.SetTemporalAA(false);
	SceneCaptureComponent2D->bAlwaysPersistRenderingState = true;
	SceneCaptureComponent2D->bCaptureEveryFrame           = false;
	SceneCaptureComponent2D->bCaptureOnMovement           = false;

	ApplyLevelPostProcessBaseline(SceneCaptureComponent2D);

	SetRgbCameraConfig(rgb_camera_config_);
	SetCameraCaptureMode(Owner->GetCameraCaptureMode());
}


void URgbCamera::DrainInstructions()
{
	std::shared_ptr<FInstruction<URgbCamera>> Instruction;
	while (InstructionQueue->Dequeue(Instruction))
	{
		Instruction->Function(*this);
		Instruction->Finished = true;
	}
}

void URgbCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	auto start = FPlatformTime::Seconds();

	DrainInstructions();

	if (bReadbackDue)
	{
		bReadbackDue = false;
		PushReadback(EnqueueReadback(RenderTarget2D, DeferredStamp, DeferredOmega, false, rgb_camera_config_.enable_hdr, rgb_camera_config_.enable_hdr));
	}

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES)
	{
		const bool bWanted = (FPlatformTime::Seconds() - camera_last_request_time_) < 1.0;

		if (bWanted && !bStreamOn)
		{
			bStreamOn = true;
			SceneCaptureComponent2D->bCaptureEveryFrame = CaptureRate <= 0.0;
			UE_LOG(LogTemp, Warning, TEXT("Turning on RGB capture"));
		}
		else if (!bWanted && bStreamOn)
		{
			bStreamOn = false;
			SceneCaptureComponent2D->bCaptureEveryFrame = false;
			CameraBufferCriticalSection->Lock();
			CameraRendered = false;
			CameraBufferCriticalSection->Unlock();
			UE_LOG(LogTemp, Warning, TEXT("Turning off RGB capture, %u frames folded"), FramesFolded);
		}
		else if (bWanted && ShouldCapture() && CaptureDue())
		{
			UpdateCamera(true, start);
		}
	}

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

  PP.bOverride_VignetteIntensity = true;
  PP.VignetteIntensity           = Lens.VignetteIntensity;

  PP.bOverride_SceneFringeIntensity = true;
  PP.SceneFringeIntensity           = Lens.ChromaticAberration;

  PP.bOverride_BloomIntensity = true;
  PP.BloomIntensity           = Lens.BloomIntensity;

  PP.bOverride_LensFlareIntensity = true;
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
    SceneCaptureComponent2D->bCaptureEveryFrame = CaptureRate <= 0.0;
  }

  CameraBufferCriticalSection->Lock();

  SceneCaptureMeshHolder->SetVisibility(Config.ShowCameraComponent);
  SceneCaptureMeshHolder->SetRelativeLocation(Config.Offset);
  SceneCaptureMeshHolder->SetRelativeRotation(FRotator(0, 0, 0));
  SceneCaptureMeshHolder->SetRelativeRotation(Config.Orientation);

  // float target so the sRGB encode reads the render's own values rather than an 8-bit
  // rounding of them, which is what costs shadow gradation
  const bool         bWant32       = CVarHdrTargetBits.GetValueOnGameThread() >= 32;
  const EPixelFormat DesiredFormat = !Config.enable_hdr ? PF_B8G8R8A8 : (bWant32 ? PF_A32B32G32R32F : PF_FloatRGBA);

  if (RenderTarget2D->GetFormat() != DesiredFormat) {
    RenderTarget2D->RenderTargetFormat = !Config.enable_hdr ? RTF_RGBA8 : (bWant32 ? RTF_RGBA32f : RTF_RGBA16f);
    RenderTarget2D->InitCustomFormat(RenderTarget2D->SizeX, RenderTarget2D->SizeY, DesiredFormat, Config.enable_hdr);
  }

  SetupCaptureProjection(Config.Intrinsics, Config.Distortion, Config.FOVAngle, Config.Width, Config.Height, {SceneCaptureComponent2D}, {RenderTarget2D});

  const bool bFilmic = CVarFilmicToneCurve.GetValueOnGameThread() != 0;

  SceneCaptureComponent2D->CaptureSource = !Config.enable_hdr ? SCS_FinalColorLDR : (bFilmic ? SCS_FinalToneCurveHDR : SCS_FinalColorHDR);

  SceneCaptureComponent2D->bUseRayTracingIfEnabled = Config.enable_raytracing;

  SceneCaptureComponent2D->TextureTarget                        = RenderTarget2D;
  SceneCaptureComponent2D->bAlwaysPersistRenderingState         = true;
  SceneCaptureComponent2D->bCaptureEveryFrame                   = false;
  SceneCaptureComponent2D->bCaptureOnMovement                   = false;
  SceneCaptureComponent2D->PostProcessSettings.MotionBlurAmount = Config.motion_blur_amount;
  SceneCaptureComponent2D->PostProcessSettings.MotionBlurMax    = Config.motion_blur_distortion;

  FPostProcessSettings& PPSettings = SceneCaptureComponent2D->PostProcessSettings;

  // Force Lumen for Global Illumination
  PPSettings.bOverride_DynamicGlobalIlluminationMethod = true;
  PPSettings.DynamicGlobalIlluminationMethod = EDynamicGlobalIlluminationMethod::Lumen;

  // Force Lumen for Reflections
  PPSettings.bOverride_ReflectionMethod = true;
  PPSettings.ReflectionMethod = EReflectionMethod::Lumen;

  SceneCaptureComponent2D->ShowFlags.SetMotionBlur(Config.enable_motion_blur);
  SceneCaptureComponent2D->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);
  SceneCaptureComponent2D->ShowFlags.SetNaniteMeshes(true);
  SceneCaptureComponent2D->ShowFlags.SetAtmosphere(true);
  SceneCaptureComponent2D->ShowFlags.SetLumenReflections(true);
  SceneCaptureComponent2D->ShowFlags.SetLumenGlobalIllumination(true);

  ApplyExposureAndLens(Config.Exposure, Config.Lens, PPSettings);


  CameraBufferCriticalSection->Unlock();

  return true;
}

// game thread; starts the capture and hands the pixel transfer to the GPU
// readback pipeline instead of stalling on ReadPixels()
void URgbCamera::UpdateCamera(bool isExternallyLocked, double stamp)
{
	if (bReadbackDue || !TakeRingSlot()) {
		return;
	}

	const FVector OmegaCamera = CaptureOmega(stamp);

	if (!SceneCaptureComponent2D->bCaptureEveryFrame && !bEverRendered)
	{
		SceneCaptureComponent2D->CaptureScene();
	}
	else if (!SceneCaptureComponent2D->bCaptureEveryFrame)
	{
		SceneCaptureComponent2D->CaptureSceneDeferred();
		DeferredStamp = stamp;
		DeferredOmega = OmegaCamera;
		bReadbackDue  = true;
		return;
	}

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

bool URgbCamera::GetRgbCameraDataFromServerThread(TArray<uint8>& OutArray, double& stamp)
{
	camera_last_request_time_ = FPlatformTime::Seconds();

	CameraBufferCriticalSection->Lock();
	const bool bHaveFrame = CameraRendered;
	CameraBufferCriticalSection->Unlock();

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND && !SceneCaptureComponent2D->bCaptureEveryFrame && CachedTargetResource.load() && bEverRendered) {
		const double Now = FPlatformTime::Seconds();
		if (!RequestFrameFromServerThread(RenderTarget2D, Now, CaptureOmega(Now), false, rgb_camera_config_.enable_hdr, rgb_camera_config_.enable_hdr, 2.0)) {
			UE_LOG(LogTemp, Warning, TEXT("URgbCamera: timed out waiting for a frame"));
		}
	} else if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND || !bHaveFrame) {
		const uint64 Baseline = FrameCounter.load();

		auto Instruction      = std::make_shared<FInstruction<URgbCamera>>();
		Instruction->Function = [](URgbCamera& _URgbCamera) { _URgbCamera.UpdateCamera(true, FPlatformTime::Seconds()); };
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
		stamp                = stamp_;
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
		ApplyNoise(RawCopy, RawSize, NoiseConfig, static_cast<uint32>(stamp * 1.0e6));
	}

	CompressFrame(RawCopy, RawSize, CompressionConfig, false, OutArray);

	return true;
}

void URgbCamera::GetConfig(std::stringstream& OutputStream)
{
	Serializable::Drone::GetRgbCameraConfig::Response ResponseRGB(true);
	ResponseRGB.config = Serializable::Drone::RgbCameraConfig{};

	ResponseRGB.config.show_debug_camera_= rgb_camera_config_.ShowCameraComponent;

	ResponseRGB.config.offset_x_ = rgb_camera_config_.Offset.X;
	ResponseRGB.config.offset_y_ = rgb_camera_config_.Offset.Y;
	ResponseRGB.config.offset_z_ = rgb_camera_config_.Offset.Z;

	ResponseRGB.config.rotation_pitch_ = rgb_camera_config_.Orientation.Pitch;
	ResponseRGB.config.rotation_yaw_ = rgb_camera_config_.Orientation.Yaw;
	ResponseRGB.config.rotation_roll_ = rgb_camera_config_.Orientation.Roll;

	ResponseRGB.config.fov_ = rgb_camera_config_.FOVAngle;

	ResponseRGB.config.width_ = rgb_camera_config_.Width;
	ResponseRGB.config.capture_rate_ = rgb_camera_config_.CaptureRate;
	ResponseRGB.config.height_ = rgb_camera_config_.Height;

	ResponseRGB.config.enable_temporal_aa_ = rgb_camera_config_.enable_temporal_aa;
	ResponseRGB.config.enable_hdr_ = rgb_camera_config_.enable_hdr;
	ResponseRGB.config.enable_raytracing_ = rgb_camera_config_.enable_raytracing;

	ResponseRGB.config.enable_motion_blur_ = rgb_camera_config_.enable_motion_blur;
	ResponseRGB.config.motion_blur_distortion_ = rgb_camera_config_.motion_blur_distortion;
	ResponseRGB.config.motion_blur_amount_ = rgb_camera_config_.motion_blur_amount;

	ResponseRGB.config.exposure_.manual_          = rgb_camera_config_.Exposure.Manual;
	ResponseRGB.config.exposure_.shutter_time_    = rgb_camera_config_.Exposure.ShutterTime;
	ResponseRGB.config.exposure_.iso_             = rgb_camera_config_.Exposure.ISO;
	ResponseRGB.config.exposure_.ev_compensation_ = rgb_camera_config_.Exposure.EvCompensation;

	ResponseRGB.config.lens_.fstop_                    = rgb_camera_config_.Lens.Fstop;
	ResponseRGB.config.lens_.focal_distance_           = rgb_camera_config_.Lens.FocalDistance;
	ResponseRGB.config.lens_.sensor_width_mm_          = rgb_camera_config_.Lens.SensorWidthMm;
	ResponseRGB.config.lens_.vignette_intensity_       = rgb_camera_config_.Lens.VignetteIntensity;
	ResponseRGB.config.lens_.chromatic_aberration_     = rgb_camera_config_.Lens.ChromaticAberration;
	ResponseRGB.config.lens_.bloom_intensity_          = rgb_camera_config_.Lens.BloomIntensity;
	ResponseRGB.config.lens_.lens_flare_intensity_     = rgb_camera_config_.Lens.LensFlareIntensity;
	ResponseRGB.config.lens_.white_temp_               = rgb_camera_config_.Lens.WhiteTemp;
	ResponseRGB.config.lens_.white_tint_               = rgb_camera_config_.Lens.WhiteTint;
	ResponseRGB.config.lens_.motion_blur_from_shutter_ = rgb_camera_config_.Lens.MotionBlurFromShutter;

	const FCameraIntrinsics Effective =
	    EffectiveIntrinsics(rgb_camera_config_.Intrinsics, rgb_camera_config_.FOVAngle, rgb_camera_config_.Width, rgb_camera_config_.Height);
	ResponseRGB.config.intrinsics_.use_custom_ = rgb_camera_config_.Intrinsics.UseCustom;
	ResponseRGB.config.intrinsics_.fx_         = Effective.Fx;
	ResponseRGB.config.intrinsics_.fy_         = Effective.Fy;
	ResponseRGB.config.intrinsics_.cx_         = Effective.Cx - WirePixelShift;
	ResponseRGB.config.intrinsics_.cy_         = Effective.Cy - WirePixelShift;

	ResponseRGB.config.distortion_.enable_ = rgb_camera_config_.Distortion.Enable;
	ResponseRGB.config.distortion_.k1_     = rgb_camera_config_.Distortion.K1;
	ResponseRGB.config.distortion_.k2_     = rgb_camera_config_.Distortion.K2;
	ResponseRGB.config.distortion_.k3_     = rgb_camera_config_.Distortion.K3;
	ResponseRGB.config.distortion_.p1_     = rgb_camera_config_.Distortion.P1;
	ResponseRGB.config.distortion_.p2_     = rgb_camera_config_.Distortion.P2;

	ResponseRGB.config.noise_.enable_     = rgb_camera_config_.Noise.Enable;
	ResponseRGB.config.noise_.shot_scale_ = rgb_camera_config_.Noise.ShotScale;
	ResponseRGB.config.noise_.read_sigma_ = rgb_camera_config_.Noise.ReadSigma;
	ResponseRGB.config.noise_.row_sigma_  = rgb_camera_config_.Noise.RowSigma;

	ResponseRGB.config.rolling_shutter_.enable_       = rgb_camera_config_.RollingShutter.Enable;
	ResponseRGB.config.rolling_shutter_.readout_time_ = rgb_camera_config_.RollingShutter.ReadoutTime;

	ResponseRGB.config.compression_.format_ =
	    rgb_camera_config_.Compression.bJpeg ? Serializable::Drone::CameraCompression::JPEG : Serializable::Drone::CameraCompression::PNG;
	ResponseRGB.config.compression_.quality_ = rgb_camera_config_.Compression.Quality;

	Serialization::DeserializeResponse(ResponseRGB, OutputStream);
	UE_LOG(LogTemp, Log, TEXT("Got CameraRGB config"));
}

void URgbCamera::SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream)
{
	Serializable::Drone::SetRgbCameraConfig::Request Request;
	Serialization::SerializeRequest(Request, *InputStream);
	
	FRgbCameraConfig Config;
	Config.ShowCameraComponent = Request.config.show_debug_camera_;

	Config.Offset = FVector(Request.config.offset_x_, Request.config.offset_y_, Request.config.offset_z_);
	Config.Orientation = FRotator(Request.config.rotation_pitch_, Request.config.rotation_yaw_, Request.config.rotation_roll_);

	Config.FOVAngle = Request.config.fov_;

	Config.Width = Request.config.width_;
	Config.CaptureRate = Request.config.capture_rate_;
	Config.Height = Request.config.height_;

	Config.enable_hdr = Request.config.enable_hdr_;
	Config.enable_temporal_aa = Request.config.enable_temporal_aa_;
	Config.enable_raytracing = Request.config.enable_raytracing_;

	Config.enable_motion_blur = Request.config.enable_motion_blur_;
	Config.motion_blur_amount = Request.config.motion_blur_amount_;
	Config.motion_blur_distortion = Request.config.motion_blur_distortion_;

	Config.Exposure.Manual         = Request.config.exposure_.manual_;
	Config.Exposure.ShutterTime    = Request.config.exposure_.shutter_time_;
	Config.Exposure.ISO            = Request.config.exposure_.iso_;
	Config.Exposure.EvCompensation = Request.config.exposure_.ev_compensation_;

	Config.Lens.Fstop                 = Request.config.lens_.fstop_;
	Config.Lens.FocalDistance         = Request.config.lens_.focal_distance_;
	Config.Lens.SensorWidthMm         = Request.config.lens_.sensor_width_mm_;
	Config.Lens.VignetteIntensity     = Request.config.lens_.vignette_intensity_;
	Config.Lens.ChromaticAberration   = Request.config.lens_.chromatic_aberration_;
	Config.Lens.BloomIntensity        = Request.config.lens_.bloom_intensity_;
	Config.Lens.LensFlareIntensity    = Request.config.lens_.lens_flare_intensity_;
	Config.Lens.WhiteTemp             = Request.config.lens_.white_temp_;
	Config.Lens.WhiteTint             = Request.config.lens_.white_tint_;
	Config.Lens.MotionBlurFromShutter = Request.config.lens_.motion_blur_from_shutter_;

	Config.Intrinsics.UseCustom = Request.config.intrinsics_.use_custom_;
	Config.Intrinsics.Fx        = Request.config.intrinsics_.fx_;
	Config.Intrinsics.Fy        = Request.config.intrinsics_.fy_;
	Config.Intrinsics.Cx        = Request.config.intrinsics_.cx_ + WirePixelShift;
	Config.Intrinsics.Cy        = Request.config.intrinsics_.cy_ + WirePixelShift;

	Config.Distortion.Enable = Request.config.distortion_.enable_;
	Config.Distortion.K1     = Request.config.distortion_.k1_;
	Config.Distortion.K2     = Request.config.distortion_.k2_;
	Config.Distortion.K3     = Request.config.distortion_.k3_;
	Config.Distortion.P1     = Request.config.distortion_.p1_;
	Config.Distortion.P2     = Request.config.distortion_.p2_;

	Config.Noise.Enable    = Request.config.noise_.enable_;
	Config.Noise.ShotScale = Request.config.noise_.shot_scale_;
	Config.Noise.ReadSigma = Request.config.noise_.read_sigma_;
	Config.Noise.RowSigma  = Request.config.noise_.row_sigma_;

	Config.RollingShutter.Enable      = Request.config.rolling_shutter_.enable_;
	Config.RollingShutter.ReadoutTime = Request.config.rolling_shutter_.readout_time_;

	Config.Compression.bJpeg   = Request.config.compression_.format_ != Serializable::Drone::CameraCompression::PNG;
	Config.Compression.Quality = Request.config.compression_.quality_;

	bool Status = false;

	// auto Instruction = std::make_shared<FInstruction<ARobotPawn>>();
	//
	//  Instruction->Function = [ID = this->SensorID, &Config, &Status](ARobotPawn& _ARobotPawn) {
	//  	Status = _ARobotPawn.SetRgbCameraConfig(Config, ID);
	//  };
	//
	//  Owner->InstructionQueue->Enqueue(Instruction);

	auto Instruction      = std::make_shared<FInstruction<URgbCamera>>();
	Instruction->Function = [&Config, &Status](URgbCamera& _URgbCamera)
	{
		Status = _URgbCamera.SetRgbCameraConfig(Config);
	};
	InstructionQueue->Enqueue(Instruction);
		
	FGenericPlatformProcess::ConditionalSleep([Instruction](){return Instruction->Finished;});

	Serializable::Drone::SetRgbCameraConfig::Response Response(Status);
	Serialization::DeserializeResponse(Response, OutputStream);
	UE_LOG(LogTemp, Log, TEXT("Set RGBCamera config"));
}

void URgbCamera::GetData(std::stringstream& OutputStream)
{
	UE_LOG(LogTemp, Log, TEXT("URgbCamera::GetData"));
	TArray<uint8> CompressedBitmap;
	double stamp;
	bool res = GetRgbCameraDataFromServerThread(CompressedBitmap, stamp);

	if (!res) {
		Serializable::Drone::GetRgbCameraData::Response Response(false);
		
		Serialization::DeserializeResponse(Response, OutputStream);
		return;
	}

	Serializable::Drone::GetRgbCameraData::Response Response(true);
	Response.image_ = std::vector<unsigned char>(CompressedBitmap.Num());

	// TODO
	// std::copy(CompressedBitmap.begin(), CompressedBitmap.end(), std::back_inserter(Response->imageData));
	for(int i = 0; i < CompressedBitmap.Num(); i++) {
		Response.image_[i] = CompressedBitmap[i];
	}

	Response.stamp_ = stamp;

	Serialization::DeserializeResponse(Response, OutputStream);
}