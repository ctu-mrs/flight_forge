#include "StereoCamera.h"

#include "Serialize.h"
#include "ueds/DronePawn.h"

UStereoCamera::UStereoCamera() : UCamera()
{
	SensorType = SensorType::STEREO_CAMERA;
	Name = TEXT("STEREO_CAMERA");

	SceneCaptureMeshHolderStereoRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SceneCaptureMeshHolderStereoRight"));
	SceneCaptureMeshHolderStereoRight->SetupAttachment(this);

	SceneCaptureComponent2DStereoRight = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2DStereoRight"));
	SceneCaptureComponent2DStereoRight->SetupAttachment(SceneCaptureMeshHolderStereoRight);

	InstructionQueue = std::make_unique<TQueue<std::shared_ptr<FInstruction<UStereoCamera>>>>();
}


void UStereoCamera::BeginPlay()
{
	Super::BeginPlay();

	RenderTarget2D = NewObject<UTextureRenderTarget2D>();
	RenderTarget2D->InitCustomFormat(640, 480, PF_B8G8R8A8, false);
	RenderTarget2D->RenderTargetFormat = RTF_RGBA8;
	RenderTarget2D->bGPUSharedFlag     = true;
	
	RenderTarget2DStereoRight = NewObject<UTextureRenderTarget2D>();
	RenderTarget2DStereoRight->InitCustomFormat(640, 480, PF_B8G8R8A8, false);
	RenderTarget2DStereoRight->RenderTargetFormat = RTF_RGBA8;
	RenderTarget2DStereoRight->bGPUSharedFlag     = true;

	SceneCaptureComponent2D->CaptureSource = SCS_FinalColorHDR;
	SceneCaptureComponent2D->TextureTarget = RenderTarget2D;
	SceneCaptureComponent2D->ShowFlags.SetTemporalAA(true);
	SceneCaptureComponent2D->bAlwaysPersistRenderingState = true;
	SceneCaptureComponent2D->bCaptureEveryFrame           = false;
	SceneCaptureComponent2D->bCaptureOnMovement           = false;
	SceneCaptureComponent2D->bUseRayTracingIfEnabled      = true;
	
	SceneCaptureComponent2DStereoRight->CaptureSource = SCS_FinalColorHDR;
	SceneCaptureComponent2DStereoRight->TextureTarget = RenderTarget2DStereoRight;
	SceneCaptureComponent2DStereoRight->ShowFlags.SetTemporalAA(true);
	SceneCaptureComponent2DStereoRight->bAlwaysPersistRenderingState = true;
	SceneCaptureComponent2DStereoRight->bCaptureEveryFrame           = false;
	SceneCaptureComponent2DStereoRight->bCaptureOnMovement           = false;
	SceneCaptureComponent2DStereoRight->bUseRayTracingIfEnabled      = true;
	
	
	stereo_camera_config_.ShowCameraComponent = false;
	stereo_camera_config_.Offset              = FVector(0, 0, 0);
	stereo_camera_config_.Orientation         = FRotator(0, 0, 0);
	stereo_camera_config_.FOVAngle            = 90;
	stereo_camera_config_.Width               = 640;
	stereo_camera_config_.Height              = 480;
	stereo_camera_config_.baseline            = 0.1;
	
	SetStereoCameraConfig(stereo_camera_config_);
	SetCameraCaptureMode(Owner->GetCameraCaptureMode());
}

void UStereoCamera::Initialize(int InSensorID)
{
	Super::Initialize(InSensorID);
	
	SceneCaptureMeshHolderStereoRight->SetMobility(EComponentMobility::Movable);
	if (!SceneCaptureMeshHolderStereoRight->IsRegistered()) 
		SceneCaptureMeshHolderStereoRight->RegisterComponent();
	
	
	SceneCaptureComponent2DStereoRight->SetMobility(EComponentMobility::Movable);
	if (!SceneCaptureComponent2DStereoRight->IsRegistered())
		SceneCaptureComponent2DStereoRight->RegisterComponent();
}


void UStereoCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	auto start = FPlatformTime::Seconds();

	std::shared_ptr<FInstruction<UStereoCamera>> Instruction;
	while (InstructionQueue->Dequeue(Instruction)) {
		Instruction->Function(*this);
		Instruction->Finished = true;
	}
	
	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES)
	{
	  CameraBufferCriticalSection->Lock();
	
	  if ((FPlatformTime::Seconds() - camera_last_request_time_) > 1.0) {
	    if (SceneCaptureComponent2D->bCaptureEveryFrame) {
	      SceneCaptureComponent2D->bCaptureEveryFrame  = false;
	      SceneCaptureComponent2DStereoRight->bCaptureEveryFrame = false;
	      CameraRendered = false;
	      UE_LOG(LogTemp, Warning, TEXT("Turning off Stereo capture"));
	    }
	  } else {
	    if (!SceneCaptureComponent2D->bCaptureEveryFrame) {
	      UE_LOG(LogTemp, Warning, TEXT("Turning on Stereo capture"));
	      SceneCaptureComponent2D->bCaptureEveryFrame  = true;
	      SceneCaptureComponent2DStereoRight->bCaptureEveryFrame = true;
	    } else {
	      UpdateCamera(true, start);
	    }
	  }
	
	  CameraBufferCriticalSection->Unlock();
	}
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UStereoCamera::BeginDestroy()
{
	StereoRightCameraBuffer.Empty();
	Super::BeginDestroy();
}

bool UStereoCamera::SetStereoCameraConfig(const FStereoCameraConfig& Config) {

  UE_LOG(LogTemp, Log, TEXT("UCamera::SetStereoCameraConfig"));

  stereo_camera_config_ = Config;

  CameraBufferCriticalSection->Lock();

  SceneCaptureMeshHolder->SetVisibility(Config.ShowCameraComponent);
  SceneCaptureMeshHolder->SetRelativeLocation(Config.Offset);
  SceneCaptureMeshHolder->SetRelativeRotation(FRotator(0, 0, 0));
  SceneCaptureMeshHolder->SetRelativeRotation(Config.Orientation);

  SceneCaptureMeshHolderStereoRight->SetVisibility(Config.ShowCameraComponent);
  SceneCaptureMeshHolderStereoRight->SetRelativeLocation(Config.Offset + FVector(0.0, 100.0 * Config.baseline, 0.0));
  SceneCaptureMeshHolderStereoRight->SetRelativeRotation(FRotator(0, 0, 0));
  SceneCaptureMeshHolderStereoRight->SetRelativeRotation(Config.Orientation);

  SceneCaptureComponent2D->FOVAngle  = Config.FOVAngle;
  SceneCaptureComponent2DStereoRight->FOVAngle = Config.FOVAngle;

  if (Config.enable_hdr) {
    SceneCaptureComponent2D->CaptureSource  = SCS_FinalColorHDR;
    SceneCaptureComponent2DStereoRight->CaptureSource = SCS_FinalColorHDR;
  } else {
    SceneCaptureComponent2D->CaptureSource  = SCS_FinalColorLDR;
    SceneCaptureComponent2DStereoRight->CaptureSource = SCS_FinalColorLDR;
  }

  SceneCaptureComponent2D->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);
  SceneCaptureComponent2DStereoRight->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);

  SceneCaptureComponent2D->bUseRayTracingIfEnabled  = Config.enable_raytracing;
  SceneCaptureComponent2DStereoRight->bUseRayTracingIfEnabled = Config.enable_raytracing;

  if (Config.Width > 0 && Config.Height > 0) {
    RenderTarget2D->ResizeTarget(Config.Width, Config.Height);
    RenderTarget2DStereoRight->ResizeTarget(Config.Width, Config.Height);
  } else {
    UE_LOG(LogTemp, Warning, TEXT("Invalid dimensions for RenderTarget2D. Width and Height should be greater than 0."));
  }

  CameraBufferCriticalSection->Unlock();

  return true;
}

