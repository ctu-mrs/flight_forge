#include "EventCamera.h"

#include "Algo/Sort.h"
#include "Async/ParallelFor.h"
#include "Engine/World.h"
#include "HAL/IConsoleManager.h"
#include "RenderingThread.h"
#include "TextureResource.h"

#include "SensorWire.h"
#include "FlightForgeCore/RobotPawn.h"
#include "FlightForgeCore/GpuWatchdog.h"
#include "FlightForgeCore/SimClock.h"

// a runaway scene (or a stalled client) must not grow the stream unboundedly: 256 MB of FStreamEvent
#define EVENT_STREAM_MAX_EVENTS (16 * 1024 * 1024)

static TAutoConsoleVariable<int32> CVarEventParity(TEXT("ff.EventCamera.Parity"), 0,
                                                   TEXT("run the CPU port beside the shader on every frame and log how far they disagree"));

static TAutoConsoleVariable<int32> CVarEventMaxBufferMB(TEXT("ff.EventCamera.MaxBufferMB"), 256,
                                                        TEXT("per-frame event buffer guard; a frame that needs more is thinned evenly and reports the rest as dropped"));

UEventCamera::UEventCamera() : UCamera()
{
	SensorType = SensorType::EVENT_CAMERA;
	Name = TEXT("EVENT_CAMERA");

	InstructionQueue      = std::make_unique<TInstructionQueue<UEventCamera>>();
	EventsCriticalSection = std::make_unique<FCriticalSection>();
	GpuState              = MakeShared<FEventGpuState, ESPMode::ThreadSafe>();
}

/* BeginPlay() //{ */

void UEventCamera::BeginPlay()
{
	Super::BeginPlay();

	// log-intensity contrast needs linear radiance, not the tonemapped image
	RenderTarget2D = NewObject<UTextureRenderTarget2D>();
	RenderTarget2D->RenderTargetFormat = RTF_RGBA16f;
	RenderTarget2D->InitCustomFormat(640, 480, PF_FloatRGBA, true);
	RenderTarget2D->bGPUSharedFlag = true;

	SceneCaptureComponent2D->CaptureSource = SCS_SceneColorHDR;
	SceneCaptureComponent2D->TextureTarget = RenderTarget2D;
	SceneCaptureComponent2D->ShowFlags.SetTemporalAA(false);
	SceneCaptureComponent2D->ShowFlags.SetMotionBlur(false);
	SceneCaptureComponent2D->bAlwaysPersistRenderingState = true;
	SceneCaptureComponent2D->bCaptureEveryFrame           = false;
	SceneCaptureComponent2D->bCaptureOnMovement           = false;

	ApplyLevelPostProcessBaseline(SceneCaptureComponent2D);

	// a moving exposure would shift every pixel's log intensity at once
	FPostProcessSettings& PP        = SceneCaptureComponent2D->PostProcessSettings;
	PP.bOverride_AutoExposureMethod = true;
	PP.AutoExposureMethod           = AEM_Manual;
	PP.bOverride_AutoExposureBias   = true;
	PP.AutoExposureBias             = 0.0f;
	PP.bOverride_CameraShutterSpeed = true;
	PP.CameraShutterSpeed           = 60.0f;
	PP.bOverride_CameraISO          = true;
	PP.CameraISO                    = 100.0f;
	PP.bOverride_DepthOfFieldFstop  = true;
	PP.DepthOfFieldFstop            = 4.0f;

	event_camera_config_.ShowCameraComponent  = false;
	event_camera_config_.Offset               = DEFAULT_CAMERA_MOUNT;
	event_camera_config_.Orientation          = FRotator(0, 0, 0);
	event_camera_config_.FOVAngle             = 90;
	event_camera_config_.Width                = 640;
	event_camera_config_.Height               = 480;
	event_camera_config_.ContrastThresholdPos = 0.2;
	event_camera_config_.ContrastThresholdNeg = 0.2;
	event_camera_config_.UseSimTime           = true;
	event_camera_config_.ThresholdSigma       = 0.03;
	event_camera_config_.RefractoryPeriod     = 1e-4;
	event_camera_config_.CutoffHz             = 300.0;
	event_camera_config_.LeakRateHz           = 0.1;
	event_camera_config_.ShotNoiseRateHz      = 0.1;
	event_camera_config_.HotPixelFraction     = 1e-4;
	event_camera_config_.MaxEventRateHz       = 0.0;
	event_camera_config_.MaxEventsPerFrame    = 0;
	event_camera_config_.UseGpu               = true;
	event_camera_config_.Seed                 = 12345;

	SetEventCameraConfig(event_camera_config_);
}

//}

/* BeginDestroy() //{ */

void UEventCamera::BeginDestroy()
{
	if (EventsCriticalSection)
	{
		EventsCriticalSection->Lock();
		EventBuffer.Empty();
		EventsCriticalSection->Unlock();
	}

	// pooled GPU resources must die on the render thread, after any command using them
	ENQUEUE_RENDER_COMMAND(FlightForgeReleaseEventState)(
	    [State = MoveTemp(GpuState), Inflight = MoveTemp(GpuInflight)](FRHICommandListImmediate&) {});

	Super::BeginDestroy();
}

//}

/* TickComponent() //{ */

