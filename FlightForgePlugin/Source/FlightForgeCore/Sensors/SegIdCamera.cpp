#include "SegIdCamera.h"

#include "TextureResource.h"

#include "FlightForgeCore/DronePawn.h"
#include "SegIdPass.h"
#include "FlightForgeCore/Segmentation/SegmentationTagger.h"
#include "Serialize.h"

USegIdCamera::USegIdCamera() : UCamera()
{
	SensorType = SensorType::INSTANCE_SEG_CAMERA;
	Name       = TEXT("INSTANCE_SEG_CAMERA");

	InstructionQueue = std::make_unique<TQueue<std::shared_ptr<FInstruction<USegIdCamera>>>>();
}

/* BeginPlay() //{ */

void USegIdCamera::BeginPlay()
{
	Super::BeginPlay();

	RenderTarget2D = NewObject<UTextureRenderTarget2D>();
	RenderTarget2D->InitCustomFormat(640, 480, PF_B8G8R8A8, false);
	RenderTarget2D->RenderTargetFormat = RTF_RGBA8;
	RenderTarget2D->bGPUSharedFlag     = true;

	// the capture only exists to make the renderer produce this view; the ID image
	// itself comes from the SegId pass. FinalColorLDR keeps the post-process chain
	// alive, which is where the pass hooks in.
	SceneCaptureComponent2D->CaptureSource                = SCS_FinalColorLDR;
	SceneCaptureComponent2D->TextureTarget                = RenderTarget2D;
	SceneCaptureComponent2D->bAlwaysPersistRenderingState = true;
	SceneCaptureComponent2D->bCaptureEveryFrame           = false;
	SceneCaptureComponent2D->bCaptureOnMovement           = false;
	SceneCaptureComponent2D->bUseRayTracingIfEnabled      = false;

	seg_camera_config_.ShowCameraComponent = false;
	seg_camera_config_.Offset              = DEFAULT_CAMERA_MOUNT;
	seg_camera_config_.Orientation         = FRotator(0, 0, 0);
	seg_camera_config_.FOVAngle            = 90;
	seg_camera_config_.Width               = 640;
	seg_camera_config_.Height              = 480;

	SetSegIdCameraConfig(seg_camera_config_);
	SetCameraCaptureMode(Owner->GetCameraCaptureMode());

	// make sure the extension exists before the first capture
	FSegIdViewExtension::Get();
}

/* //} */

/* TickComponent() //{ */

void USegIdCamera::DrainInstructions()
{
	std::shared_ptr<FInstruction<USegIdCamera>> Instruction;
	while (InstructionQueue->Dequeue(Instruction))
	{
		Instruction->Function(*this);
		Instruction->Finished = true;
	}
}

void USegIdCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	const double Start = FPlatformTime::Seconds();

	DrainInstructions();

	// every frame is a job submission, so bCaptureEveryFrame alone cannot drive this
	// camera - keep capturing while requests are fresh
	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES && (Start - camera_last_request_time_) < 1.0)
	{
		if (ShouldCapture() && CaptureDue())
		{
			UpdateCamera(true, Start);
		}
	}

	PumpReadback();

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/* //} */

/* SetSegIdCameraConfig() //{ */

bool USegIdCamera::SetSegIdCameraConfig(const FRgbCameraConfig& Config)
{
	if (!ValidDimensions(Config.Width, Config.Height))
	{
		return false;
	}

	seg_camera_config_ = Config;
	CaptureRate    = Config.CaptureRate;
	NextCaptureDue = -1.0;

	CameraBufferCriticalSection->Lock();

	SceneCaptureMeshHolder->SetVisibility(Config.ShowCameraComponent);
	SceneCaptureMeshHolder->SetRelativeLocation(Config.Offset);
	SceneCaptureMeshHolder->SetRelativeRotation(FRotator(0, 0, 0));
	SceneCaptureMeshHolder->SetRelativeRotation(Config.Orientation);

	// same projection/overscan machinery as the RGB camera, so a distorted RGB
	// calibration can be mirrored exactly (the serve path warps nearest-neighbor)
	SetupCaptureProjection(Config.Intrinsics, Config.Distortion, Config.FOVAngle, Config.Width, Config.Height, {SceneCaptureComponent2D}, {RenderTarget2D});

	CameraBufferCriticalSection->Unlock();

	return true;
}

/* //} */

/* UpdateCamera() //{ */

