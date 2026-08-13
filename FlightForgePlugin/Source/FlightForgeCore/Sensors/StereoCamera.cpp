#include "StereoCamera.h"

#include "TextureResource.h"

#include "Serialize.h"
#include "FlightForgeCore/DronePawn.h"

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
	stereo_camera_config_.Offset_left         = FVector(0, 0, 0);
	stereo_camera_config_.Orientation_left    = FRotator(0, 0, 0);
	stereo_camera_config_.Offset_right        = FVector(0, 0, 0);
	stereo_camera_config_.Orientation_right   = FRotator(0, 0, 0);
	stereo_camera_config_.FOVAngle            = 90;
	stereo_camera_config_.Width               = 640;
	stereo_camera_config_.Height              = 480;
	
	ApplyLevelPostProcessBaseline(SceneCaptureComponent2D);
	ApplyLevelPostProcessBaseline(SceneCaptureComponent2DStereoRight);

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
	  if ((FPlatformTime::Seconds() - camera_last_request_time_) > 1.0) {
	    if (SceneCaptureComponent2D->bCaptureEveryFrame) {
	      SceneCaptureComponent2D->bCaptureEveryFrame  = false;
	      SceneCaptureComponent2DStereoRight->bCaptureEveryFrame = false;
	      CameraBufferCriticalSection->Lock();
	      CameraRendered = false;
	      CameraBufferCriticalSection->Unlock();
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
	}

	PumpStereoReadback();

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
  SceneCaptureMeshHolder->SetRelativeLocation(Config.Offset_left);
  SceneCaptureMeshHolder->SetRelativeRotation(FRotator(0, 0, 0));
  SceneCaptureMeshHolder->SetRelativeRotation(Config.Orientation_left);

  SceneCaptureMeshHolderStereoRight->SetVisibility(Config.ShowCameraComponent);
  SceneCaptureMeshHolderStereoRight->SetRelativeLocation(Config.Offset_right);
  SceneCaptureMeshHolderStereoRight->SetRelativeRotation(FRotator(0, 0, 0));
  SceneCaptureMeshHolderStereoRight->SetRelativeRotation(Config.Orientation_right);

  SetupCaptureProjection(Config.Intrinsics, Config.Distortion, Config.FOVAngle, Config.Width, Config.Height,
                         {SceneCaptureComponent2D, SceneCaptureComponent2DStereoRight}, {RenderTarget2D, RenderTarget2DStereoRight});

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


  CameraBufferCriticalSection->Unlock();

  return true;
}

// game thread; starts both captures and hands the pixel transfers to the GPU
// readback pipeline instead of stalling on ReadPixels()
void UStereoCamera::UpdateCamera(bool isExternallyLocked, double stamp)
{
	if (PendingReadback || PendingReadbackRight) {
		return;  // previous transfer still in flight
	}

	if (!SceneCaptureComponent2D->bCaptureEveryFrame && !SceneCaptureComponent2DStereoRight->bCaptureEveryFrame)
	{
		SceneCaptureComponent2D->CaptureScene();
		SceneCaptureComponent2DStereoRight->CaptureScene();
	}

	PendingReadback      = EnqueueReadback(RenderTarget2D, stamp);
	PendingReadbackRight = EnqueueReadback(RenderTarget2DStereoRight, stamp);
}

// game thread: publishes only when both eyes have landed, so left/right in a
// response always come from the same capture
bool UStereoCamera::PumpStereoReadback()
{
	if (!PendingReadback || !PendingReadbackRight)
	{
		PendingReadback.Reset();
		PendingReadbackRight.Reset();
		return false;
	}

	if (PendingReadback->bCopied && PendingReadbackRight->bCopied)
	{
		CameraBufferCriticalSection->Lock();
		CameraBuffer            = MoveTemp(PendingReadback->Pixels);
		StereoRightCameraBuffer = MoveTemp(PendingReadbackRight->Pixels);
		CameraBufferSize        = PendingReadback->Size;
		stamp_                  = PendingReadback->Stamp;
		CameraRendered          = CameraBuffer.Num() > 0 && StereoRightCameraBuffer.Num() > 0;
		CameraBufferCriticalSection->Unlock();

		PendingReadback.Reset();
		PendingReadbackRight.Reset();
		NotifyFrameReady();
		return true;
	}

	if (PendingReadback->Readback && PendingReadback->Readback->IsReady() && !PendingReadback->bResolveIssued.exchange(true))
	{
		IssueResolve(PendingReadback);
	}

	if (PendingReadbackRight->Readback && PendingReadbackRight->Readback->IsReady() && !PendingReadbackRight->bResolveIssued.exchange(true))
	{
		IssueResolve(PendingReadbackRight);
	}

	return false;
}

