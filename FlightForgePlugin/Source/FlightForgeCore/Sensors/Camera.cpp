#include "Camera.h"
#include "ReadbackPass.h"

#include "ContentStreaming.h"
#include "Engine/PostProcessVolume.h"
#include "ImageUtils.h"
#include "EngineUtils.h"
#include "UnrealEngine.h"
#include "RenderingThread.h"
#include "SceneInterface.h"
#include "TextureResource.h"
#include "FlightForgeCore/RobotPawn.h"
#include "FlightForgeCore/GpuWatchdog.h"
#include "FlightForgeCore/SimClock.h"

#include "FlightForgeCore/Utils/WireFrame.h"
#include "MultiViewCapture.h"

UCamera::UCamera()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;  // lockstep parks the world paused; config requests still land
	//PrimaryComponentTick.TickGroup    = TG_PrePhysics;

	CameraBufferCriticalSection = std::make_unique<FCriticalSection>();

	FrameReadyEvent = FPlatformProcess::GetSynchEventFromPool(false);

	SceneCaptureMeshHolder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SceneCaptureMeshHolder"));
	SceneCaptureMeshHolder->SetupAttachment(this);

	SceneCaptureComponent2D = CreateDefaultSubobject<ULookCapture>(TEXT("SceneCaptureComponent2D"));
	SceneCaptureComponent2D->SetupAttachment(SceneCaptureMeshHolder);
}


void UCamera::BeginPlay()
{
	Super::BeginPlay();

	PrimaryComponentTick.AddPrerequisite(Owner, Owner->PrimaryActorTick);

	FReadbackViewExtension::Get();  // registered on the game thread once, before any server thread asks for it
}