void UEventCamera::DrainInstructions()
{
	std::shared_ptr<FInstruction<UEventCamera>> Instruction;
	while (InstructionQueue->Dequeue(Instruction))
	{
		Instruction->Run(*this);
	}
}

void UEventCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	const double Start = FPlatformTime::Seconds();

	DrainInstructions();

	// in lockstep wall time between steps means nothing, so one request keeps the stream
	const bool bRecentlyAsked = (Start - camera_last_request_time_) < 5.0;
	const bool bWanted        = bRecentlyAsked || (FFlightForgeSimClock::bLockstep.load() && camera_last_request_time_ > 0.0);

	if (bWanted)
	{
		if (!bStreaming)
		{
			bStreaming   = true;
			bNeedsSeed   = true;
			bGuardLogged = false;
			UE_LOG(LogTemp, Log, TEXT("UEventCamera: stream on"));
		}

		const double Stamp = event_camera_config_.UseSimTime && GetWorld() ? GetWorld()->GetTimeSeconds() : Start;
		if (ShouldCapture())
		{
			UpdateCamera(true, Stamp);
		}
	}
	else if (bStreaming)
	{
		bStreaming = false;
		UE_LOG(LogTemp, Log, TEXT("UEventCamera: stream off after %u frames, %u folded"), FrameIndex, FramesFolded);
	}

	PumpInflight();

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

//}

/* UpdateCamera() //{ */

void UEventCamera::UpdateCamera(bool isExternallyLocked, double stamp)
{
	const bool bParity = event_camera_config_.UseGpu && CVarEventParity.GetValueOnGameThread() != 0;

	// free-running, a GPU that falls behind folds the interval into the next capture; in lockstep every tick is a frame
	if (InflightFrames(bParity) >= MaxInflight)
	{
		if (!FFlightForgeSimClock::bLockstep.load())
		{
			FramesFolded++;
			return;
		}
		WaitForSlot(bParity);
	}

	const FTransform Now = SceneCaptureComponent2D->GetComponentTransform();
	bNeedsSeed           = bNeedsSeed || PoseJumped(Now);

	const double TPrev = bNeedsSeed ? stamp : LastCaptureStamp;

	if (!bNeedsSeed && stamp - TPrev <= 0.0)
	{
		return;  // sim time did not move
	}

	if (bNeedsSeed)
	{
		TimeEpoch     = stamp;
		LastCount     = 0;
		EventCapacity = 0;
		LastFrameDt.store(0.0);
	}

	SceneCaptureComponent2D->CaptureScene();

	FEventGenParams Params = BuildParams(TPrev, stamp);
	Params.bSeedState      = bNeedsSeed;

	if (event_camera_config_.UseGpu)
	{
		EnqueueGpuFrame(Params, TPrev, stamp);
	}

	if (!event_camera_config_.UseGpu || bParity)
	{
		TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Slot = EnqueueReadback(RenderTarget2D, stamp, FVector::ZeroVector, false, false, false, true);
		if (Slot)
		{
			CpuInflight.Add({Slot, TPrev, stamp, bParity, Params});
		}
	}

	bNeedsSeed           = false;
	LastCaptureStamp     = stamp;
	LastCaptureTransform = Now;
	FrameIndex++;
}

//}

/* InflightFrames() / WaitForSlot() //{ */

int32 UEventCamera::InflightFrames(bool bParity) const
{
	return bParity ? FMath::Max(GpuInflight.Num(), CpuInflight.Num()) : GpuInflight.Num() + CpuInflight.Num();
}

void UEventCamera::WaitForSlot(bool bParity)
{
	const double Deadline = FPlatformTime::Seconds() + 5.0;

	while (InflightFrames(bParity) >= MaxInflight && FPlatformTime::Seconds() < Deadline)
	{
		FGpuWatchdog::FlushRenderingCommands();
		PumpInflight();
		if (InflightFrames(bParity) >= MaxInflight)
		{
			FPlatformProcess::SleepNoStats(0.0002f);
		}
	}
}

//}

/* PoseJumped() //{ */

// a teleport that leaves nothing of the last frame in view is a reset, not motion
bool UEventCamera::PoseJumped(const FTransform& Now) const
{
	const double HalfFovRad  = FMath::DegreesToRadians(FMath::Clamp(event_camera_config_.FOVAngle, 1.0, 179.0)) * 0.5;
	const double AngleRad    = LastCaptureTransform.GetRotation().AngularDistance(Now.GetRotation());
	const double DistanceCm  = FVector::Dist(LastCaptureTransform.GetLocation(), Now.GetLocation());

	return AngleRad > HalfFovRad || DistanceCm > 500.0;
}

//}

/* BuildParams() //{ */