bool UStereoCamera::GetStereoCameraDataFromServerThread(TArray<uint8>& image_left, TArray<uint8>& image_right, double& stamp)
{
	camera_last_request_time_ = FPlatformTime::Seconds();

	CameraBufferCriticalSection->Lock();
	const bool bHaveFrame = CameraRendered;
	CameraBufferCriticalSection->Unlock();

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND || (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES && !bHaveFrame)) {
		const uint64 Baseline = FrameCounter.load();

		auto Instruction      = std::make_shared<FInstruction<UStereoCamera>>();
		Instruction->Function = [](UStereoCamera& _UStereoCamera) { _UStereoCamera.UpdateCamera(true, FPlatformTime::Seconds()); };
		InstructionQueue->Enqueue(Instruction);

		if (!WaitForNewFrame(Baseline, 2.0)) {
			UE_LOG(LogTemp, Warning, TEXT("UStereoCamera: timed out waiting for a frame"));
		}
	}

	TArray<FColor>                                        RawLeft;
	TArray<FColor>                                        RawRight;
	FIntPoint                                             RawSize;
	TSharedPtr<const FDistortionLUT, ESPMode::ThreadSafe> LUT;
	FCameraCompression                                    CompressionConfig;
	{
		CameraBufferCriticalSection->Lock();

		if (!CameraRendered) {
			CameraBufferCriticalSection->Unlock();
			UE_LOG(LogTemp, Warning, TEXT("UStereoCamera: stereo camera is not rendered, returning"));
			return false;
		}

		RawLeft           = CameraBuffer;
		RawRight          = StereoRightCameraBuffer;
		RawSize           = CameraBufferSize;
		stamp             = stamp_;
		LUT               = DistortionLUT;
		CompressionConfig = stereo_camera_config_.Compression;

		CameraBufferCriticalSection->Unlock();
	}

	// warp + compress outside the lock so the game thread's publish path never blocks
	if (LUT && RawSize == LUT->SourceSize) {
		TArray<FColor> Warped;
		ResampleImage(*LUT, RawLeft, Warped, false);
		RawLeft = MoveTemp(Warped);
		ResampleImage(*LUT, RawRight, Warped, false);
		RawRight = MoveTemp(Warped);
		RawSize  = LUT->OutputSize;
	}

	CompressFrame(RawLeft, RawSize, CompressionConfig, false, image_left);
	CompressFrame(RawRight, RawSize, CompressionConfig, false, image_right);

	return true;
}

