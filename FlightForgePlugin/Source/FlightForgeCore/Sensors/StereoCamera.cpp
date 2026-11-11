#include "StereoCamera.h"

#include "TextureResource.h"

#include "SensorWire.h"
#include "FlightForgeCore/RobotPawn.h"
#include "RenderingThread.h"

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
	stereo_camera_config_.Offset_left         = DEFAULT_CAMERA_MOUNT;
	stereo_camera_config_.Orientation_left    = FRotator(0, 0, 0);
	stereo_camera_config_.Offset_right        = DEFAULT_CAMERA_MOUNT;
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


void UStereoCamera::DrainInstructions()
{
	std::shared_ptr<FInstruction<UStereoCamera>> Instruction;
	while (InstructionQueue->Dequeue(Instruction))
	{
		Instruction->Function(*this);
		Instruction->Finished = true;
	}
}

void UStereoCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	auto start = FPlatformTime::Seconds();

	DrainInstructions();
	
	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES)
	{
		const bool bWanted = (FPlatformTime::Seconds() - camera_last_request_time_) < 1.0;

		if (bWanted && !bStreamOn)
		{
			bStreamOn = true;
			SceneCaptureComponent2D->bCaptureEveryFrame = SceneCaptureComponent2DStereoRight->bCaptureEveryFrame = CaptureRate <= 0.0;
			UE_LOG(LogTemp, Warning, TEXT("Turning on Stereo capture"));
		}
		else if (!bWanted && bStreamOn)
		{
			bStreamOn = false;
			SceneCaptureComponent2D->bCaptureEveryFrame = SceneCaptureComponent2DStereoRight->bCaptureEveryFrame = false;
			CameraBufferCriticalSection->Lock();
			CameraRendered = false;
			CameraBufferCriticalSection->Unlock();
			UE_LOG(LogTemp, Warning, TEXT("Turning off Stereo capture, %u frames folded"), FramesFolded);
		}
		else if (bWanted && ShouldCapture() && CaptureDue())
		{
			UpdateCamera(true, start);
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

  if (!ValidDimensions(Config.Width, Config.Height)) {
    return false;
  }

  stereo_camera_config_ = Config;
  CaptureRate    = Config.CaptureRate;
  NextCaptureDue = -1.0;
  if (bStreamOn) {
    SceneCaptureComponent2D->bCaptureEveryFrame = SceneCaptureComponent2DStereoRight->bCaptureEveryFrame = CaptureRate <= 0.0;
  }

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
	if (!TakeRingSlot()) {
		return;
	}

	if (!SceneCaptureComponent2D->bCaptureEveryFrame && !SceneCaptureComponent2DStereoRight->bCaptureEveryFrame)
	{
		SceneCaptureComponent2D->CaptureScene();
		SceneCaptureComponent2DStereoRight->CaptureScene();
	}

	TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Left  = EnqueueReadback(RenderTarget2D, stamp);
	TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Right = EnqueueReadback(RenderTarget2DStereoRight, stamp);

	if (!Left || !Right) {
		return;
	}

	PushReadback(Left);
	CameraBufferCriticalSection->Lock();
	InflightRight.Add(Right);
	CameraBufferCriticalSection->Unlock();
}

// game thread: publishes only when both eyes have landed, so left/right in a
// response always come from the same capture
bool UStereoCamera::PumpStereoReadback()
{
	bool bPublished = false;

	while (true)
	{
		CameraBufferCriticalSection->Lock();

		if (InflightReadbacks.Num() == 0 || InflightRight.Num() == 0)
		{
			InflightReadbacks.Reset();
			InflightRight.Reset();
			CameraBufferCriticalSection->Unlock();
			break;
		}

		TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Left  = InflightReadbacks[0];
		TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Right = InflightRight[0];

		if (!Left->bCopied || !Right->bCopied)
		{
			CameraBufferCriticalSection->Unlock();

			for (const auto& Slot : {Left, Right})
			{
				if (Slot->Readback && Slot->Readback->IsReady() && !Slot->bResolveIssued.exchange(true))
				{
					IssueResolve(Slot);
				}
			}
			break;
		}

		CameraBuffer            = MoveTemp(Left->Pixels);
		StereoRightCameraBuffer = MoveTemp(Right->Pixels);
		CameraBufferSize        = Left->Size;
		stamp_                  = Left->Stamp;
		CameraRendered          = CameraBuffer.Num() > 0 && StereoRightCameraBuffer.Num() > 0;
		InflightReadbacks.RemoveAt(0);
		InflightRight.RemoveAt(0);

		CameraBufferCriticalSection->Unlock();

		NotifyFrameReady();
		bPublished = true;
	}

	return bPublished;
}

void UStereoCamera::DrainReadback()
{
	const double Deadline = FPlatformTime::Seconds() + 5.0;

	while (InflightReadbacks.Num() > 0 && FPlatformTime::Seconds() < Deadline)
	{
		FlushRenderingCommands();
		if (!PumpStereoReadback() && InflightReadbacks.Num() > 0)
		{
			FPlatformProcess::SleepNoStats(0.0002f);
		}
	}
}

bool UStereoCamera::GetStereoCameraDataFromServerThread(FCameraFrame& OutLeft, FCameraFrame& OutRight)
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
		OutLeft.Stamp     = stamp_;
		OutRight.Stamp    = stamp_;
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

	CompressFrame(RawLeft, RawSize, CompressionConfig, false, OutLeft);
	CompressFrame(RawRight, RawSize, CompressionConfig, false, OutRight);

	return true;
}

