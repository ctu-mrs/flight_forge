#include "EventCamera.h"

#include "Algo/Sort.h"
#include "Async/ParallelFor.h"
#include "Engine/World.h"
#include "HAL/IConsoleManager.h"
#include "RenderingThread.h"
#include "TextureResource.h"

#include "Serialize.h"
#include "FlightForgeCore/DronePawn.h"
#include "FlightForgeCore/SimClock.h"

// a runaway scene (or a stalled client) must not grow the stream unboundedly
#define EVENT_BUFFER_MAX_EVENTS 4000000

static TAutoConsoleVariable<int32> CVarEventParity(TEXT("ff.EventCamera.Parity"), 0,
                                                   TEXT("run the CPU port beside the shader on every frame and log how far they disagree"));

UEventCamera::UEventCamera() : UCamera()
{
	SensorType = SensorType::EVENT_CAMERA;
	Name = TEXT("EVENT_CAMERA");

	InstructionQueue      = std::make_unique<TQueue<std::shared_ptr<FInstruction<UEventCamera>>>>();
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
	event_camera_config_.Offset               = FVector(0, 0, 0);
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
	event_camera_config_.MaxEventsPerFrame    = 1 << 20;
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

void UEventCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	const double Start = FPlatformTime::Seconds();

	std::shared_ptr<FInstruction<UEventCamera>> Instruction;
	while (InstructionQueue->Dequeue(Instruction)) {
		Instruction->Function(*this);
		Instruction->Finished = true;
	}

	// in lockstep wall time between steps means nothing, so one request keeps the stream
	const bool bRecentlyAsked = (Start - camera_last_request_time_) < 1.0;
	const bool bWanted        = bRecentlyAsked || (FFlightForgeSimClock::bLockstep.load() && camera_last_request_time_ > 0.0);

	if (bWanted)
	{
		if (!bStreaming)
		{
			bStreaming = true;
			bNeedsSeed = true;
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
		TimeEpoch = stamp;
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
			CpuInflight.Add({Slot, TPrev, stamp, Params.bSeedState, bParity, FrameIndex});
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
		FlushRenderingCommands();
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

FEventGenParams UEventCamera::BuildParams(double TPrev, double TCur) const
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

	// a burst the copy did not fit is read back in full next frame; otherwise track the rate with headroom
	P.MaxEvents  = static_cast<uint32>(FMath::Max(C.MaxEventsPerFrame, 1024));
	const uint32 MinCopy = FMath::Max(65536u, (uint32)(C.Width * C.Height) / 4u);
	P.CopyEvents         = bLastFrameTruncated ? P.MaxEvents : FMath::Clamp(4u * LastCount + MinCopy, MinCopy, P.MaxEvents);

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
	Slot->TPrev      = TPrev;
	Slot->TCur       = TCur;
	Slot->FrameIndex = FrameIndex;

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
			if (CVarEventParity.GetValueOnGameThread() != 0 && Slot->Count <= (uint32)Slot->Packed.Num())
			{
				CompareParity(Slot->FrameIndex, Slot->Packed, true);
			}
			PublishFrame(Slot->Packed, Slot->Count, Slot->MaxLogChange, Slot->TPrev, Slot->TCur);
			GpuInflight.RemoveAt(0);
			continue;
		}

		if (Slot->IsReady() && !Slot->bResolveIssued.exchange(true))
		{
			ENQUEUE_RENDER_COMMAND(FlightForgeEventResolve)([Slot](FRHICommandListImmediate&) { FlightForgeEventGen::Resolve_RenderThread(Slot); });
		}
		break;
	}

	while (CpuInflight.Num() > 0)
	{
		const FCpuFrame                                  Frame = CpuInflight[0];
		TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Slot  = Frame.Slot;

		if (Slot->bCopied)
		{
			FEventGenParams Params = BuildParams(Frame.TPrev, Frame.TCur);
			Params.bSeedState      = Frame.bSeed;
			Params.FrameIndex      = Frame.FrameIndex;

			TArray<uint64> Packed;
			float          MaxLogChange = 0.0f;
			RunCpuModel(*Slot, Params, Packed, MaxLogChange);

			if (Frame.bCompare)
			{
				CompareParity(Frame.FrameIndex, MoveTemp(Packed), false);
			}
			else
			{
				PublishFrame(Packed, Packed.Num(), MaxLogChange, Frame.TPrev, Frame.TCur);
			}
			CpuInflight.RemoveAt(0);
			continue;
		}

		if (Slot->Readback && Slot->Readback->IsReady() && !Slot->bResolveIssued.exchange(true))
		{
			IssueResolve(Slot);
		}
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
		FlushRenderingCommands();
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
	const float u1 = FMath::Max(Hash01(a, b, c), 1e-7f);
	const float u2 = Hash01(a, b, c + 0x9E3779B9u);
	return FMath::Sqrt(-2.0f * FMath::Loge(u1)) * FMath::Cos(6.283185307f * u2);
}

uint64 PackEvent(uint32 X, uint32 Y, bool bPositive, uint32 OffsetUs)
{
	return (static_cast<uint64>(OffsetUs) << 32) | (X | (Y << 16) | (bPositive ? 0x80000000u : 0u));
}
}  // namespace

