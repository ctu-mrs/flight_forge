#include "RgbCamera.h"

#include "Engine/World.h"
#include "TextureResource.h"

#include "Serialize.h"
#include "FlightForgeCore/DronePawn.h"


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

	SceneCaptureComponent2D->CaptureSource = SCS_FinalColorHDR;
	SceneCaptureComponent2D->TextureTarget = RenderTarget2D;
	SceneCaptureComponent2D->ShowFlags.SetTemporalAA(false);
	SceneCaptureComponent2D->bAlwaysPersistRenderingState = true;
	SceneCaptureComponent2D->bCaptureEveryFrame           = false;
	SceneCaptureComponent2D->bCaptureOnMovement           = false;
	SceneCaptureComponent2D->bUseRayTracingIfEnabled      = true;

	rgb_camera_config_.ShowCameraComponent    = false;
	rgb_camera_config_.Offset                 = FVector(0, 0, 0);
	rgb_camera_config_.Orientation            = FRotator(0, 0, 0);
	rgb_camera_config_.FOVAngle               = 90;
	rgb_camera_config_.Width                  = 640;
	rgb_camera_config_.Height                 = 480;
	rgb_camera_config_.enable_motion_blur     = true;
	rgb_camera_config_.motion_blur_amount     = 1.0;
	rgb_camera_config_.motion_blur_distortion = 50.0;

	SetRgbCameraConfig(rgb_camera_config_);
	SetCameraCaptureMode(Owner->GetCameraCaptureMode());
}


void URgbCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	auto start = FPlatformTime::Seconds();

	std::shared_ptr<FInstruction<URgbCamera>> Instruction;
	while (InstructionQueue->Dequeue(Instruction)) {
	Instruction->Function(*this);
	Instruction->Finished = true;
	}

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES)
	{
	   if ((FPlatformTime::Seconds() - camera_last_request_time_) > 1.0) {
	     if (SceneCaptureComponent2D->bCaptureEveryFrame) {
	       SceneCaptureComponent2D->bCaptureEveryFrame = false;
	       CameraBufferCriticalSection->Lock();
	       CameraRendered = false;
	       CameraBufferCriticalSection->Unlock();
	       UE_LOG(LogTemp, Warning, TEXT("Turning off RGB capture"));
	     }
	   } else {
	     if (!SceneCaptureComponent2D->bCaptureEveryFrame) {
	       SceneCaptureComponent2D->bCaptureEveryFrame = true;
	       UE_LOG(LogTemp, Warning, TEXT("Turning on RGB capture"));
	     } else {
	       UpdateCamera(true, start);
	     }
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

  rgb_camera_config_ = Config;

  CameraBufferCriticalSection->Lock();

  SceneCaptureMeshHolder->SetVisibility(Config.ShowCameraComponent);
  SceneCaptureMeshHolder->SetRelativeLocation(Config.Offset);
  SceneCaptureMeshHolder->SetRelativeRotation(FRotator(0, 0, 0));
  SceneCaptureMeshHolder->SetRelativeRotation(Config.Orientation);

  SceneCaptureComponent2D->FOVAngle = Config.FOVAngle;

  if (Config.enable_hdr) {
    SceneCaptureComponent2D->CaptureSource = SCS_FinalColorHDR;
  } else {
    SceneCaptureComponent2D->CaptureSource = SCS_FinalColorLDR;
  }

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


  if (Config.Width > 0 && Config.Height > 0) {
    RenderTarget2D->ResizeTarget(Config.Width, Config.Height);
  } else {
    UE_LOG(LogTemp, Warning, TEXT("Invalid dimensions for RenderTarget2D. Width and Height should be greater than 0."));
  }

  CameraBufferCriticalSection->Unlock();

  return true;
}

// game thread; starts the capture and hands the pixel transfer to the GPU
// readback pipeline instead of stalling on ReadPixels()
void URgbCamera::UpdateCamera(bool isExternallyLocked, double stamp)
{
	if (PendingReadback) {
		return;  // previous transfer still in flight
	}

	if (!SceneCaptureComponent2D->bCaptureEveryFrame)
	{
		SceneCaptureComponent2D->CaptureScene();
	}

	PendingReadback = EnqueueReadback(RenderTarget2D, stamp);
}

bool URgbCamera::GetRgbCameraDataFromServerThread(TArray<uint8>& OutArray, double& stamp)
{
	camera_last_request_time_ = FPlatformTime::Seconds();

	CameraBufferCriticalSection->Lock();
	const bool bHaveFrame = CameraRendered;
	CameraBufferCriticalSection->Unlock();

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND || (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES && !bHaveFrame)) {
		const uint64 Baseline = FrameCounter.load();

		auto Instruction      = std::make_shared<FInstruction<URgbCamera>>();
		Instruction->Function = [](URgbCamera& _URgbCamera) { _URgbCamera.UpdateCamera(true, FPlatformTime::Seconds()); };
		InstructionQueue->Enqueue(Instruction);

		if (!WaitForNewFrame(Baseline, 2.0)) {
			UE_LOG(LogTemp, Warning, TEXT("URgbCamera: timed out waiting for a frame"));
		}
	}

	TArray<FColor> RawCopy;
	FIntPoint      RawSize;
	{
		CameraBufferCriticalSection->Lock();

		if (!CameraRendered) {
			CameraBufferCriticalSection->Unlock();
			UE_LOG(LogTemp, Warning, TEXT("URgbCamera: camera is not rendered, returning"));
			return false;
		}

		RawCopy = CameraBuffer;
		RawSize = CameraBufferSize;
		stamp   = stamp_;

		CameraBufferCriticalSection->Unlock();
	}

	// compress outside the lock so the game thread's publish path never blocks on PNG encoding
	FImageUtils::ThumbnailCompressImageArray(RawSize.X, RawSize.Y, RawCopy, OutArray);

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

	bool Status = false;

	// auto Instruction = std::make_shared<FInstruction<ADronePawn>>();
	//
	//  Instruction->Function = [ID = this->SensorID, &Config, &Status](ADronePawn& _ADronePawn) {
	//  	Status = _ADronePawn.SetRgbCameraConfig(Config, ID);
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