void UCamera::Initialize(int InSensorID)
{
	Super::Initialize(InSensorID);
	
	
	SceneCaptureMeshHolder->SetMobility(EComponentMobility::Movable);
	if (!SceneCaptureMeshHolder->IsRegistered())
		SceneCaptureMeshHolder->RegisterComponent();

	// The holder never had a mesh, so show_debug_camera toggled the visibility of
	// nothing. A small body at the mount makes a rig checkable by eye and lets other
	// sensors see the camera the way they would see real hardware.
	if (!SceneCaptureMeshHolder->GetStaticMesh())
	{
		if (UStaticMesh* Cube = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube")))
		{
			SceneCaptureMeshHolder->SetStaticMesh(Cube);
			SceneCaptureMeshHolder->SetRelativeScale3D(FVector(0.06, 0.06, 0.06));
			SceneCaptureMeshHolder->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			SceneCaptureMeshHolder->SetCollisionObjectType(ECC_WorldStatic);
			SceneCaptureMeshHolder->SetCollisionResponseToAllChannels(ECR_Block);
			SceneCaptureMeshHolder->SetGenerateOverlapEvents(false);
			SceneCaptureMeshHolder->SetCastShadow(false);
			SceneCaptureMeshHolder->SetVisibility(false);
			SceneCaptureMeshHolder->SetHiddenInGame(true);
		}
	}
	
	
	SceneCaptureComponent2D->SetMobility(EComponentMobility::Movable);
	if (!SceneCaptureComponent2D->IsRegistered())
		SceneCaptureComponent2D->RegisterComponent();

	if (MultiView && !MultiView->IsRegistered())
	{
		MultiView->RegisterComponent();
	}
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

TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> UCamera::MakeReadbackSlot(UTextureRenderTarget2D* RT, double Stamp, const FVector& Omega, const bool bFloat16,
                                                                           const bool bEncodeSrgb, const bool bHalfRgba, const bool bLuma)
{
	TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Slot = MakeShared<FCameraReadback, ESPMode::ThreadSafe>();
	Slot->Size        = FIntPoint(RT->SizeX, RT->SizeY);
	Slot->Stamp       = Stamp;
	Slot->Omega       = Omega;
	const EPixelFormat Format = RT->GetFormat();

	Slot->bFloat16    = bFloat16;
	Slot->bLuma       = bLuma && (Format == PF_FloatRGBA || Format == PF_A32B32G32R32F);
	Slot->bLumaFloat32 = Slot->bLuma && Format == PF_A32B32G32R32F;
	Slot->bHalfRgba   = !Slot->bLuma && bHalfRgba && Format == PF_FloatRGBA;
	Slot->bFloatRgba  = !Slot->bLuma && bHalfRgba && Format == PF_A32B32G32R32F;
	Slot->bEncodeSrgb = bEncodeSrgb && !Slot->bHalfRgba && !Slot->bFloatRgba && !Slot->bLuma;
	Slot->Readback = MakeUnique<FRHIGPUTextureReadback>(TEXT("FlightForgeCameraReadback"));
	return Slot;
}

TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> UCamera::EnqueueReadback(UTextureRenderTarget2D* RT, double Stamp, const FVector& Omega, const bool bFloat16,
                                                                          const bool bEncodeSrgb, const bool bHalfRgba, const bool bLuma)
{
	FTextureRenderTargetResource* Resource = RT ? RT->GameThread_GetRenderTargetResource() : nullptr;

	if (!Resource)
	{
		return nullptr;
	}

	TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Slot = MakeReadbackSlot(RT, Stamp, Omega, bFloat16, bEncodeSrgb, bHalfRgba, bLuma);

	ENQUEUE_RENDER_COMMAND(FlightForgeEnqueueReadback)(
		[Slot, Resource](FRHICommandListImmediate& RHICmdList)
		{
			if (FRHITexture* Texture = Resource->GetRenderTargetTexture())
			{
				Slot->Readback->EnqueueCopy(RHICmdList, Texture, FResolveRect());
				WatchReadback_RenderThread(Slot);
			}
			else
			{
				// nothing to read; complete with empty pixels so the poller does not hang
				Slot->bCopied = true;
				Slot->Copied->Trigger();
			}
		});

	return Slot;
}

// copies land in order, so only the head can publish
bool UCamera::PumpReadback()
{
	bool bPublished = false;

	while (true)
	{
		CameraBufferCriticalSection->Lock();

		if (InflightReadbacks.Num() == 0)
		{
			CameraBufferCriticalSection->Unlock();
			break;
		}

		TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Head = InflightReadbacks[0];

		if (!Head->bCopied)
		{
			CameraBufferCriticalSection->Unlock();
			RequestResolve(Head);
			break;
		}

		CameraBuffer     = MoveTemp(Head->Pixels);
		CameraBufferF16  = MoveTemp(Head->Float16Pixels);
		CameraBufferSize = Head->Size;
		stamp_           = Head->Stamp;
		capture_omega_   = Head->Omega;
		CameraRendered   = CameraBuffer.Num() > 0 || CameraBufferF16.Num() > 0;
		bEverRendered |= CameraRendered;
		InflightReadbacks.RemoveAt(0);

		CameraBufferCriticalSection->Unlock();

		NotifyFrameReady();
		bPublished = true;
	}

	return bPublished;
}

void UCamera::PushReadback(const TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>& Slot)
{
	if (!Slot)
	{
		return;
	}
	CameraBufferCriticalSection->Lock();
	InflightReadbacks.Add(Slot);
	CameraBufferCriticalSection->Unlock();
}

bool UCamera::RingFull()
{
	CameraBufferCriticalSection->Lock();
	const bool bFull = InflightReadbacks.Num() >= MaxInflight;
	CameraBufferCriticalSection->Unlock();
	return bFull;
}

void UCamera::ClearRing()
{
	CameraBufferCriticalSection->Lock();
	InflightReadbacks.Reset();
	CameraBufferCriticalSection->Unlock();
}

TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> UCamera::NewestReadback()
{
	CameraBufferCriticalSection->Lock();
	TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Slot = InflightReadbacks.Num() > 0 ? InflightReadbacks.Last() : nullptr;
	CameraBufferCriticalSection->Unlock();
	return Slot;
}

bool UCamera::TakeRingSlot()
{
	if (!RingFull())
	{
		return true;
	}

	if (!FFlightForgeSimClock::bLockstep.load())
	{
		FramesFolded++;
		return false;
	}

	const double Deadline = FPlatformTime::Seconds() + 5.0;

	while (RingFull() && FPlatformTime::Seconds() < Deadline)
	{
		FGpuWatchdog::FlushRenderingCommands();
		if (!PumpFrames() && RingFull())
		{
			FPlatformProcess::SleepNoStats(0.0002f);
		}
	}

	return !RingFull();
}

bool UCamera::WaitForRequestedFrame(const std::atomic<bool>& InstructionFinished, const uint64 BaselineFrame, const double TimeoutSeconds)
{
	const double Deadline = FPlatformTime::Seconds() + TimeoutSeconds;

	while (!InstructionFinished)
	{
		if (FPlatformTime::Seconds() > Deadline)
		{
			return false;
		}
		FPlatformProcess::SleepNoStats(0.0001f);
	}

	// the copy is enqueued a tick after the capture, so the slot may not exist yet; an instruction that
	// produced no capture at all (a warm-up step) shows as no slot after a couple of frames
	TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Slot;
	const double                                     SlotDeadline = FMath::Min(Deadline, FPlatformTime::Seconds() + 0.1);
	while (true)
	{
		Slot = NewestReadback();
		if (Slot || FrameCounter.load() != BaselineFrame)
		{
			break;
		}
		if (FPlatformTime::Seconds() > SlotDeadline)
		{
			return false;
		}
		FPlatformProcess::SleepNoStats(0.0001f);
	}

	if (!Slot)
	{
		return true;
	}

	if (!WaitForSlot(Slot, Deadline))
	{
		return false;
	}

	// whichever thread gets here first publishes; the other finds nothing pending
	PumpReadback();
	return true;
}

bool UCamera::WaitForSlot(const TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>& Slot, const double Deadline)
{
	while (!Slot->bCopied)
	{
		if (FPlatformTime::Seconds() > Deadline)
		{
			return false;
		}
		RequestResolve(Slot);
		Slot->Copied->Wait(FTimespan::FromMilliseconds(1));
	}
	return true;
}

bool UCamera::RequestFrameFromServerThread(UTextureRenderTarget2D* RT, const FVector& Omega, const bool bFloat16, const bool bEncodeSrgb, const bool bHalfRgba,
                                           const double TimeoutSeconds)
{
	const double         Deadline = FPlatformTime::Seconds() + TimeoutSeconds;
	const FRenderTarget* Target   = CachedTargetResource.load();
	if (!Target || !SceneCaptureComponent2D)
	{
		return false;
	}

	TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Slot = NewestReadback();  // another request in flight shares its frame

	if (!Slot)
	{
		Slot = MakeReadbackSlot(RT, SimNow(), Omega, bFloat16, bEncodeSrgb, bHalfRgba, false);
		PushReadback(Slot);
		FReadbackViewExtension::Get()->SubmitJob(Target, Slot, SceneFrame());
		SceneCaptureComponent2D->CaptureSceneDeferred();
	}

	if (!WaitForSlot(Slot, Deadline))
	{
		return false;
	}
	PumpReadback();
	return true;
}

TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> UCamera::SubmitCaptureJob(UTextureRenderTarget2D* RT, const FVector& Omega, const bool bFloat16,
                                                                           const bool bEncodeSrgb, const bool bHalfRgba)
{
	const FTextureRenderTargetResource* Resource = RT ? RT->GameThread_GetRenderTargetResource() : nullptr;
	if (!Resource)
	{
		return nullptr;
	}

	TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Slot = MakeReadbackSlot(RT, SimNow(), Omega, bFloat16, bEncodeSrgb, bHalfRgba, false);
	FReadbackViewExtension::Get()->SubmitJob(Resource, Slot, SceneFrame());
	return Slot;
}

uint32 UCamera::SceneFrame() const
{
	const UWorld* World = GetWorld();
	return World && World->Scene ? World->Scene->GetFrameNumber() : 0;
}

double UCamera::SimNow() const
{
	const UWorld* World = GetWorld();
	return World ? World->GetTimeSeconds() : 0.0;
}

void UCamera::DrainReadback()
{
	// flushing only drains the render thread; the GPU fence still has to be polled
	const double Deadline = FPlatformTime::Seconds() + 5.0;

	const auto Pending = [this]() {
		CameraBufferCriticalSection->Lock();
		const bool bAny = InflightReadbacks.Num() > 0;
		CameraBufferCriticalSection->Unlock();
		return bAny;
	};

	while (Pending() && FPlatformTime::Seconds() < Deadline)
	{
		FGpuWatchdog::FlushRenderingCommands();
		if (!PumpReadback() && Pending())
		{
			FPlatformProcess::SleepNoStats(0.0002f);
		}
	}
}

bool UCamera::CaptureDue()
{
	if (CaptureRate <= 0.0)
	{
		return true;
	}

	const UWorld* World  = GetWorld();
	const double  Now    = World ? World->GetTimeSeconds() : 0.0;
	const double  Period = 1.0 / CaptureRate;

	// a stall produces one frame, not a burst
	if (NextCaptureDue < 0.0 || Now - NextCaptureDue > Period)
	{
		NextCaptureDue = Now;
	}

	if (Now + 1.0e-6 < NextCaptureDue)
	{
		return false;
	}

	NextCaptureDue += Period;
	return true;
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
	CachedTargetResource.store(RenderTarget2D ? RenderTarget2D->GameThread_GetRenderTargetResource() : nullptr);

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/* streaming + post-process baseline //{ */

void UCamera::PushStreamingViewHint()
{
	if (!SceneCaptureComponent2D || !RenderTarget2D)
	{
		return;
	}

	const bool bActive = SceneCaptureComponent2D->bCaptureEveryFrame || (MultiView && MultiView->CapturesEveryFrame()) || (FPlatformTime::Seconds() - camera_last_request_time_) < 2.0;

	if (!bActive)
	{
		return;
	}

	const float ScreenSize    = (float)RenderTarget2D->SizeX;
	const float HalfFovRad    = FMath::DegreesToRadians(FMath::Clamp(SceneCaptureComponent2D->FOVAngle, 1.0f, 170.0f) * 0.5f);
	const float FOVScreenSize = ScreenSize / FMath::Tan(HalfFovRad);

	IStreamingManager::Get().AddViewInformation(SceneCaptureComponent2D->GetComponentLocation(), ScreenSize, FOVScreenSize);
}

void UCamera::ApplyViewportLook(USceneCaptureComponent2D* Capture)
{ //{
	struct FGameFlags
	{
		FEngineShowFlags& Flags;
		FEngineShowFlags  Game{ESFIM_Game};
		bool OnEngineShowFlag(const uint32 Index, const FString&)
		{
			Flags.SetSingleFlag(Index, Game.GetSingleFlag(Index));
			return true;
		}
		bool OnCustomShowFlag(const uint32 Index, const FString& Name) { return OnEngineShowFlag(Index, Name); }
	} GameFlags{Capture->ShowFlags};
	FEngineShowFlags::IterateAllFlags(GameFlags);
	Capture->PostProcessSettings    = FPostProcessSettings();
	Capture->PostProcessBlendWeight = 1.0f;
	Capture->CaptureSource          = SCS_FinalColorLDR;
	Capture->bUseRayTracingIfEnabled = true;
	if (ULookCapture* Look = Cast<ULookCapture>(Capture))
	{
		Look->bViewportLook = true;
	}
	// made here, not inside the capture update that renders with its view state
	UMultiViewCapture::RobotBatch(Capture->GetOwner());
} //}

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

bool UCamera::ValidDimensions(const int32 Width, const int32 Height)
{
	constexpr int32 MaxSide = 16384;  // GMaxTextureDimensions on every RHI we ship
	if (Width > 0 && Height > 0 && Width <= MaxSide && Height <= MaxSide)
	{
		return true;
	}
	UE_LOG(LogTemp, Error, TEXT("FlightForge: rejecting camera config with %dx%d pixels"), Width, Height);
	return false;
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

/* PinholeInfo() //{ */

flightforge::wire::CameraInfoT UCamera::PinholeInfo(const int SensorId, const ::SensorType Type, const FCameraIntrinsics& Intrinsics,
                                                    const FCameraDistortion& Distortion, const double FovDeg, const int32 Width, const int32 Height,
                                                    const FVector& Offset, const FRotator& Orientation)
{
	const FCameraIntrinsics K = EffectiveIntrinsics(Intrinsics, FovDeg, Width, Height);

	flightforge::wire::CameraInfoT Info;
	Info.sensor_id        = SensorId;
	Info.sensor_type      = static_cast<flightforge::wire::SensorType>(Type);
	Info.width            = Width;
	Info.height           = Height;
	Info.projection       = flightforge::wire::Projection::PINHOLE;
	Info.distortion_model = "plumb_bob";
	Info.fx               = K.Fx;
	Info.fy               = K.Fy;
	Info.cx               = K.Cx - WirePixelShift;
	Info.cy               = K.Cy - WirePixelShift;
	Info.d = Distortion.Enable ? std::vector<double>{Distortion.K1, Distortion.K2, Distortion.P1, Distortion.P2, Distortion.K3} : std::vector<double>(5, 0.0);
	// custom K has no single FOV; report the horizontal one the effective fx implies
	Info.fov   = K.Fx > 0.0 ? FMath::RadiansToDegrees(2.0 * FMath::Atan(Width * 0.5 / K.Fx)) : FovDeg;
	Info.link  = "base_link";
	Info.mount = std::make_unique<flightforge::wire::Pose>(FWireFrame::ToMountPose(Offset, Orientation));
	return Info;
}

//}

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

/* distortion LUT //{ */

TSharedPtr<const FDistortionLUT, ESPMode::ThreadSafe> UCamera::BuildDistortionLUT(const FCameraDistortion& Distortion, const FCameraIntrinsics& EffectiveK,
                                                                                  const int32 Width, const int32 Height)
{
	if (Width <= 0 || Height <= 0 || EffectiveK.Fx <= 0.0 || EffectiveK.Fy <= 0.0)
	{
		return nullptr;
	}

	TSharedRef<FDistortionLUT, ESPMode::ThreadSafe> LUT = MakeShared<FDistortionLUT, ESPMode::ThreadSafe>();
	LUT->OutputSize                                     = FIntPoint(Width, Height);
	LUT->Source.SetNumUninitialized(Width * Height);

	// For each output pixel of the distorted image, invert the Brown-Conrady model
	// (fixed-point iteration, the standard cv::undistortPoints scheme) to find the
	// ideal-pinhole ray, i.e. where that pixel's content lives in the clean render.
	TArray<FVector2d> Pinhole;
	Pinhole.SetNumUninitialized(Width * Height);

	double MinX = TNumericLimits<double>::Max(), MaxX = -TNumericLimits<double>::Max();
	double MinY = TNumericLimits<double>::Max(), MaxY = -TNumericLimits<double>::Max();

	for (int32 V = 0; V < Height; ++V)
	{
		for (int32 U = 0; U < Width; ++U)
		{
			const double Xd = (U + 0.5 - EffectiveK.Cx) / EffectiveK.Fx;
			const double Yd = (V + 0.5 - EffectiveK.Cy) / EffectiveK.Fy;

			double Xu = Xd;
			double Yu = Yd;

			// The LUT is rebuilt only when the config changes, so run the fixed point to
			// convergence; a fixed count leaves the corners short under strong barrel terms.
			for (int32 Iteration = 0; Iteration < 64; ++Iteration)
			{
				const double R2     = Xu * Xu + Yu * Yu;
				const double Radial = 1.0 + R2 * (Distortion.K1 + R2 * (Distortion.K2 + R2 * Distortion.K3));
				const double Dx     = 2.0 * Distortion.P1 * Xu * Yu + Distortion.P2 * (R2 + 2.0 * Xu * Xu);
				const double Dy     = Distortion.P1 * (R2 + 2.0 * Yu * Yu) + 2.0 * Distortion.P2 * Xu * Yu;

				const double NextXu = (Xd - Dx) / Radial;
				const double NextYu = (Yd - Dy) / Radial;

				const bool bSettled = FMath::Abs(NextXu - Xu) < 1.0e-12 && FMath::Abs(NextYu - Yu) < 1.0e-12;

				Xu = NextXu;
				Yu = NextYu;

				if (bSettled)
				{
					break;
				}
			}

			const double Sx = Xu * EffectiveK.Fx + EffectiveK.Cx;
			const double Sy = Yu * EffectiveK.Fy + EffectiveK.Cy;

			MinX = FMath::Min(MinX, Sx);
			MaxX = FMath::Max(MaxX, Sx);
			MinY = FMath::Min(MinY, Sy);
			MaxY = FMath::Max(MaxY, Sy);

			Pinhole[V * Width + U] = FVector2d(Sx, Sy);
		}
	}

	// Overscan sized from the actual excursion outside the nominal image, +1px of
	// bilinear margin, capped so pathological coefficients cannot explode the target.
	const int32 PadX = FMath::Clamp(FMath::CeilToInt32(FMath::Max3(0.0, -MinX, MaxX - Width)) + 1, 0, Width / 2);
	const int32 PadY = FMath::Clamp(FMath::CeilToInt32(FMath::Max3(0.0, -MinY, MaxY - Height)) + 1, 0, Height / 2);

	LUT->Pad        = FIntPoint(PadX, PadY);
	LUT->SourceSize = FIntPoint(Width + 2 * PadX, Height + 2 * PadY);

	for (int32 Index = 0; Index < Pinhole.Num(); ++Index)
	{
		// pixel-center coordinates to array-index space: index = center - 0.5
		LUT->Source[Index] = FVector2f(Pinhole[Index].X + PadX - 0.5, Pinhole[Index].Y + PadY - 0.5);
	}

	return LUT;
}

void UCamera::ResampleImage(const FDistortionLUT& LUT, const TArray<FColor>& Source, TArray<FColor>& OutResampled, const bool bNearestNeighbor)
{
	const int32 SrcW = LUT.SourceSize.X;
	const int32 SrcH = LUT.SourceSize.Y;

	OutResampled.SetNumUninitialized(LUT.Source.Num());

	const FColor* Src = Source.GetData();
	FColor*       Dst = OutResampled.GetData();

	for (int32 Index = 0; Index < LUT.Source.Num(); ++Index)
	{
		const float X = LUT.Source[Index].X;
		const float Y = LUT.Source[Index].Y;

		if (bNearestNeighbor)
		{
			const int32 U = FMath::Clamp(FMath::RoundToInt32(X), 0, SrcW - 1);
			const int32 V = FMath::Clamp(FMath::RoundToInt32(Y), 0, SrcH - 1);

			Dst[Index] = Src[V * SrcW + U];
			continue;
		}

		const int32 U0 = FMath::Clamp(FMath::FloorToInt32(X), 0, SrcW - 1);
		const int32 V0 = FMath::Clamp(FMath::FloorToInt32(Y), 0, SrcH - 1);
		const int32 U1 = FMath::Min(U0 + 1, SrcW - 1);
		const int32 V1 = FMath::Min(V0 + 1, SrcH - 1);

		const float FracX = FMath::Clamp(X - U0, 0.0f, 1.0f);
		const float FracY = FMath::Clamp(Y - V0, 0.0f, 1.0f);

		const FColor& C00 = Src[V0 * SrcW + U0];
		const FColor& C10 = Src[V0 * SrcW + U1];
		const FColor& C01 = Src[V1 * SrcW + U0];
		const FColor& C11 = Src[V1 * SrcW + U1];

		const auto Lerp2 = [FracX, FracY](const uint8 A00, const uint8 A10, const uint8 A01, const uint8 A11) {
			const float Top    = A00 + (A10 - A00) * FracX;
			const float Bottom = A01 + (A11 - A01) * FracX;
			return static_cast<uint8>(FMath::RoundToInt32(Top + (Bottom - Top) * FracY));
		};

		Dst[Index] = FColor(Lerp2(C00.R, C10.R, C01.R, C11.R), Lerp2(C00.G, C10.G, C01.G, C11.G), Lerp2(C00.B, C10.B, C01.B, C11.B), 255);
	}
}

void UCamera::SetupCaptureProjection(const FCameraIntrinsics& Intrinsics, const FCameraDistortion& Distortion, const double FovDeg, const int32 Width,
                                     const int32 Height, std::initializer_list<USceneCaptureComponent2D*> Captures,
                                     std::initializer_list<UTextureRenderTarget2D*> Targets)
{
	if (Width <= 0 || Height <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid dimensions for RenderTarget2D. Width and Height should be greater than 0."));
		return;
	}

	if (!Distortion.Enable)
	{
		DistortionLUT.Reset();

		for (USceneCaptureComponent2D* Capture : Captures)
		{
			ApplyProjection(Capture, Intrinsics, FovDeg, Width, Height);
		}
		for (UTextureRenderTarget2D* Target : Targets)
		{
			Target->ResizeTarget(Width, Height);
		}

		return;
	}

	const FCameraIntrinsics EffectiveK = EffectiveIntrinsics(Intrinsics, FovDeg, Width, Height);

	TSharedPtr<const FDistortionLUT, ESPMode::ThreadSafe> LUT = BuildDistortionLUT(Distortion, EffectiveK, Width, Height);
	if (!LUT)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCamera: could not build the distortion LUT, rendering undistorted"));
		DistortionLUT.Reset();

		for (USceneCaptureComponent2D* Capture : Captures)
		{
			ApplyProjection(Capture, Intrinsics, FovDeg, Width, Height);
		}
		for (UTextureRenderTarget2D* Target : Targets)
		{
			Target->ResizeTarget(Width, Height);
		}

		return;
	}

	// The clean source render: same focal lengths, principal point shifted by the
	// padding, rendered at the padded size — the wider frustum the warp samples from.
	FCameraIntrinsics PaddedK = EffectiveK;
	PaddedK.UseCustom         = true;
	PaddedK.Cx += LUT->Pad.X;
	PaddedK.Cy += LUT->Pad.Y;

	for (USceneCaptureComponent2D* Capture : Captures)
	{
		ApplyProjection(Capture, PaddedK, FovDeg, LUT->SourceSize.X, LUT->SourceSize.Y);
	}
	for (UTextureRenderTarget2D* Target : Targets)
	{
		Target->ResizeTarget(LUT->SourceSize.X, LUT->SourceSize.Y);
	}

	DistortionLUT = LUT;
}

//}

/* WarpFrame() //{ */

namespace
{

FORCEINLINE FColor SampleBilinear(const FColor* Src, const int32 SrcW, const int32 SrcH, const float X, const float Y)
{
	const int32 U0 = FMath::Clamp(FMath::FloorToInt32(X), 0, SrcW - 1);
	const int32 V0 = FMath::Clamp(FMath::FloorToInt32(Y), 0, SrcH - 1);
	const int32 U1 = FMath::Min(U0 + 1, SrcW - 1);
	const int32 V1 = FMath::Min(V0 + 1, SrcH - 1);

	const float FracX = FMath::Clamp(X - U0, 0.0f, 1.0f);
	const float FracY = FMath::Clamp(Y - V0, 0.0f, 1.0f);

	const FColor& C00 = Src[V0 * SrcW + U0];
	const FColor& C10 = Src[V0 * SrcW + U1];
	const FColor& C01 = Src[V1 * SrcW + U0];
	const FColor& C11 = Src[V1 * SrcW + U1];

	const auto Lerp2 = [FracX, FracY](const uint8 A00, const uint8 A10, const uint8 A01, const uint8 A11) {
		const float Top    = A00 + (A10 - A00) * FracX;
		const float Bottom = A01 + (A11 - A01) * FracX;
		return static_cast<uint8>(FMath::RoundToInt32(Top + (Bottom - Top) * FracY));
	};

	return FColor(Lerp2(C00.R, C10.R, C01.R, C11.R), Lerp2(C00.G, C10.G, C01.G, C11.G), Lerp2(C00.B, C10.B, C01.B, C11.B), 255);
}

}  // namespace

void UCamera::WarpFrame(const TArray<FColor>& Source, const FIntPoint SourceSize, TArray<FColor>& OutFrame, const FIntPoint OutputSize,
                        const FDistortionLUT* LUT, const FCameraIntrinsics& EffectiveK, const FVector& OmegaCamera, const double ReadoutTime)
{
	const int32 Width  = OutputSize.X;
	const int32 Height = OutputSize.Y;

	OutFrame.SetNumUninitialized(Width * Height);

	const FColor* Src = Source.GetData();
	FColor*       Dst = OutFrame.GetData();

	const bool bRollingShutter = ReadoutTime > 0.0 && !OmegaCamera.IsNearlyZero();

	const double Wx = OmegaCamera.X;
	const double Wy = OmegaCamera.Y;
	const double Wz = OmegaCamera.Z;

	for (int32 V = 0; V < Height; ++V)
	{
		// row delay relative to the mid-frame render: top -readout/2, bottom +readout/2
		const double Tau = bRollingShutter ? ReadoutTime * ((V + 0.5) / Height - 0.5) : 0.0;

		for (int32 U = 0; U < Width; ++U)
		{
			const int32 Index = V * Width + U;

			float X = LUT ? LUT->Source[Index].X : static_cast<float>(U);
			float Y = LUT ? LUT->Source[Index].Y : static_cast<float>(V);

			if (bRollingShutter)
			{
				// rotational optical flow in normalized coords (x right, y down, z forward)
				const double Xn = (U + 0.5 - EffectiveK.Cx) / EffectiveK.Fx;
				const double Yn = (V + 0.5 - EffectiveK.Cy) / EffectiveK.Fy;

				const double FlowX = Xn * Yn * Wx - (1.0 + Xn * Xn) * Wy + Yn * Wz;
				const double FlowY = (1.0 + Yn * Yn) * Wx - Xn * Yn * Wy - Xn * Wz;

				X += static_cast<float>(EffectiveK.Fx * FlowX * Tau);
				Y += static_cast<float>(EffectiveK.Fy * FlowY * Tau);
			}

			Dst[Index] = SampleBilinear(Src, SourceSize.X, SourceSize.Y, X, Y);
		}
	}
}

/* //} */

/* ApplyNoise() //{ */

void UCamera::ApplyNoise(TArray<FColor>& Pixels, const FIntPoint Size, const FCameraNoise& Noise, const uint32 Seed)
{
	// linearization tables built once; sRGB in, sRGB out with a finer inverse table
	static const struct FTables
	{
		float  ToLinear[256];
		uint8  ToSrgb[2048];

		FTables()
		{
			for (int32 Index = 0; Index < 256; ++Index)
			{
				const float C   = Index / 255.0f;
				ToLinear[Index] = C <= 0.04045f ? C / 12.92f : FMath::Pow((C + 0.055f) / 1.055f, 2.4f);
			}
			for (int32 Index = 0; Index < 2048; ++Index)
			{
				const float L  = Index / 2047.0f;
				const float C  = L <= 0.0031308f ? L * 12.92f : 1.055f * FMath::Pow(L, 1.0f / 2.4f) - 0.055f;
				ToSrgb[Index]  = static_cast<uint8>(FMath::RoundToInt32(FMath::Clamp(C, 0.0f, 1.0f) * 255.0f));
			}
		}
	} Tables;

	FRandomStream Stream(static_cast<int32>(Seed));

	// Box-Muller pairs
	bool  bHaveSpare = false;
	float Spare      = 0.0f;
	const auto Gauss = [&Stream, &bHaveSpare, &Spare]() {
		if (bHaveSpare)
		{
			bHaveSpare = false;
			return Spare;
		}
		const float U1 = FMath::Max(Stream.FRand(), 1.0e-7f);
		const float U2 = Stream.FRand();
		const float R  = FMath::Sqrt(-2.0f * FMath::Loge(U1));
		Spare          = R * FMath::Sin(2.0f * PI * U2);
		bHaveSpare     = true;
		return R * FMath::Cos(2.0f * PI * U2);
	};

	const float ShotScale = Noise.ShotScale;
	const float ReadVar   = Noise.ReadSigma * Noise.ReadSigma;

	FColor* Data = Pixels.GetData();

	for (int32 V = 0; V < Size.Y; ++V)
	{
		const float RowOffset = Noise.RowSigma > 0.0 ? Gauss() * static_cast<float>(Noise.RowSigma) : 0.0f;

		for (int32 U = 0; U < Size.X; ++U)
		{
			FColor& Pixel = Data[V * Size.X + U];

			const auto Perturb = [&](const uint8 Channel) {
				float Linear      = Tables.ToLinear[Channel];
				const float Sigma = FMath::Sqrt(ShotScale * Linear + ReadVar);
				Linear            = FMath::Clamp(Linear + Gauss() * Sigma + RowOffset, 0.0f, 1.0f);
				return Tables.ToSrgb[static_cast<int32>(Linear * 2047.0f)];
			};

			Pixel = FColor(Perturb(Pixel.R), Perturb(Pixel.G), Perturb(Pixel.B), 255);
		}
	}
}

/* //} */

/* CompressFrame() //{ */

void UCamera::CompressFrame(const TArray<FColor>& Pixels, const FIntPoint Size, const FCameraCompression& Compression, const bool bLossless,
                            FCameraFrame& OutFrame)
{
	OutFrame.Bytes.Reset();
	OutFrame.Size  = Size;
	OutFrame.Codec = Compression.Codec == ECameraCodec::Jpeg && bLossless ? ECameraCodec::Png : Compression.Codec;

	if (Pixels.Num() < Size.X * Size.Y || Size.X <= 0 || Size.Y <= 0)
	{
		return;
	}

	if (OutFrame.Codec == ECameraCodec::Raw)
	{
		OutFrame.Bytes.Append(reinterpret_cast<const uint8*>(Pixels.GetData()), Size.X * Size.Y * sizeof(FColor));
		return;
	}

	const bool  bJpeg   = OutFrame.Codec == ECameraCodec::Jpeg;
	const int32 Quality = FMath::Clamp(Compression.Quality, 1, 100);

	const FImageView Image(Pixels.GetData(), Size.X, Size.Y, EGammaSpace::sRGB);

	TArray64<uint8> Compressed;
	if (FImageUtils::CompressImage(Compressed, bJpeg ? TEXT("jpg") : TEXT("png"), Image, bJpeg ? Quality : 0))
	{
		OutFrame.Bytes.Append(Compressed.GetData(), Compressed.Num());
	}
}

/* //} */


void UCamera::SetCameraCaptureMode(CameraCaptureModeEnum CaptureMode) {

	CameraCaptureMode = CaptureMode;

}

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