void UStereoCamera::UpdateCamera(bool isExternallyLocked, double stamp)
{
	UE_LOG(LogTemp, Log, TEXT("UStereoCamera::UpdateCamera"));
	
	if (!isExternallyLocked) {
	  CameraBufferCriticalSection->Lock();
	}

	if (!SceneCaptureComponent2D->bCaptureEveryFrame && !SceneCaptureComponent2DStereoRight->bCaptureEveryFrame)
	{
		SceneCaptureComponent2D->CaptureScene();
		SceneCaptureComponent2DStereoRight->CaptureScene();
	}
	
	const auto ResourceLeft = RenderTarget2D->GameThread_GetRenderTargetResource();
	
	const auto ResourceRight = RenderTarget2DStereoRight->GameThread_GetRenderTargetResource();
	
	ResourceLeft->ReadPixels(CameraBuffer);
	ResourceRight->ReadPixels(StereoRightCameraBuffer);
	
	CameraDataNeedsCompress = true;
	
	CameraRendered = true;
	
	stamp_ = stamp;
	
	if (!isExternallyLocked) {
	  CameraBufferCriticalSection->Unlock();
	}
}

bool UStereoCamera::GetStereoCameraDataFromServerThread(TArray<uint8>& image_left, TArray<uint8>& image_right, double& stamp)
{
	{
		CameraBufferCriticalSection->Lock();

		camera_last_request_time_ = FPlatformTime::Seconds();

		if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND || (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES && !CameraRendered)) {
			// auto Instruction      = std::make_shared<FInstruction<ADronePawn>>();
			// Instruction->Function = [ID = this->SensorID](ADronePawn& _DronePawn) { _DronePawn.UpdateCamera(ID, true, 0.0); };
			// Owner->InstructionQueue->Enqueue(Instruction);

			auto Instruction      = std::make_shared<FInstruction<UStereoCamera>>();
			Instruction->Function = [ID = this->SensorID](UStereoCamera& _URgbCamera) { _URgbCamera.UpdateCamera(true, 0.0); };
			InstructionQueue->Enqueue(Instruction);
			
			FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });
		}

		if (!CameraRendered) {
			UE_LOG(LogTemp, Warning, TEXT("DronePawn(): stereo camera is not rendered, returning"));
			CameraBufferCriticalSection->Unlock();
			return false;
		}

		if (CameraDataNeedsCompress) {

			FImageUtils::ThumbnailCompressImageArray(RenderTarget2D->SizeX, RenderTarget2D->SizeY, CameraBuffer,
													 *CompressedCameraData);
			FImageUtils::ThumbnailCompressImageArray(RenderTarget2DStereoRight->SizeX, RenderTarget2DStereoRight->SizeY, StereoRightCameraBuffer,
													 *CompressedStereoRightCameraData);
			CameraDataNeedsCompress = false;
		}

		{
			const auto Size = (*CompressedCameraData).Num();
			image_left.SetNumUninitialized(Size);
			FMemory::Memcpy(image_left.GetData(), (*CompressedCameraData).GetData(), Size * sizeof(uint8));
		}

		{
			const auto Size = (*CompressedStereoRightCameraData).Num();
			image_right.SetNumUninitialized(Size);
			FMemory::Memcpy(image_right.GetData(), (*CompressedStereoRightCameraData).GetData(), Size * sizeof(uint8));
		}

		stamp = stamp_;

		CameraBufferCriticalSection->Unlock();

		return true;
	}
}