FEventGenParams UEventCamera::BuildParams(double TPrev, double TCur)
{
	const FEventCameraConfig& C = event_camera_config_;

	FEventGenParams P;
	P.Size       = FIntPoint(C.Width, C.Height);
	P.Dt         = static_cast<float>(FMath::Max(TCur - TPrev, 0.0));
	P.BaseUs     = static_cast<uint32>(static_cast<uint64>(FMath::Max(llround((TPrev - TimeEpoch) * 1e6), 0ll)));
	P.DtUs       = static_cast<uint32>(FMath::Max(llround((TCur - TPrev) * 1e6), 0ll));
	P.FrameIndex = FrameIndex;
	P.Seed       = C.Seed;

	P.ThresholdPos     = static_cast<float>(FMath::Max(C.ContrastThresholdPos, 0.01));
	P.ThresholdNeg     = static_cast<float>(FMath::Max(C.ContrastThresholdNeg, 0.01));
	P.ThresholdSigma   = static_cast<float>(FMath::Max(C.ThresholdSigma, 0.0));
	P.RefractoryUs     = static_cast<uint32>(FMath::Max(llround(C.RefractoryPeriod * 1e6), 0ll));
	P.CutoffHz         = static_cast<float>(FMath::Max(C.CutoffHz, 0.0));
	P.LeakRateHz       = static_cast<float>(FMath::Max(C.LeakRateHz, 0.0));
	P.ShotNoiseRateHz  = static_cast<float>(FMath::Max(C.ShotNoiseRateHz, 0.0));
	P.HotPixelFraction = static_cast<float>(FMath::Clamp(C.HotPixelFraction, 0.0, 1.0));

	uint64 Ceiling = (uint64)FMath::Max(CVarEventMaxBufferMB.GetValueOnGameThread(), 1) * 1024 * 1024 / (sizeof(uint32) * 2);
	if (C.MaxEventsPerFrame > 0)
	{
		Ceiling = FMath::Min<uint64>(Ceiling, C.MaxEventsPerFrame);
	}
	Ceiling = FMath::Clamp<uint64>(Ceiling, 1024, MAX_uint32 / 2);

	// counts come back a few frames late, so the buffer keeps 2-4x the last one and only grows within a stream
	const double LastDt = LastFrameDt.load();
	const double Scale  = LastDt > 0.0 ? FMath::Max(P.Dt / LastDt, 1.0) : 1.0;
	const uint64 Want   = FMath::Max<uint64>(FMath::Max<uint64>(2ull * C.Width * C.Height, 1u << 20), (uint64)(2.0 * LastCount * Scale));
	EventCapacity       = (uint32)FMath::Min<uint64>(FMath::Max<uint64>(EventCapacity, FMath::RoundUpToPowerOfTwo64(Want)), Ceiling);
	P.MaxEvents         = EventCapacity;

	// thinning lands within a few sigma of the budget; 1/64 slack keeps it inside the buffer
	const uint32 Fits  = P.MaxEvents - P.MaxEvents / 64 - 64;
	uint32       Quota = MAX_uint32;
	if (C.MaxEventRateHz > 0.0)
	{
		Quota = static_cast<uint32>(FMath::Clamp<double>(C.MaxEventRateHz * (TCur - TPrev), 1.0, (double)MAX_uint32));
	}
	P.Budget         = FMath::Min(Quota, Fits);
	P.bBudgetIsGuard = Fits < Quota;

	if (!C.UseGpu)
	{
		P.CopyEvents = P.MaxEvents;
		return P;
	}

	// the first readback follows the last rate; a frame that kept more is copied again, which costs a frame of latency
	const uint64 MinCopy = FMath::Max<uint64>(65536, (uint64)C.Width * C.Height / 4);
	const uint64 Copy    = LastDt > 0.0 ? (uint64)(2.0 * LastCount * Scale) + MinCopy : (uint64)P.MaxEvents;
	P.CopyEvents         = (uint32)FMath::Min<uint64>(FMath::Min<uint64>(Copy, (uint64)P.Budget + P.Budget / 32 + 64), P.MaxEvents);

	return P;
}

//}

/* EnqueueGpuFrame() //{ */

void UEventCamera::EnqueueGpuFrame(const FEventGenParams& Params, double TPrev, double TCur)
{
	FTextureRenderTargetResource* Resource = RenderTarget2D ? RenderTarget2D->GameThread_GetRenderTargetResource() : nullptr;
	if (!Resource)
	{
		return;
	}

	TSharedPtr<FEventReadback, ESPMode::ThreadSafe> Slot = MakeShared<FEventReadback, ESPMode::ThreadSafe>();
	Slot->TPrev          = TPrev;
	Slot->TCur           = TCur;
	Slot->FrameIndex     = FrameIndex;
	Slot->bBudgetIsGuard = Params.bBudgetIsGuard;

	TSharedPtr<FEventGpuState, ESPMode::ThreadSafe> State = GpuState;

	ENQUEUE_RENDER_COMMAND(FlightForgeEventGen)(
	    [Resource, Slot, State, Params](FRHICommandListImmediate& RHICmdList)
	    {
		    FlightForgeEventGen::Dispatch_RenderThread(RHICmdList, Resource->GetRenderTargetTexture(), *State, Params, Slot);
	    });

	GpuInflight.Add(Slot);
}

//}

/* PumpInflight() //{ */

