#include "RgbSegCamera.h"

#include "Serialize.h"
#include "ueds/DronePawn.h"

URgbSegCamera::URgbSegCamera() : UCamera()
{
	SensorType = SensorType::STEREO_CAMERA;
	Name = TEXT("STEREO_CAMERA");

	InstructionQueue = std::make_unique<TQueue<std::shared_ptr<FInstruction<URgbSegCamera>>>>();
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
	SceneCaptureComponent2D->AddOrUpdateBlendable(PostProcessMaterial, 1);

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


void URgbSegCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	auto start = FPlatformTime::Seconds();

	std::shared_ptr<FInstruction<URgbSegCamera>> Instruction;
	while (InstructionQueue->Dequeue(Instruction)) {
		Instruction->Function(*this);
		Instruction->Finished = true;
	}
	
	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES)
	{
		CameraBufferCriticalSection->Lock();

		if ((FPlatformTime::Seconds() - camera_last_request_time_) > 1.0) {
		  if (SceneCaptureComponent2D->bCaptureEveryFrame) {
		    SceneCaptureComponent2D->bCaptureEveryFrame = false;
		    CameraRendered                              = false;
		    UE_LOG(LogTemp, Warning, TEXT("Turning off RGB segmented capture"));
		  }
		} else {
		  if (!SceneCaptureComponent2D->bCaptureEveryFrame) {
		    SceneCaptureComponent2D->bCaptureEveryFrame = true;
		    UE_LOG(LogTemp, Warning, TEXT("Turning on RGB segmented capture"));
		  } else {
		    UpdateCamera(true, start);
		  }
		}

		CameraBufferCriticalSection->Unlock();
	}


	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool URgbSegCamera::SetRgbSegCameraConfig(const FRgbCameraConfig& Config)
{
	UE_LOG(LogTemp, Log, TEXT("URgbSegCamera::SetRgbSegCameraConfig"));

	rgb_camera_config_ = Config;

	CameraBufferCriticalSection->Lock();

	SceneCaptureComponent2D->FOVAngle = Config.FOVAngle;

	SceneCaptureComponent2D->TextureTarget                = RenderTarget2D;
	SceneCaptureComponent2D->bAlwaysPersistRenderingState = true;
	SceneCaptureComponent2D->bCaptureEveryFrame           = false;
	SceneCaptureComponent2D->bCaptureOnMovement           = false;
	SceneCaptureComponent2D->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);

	if (Config.Width > 0 && Config.Height > 0) {
		RenderTarget2D->ResizeTarget(Config.Width, Config.Height);
		///RenderTarget2DRgbSeg->ResizeTarget(Config.Width, Config.Height);
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Invalid dimensions for RenderTarget2D. Width and Height should be greater than 0."));
	}

	CameraBufferCriticalSection->Unlock();
	
	return true;
}

void URgbSegCamera::UpdateCamera(bool isExternallyLocked, double stamp)
{
	UE_LOG(LogTemp, Log, TEXT("URgbSegCamera::UpdateCamera"));
	
	SceneCaptureComponent2D->CaptureSource = SCS_FinalColorLDR;
	SceneCaptureComponent2D->TextureTarget = RenderTarget2D;
	
	if (!isExternallyLocked) {
	  CameraBufferCriticalSection->Lock();
	}

	if (!SceneCaptureComponent2D->bCaptureEveryFrame)
	{
		SceneCaptureComponent2D->CaptureScene();
	}
	
	const auto ResourceSeg = RenderTarget2D->GameThread_GetRenderTargetResource();
	
	ResourceSeg->ReadPixels(CameraBuffer);
	
	CameraDataNeedsCompress = false;
	
	CameraRendered = true;
	
	stamp_ = stamp;
	
	if (!isExternallyLocked) {
	  CameraBufferCriticalSection->Unlock();
	}
}

