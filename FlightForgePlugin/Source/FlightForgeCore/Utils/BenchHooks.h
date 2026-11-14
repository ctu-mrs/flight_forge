#pragma once

#include "CoreMinimal.h"

/* FBenchHooks //{ */

// for tools/bench_cameras.py: FlightForge.Bench.StatsInterval > 0 logs mean frame timings; -FlightForgeBenchCommands=<file> runs console lines appended to it
class FBenchHooks
{
public:
	static void Start();
	static void Stop();
};

/* //} */