void UEventCamera::RunCpuModel(const FCameraReadback& Frame, const FEventGenParams& P, TArray<uint64>& OutPacked, float& OutMaxLogChange)
{
	OutMaxLogChange = 0.0f;

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
			const float L   = FMath::Loge(FMath::Max(Frame.LumaPixels[i], 0.0f) + P.LogEps);
			CpuState[i]     = FVector4f(L, L, L, L);
			CpuLastEvent[i] = P.BaseUs - P.RefractoryUs;
		}
		return;
	}

	const int32 NumChunks = FMath::Clamp(NumPixels / 65536, 1, 16);
	const int32 ChunkSize = FMath::DivideAndRoundUp(NumPixels, NumChunks);

	TArray<TArray<uint64>> ChunkEvents;
	TArray<float>          ChunkMax;
	ChunkEvents.SetNum(NumChunks);
	ChunkMax.Init(0.0f, NumChunks);

	const uint32 MaxPerPixel = FMath::Min(64u, P.RefractoryUs > 0 ? P.DtUs / P.RefractoryUs + 1u : 64u);

	ParallelFor(NumChunks, [&](int32 Chunk) {
		const int32 First = Chunk * ChunkSize;
		const int32 Last  = FMath::Min(First + ChunkSize, NumPixels);

		TArray<uint64>& Local    = ChunkEvents[Chunk];
		float&          LocalMax = ChunkMax[Chunk];

		for (int32 i = First; i < Last; ++i)
		{
			const uint32 X = (uint32)(i % Size.X);
			const uint32 Y = (uint32)(i / Size.X);

			const float I       = FMath::Max(Frame.LumaPixels[i], 0.0f);
			const float Inten01 = FMath::Clamp(I, 0.0f, 1.0f);
			const float L       = FMath::Loge(I + P.LogEps);

			const uint32 PixelKey = X + Y * 65536u;

			FVector4f&  State = CpuState[i];
			float       Ref   = State.X;
			float       Lp1   = State.Y;
			float       Lp2   = State.Z;
			const float LPrev = State.W;

			if (P.CutoffHz > 0.0f)
			{
				const float Alpha = 1.0f - FMath::Exp(-6.283185307f * P.CutoffHz * FMath::Lerp(0.05f, 1.0f, Inten01) * P.Dt);
				Lp1 += Alpha * (L - Lp1);
				Lp2 += Alpha * (Lp1 - Lp2);
			}
			else
			{
				Lp1 = L;
				Lp2 = L;
			}
			const float LCur = Lp2;

			Ref -= P.LeakRateHz * P.ThresholdPos * P.Dt * (0.9f + 0.2f * Hash01(PixelKey, P.Seed, 1u));

			const float Cpos = FMath::Max(P.ThresholdPos + P.ThresholdSigma * Gaussian(PixelKey, P.Seed, 2u), 0.01f);
			const float Cneg = FMath::Max(P.ThresholdNeg + P.ThresholdSigma * Gaussian(PixelKey, P.Seed, 3u), 0.01f);

			uint32 LastEv = CpuLastEvent[i];

			const float dL = LCur - LPrev;
			LocalMax       = FMath::Max(LocalMax, FMath::Abs(dL));

			uint32 Emitted = 0;

			if (dL > 0.0f)
			{
				while (Ref + Cpos <= LCur && Emitted < MaxPerPixel)
				{
					Ref += Cpos;
					const float  Frac   = FMath::Clamp((Ref - LPrev) / dL, 0.0f, 1.0f);
					const uint32 Offset = (uint32)FMath::RoundToInt(Frac * P.DtUs);
					const uint32 TUs    = P.BaseUs + Offset;
					if (TUs - LastEv >= P.RefractoryUs)
					{
						Local.Add(PackEvent(X, Y, true, Offset));
						LastEv = TUs;
					}
					Emitted++;
				}
			}
			else if (dL < 0.0f)
			{
				while (Ref - Cneg >= LCur && Emitted < MaxPerPixel)
				{
					Ref -= Cneg;
					const float  Frac   = FMath::Clamp((Ref - LPrev) / dL, 0.0f, 1.0f);
					const uint32 Offset = (uint32)FMath::RoundToInt(Frac * P.DtUs);
					const uint32 TUs    = P.BaseUs + Offset;
					if (TUs - LastEv >= P.RefractoryUs)
					{
						Local.Add(PackEvent(X, Y, false, Offset));
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
					Rate *= 100.0f;
				}

				const float U = Hash01(PixelKey, P.Seed ^ (P.FrameIndex * 0x85EBCA6Bu), 5u);
				if (U < Rate * P.Dt)
				{
					const float  V      = Hash01(PixelKey, P.Seed ^ (P.FrameIndex * 0xC2B2AE35u), 6u);
					const uint32 Offset = (uint32)(V * P.DtUs);
					const uint32 TUs    = P.BaseUs + Offset;
					if (TUs - LastEv >= P.RefractoryUs)
					{
						Local.Add(PackEvent(X, Y, Hash01(PixelKey, P.Seed ^ P.FrameIndex, 7u) < 0.5f, Offset));
						LastEv = TUs;
					}
				}
			}

			State           = FVector4f(Ref, Lp1, Lp2, LCur);
			CpuLastEvent[i] = LastEv;
		}
	});

	int32 Total = 0;
	for (int32 Chunk = 0; Chunk < NumChunks; ++Chunk)
	{
		Total += ChunkEvents[Chunk].Num();
		OutMaxLogChange = FMath::Max(OutMaxLogChange, ChunkMax[Chunk]);
	}

	OutPacked.Reset(Total);
	for (const TArray<uint64>& Local : ChunkEvents)
	{
		OutPacked.Append(Local);
	}
}