bool URgbSegCamera::GetRgbSegCameraFromServerThread(TArray<uint8>& OutArray, double& stamp) {

	CameraBufferCriticalSection->Lock();

	camera_last_request_time_ = FPlatformTime::Seconds();

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND || (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES && !CameraRendered)) {
		// auto Instruction      = std::make_shared<FInstruction<ADronePawn>>();
		// Instruction->Function = [ID = this->SensorID](ADronePawn& _DronePawn) { _DronePawn.UpdateCamera(ID, true, 0.0); };
		// Owner->InstructionQueue->Enqueue(Instruction);

		auto Instruction      = std::make_shared<FInstruction<URgbSegCamera>>();
		Instruction->Function = [ID = this->SensorID](URgbSegCamera& _URgbSegCamera) { _URgbSegCamera.UpdateCamera(true, 0.0); };
		InstructionQueue->Enqueue(Instruction);
		
		FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });
	}

	if (!CameraRendered) {
		UE_LOG(LogTemp, Warning, TEXT("DronePawn(): rgb seg camera is not rendered, returning"));
		CameraBufferCriticalSection->Unlock();
		return false;
	}
	
	/* if (RgbSegCameraDataNeedsCompress) { */
	/*   FImageUtils::ThumbnailCompressImageArray(RenderTarget2DRgbSeg->SizeX, RenderTarget2DRgbSeg->SizeY, RgbSegCameraBuffer, *CompressedRgbSegCameraData); */
	/*   RgbSegCameraDataNeedsCompress = false; */
	/* } */

	auto byteArray = MakeUnique<TArray<uint8>>();
	TransformImageArray(RenderTarget2D->SizeX, RenderTarget2D->SizeY, CameraBuffer, *byteArray);

	const auto Size = (*byteArray).Num();
	OutArray.SetNumUninitialized(Size);
	FMemory::Memcpy(OutArray.GetData(), (*byteArray).GetData(), Size * sizeof(uint8));

	CameraBufferCriticalSection->Unlock();

	stamp = stamp_;

	return true;
}

void URgbSegCamera::GetConfig(std::stringstream& OutputStream)
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
	UE_LOG(LogTemp, Log, TEXT("Got CameraRGBSeg config"));
}

void URgbSegCamera::SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream)
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

	// auto Instruction = std::make_shared<FInstruction<ADronePawn>>();
	//
	// Instruction->Function = [ID = this->SensorID, &Config, &Status](ADronePawn& _ADronePawn) {
	// 	Status = _ADronePawn.SetRgbCameraConfig(Config, ID); //wrong
	// };
	//
	// Owner->InstructionQueue->Enqueue(Instruction);

	auto Instruction      = std::make_shared<FInstruction<URgbSegCamera>>();
	Instruction->Function = [&Config, &Status](URgbSegCamera& _URgbSegCamera)
	{
		Status = _URgbSegCamera.SetRgbSegCameraConfig(Config);
	};
	InstructionQueue->Enqueue(Instruction);
		
	FGenericPlatformProcess::ConditionalSleep([Instruction](){return Instruction->Finished;});

	Serializable::Drone::SetRgbCameraConfig::Response Response(Status);
	Serialization::DeserializeResponse(Response, OutputStream);
	UE_LOG(LogTemp, Log, TEXT("Set RGBSegCamera config"));
}

void URgbSegCamera::GetData(std::stringstream& OutputStream)
{
	TArray<uint8> CompressedBitmap;
	double stamp;
	bool res = GetRgbSegCameraFromServerThread(CompressedBitmap, stamp);

	if (!res) {
		Serializable::Drone::GetRgbSegCameraData::Response Response(false);

		Serialization::DeserializeResponse(Response, OutputStream);
		return;
	}

	Serializable::Drone::GetRgbSegCameraData::Response Response(true);
	Response.image_ = std::vector<unsigned char>(CompressedBitmap.Num());

	for(int i = 0; i < CompressedBitmap.Num(); i++) {
		Response.image_[i] = CompressedBitmap[i];
	}

	Response.stamp_ = stamp;

	Serialization::DeserializeResponse(Response, OutputStream);
}