/* wire //{ */

void UStereoCamera::GetConfig(FWireReply& Reply)
{
	const FStereoCameraConfig& C = stereo_camera_config_;

	flightforge::wire::StereoCameraConfigT Table;
	Table.show_debug_camera  = C.ShowCameraComponent;
	Table.mount_left         = FSensorWire::Mount(C.Offset_left, C.Orientation_left);
	Table.mount_right        = FSensorWire::Mount(C.Offset_right, C.Orientation_right);
	Table.fov                = C.FOVAngle;
	Table.width              = C.Width;
	Table.height             = C.Height;
	Table.enable_temporal_aa = C.enable_temporal_aa;
	Table.enable_raytracing  = C.enable_raytracing;
	Table.enable_hdr         = C.enable_hdr;
	Table.intrinsics         = FSensorWire::Intrinsics(C.Intrinsics.UseCustom, EffectiveIntrinsics(C.Intrinsics, C.FOVAngle, C.Width, C.Height));
	Table.distortion         = FSensorWire::Distortion(C.Distortion);
	Table.encoding           = FSensorWire::Encoding(C.Compression);
	Table.capture_rate       = C.CaptureRate;

	const auto Offset = flightforge::wire::CreateStereoCameraConfig(Reply.Builder, &Table);
	Reply.Set(flightforge::wire::CreateGetStereoCameraConfigResponse(Reply.Builder, Offset));
}

void UStereoCamera::SetConfig(const flightforge::wire::Request& Request, FWireReply& Reply)
{
	const flightforge::wire::SetStereoCameraConfigRequest* Set = Request.body_as_SetStereoCameraConfigRequest();
	if (!Set || !Set->config())
	{
		Reply.Fail(TEXT("set_stereo_camera_config carries no config"));
		return;
	}

	const std::unique_ptr<flightforge::wire::StereoCameraConfigT> In(Set->config()->UnPack());
	if (!FSensorWire::ValidPinhole(In->fov, In->width, In->height, Reply))
	{
		return;
	}

	FStereoCameraConfig Config;
	Config.ShowCameraComponent = In->show_debug_camera;
	FSensorWire::Mount(In->mount_left, Config.Offset_left, Config.Orientation_left);
	FSensorWire::Mount(In->mount_right, Config.Offset_right, Config.Orientation_right);
	Config.FOVAngle           = In->fov;
	Config.Width              = In->width;
	Config.Height             = In->height;
	Config.enable_temporal_aa = In->enable_temporal_aa;
	Config.enable_raytracing  = In->enable_raytracing;
	Config.enable_hdr         = In->enable_hdr;
	Config.Intrinsics         = FSensorWire::Intrinsics(In->intrinsics.get());
	Config.Distortion         = FSensorWire::Distortion(In->distortion.get());
	Config.Compression        = FSensorWire::Encoding(In->encoding.get());
	Config.CaptureRate        = In->capture_rate;

	bool bApplied = false;
	RunOnSensorQueue(*this, [&Config, &bApplied](UStereoCamera& Camera) { bApplied = Camera.SetStereoCameraConfig(Config); });

	if (!bApplied)
	{
		Reply.Fail(TEXT("the stereo camera rejected the config"));
	}
}

// the pose places the left eye; the right keeps its offset from it
void UStereoCamera::SetMount(const FVector& OffsetCm, const FRotator& Rotation)
{
	RunOnSensorQueue(*this, [&](UStereoCamera& Camera) {
		FStereoCameraConfig Config   = Camera.stereo_camera_config_;
		const FTransform    OldLeft(Config.Orientation_left, Config.Offset_left);
		const FTransform    NewLeft(Rotation, OffsetCm);
		const FTransform    Right    = FTransform(Config.Orientation_right, Config.Offset_right).GetRelativeTransform(OldLeft) * NewLeft;
		Config.Offset_left           = OffsetCm;
		Config.Orientation_left      = Rotation;
		Config.Offset_right          = Right.GetLocation();
		Config.Orientation_right     = Right.Rotator();
		Camera.SetStereoCameraConfig(Config);
	});
}

void UStereoCamera::GetData(const flightforge::wire::Request& Request, FWireReply& Reply)
{
	FCameraFrame Left;
	FCameraFrame Right;
	if (!GetStereoCameraDataFromServerThread(Left, Right))
	{
		Reply.Fail(TEXT("the stereo camera has no frame yet"));
		return;
	}

	const auto LeftImage  = FSensorWire::Image(Reply, Left, FSensorWire::ColourFormat(Left.Codec));
	const auto RightImage = FSensorWire::Image(Reply, Right, FSensorWire::ColourFormat(Right.Codec));
	Reply.Set(flightforge::wire::CreateStereoCameraDataResponse(Reply.Builder, Left.Stamp, LeftImage, RightImage));
}

/* //} */