//}

/* PublishFrame() //{ */

void UEventCamera::PublishFrame(TArray<uint64>& Packed, uint32 Count, float MaxLogChange, double TPrev, double TCur)
{
	Algo::Sort(Packed);

	const double Dt          = TCur - TPrev;
	uint32       DroppedHere = Count > (uint32)Packed.Num() ? Count - (uint32)Packed.Num() : 0;
	bLastFrameTruncated      = DroppedHere > 0;

	// a saturated readout keeps a uniform sample of the burst rather than its head
	if (event_camera_config_.MaxEventRateHz > 0.0 && Dt > 0.0)
	{
		const int64 Quota = llround(event_camera_config_.MaxEventRateHz * Dt);
		const int64 N     = Packed.Num();

		if (N > Quota)
		{
			for (int64 j = 0; j < Quota; ++j)
			{
				Packed[j] = Packed[(j * N) / Quota];
			}
			Packed.SetNum((int32)Quota, EAllowShrinking::No);
			DroppedHere += (uint32)(N - Quota);
		}
	}

	// offsets are rounded microseconds; scale by the exact interval so a frame never overruns the next
	const double DtUs      = FMath::Max((double)llround(Dt * 1e6), 1.0);
	const double PerOffset = Dt / DtUs;

	EventsCriticalSection->Lock();

	if (EventBuffer.Num() + Packed.Num() > EVENT_BUFFER_MAX_EVENTS)
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
	LastCount = 0;

	EventsCriticalSection->Lock();
	EventBuffer.Empty();
	Dropped = 0;
	EventsCriticalSection->Unlock();

	return true;
}

//}

