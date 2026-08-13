#pragma once

#include <atomic>

#include "CoreMinimal.h"
#include "RHIGPUReadback.h"

/* FCameraReadback //{ */

// One in-flight GPU->CPU copy of a render target. The game thread enqueues the
// copy right after the capture and keeps ticking; render-thread commands fill
// Pixels and flip bCopied when the transfer lands, typically a frame later.
// Shared ownership: the render commands hold a reference, so the slot outlives
// the component even if it is torn down mid-flight.
//
// Lives in the shaders module rather than beside the cameras because the SegId
// mesh pass writes into one of these, and that pass has to load at
// PostConfigInit to register its shader types - long before the camera
// components exist.
struct FCameraReadback
{
	TUniquePtr<FRHIGPUTextureReadback> Readback;
	TArray<FColor>    Pixels;

	// R16F targets (depth) resolve here instead of Pixels
	bool              bFloat16 = false;
	TArray<FFloat16>  Float16Pixels;

	FIntPoint         Size = FIntPoint::ZeroValue;
	double            Stamp = 0.0;

	// camera-frame angular velocity at capture time (x right, y down, z forward;
	// rad/s), carried with the frame for the rolling-shutter warp
	FVector           Omega = FVector::ZeroVector;

	std::atomic<bool> bResolveIssued{false};
	std::atomic<bool> bCopied{false};
};

/* //} */