void UStereoCamera::GetConfig(std::stringstream& OutputStream)
{
	Serializable::Drone::GetStereoCameraConfig::Response ResponseStereo(true);
	ResponseStereo.config = Serializable::Drone::StereoCameraConfig{};

	ResponseStereo.config.show_debug_camera_= stereo_camera_config_.ShowCameraComponent;

	ResponseStereo.config.offset_x_ = stereo_camera_config_.Offset.X;
	ResponseStereo.config.offset_y_ = stereo_camera_config_.Offset.Y;
	ResponseStereo.config.offset_z_ = stereo_camera_config_.Offset.Z;

	ResponseStereo.config.rotation_pitch_ = stereo_camera_config_.Orientation.Pitch;
	ResponseStereo.config.rotation_yaw_ = stereo_camera_config_.Orientation.Yaw;
	ResponseStereo.config.rotation_roll_ = stereo_camera_config_.Orientation.Roll;

	ResponseStereo.config.fov_ = stereo_camera_config_.FOVAngle;

	ResponseStereo.config.width_ = stereo_camera_config_.Width;
	ResponseStereo.config.height_ = stereo_camera_config_.Height;

	ResponseStereo.config.baseline_ = stereo_camera_config_.baseline;

	ResponseStereo.config.enable_temporal_aa_ = stereo_camera_config_.enable_temporal_aa;
	ResponseStereo.config.enable_hdr_ = stereo_camera_config_.enable_hdr;
	ResponseStereo.config.enable_raytracing_ = stereo_camera_config_.enable_raytracing;

	Serialization::DeserializeResponse(ResponseStereo, OutputStream);
	UE_LOG(LogTemp, Warning, TEXT("Got CameraStereo config"));
}

void UStereoCamera::SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream)
{
	Serializable::Drone::SetStereoCameraConfig::Request Request;
				
	FStereoCameraConfig Config;
	Config.ShowCameraComponent = Request.config.show_debug_camera_;

	Config.Offset = FVector(Request.config.offset_x_, Request.config.offset_y_, Request.config.offset_z_);
	Config.Orientation = FRotator(Request.config.rotation_pitch_, Request.config.rotation_yaw_, Request.config.rotation_roll_);

	Config.FOVAngle = Request.config.fov_;

	Config.Width = Request.config.width_;
	Config.Height = Request.config.height_;

	Config.baseline = Request.config.baseline_;

	Config.enable_hdr = Request.config.enable_hdr_;
	Config.enable_temporal_aa = Request.config.enable_temporal_aa_;
	Config.enable_raytracing = Request.config.enable_raytracing_; 

	bool Status = false;

	// auto Instruction = std::make_shared<FInstruction<ADronePawn>>();
	//
	// Instruction->Function = [ID = this->SensorID, &Config, &Status](ADronePawn& _ADronePawn) {
	// 	Status = _ADronePawn.SetStereoCameraConfig(Config, ID);
	// };
	//
	// Owner->InstructionQueue->Enqueue(Instruction);
	
	auto Instruction      = std::make_shared<FInstruction<UStereoCamera>>();
	Instruction->Function = [&Config, &Status](UStereoCamera& _UStereoCamera)
	{
		Status = _UStereoCamera.SetStereoCameraConfig(Config);
	};
	InstructionQueue->Enqueue(Instruction);
		
	FGenericPlatformProcess::ConditionalSleep([Instruction](){return Instruction->Finished;});

	Serializable::Drone::SetStereoCameraConfig::Response Response(Status);
	Serialization::DeserializeResponse(Response, OutputStream);
	UE_LOG(LogTemp, Log, TEXT("Set Stereo Camera config"));
}

void UStereoCamera::GetData(std::stringstream& OutputStream)
{
	TArray<uint8> image_left_;
	TArray<uint8> image_right_;
	double stamp;
	bool res = GetStereoCameraDataFromServerThread(image_left_, image_right_, stamp);

	if (!res) {
		Serializable::Drone::GetStereoCameraData::Response Response(false);
		Serialization::DeserializeResponse(Response, OutputStream);

		return;
	}

	Serializable::Drone::GetStereoCameraData::Response Response(true);

	Response.image_left_ = std::vector<unsigned char>(image_left_.Num());
	Response.image_right_ = std::vector<unsigned char>(image_right_.Num());

	// TODO
	// std::copy(CompressedBitmap.begin(), CompressedBitmap.end(), std::back_inserter(Response->imageData));
	for(int i = 0; i < image_left_.Num(); i++) {
		Response.image_left_[i] = image_left_[i];
	}

	for(int i = 0; i < image_right_.Num(); i++) {
		Response.image_right_[i] = image_right_[i];
	}

	Response.stamp_ = stamp;

	Serialization::DeserializeResponse(Response, OutputStream);
}