/* GetConfig() / SetConfig() //{ */

void UEventCamera::GetConfig(std::stringstream& OutputStream)
{
	Serializable::Drone::GetEventCameraConfig::Response Response(true);
	Response.config = Serializable::Drone::EventCameraConfig{};

	const FEventCameraConfig& C = event_camera_config_;
	auto&                     W = Response.config;

	W.show_debug_camera_ = C.ShowCameraComponent;

	W.offset_x_ = C.Offset.X;
	W.offset_y_ = C.Offset.Y;
	W.offset_z_ = C.Offset.Z;

	W.rotation_pitch_ = C.Orientation.Pitch;
	W.rotation_yaw_   = C.Orientation.Yaw;
	W.rotation_roll_  = C.Orientation.Roll;

	W.fov_    = C.FOVAngle;
	W.width_  = C.Width;
	W.height_ = C.Height;

	W.contrast_threshold_pos_ = C.ContrastThresholdPos;
	W.contrast_threshold_neg_ = C.ContrastThresholdNeg;
	W.use_sim_time_           = C.UseSimTime;

	W.threshold_sigma_      = C.ThresholdSigma;
	W.refractory_period_    = C.RefractoryPeriod;
	W.cutoff_hz_            = C.CutoffHz;
	W.leak_rate_hz_         = C.LeakRateHz;
	W.shot_noise_rate_hz_   = C.ShotNoiseRateHz;
	W.hot_pixel_fraction_   = C.HotPixelFraction;
	W.max_event_rate_hz_    = C.MaxEventRateHz;
	W.max_events_per_frame_ = C.MaxEventsPerFrame;
	W.use_gpu_              = C.UseGpu;
	W.seed_                 = C.Seed;

	Serialization::DeserializeResponse(Response, OutputStream);
}

void UEventCamera::SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream)
{
	Serializable::Drone::SetEventCameraConfig::Request Request;
	Serialization::SerializeRequest(Request, *InputStream);

	const auto& W = Request.config;

	FEventCameraConfig Config;
	Config.ShowCameraComponent = W.show_debug_camera_;

	Config.Offset      = FVector(W.offset_x_, W.offset_y_, W.offset_z_);
	Config.Orientation = FRotator(W.rotation_pitch_, W.rotation_yaw_, W.rotation_roll_);

	Config.FOVAngle = W.fov_;
	Config.Width    = W.width_;
	Config.Height   = W.height_;

	Config.ContrastThresholdPos = W.contrast_threshold_pos_;
	Config.ContrastThresholdNeg = W.contrast_threshold_neg_;
	Config.UseSimTime           = W.use_sim_time_;

	Config.ThresholdSigma    = W.threshold_sigma_;
	Config.RefractoryPeriod  = W.refractory_period_;
	Config.CutoffHz          = W.cutoff_hz_;
	Config.LeakRateHz        = W.leak_rate_hz_;
	Config.ShotNoiseRateHz   = W.shot_noise_rate_hz_;
	Config.HotPixelFraction  = W.hot_pixel_fraction_;
	Config.MaxEventRateHz    = W.max_event_rate_hz_;
	Config.MaxEventsPerFrame = W.max_events_per_frame_;
	Config.UseGpu            = W.use_gpu_;
	Config.Seed              = W.seed_;

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

	const int32 N = EventBuffer.Num();

	Response.base_stamp_ = N > 0 ? EventBuffer[0].Stamp : LastFrameStamp;
	Response.stamp_      = LastFrameStamp;
	Response.dropped_    = Dropped;
	Response.width_      = event_camera_config_.Width;
	Response.height_     = event_camera_config_.Height;

	Response.xyp_.resize(N);
	Response.t_us_.resize(N);

	for (int32 i = 0; i < N; ++i)
	{
		Response.xyp_[i]  = EventBuffer[i].XYP;
		Response.t_us_[i] = static_cast<uint32_t>(FMath::Clamp(llround((EventBuffer[i].Stamp - Response.base_stamp_) * 1e6), 0ll, (long long)0xFFFFFFFFu));
	}

	EventBuffer.Reset();
	Dropped = 0;

	EventsCriticalSection->Unlock();

	Serialization::DeserializeResponse(Response, OutputStream);
}

//}