void UEventCamera::PumpInflight()
{
	// frames complete in order; the head is either published, resolved, or still on the GPU
	while (GpuInflight.Num() > 0)
	{
		TSharedPtr<FEventReadback, ESPMode::ThreadSafe> Slot = GpuInflight[0];

		if (Slot->bCopied)
		{
			if (CVarEventParity.GetValueOnGameThread() != 0 && Slot->Kept <= (uint32)Slot->Packed.Num())
			{
				CompareParity(Slot->FrameIndex, Slot->Packed, true);
			}
			PublishFrame(Slot->Packed, Slot->Count, Slot->MaxLogChange, Slot->TPrev, Slot->TCur, Slot->bBudgetIsGuard);
			GpuInflight.RemoveAt(0);
			continue;
		}

		if (!Slot->bResolveIssued && !Slot->bResolveQueued.exchange(true))
		{
			ENQUEUE_RENDER_COMMAND(FlightForgeEventResolve)([Slot](FRHICommandListImmediate& RHICmdList) {
				Slot->bResolveQueued = false;
				if (Slot->IsReady() && !Slot->bResolveIssued.exchange(true))
				{
					FlightForgeEventGen::Resolve_RenderThread(RHICmdList, Slot);
				}
			});
		}
		break;
	}

	while (CpuInflight.Num() > 0)
	{
		const FCpuFrame                                  Frame = CpuInflight[0];
		TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Slot  = Frame.Slot;

		if (Slot->bCopied)
		{
			TArray<uint64> Packed;
			uint32         Count        = 0;
			float          MaxLogChange = 0.0f;
			RunCpuModel(*Slot, Frame.Params, Packed, Count, MaxLogChange);

			if (Frame.bCompare)
			{
				CompareParity(Frame.Params.FrameIndex, MoveTemp(Packed), false);
			}
			else
			{
				PublishFrame(Packed, Count, MaxLogChange, Frame.TPrev, Frame.TCur, Frame.Params.bBudgetIsGuard);
			}
			CpuInflight.RemoveAt(0);
			continue;
		}

		RequestResolve(Slot);
		break;
	}
}

//}

/* CompareParity() //{ */

