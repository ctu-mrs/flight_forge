#pragma once

#include "CoreMinimal.h"

/* FDistanceFieldQuery //{ */

/**
 * Exact Euclidean distance from query points to the nearest collision surface,
 * by sphere-overlap binary search - undiscretized ESDF ground truth. Distances
 * are measured against collision geometry, the same surfaces the ray-traced
 * sensors hit. Runs on the game thread and blocks it for the duration; meant
 * for offline evaluation between flights.
 */
class FLIGHTFORGECORE_API FDistanceFieldQuery
{
public:
	/**
	 * Points are packed xyz in the scene frame (right-handed metres relative to
	 * WorldOrigin); distances come back in metres, max_radius when truncated and
	 * 0 when the point is inside geometry. TerrainOnly restricts hits to
	 * Landscape or actors tagged "terrain".
	 */
	static bool Batch(UWorld* World, const FVector& WorldOrigin, const TArray<float>& QueryPoints, double MaxRadiusM, double ToleranceM, bool bTerrainOnly,
	                  TArray<float>& OutDistances);
};

/* //} */