void USegIdCamera::UpdateCamera(bool isExternallyLocked, double stamp)
{
	if (RingFull())
	{
		// if the pass never consumed the jobs (hook did not fire), do not stay stuck
		if (stamp - LastSubmitTime < 5.0)
		{
			if (!TakeRingSlot())
			{
				return;
			}
		}
		else
		{
			ClearRing();
		}
	}

	FTextureRenderTargetResource* Resource = RenderTarget2D ? RenderTarget2D->GameThread_GetRenderTargetResource() : nullptr;
	UWorld*                       World    = GetWorld();

	if (!Resource || !World)
	{
		return;
	}

	USegmentationTagger* Tagger = World->GetSubsystem<USegmentationTagger>();
	if (!Tagger)
	{
		return;
	}

	TArray<FSegIdDrawItem>    Items;
	TArray<FSegInstanceEntry> Map;
	TArray<FLinearColor>      StencilSlots;
	Tagger->CollectInstanceItems(Items, Map, StencilSlots);

	CameraBufferCriticalSection->Lock();
	InstanceMap = MoveTemp(Map);
	CameraBufferCriticalSection->Unlock();

	// the pass renders at the (possibly overscan-padded) render target size; the
	// serve path warps back to the nominal resolution through the distortion LUT
	TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Slot = MakeShared<FCameraReadback, ESPMode::ThreadSafe>();
	Slot->Size                                            = FIntPoint(RenderTarget2D->SizeX, RenderTarget2D->SizeY);
	Slot->Stamp                                           = stamp;
	Slot->Readback                                        = MakeUnique<FRHIGPUTextureReadback>(TEXT("FlightForgeSegIdReadback"));

	FSegIdViewExtension::Get()->SubmitJob_GameThread(Resource, Slot->Size, MoveTemp(Items), MoveTemp(StencilSlots), Slot);

	if (bEverRendered)
	{
		SceneCaptureComponent2D->CaptureSceneDeferred();
	}
	else
	{
		SceneCaptureComponent2D->CaptureScene();
	}

	PushReadback(Slot);
	LastSubmitTime = stamp;
}

/* //} */

/* GetInstanceSegFromServerThread() //{ */

bool USegIdCamera::GetInstanceSegFromServerThread(TArray<uint8>& OutArray, double& stamp)
{
	camera_last_request_time_ = FPlatformTime::Seconds();

	CameraBufferCriticalSection->Lock();
	const bool bHaveFrame = CameraRendered;
	CameraBufferCriticalSection->Unlock();

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND || (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES && !bHaveFrame))
	{
		const uint64 Baseline = FrameCounter.load();

		auto Instruction      = std::make_shared<FInstruction<USegIdCamera>>();
		Instruction->Function = [](USegIdCamera& _Camera) { _Camera.UpdateCamera(true, FPlatformTime::Seconds()); };
		InstructionQueue->Enqueue(Instruction);

		if (!WaitForRequestedFrame(Instruction->Finished, Baseline, 2.0))
		{
			UE_LOG(LogTemp, Warning, TEXT("USegIdCamera: timed out waiting for a frame"));
		}
	}

	TArray<FColor>                                        RawCopy;
	FIntPoint                                             RawSize;
	TSharedPtr<const FDistortionLUT, ESPMode::ThreadSafe> LUT;
	{
		CameraBufferCriticalSection->Lock();

		if (!CameraRendered)
		{
			CameraBufferCriticalSection->Unlock();
			UE_LOG(LogTemp, Warning, TEXT("USegIdCamera: camera is not rendered, returning"));
			return false;
		}

		RawCopy = CameraBuffer;
		RawSize = CameraBufferSize;
		stamp   = stamp_;
		LUT     = DistortionLUT;

		CameraBufferCriticalSection->Unlock();
	}

	// nearest-neighbor: ids must never blend
	if (LUT && RawSize == LUT->SourceSize)
	{
		TArray<FColor> Warped;
		ResampleImage(*LUT, RawCopy, Warped, true);
		RawCopy = MoveTemp(Warped);
		RawSize = LUT->OutputSize;
	}

	// lossless always: these pixels are 24-bit instance ids, not colour
	CompressFrame(RawCopy, RawSize, FCameraCompression{}, true, OutArray);

	return true;
}

/* //} */

/* CopyLastFrame() //{ */

bool USegIdCamera::CopyLastFrame(TArray<FColor>& OutIds, FIntPoint& OutSize, TArray<FSegInstanceEntry>& OutMap)
{
	CameraBufferCriticalSection->Lock();
	const bool bHave = CameraRendered;
	if (bHave)
	{
		OutIds  = CameraBuffer;
		OutSize = CameraBufferSize;
		OutMap  = InstanceMap;
	}
	CameraBufferCriticalSection->Unlock();
	return bHave;
}

//}

/* GetData() / GetMapData() //{ */

