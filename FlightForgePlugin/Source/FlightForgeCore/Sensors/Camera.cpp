#include "Camera.h"

#include "ContentStreaming.h"
#include "Engine/PostProcessVolume.h"
#include "EngineUtils.h"
#include "UnrealEngine.h"
#include "RenderingThread.h"
#include "TextureResource.h"
#include "FlightForgeCore/DronePawn.h"

#include "Serialize.h"

UCamera::UCamera()
{
	PrimaryComponentTick.bCanEverTick = true;
	//PrimaryComponentTick.TickGroup    = TG_PrePhysics;

	CameraBufferCriticalSection = std::make_unique<FCriticalSection>();

	FrameReadyEvent = FPlatformProcess::GetSynchEventFromPool(false);

	SceneCaptureMeshHolder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SceneCaptureMeshHolder"));
	SceneCaptureMeshHolder->SetupAttachment(this);

	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2D"));
	SceneCaptureComponent2D->SetupAttachment(SceneCaptureMeshHolder);
}


void UCamera::BeginPlay()
{
	Super::BeginPlay();

	//Tick should be called after DronePawn empties InstructionQueue
	PrimaryComponentTick.AddPrerequisite(Owner, Owner->PrimaryActorTick);

	// TODO check https://github.com/TimmHess/UnrealImageCapture
}

void UCamera::Initialize(int InSensorID)
{
	Super::Initialize(InSensorID);
	
	
	SceneCaptureMeshHolder->SetMobility(EComponentMobility::Movable);
	if (!SceneCaptureMeshHolder->IsRegistered()) 
		SceneCaptureMeshHolder->RegisterComponent();
	
	
	SceneCaptureComponent2D->SetMobility(EComponentMobility::Movable);
	if (!SceneCaptureComponent2D->IsRegistered())
		SceneCaptureComponent2D->RegisterComponent();
}

void UCamera::BeginDestroy()
{
	if (CameraBufferCriticalSection)
	{
		CameraBufferCriticalSection->Lock();
		CameraRendered = false;
		CameraBuffer.Empty();
		CameraBufferCriticalSection->Unlock();
	}

	// wake any server thread still waiting on a frame; it will see CameraRendered == false
	if (FrameReadyEvent)
	{
		FrameCounter++;
		FrameReadyEvent->Trigger();
		FPlatformProcess::ReturnSynchEventToPool(FrameReadyEvent);
		FrameReadyEvent = nullptr;
	}

	Super::BeginDestroy();
}

/* async readback machinery //{ */

TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> UCamera::EnqueueReadback(UTextureRenderTarget2D* RT, double Stamp)
{
	FTextureRenderTargetResource* Resource = RT ? RT->GameThread_GetRenderTargetResource() : nullptr;

	if (!Resource)
	{
		return nullptr;
	}

	TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Slot = MakeShared<FCameraReadback, ESPMode::ThreadSafe>();
	Slot->Size     = FIntPoint(RT->SizeX, RT->SizeY);
	Slot->Stamp    = Stamp;
	Slot->Readback = MakeUnique<FRHIGPUTextureReadback>(TEXT("FlightForgeCameraReadback"));

	ENQUEUE_RENDER_COMMAND(FlightForgeEnqueueReadback)(
		[Slot, Resource](FRHICommandListImmediate& RHICmdList)
		{
			if (FRHITexture* Texture = Resource->GetRenderTargetTexture())
			{
				Slot->Readback->EnqueueCopy(RHICmdList, Texture, FResolveRect());
			}
			else
			{
				// nothing to read; complete with empty pixels so the poller does not hang
				Slot->bCopied = true;
			}
		});

	return Slot;
}