void UStereoCamera::GetConfig(std::stringstream& OutputStream)
{
	Serializable::Drone::GetStereoCameraConfig::Response ResponseStereo(true);
	ResponseStereo.config = Serializable::Drone::StereoCameraConfig{};

	ResponseStereo.config.show_debug_camera_= stereo_camera_config_.ShowCameraComponent;

	ResponseStereo.config.offset_x_left_ = stereo_camera_config_.Offset_left.X;
	ResponseStereo.config.offset_y_left_ = stereo_camera_config_.Offset_left.Y;
	ResponseStereo.config.offset_z_left_ = stereo_camera_config_.Offset_left.Z;

	ResponseStereo.config.offset_x_right_ = stereo_camera_config_.Offset_right.X;
	ResponseStereo.config.offset_y_right_ = stereo_camera_config_.Offset_right.Y;
	ResponseStereo.config.offset_z_right_ = stereo_camera_config_.Offset_right.Z;

	ResponseStereo.config.rotation_pitch_left_ = stereo_camera_config_.Orientation_left.Pitch;
	ResponseStereo.config.rotation_yaw_left_ = stereo_camera_config_.Orientation_left.Yaw;
	ResponseStereo.config.rotation_roll_left_ = stereo_camera_config_.Orientation_left.Roll;

	ResponseStereo.config.rotation_pitch_right_ = stereo_camera_config_.Orientation_right.Pitch;
	ResponseStereo.config.rotation_yaw_right_ = stereo_camera_config_.Orientation_right.Yaw;
	ResponseStereo.config.rotation_roll_right_ = stereo_camera_config_.Orientation_right.Roll;

	ResponseStereo.config.fov_ = stereo_camera_config_.FOVAngle;

	ResponseStereo.config.width_ = stereo_camera_config_.Width;
	ResponseStereo.config.height_ = stereo_camera_config_.Height;

	ResponseStereo.config.enable_temporal_aa_ = stereo_camera_config_.enable_temporal_aa;
	ResponseStereo.config.enable_hdr_ = stereo_camera_config_.enable_hdr;
	ResponseStereo.config.enable_raytracing_ = stereo_camera_config_.enable_raytracing;

	const FCameraIntrinsics Effective =
	    EffectiveIntrinsics(stereo_camera_config_.Intrinsics, stereo_camera_config_.FOVAngle, stereo_camera_config_.Width, stereo_camera_config_.Height);
	ResponseStereo.config.intrinsics_.use_custom_ = stereo_camera_config_.Intrinsics.UseCustom;
	ResponseStereo.config.intrinsics_.fx_         = Effective.Fx;
	ResponseStereo.config.intrinsics_.fy_         = Effective.Fy;
	ResponseStereo.config.intrinsics_.cx_         = Effective.Cx;
	ResponseStereo.config.intrinsics_.cy_         = Effective.Cy;

	ResponseStereo.config.distortion_.enable_ = stereo_camera_config_.Distortion.Enable;
	ResponseStereo.config.distortion_.k1_     = stereo_camera_config_.Distortion.K1;
	ResponseStereo.config.distortion_.k2_     = stereo_camera_config_.Distortion.K2;
	ResponseStereo.config.distortion_.k3_     = stereo_camera_config_.Distortion.K3;
	ResponseStereo.config.distortion_.p1_     = stereo_camera_config_.Distortion.P1;
	ResponseStereo.config.distortion_.p2_     = stereo_camera_config_.Distortion.P2;

	ResponseStereo.config.compression_.format_ =
	    stereo_camera_config_.Compression.bJpeg ? Serializable::Drone::CameraCompression::JPEG : Serializable::Drone::CameraCompression::PNG;
	ResponseStereo.config.compression_.quality_ = stereo_camera_config_.Compression.Quality;

	Serialization::DeserializeResponse(ResponseStereo, OutputStream);
	UE_LOG(LogTemp, Warning, TEXT("Got CameraStereo config"));
}

void UStereoCamera::SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream)
{
	Serializable::Drone::SetStereoCameraConfig::Request Request;
	Serialization::SerializeRequest(Request, *InputStream);

	FStereoCameraConfig Config;
	Config.ShowCameraComponent = Request.config.show_debug_camera_;

	Config.Offset_left = FVector(Request.config.offset_x_left_, Request.config.offset_y_left_, Request.config.offset_z_left_);
	Config.Offset_right = FVector(Request.config.offset_x_right_, Request.config.offset_y_right_, Request.config.offset_z_right_);
	Config.Orientation_left = FRotator(Request.config.rotation_pitch_left_, Request.config.rotation_yaw_left_, Request.config.rotation_roll_left_);
	Config.Orientation_right = FRotator(Request.config.rotation_pitch_right_, Request.config.rotation_yaw_right_, Request.config.rotation_roll_right_);

	Config.FOVAngle = Request.config.fov_;

	Config.Width = Request.config.width_;
	Config.Height = Request.config.height_;

	Config.enable_hdr = Request.config.enable_hdr_;
	Config.enable_temporal_aa = Request.config.enable_temporal_aa_;
	Config.enable_raytracing = Request.config.enable_raytracing_;

	Config.Intrinsics.UseCustom = Request.config.intrinsics_.use_custom_;
	Config.Intrinsics.Fx        = Request.config.intrinsics_.fx_;
	Config.Intrinsics.Fy        = Request.config.intrinsics_.fy_;
	Config.Intrinsics.Cx        = Request.config.intrinsics_.cx_;
	Config.Intrinsics.Cy        = Request.config.intrinsics_.cy_;

	Config.Distortion.Enable = Request.config.distortion_.enable_;
	Config.Distortion.K1     = Request.config.distortion_.k1_;
	Config.Distortion.K2     = Request.config.distortion_.k2_;
	Config.Distortion.K3     = Request.config.distortion_.k3_;
	Config.Distortion.P1     = Request.config.distortion_.p1_;
	Config.Distortion.P2     = Request.config.distortion_.p2_;

	Config.Compression.bJpeg   = Request.config.compression_.format_ != Serializable::Drone::CameraCompression::PNG;
	Config.Compression.Quality = Request.config.compression_.quality_;

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


