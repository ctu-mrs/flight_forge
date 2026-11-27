#pragma once

#include <atomic>

#include "CoreMinimal.h"
#include "RHIGPUReadback.h"
#include "RenderGraphResources.h"

/* FEventGenParams //{ */

// one frame's uniforms; BaseUs wraps, only differences are taken from it
struct FEventGenParams
{
	FIntPoint Size = FIntPoint::ZeroValue;

	float  Dt         = 0.0f;
	uint32 BaseUs     = 0;
	uint32 DtUs       = 0;
	uint32 FrameIndex = 0;
	uint32 Seed       = 0;
	bool   bSeedState = false;

	float  ThresholdPos     = 0.2f;
	float  ThresholdNeg     = 0.2f;
	float  ThresholdSigma   = 0.03f;
	uint32 RefractoryUs     = 100;
	float  CutoffHz         = 300.0f;
	float  LeakRateHz       = 0.1f;
	float  ShotNoiseRateHz  = 0.1f;
	float  HotPixelFraction = 1e-4f;
	float  LogEps           = 1e-3f;

	uint32 MaxEvents  = 1u << 20;  // this frame's buffer; the camera sizes it from the last frames
	uint32 CopyEvents = 16384;     // first readback; a frame that kept more is copied again in full
	uint32 Budget     = 1u << 20;  // events delivered at most, the rest thinned uniformly
	bool   bBudgetIsGuard = false;  // the buffer, not the rate ceiling, set Budget; CPU side only
};

//}

/* FEventReadback //{ */

// one frame in flight; Packed = offset_us << 32 | x | y << 16 | brighter << 31, so a plain sort is time order
struct FEventReadback
{
	TUniquePtr<FRHIGPUBufferReadback> Header;
	TUniquePtr<FRHIGPUBufferReadback> Events;
	TRefCountPtr<FRDGPooledBuffer>    Buffer;  // the frame's own events, held until they are read back

	uint32 Capacity       = 0;
	uint32 CopiedCapacity = 0;
	bool   bHeaderRead    = false;

	// filled by the resolve
	uint32         Count        = 0;  // as the pixels fired, before thinning
	uint32         Kept         = 0;  // as appended, may exceed Capacity
	float          MaxLogChange = 0.0f;
	TArray<uint64> Packed;

	double TPrev          = 0.0;
	double TCur           = 0.0;
	uint32 FrameIndex     = 0;
	bool   bBudgetIsGuard = false;

	std::atomic<bool> bResolveIssued{false};
	std::atomic<bool> bResolveQueued{false};
	std::atomic<bool> bCopied{false};

	// render thread only
	bool IsReady() const
	{
		return Header && Events && Header->IsReady() && Events->IsReady();
	}
};

//}

/* FEventGpuState //{ */

// per-camera persistent GPU state, render thread only
struct FEventGpuState
{
	TRefCountPtr<IPooledRenderTarget> State;
	TRefCountPtr<IPooledRenderTarget> LastEvent;

	FIntPoint Size = FIntPoint::ZeroValue;
};

//}

namespace FlightForgeEventGen
{
// runs the model on Input and enqueues the readbacks; a size change reseeds instead of emitting
FLIGHTFORGESHADERS_API void Dispatch_RenderThread(FRHICommandListImmediate& RHICmdList, FRHITexture* Input, FEventGpuState& State, FEventGenParams Params,
                                                   const TSharedPtr<FEventReadback, ESPMode::ThreadSafe>& Slot);

// copies a frame that outgrew its first readback again, else unpacks it and sets bCopied
FLIGHTFORGESHADERS_API void Resolve_RenderThread(FRHICommandListImmediate& RHICmdList, const TSharedPtr<FEventReadback, ESPMode::ThreadSafe>& Slot);
}  // namespace FlightForgeEventGen
