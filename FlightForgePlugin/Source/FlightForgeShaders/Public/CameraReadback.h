#pragma once

#include <atomic>

#include "CoreMinimal.h"
#include "RHIGPUReadback.h"

/* FCameraReadback //{ */

struct FCameraReadback
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

	FIntPoint         Size = FIntPoint::ZeroValue;
	double            Stamp = 0.0;

	FVector           Omega = FVector::ZeroVector;

	std::atomic<bool> bResolveIssued{false};
	std::atomic<bool> bCopied{false};
};

/* //} */
