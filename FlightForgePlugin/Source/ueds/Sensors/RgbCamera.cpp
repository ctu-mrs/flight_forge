#include "RgbCamera.h"

#include "Serialize.h"
#include "ueds/DronePawn.h"


URgbCamera::URgbCamera() : UCamera()
{
	SensorType = SensorType::RGB_CAMERA;
	Name = TEXT("RGB_CAMERA");
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
	SceneCaptureComponent2D->ShowFlags.SetTemporalAA(true);
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

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES)
	{
		CameraBufferCriticalSection->Lock();
	
	   if ((FPlatformTime::Seconds() - camera_last_request_time_) > 1.0) {
	     if (SceneCaptureComponent2D->bCaptureEveryFrame) {
	       SceneCaptureComponent2D->bCaptureEveryFrame = false;
	       CameraRendered = false;
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
	
	   CameraBufferCriticalSection->Unlock();
	}
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

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

  SceneCaptureComponent2D->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);

  SceneCaptureComponent2D->bUseRayTracingIfEnabled = Config.enable_raytracing;

  SceneCaptureComponent2D->TextureTarget                        = RenderTarget2D;
  SceneCaptureComponent2D->bAlwaysPersistRenderingState         = true;
  SceneCaptureComponent2D->bCaptureEveryFrame                   = false;
  SceneCaptureComponent2D->bCaptureOnMovement                   = false;
  SceneCaptureComponent2D->PostProcessSettings.MotionBlurAmount = Config.motion_blur_amount;
  SceneCaptureComponent2D->PostProcessSettings.MotionBlurMax    = Config.motion_blur_distortion;
  SceneCaptureComponent2D->ShowFlags.SetMotionBlur(Config.enable_motion_blur);


  if (Config.Width > 0 && Config.Height > 0) {
    RenderTarget2D->ResizeTarget(Config.Width, Config.Height);
  } else {
    UE_LOG(LogTemp, Warning, TEXT("Invalid dimensions for RenderTarget2D. Width and Height should be greater than 0."));
  }

  CameraBufferCriticalSection->Unlock();

  return true;
}

void URgbCamera::UpdateCamera(bool isExternallyLocked, double stamp)
{
	UE_LOG(LogTemp, Log, TEXT("URgbCamera::UpdateCamera"));
	if (!isExternallyLocked) {
	  CameraBufferCriticalSection->Lock();
	}
	
	//SceneCaptureComponent2D->CaptureScene();
	const auto Resource = RenderTarget2D->GameThread_GetRenderTargetResource();
	Resource->ReadPixels(CameraBuffer);
	  
	CameraDataNeedsCompress = true;
	
	stamp_ = stamp;
	
	CameraRendered = true;
	
	if (!isExternallyLocked) {
	  CameraBufferCriticalSection->Unlock();
	}
}

bool URgbCamera::GetRgbCameraDataFromServerThread(TArray<uint8>& OutArray, double& stamp)
{
	CameraBufferCriticalSection->Lock();

	camera_last_request_time_ = FPlatformTime::Seconds();

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND) {
		auto Instruction      = std::make_shared<FInstruction<ADronePawn>>();
		Instruction->Function = [ID = this->SensorID](ADronePawn& _DronePawn) { _DronePawn.UpdateCamera(ID, true, 0.0); };
		Owner->InstructionQueue->Enqueue(Instruction);

		//UE_LOG(LogTemp, Log, TEXT("Rgb camera locked and asleep"));
		FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });
		//UE_LOG(LogTemp, Log, TEXT("Rgb camera awoken"));
		
	}

	if (!CameraRendered) {
		UE_LOG(LogTemp, Warning, TEXT("DronePawn(): rgb camera is not rendered, returning"));
		CameraBufferCriticalSection->Unlock();
		return false;
	}

	if (CameraDataNeedsCompress) {
		FImageUtils::ThumbnailCompressImageArray(RenderTarget2D->SizeX, RenderTarget2D->SizeY, CameraBuffer, *CompressedCameraData);
		CameraDataNeedsCompress = false;
	}

	const auto Size = (*CompressedCameraData).Num();
	OutArray.SetNumUninitialized(Size);
	FMemory::Memcpy(OutArray.GetData(), (*CompressedCameraData).GetData(), Size * sizeof(uint8));

	stamp = stamp_;

	CameraBufferCriticalSection->Unlock();

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

	bool Status = false;

	auto Instruction = std::make_shared<FInstruction<ADronePawn>>();
	
	Instruction->Function = [ID = this->SensorID, &Config, &Status](ADronePawn& _ADronePawn) {
		Status = _ADronePawn.SetRgbCameraConfig(Config, ID);
	};
	
	Owner->InstructionQueue->Enqueue(Instruction);
		
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