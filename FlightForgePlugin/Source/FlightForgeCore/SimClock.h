#pragma once

#include <atomic>

#include "CoreMinimal.h"

// clock mode written by the game mode, read by sensors
struct FLIGHTFORGECORE_API FFlightForgeSimClock
{
	static std::atomic<bool>   bLockstep;
	static std::atomic<double> FixedDt;         // 0 = variable timestep
	static std::atomic<double> AdaptiveMinDt;   // 0 = no adaptive shrinking
};
