#pragma once

#include <atomic>

#include "CoreMinimal.h"
#include "RHIGPUReadback.h"

/* FCameraReadback //{ */

struct FLIGHTFORGESHADERS_API FCameraReadback
{
	TUniquePtr<FRHIGPUTextureReadback> Readback;
	TArray<FColor>    Pixels;

	bool              bFloat16 = false;
	TArray<FFloat16>  Float16Pixels;

	bool              bLuma        = false;  // linear BT.709 luminance from a float target
	bool              bLumaFloat32 = false;
	TArray<float>     LumaPixels;

	// the HDR capture sources skip the display gamma, so the resolve encodes to sRGB itself
	bool bEncodeSrgb = false;

	// read a float RGBA target and encode straight to 8 bit, so the encode never sees an
	// already-quantised value
	bool bHalfRgba  = false;
	bool bFloatRgba = false;

	bool                 bRawFloat4 = false;
	TArray<FLinearColor> Float4Pixels;

	FIntPoint         Size = FIntPoint::ZeroValue;
	double            Stamp = 0.0;

	FVector           Omega = FVector::ZeroVector;

	std::atomic<bool> bResolveIssued{false};
	std::atomic<bool> bResolveQueued{false};
	std::atomic<bool> bCopied{false};

	// manual-reset; triggered once Pixels are host-side, so a waiting server thread wakes without a game tick in between
	FEvent* Copied = nullptr;

	FCameraReadback();
	~FCameraReadback();
};

// copies the staging buffer into the slot's pixel array; render thread, after the fence signalled
FLIGHTFORGESHADERS_API void ResolveReadback_RenderThread(FCameraReadback& Slot);

// polls the slot every render frame and resolves it as soon as the GPU is done; render thread
FLIGHTFORGESHADERS_API void WatchReadback_RenderThread(const TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>& Slot);

// any thread: resolves on the render thread if the GPU is done; polling or locking a readback anywhere else races the immediate command list
FLIGHTFORGESHADERS_API void RequestResolve(const TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>& Slot);

/* //} */
