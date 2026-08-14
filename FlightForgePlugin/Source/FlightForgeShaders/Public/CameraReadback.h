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

	FIntPoint         Size = FIntPoint::ZeroValue;
	double            Stamp = 0.0;

	FVector           Omega = FVector::ZeroVector;

	std::atomic<bool> bResolveIssued{false};
	std::atomic<bool> bCopied{false};
};

/* //} */