void USegIdCamera::GetData(std::stringstream& OutputStream)
{
	TArray<uint8> CompressedBitmap;
	double        stamp;

	const bool res = GetInstanceSegFromServerThread(CompressedBitmap, stamp);

	if (!res)
	{
		Serializable::Drone::GetInstanceSegData::Response Response(false);
		Serialization::DeserializeResponse(Response, OutputStream);
		return;
	}

	Serializable::Drone::GetInstanceSegData::Response Response(true);
	Response.image_.resize(CompressedBitmap.Num());
	for (int i = 0; i < CompressedBitmap.Num(); i++)
	{
		Response.image_[i] = CompressedBitmap[i];
	}
	Response.stamp_ = stamp;
	Response.size_  = CompressedBitmap.Num();

	Serialization::DeserializeResponse(Response, OutputStream);
}

void USegIdCamera::GetMapData(std::stringstream& OutputStream)
{
	TArray<FSegInstanceEntry> MapCopy;

	CameraBufferCriticalSection->Lock();
	MapCopy = InstanceMap;
	CameraBufferCriticalSection->Unlock();

	Serializable::Drone::GetInstanceSegMap::Response Response(true);

	for (const FSegInstanceEntry& Entry : MapCopy)
	{
		Serializable::Drone::InstanceSegMapEntry Out;
		Out.id       = Entry.Id;
		Out.actor    = TCHAR_TO_UTF8(*Entry.Actor);
		Out.class_id = Entry.ClassId;
		Response.entries_.push_back(Out);
	}

	Serialization::DeserializeResponse(Response, OutputStream);
}

/* //} */

/* GetConfig() / SetConfig() //{ */

void USegIdCamera::GetConfig(std::stringstream& OutputStream)
{
	Serializable::Drone::GetRgbCameraConfig::Response Response(true);
	Response.config = Serializable::Drone::RgbCameraConfig{};

	Response.config.show_debug_camera_ = seg_camera_config_.ShowCameraComponent;

	Response.config.offset_x_ = seg_camera_config_.Offset.X;
	Response.config.offset_y_ = seg_camera_config_.Offset.Y;
	Response.config.offset_z_ = seg_camera_config_.Offset.Z;

	Response.config.rotation_pitch_ = seg_camera_config_.Orientation.Pitch;
	Response.config.rotation_yaw_   = seg_camera_config_.Orientation.Yaw;
	Response.config.rotation_roll_  = seg_camera_config_.Orientation.Roll;

	Response.config.fov_ = seg_camera_config_.FOVAngle;

	Response.config.width_  = seg_camera_config_.Width;
	Response.config.capture_rate_ = seg_camera_config_.CaptureRate;
	Response.config.height_ = seg_camera_config_.Height;

	const FCameraIntrinsics Effective =
	    EffectiveIntrinsics(seg_camera_config_.Intrinsics, seg_camera_config_.FOVAngle, seg_camera_config_.Width, seg_camera_config_.Height);
	Response.config.intrinsics_.use_custom_ = seg_camera_config_.Intrinsics.UseCustom;
	Response.config.intrinsics_.fx_         = Effective.Fx;
	Response.config.intrinsics_.fy_         = Effective.Fy;
	Response.config.intrinsics_.cx_         = Effective.Cx - WirePixelShift;
	Response.config.intrinsics_.cy_         = Effective.Cy - WirePixelShift;

	Response.config.distortion_.enable_ = seg_camera_config_.Distortion.Enable;
	Response.config.distortion_.k1_     = seg_camera_config_.Distortion.K1;
	Response.config.distortion_.k2_     = seg_camera_config_.Distortion.K2;
	Response.config.distortion_.k3_     = seg_camera_config_.Distortion.K3;
	Response.config.distortion_.p1_     = seg_camera_config_.Distortion.P1;
	Response.config.distortion_.p2_     = seg_camera_config_.Distortion.P2;

	Serialization::DeserializeResponse(Response, OutputStream);
}

void USegIdCamera::SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream)
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

	Config.Distortion.Enable = Request.config.distortion_.enable_;
	Config.Distortion.K1     = Request.config.distortion_.k1_;
	Config.Distortion.K2     = Request.config.distortion_.k2_;
	Config.Distortion.K3     = Request.config.distortion_.k3_;
	Config.Distortion.P1     = Request.config.distortion_.p1_;
	Config.Distortion.P2     = Request.config.distortion_.p2_;

	bool Status = false;

	auto Instruction      = std::make_shared<FInstruction<USegIdCamera>>();
	Instruction->Function = [&Config, &Status](USegIdCamera& _Camera) { Status = _Camera.SetSegIdCameraConfig(Config); };
	InstructionQueue->Enqueue(Instruction);

	FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });

	Serializable::Drone::SetRgbCameraConfig::Response Response(Status);
	Serialization::DeserializeResponse(Response, OutputStream);
}

/* //} */