void UCamera::IssueResolve(const TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>& Slot)
{
	ENQUEUE_RENDER_COMMAND(FlightForgeResolveReadback)(
		[Slot](FRHICommandListImmediate& RHICmdList)
		{
			void* Data             = nullptr;
			int32 RowPitchInPixels = 0;
			Slot->Readback->LockTexture(RHICmdList, Data, RowPitchInPixels);

			if (Data)
			{
				const int32 W = Slot->Size.X;
				const int32 H = Slot->Size.Y;
				Slot->Pixels.SetNumUninitialized(W * H);

				const FColor* Src = static_cast<const FColor*>(Data);

				if (RowPitchInPixels == W)
				{
					FMemory::Memcpy(Slot->Pixels.GetData(), Src, (int64)W * H * sizeof(FColor));
				}
				else
				{
					for (int32 Row = 0; Row < H; ++Row)
					{
						FMemory::Memcpy(Slot->Pixels.GetData() + (int64)Row * W, Src + (int64)Row * RowPitchInPixels, W * sizeof(FColor));
					}
				}
			}

			Slot->Readback->Unlock();
			Slot->bCopied = true;
		});
}

bool UCamera::PumpReadback()
{
	if (!PendingReadback)
	{
		return false;
	}

	if (PendingReadback->bCopied)
	{
		CameraBufferCriticalSection->Lock();
		CameraBuffer     = MoveTemp(PendingReadback->Pixels);
		CameraBufferSize = PendingReadback->Size;
		stamp_           = PendingReadback->Stamp;
		CameraRendered   = CameraBuffer.Num() > 0;
		CameraBufferCriticalSection->Unlock();

		PendingReadback.Reset();
		NotifyFrameReady();
		return true;
	}

	if (PendingReadback->Readback && PendingReadback->Readback->IsReady() && !PendingReadback->bResolveIssued.exchange(true))
	{
		IssueResolve(PendingReadback);
	}

	return false;
}

void UCamera::NotifyFrameReady()
{
	FrameCounter++;

	if (FrameReadyEvent)
	{
		FrameReadyEvent->Trigger();
	}
}

bool UCamera::WaitForNewFrame(uint64 BaselineFrame, double TimeoutSeconds)
{
	const double Deadline = FPlatformTime::Seconds() + TimeoutSeconds;

	while (FrameCounter.load() == BaselineFrame)
	{
		const double Remaining = Deadline - FPlatformTime::Seconds();

		if (Remaining <= 0.0 || !FrameReadyEvent)
		{
			return false;
		}

		FrameReadyEvent->Wait(FTimespan::FromSeconds(FMath::Min(Remaining, 0.1)));
	}

	return true;
}

//}

void UCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	PushStreamingViewHint();

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/* streaming + post-process baseline //{ */

void UCamera::PushStreamingViewHint()
{
	if (!SceneCaptureComponent2D || !RenderTarget2D)
	{
		return;
	}

	const bool bActive = SceneCaptureComponent2D->bCaptureEveryFrame || (FPlatformTime::Seconds() - camera_last_request_time_) < 2.0;

	if (!bActive)
	{
		return;
	}

	const float ScreenSize    = (float)RenderTarget2D->SizeX;
	const float HalfFovRad    = FMath::DegreesToRadians(FMath::Clamp(SceneCaptureComponent2D->FOVAngle, 1.0f, 170.0f) * 0.5f);
	const float FOVScreenSize = ScreenSize / FMath::Tan(HalfFovRad);

	IStreamingManager::Get().AddViewInformation(SceneCaptureComponent2D->GetComponentLocation(), ScreenSize, FOVScreenSize);
}

void UCamera::ApplyLevelPostProcessBaseline(USceneCaptureComponent2D* Capture)
{
	UWorld* World = GetWorld();

	if (!Capture || !World)
	{
		return;
	}

	for (TActorIterator<APostProcessVolume> It(World); It; ++It)
	{
		if (It->bEnabled && It->bUnbound)
		{
			Capture->PostProcessSettings    = It->Settings;
			Capture->PostProcessBlendWeight = It->BlendWeight;
			return;
		}
	}
}

//}

/* intrinsics projection //{ */

