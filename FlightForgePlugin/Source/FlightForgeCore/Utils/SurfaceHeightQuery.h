#pragma once

#include "CoreMinimal.h"

/* FSurfaceHeightQuery //{ */

class FLIGHTFORGECORE_API FSurfaceHeightQuery
{
public:
	// game thread; points in the ground frame, m, a NaN z starts at the top of the world; heights in m, NaN where nothing is below
	static void Batch(UWorld* World, const FVector& GroundOrigin, const TArray<FVector>& PointsM, bool bIgnoreRobots, TArray<double>& OutHeightsM);
};

/* //} */