void UEventCamera::CompareParity(uint32 Frame, TArray<uint64> Packed, bool bFromGpu)
{
	Algo::Sort(Packed);

	TArray<uint64>* Other = ParityPending.Find(Frame);
	if (!Other)
	{
		if (ParityPending.Num() > 8)
		{
			ParityPending.Reset();  // one side skipped frames; do not keep waiting for them
		}
		ParityPending.Add(Frame, MoveTemp(Packed));
		return;
	}

	const TArray<uint64>& Gpu = bFromGpu ? Packed : *Other;
	const TArray<uint64>& Cpu = bFromGpu ? *Other : Packed;

	// exact match, then a match on pixel and polarity alone to tell timestamp rounding from model drift
	int32 Common = 0;
	for (int32 i = 0, j = 0; i < Gpu.Num() && j < Cpu.Num();)
	{
		if (Gpu[i] == Cpu[j])
		{
			++Common, ++i, ++j;
		}
		else if (Gpu[i] < Cpu[j])
		{
			++i;
		}
		else
		{
			++j;
		}
	}

	TArray<uint32> GpuPixels, CpuPixels;
	GpuPixels.Reserve(Gpu.Num());
	CpuPixels.Reserve(Cpu.Num());
	for (const uint64 E : Gpu)
	{
		GpuPixels.Add(static_cast<uint32>(E & 0xFFFFFFFFu));
	}
	for (const uint64 E : Cpu)
	{
		CpuPixels.Add(static_cast<uint32>(E & 0xFFFFFFFFu));
	}
	Algo::Sort(GpuPixels);
	Algo::Sort(CpuPixels);

	int32 SamePixel = 0;
	for (int32 i = 0, j = 0; i < GpuPixels.Num() && j < CpuPixels.Num();)
	{
		if (GpuPixels[i] == CpuPixels[j])
		{
			++SamePixel, ++i, ++j;
		}
		else if (GpuPixels[i] < CpuPixels[j])
		{
			++i;
		}
		else
		{
			++j;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("UEventCamera parity frame %u: gpu %d cpu %d exact %d pixel %d"), Frame, Gpu.Num(), Cpu.Num(), Common, SamePixel);
	ParityPending.Remove(Frame);
}

//}

/* DrainReadback() //{ */

void UEventCamera::DrainReadback()
{
	// flushing only drains the render thread; the GPU fence still has to be polled
	const double Deadline = FPlatformTime::Seconds() + 5.0;

	while ((GpuInflight.Num() > 0 || CpuInflight.Num() > 0) && FPlatformTime::Seconds() < Deadline)
	{
		FGpuWatchdog::FlushRenderingCommands();
		PumpInflight();
		if (GpuInflight.Num() > 0 || CpuInflight.Num() > 0)
		{
			FPlatformProcess::SleepNoStats(0.0002f);
		}
	}
}

//}

/* SuggestedTimestep() //{ */

double UEventCamera::SuggestedTimestep(double CurrentDt, double MinDt) const
{
	const float  MaxChange = LastMaxLogChange.load();
	const double Dt        = LastFrameDt.load();

	if (!(MaxChange > 0.0f) || Dt <= 0.0 || !bStreaming)
	{
		return CurrentDt;
	}

	const double Threshold = FMath::Min(event_camera_config_.ContrastThresholdPos, event_camera_config_.ContrastThresholdNeg);

	return FMath::Clamp(Dt * Threshold / MaxChange, MinDt, CurrentDt);
}

//}

/* RunCpuModel() //{ */

// port of EventGen.usf; keep the two in step
namespace
{
template <typename To, typename From>
To BitCast(From V)
{
	static_assert(sizeof(To) == sizeof(From));
	To Out;
	FMemory::Memcpy(&Out, &V, sizeof(To));
	return Out;
}

uint32 Pcg(uint32 v)
{
	const uint32 state = v * 747796405u + 2891336453u;
	const uint32 word  = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
	return (word >> 22u) ^ word;
}

float Hash01(uint32 a, uint32 b, uint32 c)
{
	const uint32 h = Pcg(a ^ Pcg(b ^ Pcg(c)));
	return (h & 0xFFFFFFu) / 16777216.0f;
}

float Gaussian(uint32 a, uint32 b, uint32 c)
{
	const float Sum = Hash01(a, b, c) + Hash01(a, b, c + 0x9E3779B9u) + Hash01(a, b, c + 0x3C6EF372u) + Hash01(a, b, c + 0xDAA66D2Bu);
	return (Sum - 2.0f) * 1.73205081f;
}

// bit-identical to the shader's; UBT builds with -ffp-contract=off, so nothing here is fused
float DetRcp(float X) //{
{
	float R = BitCast<float>(0x7EF311C3u - BitCast<uint32>(X));
	R       = R * (2.0f - X * R);
	R       = R * (2.0f - X * R);
	R       = R * (2.0f - X * R);
	return R;
} //}

float DetLog(float X) //{
{
	const uint32 Bits = BitCast<uint32>(X);
	int32        E    = (int32)(Bits >> 23) - 127;
	float        M    = BitCast<float>((Bits & 0x007FFFFFu) | 0x3F800000u);
	if (M > 1.41421356f)
	{
		M = M * 0.5f;
		E += 1;
	}
	const float F    = M - 1.0f;
	const float S    = F * DetRcp(F + 2.0f);
	const float S2   = S * S;
	const float Poly = 1.0f + S2 * (0.333333333f + S2 * (0.2f + S2 * (0.142857143f + S2 * 0.111111111f)));
	return (float)E * 0.693147181f + 2.0f * S * Poly;
} //}

float DetExp(float X) //{
{
	const float N = FMath::FloorToFloat(X * 1.44269504f + 0.5f);
	if (N < -126.0f)
	{
		return 0.0f;
	}
	const float R = X - N * 0.693147181f;
	const float P = 1.0f + R * (1.0f + R * (0.5f + R * (0.166666667f + R * (0.0416666667f + R * (0.00833333333f + R * (0.00138888889f + R * 0.000198412698f))))));
	return P * BitCast<float>((uint32)((int32)N + 127) << 23);
} //}

uint32 OffsetUs(float Frac, uint32 DtUs)
{
	return (uint32)FMath::FloorToFloat(Frac * (float)DtUs + 0.5f);
}

uint64 PackEvent(uint32 X, uint32 Y, bool bPositive, uint32 OffsetUs)
{
	return (static_cast<uint64>(OffsetUs) << 32) | (X | (Y << 16) | (bPositive ? 0x80000000u : 0u));
}

float ThinKey(uint32 PixelKey, const FEventGenParams& P, uint32 Ordinal)
{
	return Hash01(PixelKey, P.Seed ^ (P.FrameIndex * 0x27D4EB2Fu), 0x300u + Ordinal);
}

// LSD radix on 16-bit digits, skipping digits every key shares; a frame's offsets rarely need the top one
void SortPacked(TArray<uint64>& Keys) //{
{
	const int32 N = Keys.Num();
	if (N < 4096)
	{
		Algo::Sort(Keys);
		return;
	}

	TArray<uint64> Scratch;
	Scratch.SetNumUninitialized(N);
	TArray<uint32> Bucket;
	Bucket.SetNumUninitialized(65536);

	uint64* Src = Keys.GetData();
	uint64* Dst = Scratch.GetData();

	for (int32 Shift = 0; Shift < 64; Shift += 16)
	{
		FMemory::Memzero(Bucket.GetData(), Bucket.Num() * sizeof(uint32));
		for (int32 i = 0; i < N; ++i)
		{
			Bucket[(Src[i] >> Shift) & 0xFFFF]++;
		}
		if (Bucket[(Src[0] >> Shift) & 0xFFFF] == (uint32)N)
		{
			continue;
		}

		uint32 Sum = 0;
		for (uint32& B : Bucket)
		{
			const uint32 Here = B;
			B                 = Sum;
			Sum += Here;
		}
		for (int32 i = 0; i < N; ++i)
		{
			Dst[Bucket[(Src[i] >> Shift) & 0xFFFF]++] = Src[i];
		}
		Swap(Src, Dst);
	}

	if (Src != Keys.GetData())
	{
		FMemory::Memcpy(Keys.GetData(), Src, N * sizeof(uint64));
	}
} //}
}  // namespace

void UEventCamera::RunCpuModel(const FCameraReadback& Frame, const FEventGenParams& P, TArray<uint64>& OutPacked, uint32& OutCount, float& OutMaxLogChange)
{
	OutMaxLogChange = 0.0f;
	OutCount        = 0;
	OutPacked.Reset();

	const FIntPoint Size      = Frame.Size;
	const int32     NumPixels = Size.X * Size.Y;

	if (NumPixels <= 0 || Frame.LumaPixels.Num() != NumPixels)
	{
		return;
	}

	const bool bSeed = P.bSeedState || CpuStateSize != Size || CpuState.Num() != NumPixels;

	if (bSeed)
	{
		CpuState.SetNumUninitialized(NumPixels);
		CpuLastEvent.SetNumUninitialized(NumPixels);
		CpuStateSize = Size;

		for (int32 i = 0; i < NumPixels; ++i)
		{
			const float L   = DetLog(FMath::Max(Frame.LumaPixels[i], 0.0f) + P.LogEps);
			CpuState[i]     = FVector4f(L, L, L, L);
			CpuLastEvent[i] = P.BaseUs - P.RefractoryUs;
		}
		return;
	}

	const int32 NumChunks = FMath::Clamp(NumPixels / 65536, 1, 16);
	const int32 ChunkSize = FMath::DivideAndRoundUp(NumPixels, NumChunks);

	TArray<TArray<uint64>> ChunkEvents;
	TArray<TArray<float>>  ChunkThin;
	TArray<float>          ChunkMax;
	ChunkEvents.SetNum(NumChunks);
	ChunkThin.SetNum(NumChunks);
	ChunkMax.Init(0.0f, NumChunks);

	const uint32 MaxPerPixel = FMath::Min(64u, P.RefractoryUs > 0 ? P.DtUs / P.RefractoryUs + 1u : 64u);

	ParallelFor(NumChunks, [&](int32 Chunk) {
		const int32 First = Chunk * ChunkSize;
		const int32 Last  = FMath::Min(First + ChunkSize, NumPixels);

		TArray<uint64>& Local     = ChunkEvents[Chunk];
		TArray<float>&  LocalThin = ChunkThin[Chunk];
		float&          LocalMax  = ChunkMax[Chunk];

		for (int32 i = First; i < Last; ++i)
		{
			const uint32 X = (uint32)(i % Size.X);
			const uint32 Y = (uint32)(i / Size.X);

			const float I       = FMath::Max(Frame.LumaPixels[i], 0.0f);
			const float Inten01 = FMath::Clamp(I, 0.0f, 1.0f);
			const float L       = DetLog(I + P.LogEps);

			const uint32 PixelKey = X + Y * 65536u;
			uint32       Ordinal  = 0;

			auto Emit = [&](bool bPositive, uint32 Offset) {
				Local.Add(PackEvent(X, Y, bPositive, Offset));
				LocalThin.Add(ThinKey(PixelKey, P, Ordinal++));
			};

			FVector4f&  State = CpuState[i];
			float       Ref   = State.X;
			float       Lp1   = State.Y;
			float       Lp2   = State.Z;
			const float LPrev = State.W;

			if (P.CutoffHz > 0.0f)
			{
				const float Alpha = 1.0f - DetExp(-6.283185307f * P.CutoffHz * (0.05f + 0.95f * Inten01) * P.Dt);
				Lp1               = Lp1 + Alpha * (L - Lp1);
				Lp2               = Lp2 + Alpha * (Lp1 - Lp2);
			}
			else
			{
				Lp1 = L;
				Lp2 = L;
			}
			const float LCur = Lp2;

			Ref = Ref - P.LeakRateHz * P.ThresholdPos * P.Dt * (0.9f + 0.2f * Hash01(PixelKey, P.Seed, 1u));

			const float Cpos = FMath::Max(P.ThresholdPos + P.ThresholdSigma * Gaussian(PixelKey, P.Seed, 2u), 0.01f);
			const float Cneg = FMath::Max(P.ThresholdNeg + P.ThresholdSigma * Gaussian(PixelKey, P.Seed, 3u), 0.01f);

			uint32 LastEv = CpuLastEvent[i];

			const float dL = LCur - LPrev;
			LocalMax       = FMath::Max(LocalMax, FMath::Abs(dL));

			uint32 Emitted = 0;

			if (dL > 0.0f)
			{
				const float InvDl = DetRcp(dL);
				while (Emitted < MaxPerPixel)
				{
					const float Next = Ref + Cpos;
					if (Next > LCur)
					{
						break;
					}
					Ref                = Next;
					const float  Frac  = FMath::Clamp((Ref - LPrev) * InvDl, 0.0f, 1.0f);
					const uint32 Off   = OffsetUs(Frac, P.DtUs);
					const uint32 TUs   = P.BaseUs + Off;
					if (TUs - LastEv >= P.RefractoryUs)
					{
						Emit(true, Off);
						LastEv = TUs;
					}
					Emitted++;
				}
			}
			else if (dL < 0.0f)
			{
				const float InvDl = DetRcp(-dL);
				while (Emitted < MaxPerPixel)
				{
					const float Next = Ref - Cneg;
					if (Next < LCur)
					{
						break;
					}
					Ref                = Next;
					const float  Frac  = FMath::Clamp((LPrev - Ref) * InvDl, 0.0f, 1.0f);
					const uint32 Off   = OffsetUs(Frac, P.DtUs);
					const uint32 TUs   = P.BaseUs + Off;
					if (TUs - LastEv >= P.RefractoryUs)
					{
						Emit(false, Off);
						LastEv = TUs;
					}
					Emitted++;
				}
			}

			if (Emitted >= MaxPerPixel)
			{
				Ref = LCur;
			}

			if (P.ShotNoiseRateHz > 0.0f)
			{
				float Rate = P.ShotNoiseRateHz * (1.0f + 3.0f * (1.0f - Inten01));
				if (Hash01(PixelKey, P.Seed, 4u) < P.HotPixelFraction)
				{
					Rate = Rate * 100.0f;
				}

				const float  InvRate  = DetRcp(Rate);
				const float  InvDt    = DetRcp(P.Dt);
				const uint32 NoiseKey = P.Seed ^ (P.FrameIndex * 0x85EBCA6Bu);
				float        T        = 0.0f;
				for (uint32 j = 0; j < MaxPerPixel; ++j)
				{
					T = T - DetLog(FMath::Max(Hash01(PixelKey, NoiseKey, 0x100u + j), 1e-7f)) * InvRate;
					if (T >= P.Dt)
					{
						break;
					}
					const uint32 Off = OffsetUs(T * InvDt, P.DtUs);
					const uint32 TUs = P.BaseUs + Off;
					if (TUs - LastEv >= P.RefractoryUs)
					{
						Emit(Hash01(PixelKey, NoiseKey, 0x200u + j) < 0.5f, Off);
						LastEv = TUs;
					}
				}
			}

			State           = FVector4f(Ref, Lp1, Lp2, LCur);
			CpuLastEvent[i] = LastEv;
		}
	});

	uint32 Total = 0;
	for (int32 Chunk = 0; Chunk < NumChunks; ++Chunk)
	{
		Total += ChunkEvents[Chunk].Num();
		OutMaxLogChange = FMath::Max(OutMaxLogChange, ChunkMax[Chunk]);
	}
	OutCount = Total;

	const bool  bThin  = Total > P.Budget;
	const float Count  = (float)Total;
	const float Budget = (float)P.Budget;

	OutPacked.Reserve((int32)FMath::Min(Total, P.MaxEvents));
	for (int32 Chunk = 0; Chunk < NumChunks; ++Chunk)
	{
		for (int32 i = 0; i < ChunkEvents[Chunk].Num() && (uint32)OutPacked.Num() < P.MaxEvents; ++i)
		{
			if (!bThin || ChunkThin[Chunk][i] * Count < Budget)
			{
				OutPacked.Add(ChunkEvents[Chunk][i]);
			}
		}
	}
}

//}

/* PublishFrame() //{ */

void UEventCamera::PublishFrame(TArray<uint64>& Packed, uint32 Count, float MaxLogChange, double TPrev, double TCur, bool bBudgetIsGuard)
{
	SortPacked(Packed);

	const double Dt          = TCur - TPrev;
	uint32       DroppedHere = Count > (uint32)Packed.Num() ? Count - (uint32)Packed.Num() : 0;

	if (DroppedHere > 0 && bBudgetIsGuard && !bGuardLogged)
	{
		bGuardLogged = true;
		UE_LOG(LogTemp, Warning, TEXT("UEventCamera: a frame fired %u events and its buffer (guard ff.EventCamera.MaxBufferMB=%d) kept %d; thinned evenly, the rest reported as dropped"),
		       Count, CVarEventMaxBufferMB.GetValueOnGameThread(), Packed.Num());
	}

	// offsets are rounded microseconds; scale by the exact interval so a frame never overruns the next
	const double DtUs      = FMath::Max((double)llround(Dt * 1e6), 1.0);
	const double PerOffset = Dt / DtUs;

	EventsCriticalSection->Lock();

	if (EventBuffer.Num() + Packed.Num() > EVENT_STREAM_MAX_EVENTS)
	{
		DroppedHere += Packed.Num();
	}
	else
	{
		EventBuffer.Reserve(EventBuffer.Num() + Packed.Num());
		for (const uint64 E : Packed)
		{
			EventBuffer.Add({static_cast<uint32>(E & 0xFFFFFFFFu), TPrev + FMath::Min(static_cast<double>(E >> 32), DtUs) * PerOffset});
		}
	}

	Dropped += DroppedHere;
	LastFrameStamp = TCur;

	EventsCriticalSection->Unlock();

	LastCount = Count;
	LastFrameDt.store(Dt);
	LastMaxLogChange.store(MaxLogChange);
}

//}

/* SetEventCameraConfig() //{ */

bool UEventCamera::SetEventCameraConfig(const FEventCameraConfig& Config)
{
	if (!ValidDimensions(Config.Width, Config.Height))
	{
		return false;
	}

	// frames from the old optics must not land in the new stream
	DrainReadback();

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

	bNeedsSeed = true;
	CpuState.Empty();
	CpuLastEvent.Empty();
	LastCount     = 0;
	EventCapacity = 0;

	EventsCriticalSection->Lock();
	EventBuffer.Empty();
	Dropped = 0;
	EventsCriticalSection->Unlock();

	return true;
}

//}

/* GetConfig() / SetConfig() //{ */

void UEventCamera::GetConfig(FWireReply& Reply)
{
	const FEventCameraConfig& C = event_camera_config_;

	flightforge::wire::EventCameraConfigT Table;
	Table.show_debug_camera      = C.ShowCameraComponent;
	Table.mount                  = FSensorWire::Mount(C.Offset, C.Orientation);
	Table.fov                    = C.FOVAngle;
	Table.width                  = C.Width;
	Table.height                 = C.Height;
	Table.contrast_threshold_pos = C.ContrastThresholdPos;
	Table.contrast_threshold_neg = C.ContrastThresholdNeg;
	Table.use_sim_time           = C.UseSimTime;
	Table.threshold_sigma        = C.ThresholdSigma;
	Table.refractory_period      = C.RefractoryPeriod;
	Table.cutoff_hz              = C.CutoffHz;
	Table.leak_rate_hz           = C.LeakRateHz;
	Table.shot_noise_rate_hz     = C.ShotNoiseRateHz;
	Table.hot_pixel_fraction     = C.HotPixelFraction;
	Table.max_event_rate_hz      = C.MaxEventRateHz;
	Table.max_events_per_frame   = static_cast<uint32_t>(FMath::Max(C.MaxEventsPerFrame, 0));
	Table.use_gpu                = C.UseGpu;
	Table.seed                   = C.Seed;

	const auto Offset = flightforge::wire::CreateEventCameraConfig(Reply.Builder, &Table);
	Reply.Set(flightforge::wire::CreateGetEventCameraConfigResponse(Reply.Builder, Offset));
}

void UEventCamera::SetConfig(const flightforge::wire::Request& Request, FWireReply& Reply)
{
	const flightforge::wire::SetEventCameraConfigRequest* Set = Request.body_as_SetEventCameraConfigRequest();
	if (!Set || !Set->config())
	{
		Reply.Fail(TEXT("set_event_camera_config carries no config"));
		return;
	}

	const std::unique_ptr<flightforge::wire::EventCameraConfigT> In(Set->config()->UnPack());
	if (!FSensorWire::ValidPinhole(In->fov, In->width, In->height, Reply))
	{
		return;
	}

	FEventCameraConfig Config;
	Config.ShowCameraComponent = In->show_debug_camera;
	FSensorWire::Mount(In->mount, Config.Offset, Config.Orientation);
	Config.FOVAngle             = In->fov;
	Config.Width                = In->width;
	Config.Height               = In->height;
	Config.ContrastThresholdPos = In->contrast_threshold_pos;
	Config.ContrastThresholdNeg = In->contrast_threshold_neg;
	Config.UseSimTime           = In->use_sim_time;
	Config.ThresholdSigma       = In->threshold_sigma;
	Config.RefractoryPeriod     = In->refractory_period;
	Config.CutoffHz             = In->cutoff_hz;
	Config.LeakRateHz           = In->leak_rate_hz;
	Config.ShotNoiseRateHz      = In->shot_noise_rate_hz;
	Config.HotPixelFraction     = In->hot_pixel_fraction;
	Config.MaxEventRateHz       = In->max_event_rate_hz;
	Config.MaxEventsPerFrame    = static_cast<int>(FMath::Min<uint32>(In->max_events_per_frame, MAX_int32));
	Config.UseGpu               = In->use_gpu;
	Config.Seed                 = In->seed;

	bool bApplied = false;
	RunOnSensorQueue(*this, [&Config, &bApplied](UEventCamera& Camera) { bApplied = Camera.SetEventCameraConfig(Config); });

	if (!bApplied)
	{
		Reply.Fail(TEXT("the event camera rejected the config"));
	}
}

void UEventCamera::SetMount(const FVector& OffsetCm, const FRotator& Rotation)
{
	RunOnSensorQueue(*this, [&](UEventCamera& Camera) {
		FEventCameraConfig Config = Camera.event_camera_config_;
		Config.Offset             = OffsetCm;
		Config.Orientation        = Rotation;
		Camera.SetEventCameraConfig(Config);
	});
}

//}

/* GetData() //{ */

void UEventCamera::GetData(const flightforge::wire::Request& Request, FWireReply& Reply)
{
	// mark the stream as wanted so the tick keeps (or starts) capturing
	camera_last_request_time_ = FPlatformTime::Seconds();

	EventsCriticalSection->Lock();

	const int32  N         = EventBuffer.Num();
	const double BaseStamp = N > 0 ? EventBuffer[0].Stamp : LastFrameStamp;
	const double Stamp     = LastFrameStamp;
	const uint32 Lost      = Dropped;

	// a second vector can move the builder's buffer, so each is filled before the next is made
	uint32_t*  Xyp  = nullptr;
	const auto XypV = Reply.Builder.CreateUninitializedVector<uint32_t>(N, &Xyp);
	for (int32 i = 0; i < N; ++i)
	{
		Xyp[i] = EventBuffer[i].XYP;
	}

	uint32_t*  TUs  = nullptr;
	const auto TUsV = Reply.Builder.CreateUninitializedVector<uint32_t>(N, &TUs);
	for (int32 i = 0; i < N; ++i)
	{
		TUs[i] = static_cast<uint32_t>(FMath::Clamp(llround((EventBuffer[i].Stamp - BaseStamp) * 1e6), 0ll, (long long)0xFFFFFFFFu));
	}

	EventBuffer.Reset();
	Dropped = 0;

	EventsCriticalSection->Unlock();

	Reply.Set(flightforge::wire::CreateEventCameraDataResponse(Reply.Builder, BaseStamp, Stamp, event_camera_config_.Width, event_camera_config_.Height, Lost, XypV,
	                                                           TUsV));
}

//}