FMatrix UCamera::IntrinsicsProjectionMatrix(const FCameraIntrinsics& Intrinsics, const int32 Width, const int32 Height)
{
	// Reversed-Z with an infinite far plane, matching FReversedZPerspectiveMatrix; in
	// the centered case (cx = W/2, cy = H/2, fx per the FOV) the two are identical.
	// The off-axis terms shift NDC so that pixel (u,v) = (fx·X/Z + cx, fy·(-Y)/Z + cy)
	// with the image origin top-left and view-space Y up.
	return FMatrix(
	    FPlane(2.0 * Intrinsics.Fx / Width, 0.0, 0.0, 0.0),
	    FPlane(0.0, 2.0 * Intrinsics.Fy / Height, 0.0, 0.0),
	    FPlane(2.0 * Intrinsics.Cx / Width - 1.0, 1.0 - 2.0 * Intrinsics.Cy / Height, 0.0, 1.0),
	    FPlane(0.0, 0.0, GNearClippingPlane, 0.0));
}

FCameraIntrinsics UCamera::EffectiveIntrinsics(const FCameraIntrinsics& Intrinsics, const double FovDeg, const int32 Width, const int32 Height)
{
	if (Intrinsics.UseCustom && Intrinsics.Fx > 0.0 && Intrinsics.Fy > 0.0)
	{
		return Intrinsics;
	}

	FCameraIntrinsics Effective;
	Effective.Fx = (Width * 0.5) / FMath::Tan(FMath::DegreesToRadians(FovDeg) * 0.5);
	Effective.Fy = Effective.Fx;
	Effective.Cx = Width * 0.5;
	Effective.Cy = Height * 0.5;

	return Effective;
}

void UCamera::ApplyProjection(USceneCaptureComponent2D* Capture, const FCameraIntrinsics& Intrinsics, const double FovDeg, const int32 Width,
                              const int32 Height)
{
	// FOVAngle stays meaningful either way: it drives culling bounds and is the
	// fallback when the custom matrix is disabled again.
	Capture->FOVAngle = FovDeg;

	const bool bValid = Intrinsics.UseCustom && Intrinsics.Fx > 0.0 && Intrinsics.Fy > 0.0 && Width > 0 && Height > 0;

	Capture->bUseCustomProjectionMatrix = bValid;

	if (bValid)
	{
		Capture->CustomProjectionMatrix = IntrinsicsProjectionMatrix(Intrinsics, Width, Height);
	}
}

//}


void UCamera::SetCameraCaptureMode(CameraCaptureModeEnum CaptureMode) {

	CameraCaptureMode = CaptureMode;

	// if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES || CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_MOVEMENT) {
	// 	//UE_LOG(LogTemp, Log, TEXT("Camera updating after capture mode change"));
	//     UpdateCamera(false, 0.0); //used to update only in rgb mode
	// }
}

// packs BGRA pixels into the 3-byte-per-pixel wire layout
void UCamera::TransformImageArray(int32 ImageWidth, int32 ImageHeight, const TArray<FColor>& SrcData, TArray<uint8>& DstData) {
	const int32 NumPixels = ImageWidth * ImageHeight;

	DstData.SetNumUninitialized(NumPixels * 3);

	const FColor* SrcDataPtr = SrcData.GetData();
	uint8*        DstDataPtr = DstData.GetData();

	for (int32 Index = 0; Index < NumPixels; Index++) {
		DstDataPtr[Index * 3 + 0] = SrcDataPtr[Index].B;
		DstDataPtr[Index * 3 + 1] = SrcDataPtr[Index].G;
		DstDataPtr[Index * 3 + 2] = SrcDataPtr[Index].R;
	}
}

void UCamera::UpdateCamera(bool isExternallyLocked, double stamp = 0.0) {}

void UCamera::GetConfig(std::stringstream& OutputStream) {}

void UCamera::SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream) {}

void UCamera::GetData(std::stringstream& OutputStream) {}

