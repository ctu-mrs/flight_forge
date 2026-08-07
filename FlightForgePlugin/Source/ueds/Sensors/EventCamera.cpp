#include "EventCamera.h"

#include "Async/ParallelFor.h"
#include "TextureResource.h"

#include "Serialize.h"
#include "ueds/DronePawn.h"

// a runaway scene (or a stalled client) must not grow the buffer unboundedly
#define EVENT_BUFFER_MAX_EVENTS 2000000

// at most this many events per pixel per frame; larger jumps saturate, which
// matches real sensors far better than an event burst does
#define EVENT_MAX_PER_PIXEL 4

UEventCamera::UEventCamera() : UCamera()
{
	SensorType = SensorType::EVENT_CAMERA;
	Name = TEXT("EVENT_CAMERA");

	InstructionQueue = std::make_unique<TQueue<std::shared_ptr<FInstruction<UEventCamera>>>>();
	EventsCriticalSection = std::make_unique<FCriticalSection>();
}

/* BeginPlay() //{ */

void UEventCamera::BeginPlay()
{
	Super::BeginPlay();

	RenderTarget2D = NewObject<UTextureRenderTarget2D>();
	RenderTarget2D->InitCustomFormat(640, 480, PF_B8G8R8A8, false);
	RenderTarget2D->RenderTargetFormat = RTF_RGBA8;
	RenderTarget2D->bGPUSharedFlag     = true;

	// LDR, no TAA, no motion blur: temporal filtering would fabricate events
	SceneCaptureComponent2D->CaptureSource = SCS_FinalColorLDR;
	SceneCaptureComponent2D->TextureTarget = RenderTarget2D;
	SceneCaptureComponent2D->ShowFlags.SetTemporalAA(false);
	SceneCaptureComponent2D->ShowFlags.SetMotionBlur(false);
	SceneCaptureComponent2D->bAlwaysPersistRenderingState = true;
	SceneCaptureComponent2D->bCaptureEveryFrame           = false;
	SceneCaptureComponent2D->bCaptureOnMovement           = false;

	event_camera_config_.ShowCameraComponent  = false;
	event_camera_config_.Offset               = FVector(0, 0, 0);
	event_camera_config_.Orientation          = FRotator(0, 0, 0);
	event_camera_config_.FOVAngle             = 90;
	event_camera_config_.Width                = 640;
	event_camera_config_.Height               = 480;
	event_camera_config_.ContrastThresholdPos = 0.2;
	event_camera_config_.ContrastThresholdNeg = 0.2;

	SetEventCameraConfig(event_camera_config_);
}

//}

void UEventCamera::BeginDestroy()
{
	if (EventsCriticalSection)
	{
		EventsCriticalSection->Lock();
		EventBuffer.Empty();
		EventsCriticalSection->Unlock();
	}

	Super::BeginDestroy();
}

/* TickComponent() //{ */

void UEventCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	auto start = FPlatformTime::Seconds();

	std::shared_ptr<FInstruction<UEventCamera>> Instruction;
	while (InstructionQueue->Dequeue(Instruction)) {
		Instruction->Function(*this);
		Instruction->Finished = true;
	}

	// events are inherently a stream: capture every frame while a client is
	// listening, regardless of the drone's camera capture mode
	if ((FPlatformTime::Seconds() - camera_last_request_time_) > 1.0) {
		if (SceneCaptureComponent2D->bCaptureEveryFrame) {
			SceneCaptureComponent2D->bCaptureEveryFrame = false;
			ReferenceLogIntensity.Empty();
			UE_LOG(LogTemp, Warning, TEXT("Turning off event camera capture"));
		}
	} else {
		if (!SceneCaptureComponent2D->bCaptureEveryFrame) {
			SceneCaptureComponent2D->bCaptureEveryFrame = true;
			UE_LOG(LogTemp, Warning, TEXT("Turning on event camera capture"));
		} else {
			UpdateCamera(true, start);
		}
	}

	if (PumpReadback())
	{
		ProcessPublishedFrame();
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

//}

void UEventCamera::UpdateCamera(bool isExternallyLocked, double stamp)
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

/* ProcessPublishedFrame() //{ */

void UEventCamera::ProcessPublishedFrame()
{
	TArray<FColor> Frame;
	FIntPoint      Size;
	double         Stamp;
	{
		CameraBufferCriticalSection->Lock();
		Frame = CameraBuffer;
		Size  = CameraBufferSize;
		Stamp = stamp_;
		CameraBufferCriticalSection->Unlock();
	}

	const int32 NumPixels = Size.X * Size.Y;

	if (NumPixels <= 0 || Frame.Num() != NumPixels)
	{
		return;
	}

	// first frame after enabling or resizing seeds the reference, no events
	if (ReferenceLogIntensity.Num() != NumPixels || ReferenceSize != Size)
	{
		ReferenceLogIntensity.SetNumUninitialized(NumPixels);
		ReferenceSize = Size;

		for (int32 i = 0; i < NumPixels; ++i)
		{
			const FColor& C = Frame[i];
			ReferenceLogIntensity[i] = FMath::Loge(0.2126f * C.R + 0.7152f * C.G + 0.0722f * C.B + 1.0f);
		}

		LastFrameStamp = Stamp;
		return;
	}

	const float Cpos = FMath::Max((float)event_camera_config_.ContrastThresholdPos, 0.01f);
	const float Cneg = FMath::Max((float)event_camera_config_.ContrastThresholdNeg, 0.01f);

	// per-chunk local buffers so the pixel sweep parallelizes without contention
	const int32 NumChunks = FMath::Clamp(NumPixels / 65536, 1, 16);
	const int32 ChunkSize = FMath::DivideAndRoundUp(NumPixels, NumChunks);

	TArray<TArray<FCameraEvent>> ChunkEvents;
	ChunkEvents.SetNum(NumChunks);

	ParallelFor(NumChunks, [&](int32 Chunk) {
		const int32 First = Chunk * ChunkSize;
		const int32 Last  = FMath::Min(First + ChunkSize, NumPixels);

		TArray<FCameraEvent>& Local = ChunkEvents[Chunk];

		for (int32 i = First; i < Last; ++i)
		{
			const FColor& C = Frame[i];
			const float   L = FMath::Loge(0.2126f * C.R + 0.7152f * C.G + 0.0722f * C.B + 1.0f);

			float Delta = L - ReferenceLogIntensity[i];

			int32 Count    = 0;
			int8  Polarity = 0;

			if (Delta >= Cpos)
			{
				Count    = FMath::Min((int32)(Delta / Cpos), EVENT_MAX_PER_PIXEL);
				Polarity = 1;
				ReferenceLogIntensity[i] += Count * Cpos;
			}
			else if (Delta <= -Cneg)
			{
				Count    = FMath::Min((int32)(-Delta / Cneg), EVENT_MAX_PER_PIXEL);
				Polarity = -1;
				ReferenceLogIntensity[i] -= Count * Cneg;
			}

			if (Count > 0)
			{
				const uint16 X = (uint16)(i % Size.X);
				const uint16 Y = (uint16)(i / Size.X);

				for (int32 e = 0; e < Count; ++e)
				{
					Local.Add({X, Y, Polarity, Stamp});
				}
			}
		}
	});

	EventsCriticalSection->Lock();

	for (const TArray<FCameraEvent>& Local : ChunkEvents)
	{
		if (EventBuffer.Num() + Local.Num() > EVENT_BUFFER_MAX_EVENTS)
		{
			UE_LOG(LogTemp, Warning, TEXT("UEventCamera: event buffer full, dropping events"));
			break;
		}
		EventBuffer.Append(Local);
	}

	LastFrameStamp = Stamp;

	EventsCriticalSection->Unlock();
}

//}

/* SetEventCameraConfig() //{ */

bool UEventCamera::SetEventCameraConfig(const FEventCameraConfig& Config)
{
	event_camera_config_ = Config;

	CameraBufferCriticalSection->Lock();

	SceneCaptureMeshHolder->SetVisibility(Config.ShowCameraComponent);
	SceneCaptureMeshHolder->SetRelativeLocation(Config.Offset);
	SceneCaptureMeshHolder->SetRelativeRotation(FRotator(0, 0, 0));
	SceneCaptureMeshHolder->SetRelativeRotation(Config.Orientation);

	SceneCaptureComponent2D->FOVAngle = Config.FOVAngle;

	if (Config.Width > 0 && Config.Height > 0) {
		RenderTarget2D->ResizeTarget(Config.Width, Config.Height);
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Invalid dimensions for RenderTarget2D. Width and Height should be greater than 0."));
	}

	CameraBufferCriticalSection->Unlock();

	// resolution or optics changed: reseed the reference on the next frame
	ReferenceLogIntensity.Empty();

	EventsCriticalSection->Lock();
	EventBuffer.Empty();
	EventsCriticalSection->Unlock();

	return true;
}

//}

/* GetConfig() / SetConfig() //{ */

void UEventCamera::GetConfig(std::stringstream& OutputStream)
{
	Serializable::Drone::GetEventCameraConfig::Response Response(true);
	Response.config = Serializable::Drone::EventCameraConfig{};

	Response.config.show_debug_camera_ = event_camera_config_.ShowCameraComponent;

	Response.config.offset_x_ = event_camera_config_.Offset.X;
	Response.config.offset_y_ = event_camera_config_.Offset.Y;
	Response.config.offset_z_ = event_camera_config_.Offset.Z;

	Response.config.rotation_pitch_ = event_camera_config_.Orientation.Pitch;
	Response.config.rotation_yaw_   = event_camera_config_.Orientation.Yaw;
	Response.config.rotation_roll_  = event_camera_config_.Orientation.Roll;

	Response.config.fov_    = event_camera_config_.FOVAngle;
	Response.config.width_  = event_camera_config_.Width;
	Response.config.height_ = event_camera_config_.Height;

	Response.config.contrast_threshold_pos_ = event_camera_config_.ContrastThresholdPos;
	Response.config.contrast_threshold_neg_ = event_camera_config_.ContrastThresholdNeg;

	Serialization::DeserializeResponse(Response, OutputStream);
}

void UEventCamera::SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream)
{
	Serializable::Drone::SetEventCameraConfig::Request Request;
	Serialization::SerializeRequest(Request, *InputStream);

	FEventCameraConfig Config;
	Config.ShowCameraComponent = Request.config.show_debug_camera_;

	Config.Offset      = FVector(Request.config.offset_x_, Request.config.offset_y_, Request.config.offset_z_);
	Config.Orientation = FRotator(Request.config.rotation_pitch_, Request.config.rotation_yaw_, Request.config.rotation_roll_);

	Config.FOVAngle = Request.config.fov_;
	Config.Width    = Request.config.width_;
	Config.Height   = Request.config.height_;

	Config.ContrastThresholdPos = Request.config.contrast_threshold_pos_;
	Config.ContrastThresholdNeg = Request.config.contrast_threshold_neg_;

	bool Status = false;

	auto Instruction      = std::make_shared<FInstruction<UEventCamera>>();
	Instruction->Function = [&Config, &Status](UEventCamera& _UEventCamera)
	{
		Status = _UEventCamera.SetEventCameraConfig(Config);
	};
	InstructionQueue->Enqueue(Instruction);

	FGenericPlatformProcess::ConditionalSleep([Instruction](){return Instruction->Finished;});

	Serializable::Drone::SetEventCameraConfig::Response Response(Status);
	Serialization::DeserializeResponse(Response, OutputStream);
}

//}

/* GetData() //{ */

void UEventCamera::GetData(std::stringstream& OutputStream)
{
	// mark the stream as wanted so the tick keeps (or starts) capturing
	camera_last_request_time_ = FPlatformTime::Seconds();

	Serializable::Drone::GetEventCameraData::Response Response(true);

	EventsCriticalSection->Lock();

	Response.events_.reserve(EventBuffer.Num());
	for (const FCameraEvent& E : EventBuffer)
	{
		Serializable::Drone::GetEventCameraData::Event Out;
		Out.x        = E.X;
		Out.y        = E.Y;
		Out.polarity = E.Polarity;
		Out.stamp    = E.Stamp;
		Response.events_.push_back(Out);
	}
	EventBuffer.Reset();

	Response.stamp_ = LastFrameStamp;

	EventsCriticalSection->Unlock();

	Serialization::DeserializeResponse(Response, OutputStream);
}

//}
