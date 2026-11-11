#include "DepthCamera.h"

#include "TextureResource.h"

#include "FlightForgeCore/RobotPawn.h"
#include "Serialize.h"

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

/* GetData() //{ */

void UDepthCamera::GetData(std::stringstream& OutputStream)
{
	TArray<uint16> DepthMm;
	FIntPoint      Size;
	double         stamp;

	const bool res = GetDepthCameraDataFromServerThread(DepthMm, Size, stamp);

	if (!res)
	{
		Serializable::Drone::GetDepthCameraData::Response Response(false);
		Serialization::DeserializeResponse(Response, OutputStream);
		return;
	}

	Serializable::Drone::GetDepthCameraData::Response Response(true);
	Response.image_.resize(DepthMm.Num());
	for (int i = 0; i < DepthMm.Num(); i++)
	{
		Response.image_[i] = DepthMm[i];
	}
	Response.width_  = Size.X;
	Response.height_ = Size.Y;
	Response.stamp_  = stamp;

	Serialization::DeserializeResponse(Response, OutputStream);
}

/* //} */

/* GetConfig() / SetConfig() //{ */

void UDepthCamera::GetConfig(std::stringstream& OutputStream)
{
	Serializable::Drone::GetRgbCameraConfig::Response Response(true);
	Response.config = Serializable::Drone::RgbCameraConfig{};

	Response.config.show_debug_camera_ = depth_camera_config_.ShowCameraComponent;

	Response.config.offset_x_ = depth_camera_config_.Offset.X;
	Response.config.offset_y_ = depth_camera_config_.Offset.Y;
	Response.config.offset_z_ = depth_camera_config_.Offset.Z;

	Response.config.rotation_pitch_ = depth_camera_config_.Orientation.Pitch;
	Response.config.rotation_yaw_   = depth_camera_config_.Orientation.Yaw;
	Response.config.rotation_roll_  = depth_camera_config_.Orientation.Roll;

	Response.config.fov_ = depth_camera_config_.FOVAngle;

	Response.config.width_  = depth_camera_config_.Width;
	Response.config.capture_rate_ = depth_camera_config_.CaptureRate;
	Response.config.height_ = depth_camera_config_.Height;

	const FCameraIntrinsics Effective =
	    EffectiveIntrinsics(depth_camera_config_.Intrinsics, depth_camera_config_.FOVAngle, depth_camera_config_.Width, depth_camera_config_.Height);
	Response.config.intrinsics_.use_custom_ = depth_camera_config_.Intrinsics.UseCustom;
	Response.config.intrinsics_.fx_         = Effective.Fx;
	Response.config.intrinsics_.fy_         = Effective.Fy;
	Response.config.intrinsics_.cx_         = Effective.Cx - WirePixelShift;
	Response.config.intrinsics_.cy_         = Effective.Cy - WirePixelShift;

	Serialization::DeserializeResponse(Response, OutputStream);
}

void UDepthCamera::SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream)
{
	Serializable::Drone::SetRgbCameraConfig::Request Request;
	Serialization::SerializeRequest(Request, *InputStream);

	FRgbCameraConfig Config;
	Config.ShowCameraComponent = Request.config.show_debug_camera_;

	Config.Offset      = FVector(Request.config.offset_x_, Request.config.offset_y_, Request.config.offset_z_);
	Config.Orientation = FRotator(Request.config.rotation_pitch_, Request.config.rotation_yaw_, Request.config.rotation_roll_);

	Config.FOVAngle = Request.config.fov_;

	Config.Width  = Request.config.width_;
	Config.CaptureRate = Request.config.capture_rate_;
	Config.Height = Request.config.height_;

	Config.Intrinsics.UseCustom = Request.config.intrinsics_.use_custom_;
	Config.Intrinsics.Fx        = Request.config.intrinsics_.fx_;
	Config.Intrinsics.Fy        = Request.config.intrinsics_.fy_;
	Config.Intrinsics.Cx        = Request.config.intrinsics_.cx_ + WirePixelShift;
	Config.Intrinsics.Cy        = Request.config.intrinsics_.cy_ + WirePixelShift;

	bool Status = false;

	auto Instruction      = std::make_shared<FInstruction<UDepthCamera>>();
	Instruction->Function = [&Config, &Status](UDepthCamera& _Camera) { Status = _Camera.SetDepthCameraConfig(Config); };
	InstructionQueue->Enqueue(Instruction);

	FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

	Serializable::Drone::SetRgbCameraConfig::Response Response(Status);
	Serialization::DeserializeResponse(Response, OutputStream);
}

/* //} */
