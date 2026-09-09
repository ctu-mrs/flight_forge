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

	uint32 MaxEvents  = 1u << 20;  // GPU append capacity; CopyEvents of it come back this frame
	uint32 CopyEvents = 16384;
};

//}

/* FEventReadback //{ */

// one frame in flight; Packed = offset_us << 32 | x | y << 16 | brighter << 31, so a plain sort is time order
struct FEventReadback
{
	TUniquePtr<FRHIGPUBufferReadback> Header;
	TUniquePtr<FRHIGPUBufferReadback> Events;

	uint32 CopiedCapacity = 0;

	// filled by the resolve
	uint32         Count        = 0;  // as appended, may exceed CopiedCapacity
	float          MaxLogChange = 0.0f;
	TArray<uint64> Packed;

	double TPrev      = 0.0;
	double TCur       = 0.0;
	uint32 FrameIndex = 0;

	std::atomic<bool> bResolveIssued{false};
	std::atomic<bool> bCopied{false};

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
	TRefCountPtr<FRDGPooledBuffer>    Events;

	FIntPoint Size     = FIntPoint::ZeroValue;
	uint32    Capacity = 0;
};

//}

namespace FlightForgeEventGen
{
// runs the model on Input and enqueues the readbacks; a size change reseeds instead of emitting
FLIGHTFORGESHADERS_API void Dispatch_RenderThread(FRHICommandListImmediate& RHICmdList, FRHITexture* Input, FEventGpuState& State, FEventGenParams Params,
                                                   const TSharedPtr<FEventReadback, ESPMode::ThreadSafe>& Slot);

FLIGHTFORGESHADERS_API void Resolve_RenderThread(const TSharedPtr<FEventReadback, ESPMode::ThreadSafe>& Slot);
}  // namespace FlightForgeEventGen
