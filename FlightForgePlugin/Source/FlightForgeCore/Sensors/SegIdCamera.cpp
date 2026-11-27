#include "SegIdCamera.h"

#include "TextureResource.h"

#include "FlightForgeCore/RobotPawn.h"
#include "SegIdPass.h"
#include "FlightForgeCore/Segmentation/SegmentationTagger.h"
#include "SensorWire.h"

static TAutoConsoleVariable<int32> CVarSegIdLeanCapture(TEXT("FlightForge.SegId.LeanCapture"), 1,
                                                        TEXT("1: the instance camera's capture skips lighting, shadows, sky and post effects, which its id image never shows"));

USegIdCamera::USegIdCamera() : UCamera()
{
	SensorType = SensorType::INSTANCE_SEG_CAMERA;
	Name       = TEXT("INSTANCE_SEG_CAMERA");

	InstructionQueue = std::make_unique<TInstructionQueue<USegIdCamera>>();
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

	BaseShowFlags = SceneCaptureComponent2D->ShowFlags;
	ApplyLeanCapture();

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
		Instruction->Run(*this);
	}
}

void USegIdCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	DrainInstructions();

	// every frame is a job submission, so bCaptureEveryFrame alone cannot drive this
	// camera - keep capturing while requests are fresh
	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES && StreamWanted())
	{
		if (ShouldCapture() && CaptureDue())
		{
			UpdateCamera(true, SimNow());
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
		if (FPlatformTime::Seconds() - LastSubmitWall < 5.0)
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

	ApplyLeanCapture();

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
	LastSubmitWall = FPlatformTime::Seconds();
}

/* //} */

/* ApplyLeanCapture() //{ */

// depth, visibility and the post-process hook stay as they were
void USegIdCamera::ApplyLeanCapture()
{
	const int32 Lean = CVarSegIdLeanCapture.GetValueOnGameThread() != 0;
	if (Lean == LeanApplied)
	{
		return;
	}
	LeanApplied = Lean;

	FEngineShowFlags& Flags = SceneCaptureComponent2D->ShowFlags;
	Flags                   = BaseShowFlags;
	if (!Lean)
	{
		return;
	}
	Flags.SetLighting(false);
	Flags.SetDynamicShadows(false);
	Flags.SetLightFunctions(false);
	Flags.SetGlobalIllumination(false);
	Flags.SetLumenGlobalIllumination(false);
	Flags.SetLumenReflections(false);
	Flags.SetReflectionEnvironment(false);
	Flags.SetScreenSpaceReflections(false);
	Flags.SetAmbientOcclusion(false);
	Flags.SetDistanceFieldAO(false);
	Flags.SetContactShadows(false);
	Flags.SetSkyLighting(false);
	Flags.SetAtmosphere(false);
	Flags.SetFog(false);
	Flags.SetVolumetricFog(false);
	Flags.SetCloud(false);
	Flags.SetBloom(false);
	Flags.SetEyeAdaptation(false);
	Flags.SetLensFlares(false);
	Flags.SetUnlitRichView(false);
	Flags.SetUnlitViewmode(false);
}

//}

/* GetInstanceSegFromServerThread() //{ */

bool USegIdCamera::GetInstanceSegFromServerThread(FCameraFrame& OutFrame)
{
	camera_last_request_time_ = FPlatformTime::Seconds();

	CameraBufferCriticalSection->Lock();
	const bool bHaveFrame = CameraRendered;
	CameraBufferCriticalSection->Unlock();

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND || (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES && !bHaveFrame))
	{
		const uint64 Baseline = FrameCounter.load();

		auto Instruction      = std::make_shared<FInstruction<USegIdCamera>>();
		Instruction->Function = [](USegIdCamera& _Camera) { _Camera.UpdateCamera(true, _Camera.SimNow()); };
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

		RawCopy        = CameraBuffer;
		RawSize        = CameraBufferSize;
		OutFrame.Stamp = stamp_;
		LUT            = DistortionLUT;

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
	CompressFrame(RawCopy, RawSize, FCameraCompression{}, true, OutFrame);

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

/* wire //{ */

void USegIdCamera::GetData(const flightforge::wire::Request& Request, FWireReply& Reply)
{
	if (Request.body_type() == flightforge::wire::RequestBody::GetInstanceSegMapRequest)
	{
		GetMapData(Reply);
		return;
	}

	FCameraFrame Frame;
	if (!GetInstanceSegFromServerThread(Frame))
	{
		Reply.Fail(TEXT("the instance camera has no frame yet"));
		return;
	}

	const auto Image = FSensorWire::Image(Reply, Frame, flightforge::wire::PixelFormat::RGBA8);
	Reply.Set(flightforge::wire::CreateInstanceSegDataResponse(Reply.Builder, Frame.Stamp, Image));
}

void USegIdCamera::GetMapData(FWireReply& Reply)
{
	TArray<FSegInstanceEntry> MapCopy;

	CameraBufferCriticalSection->Lock();
	MapCopy = InstanceMap;
	CameraBufferCriticalSection->Unlock();

	std::vector<flatbuffers::Offset<flightforge::wire::InstanceSegEntry>> Entries;
	Entries.reserve(MapCopy.Num());
	for (const FSegInstanceEntry& Entry : MapCopy)
	{
		Entries.push_back(flightforge::wire::CreateInstanceSegEntry(Reply.Builder, Entry.Id, Reply.String(Entry.Actor), Entry.ClassId));
	}

	Reply.Set(flightforge::wire::CreateInstanceSegMapResponse(Reply.Builder, Reply.Builder.CreateVector(Entries)));
}

void USegIdCamera::GetConfig(FWireReply& Reply)
{
	FSensorWire::ReplyConfig(seg_camera_config_, Reply);
}

void USegIdCamera::SetConfig(const flightforge::wire::Request& Request, FWireReply& Reply)
{
	FRgbCameraConfig Config;
	if (!FSensorWire::FromWire(Request, Config, Reply))
	{
		return;
	}

	bool bApplied = false;
	RunOnSensorQueue(*this, [&Config, &bApplied](USegIdCamera& Camera) { bApplied = Camera.SetSegIdCameraConfig(Config); });

	if (!bApplied)
	{
		Reply.Fail(TEXT("the instance camera rejected the config"));
	}
}

void USegIdCamera::SetMount(const FVector& OffsetCm, const FRotator& Rotation)
{
	RunOnSensorQueue(*this, [&](USegIdCamera& Camera) {
		FRgbCameraConfig Config = Camera.seg_camera_config_;
		Config.Offset           = OffsetCm;
		Config.Orientation      = Rotation;
		Camera.SetSegIdCameraConfig(Config);
	});
}

/* //} */
