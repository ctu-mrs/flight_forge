#pragma once

#include "CoreMinimal.h"

// exits the process when the render thread leaves a queued command unrun too long (a lost GPU never signals its fences)
struct FLIGHTFORGECORE_API FGpuWatchdog
{
	static void Start();
	static void Stop();

	// game thread
	static void Probe();
	static void FlushRenderingCommands();

	static bool    IsStalled();
	static FString StallMessage();
};
