#pragma once

#include "CoreMinimal.h"

/* FWorldMeshResult //{ */

struct FWorldMeshResult
{
	// xyz triplets in the scene frame: right-handed metres relative to the world origin
	TArray<float>  Vertices;
	TArray<uint32> Indices;

	bool  bTruncated  = false;
	int32 SourceCount = 0;
};

/* //} */

/* FWorldMeshExtractor //{ */

/**
 * Collects the world's static geometry as a triangle soup for ground-truth use
 * (map evaluation, sim-to-real comparison). Reads LOD0 of every visible static
 * mesh component plus landscape heightfields; drones are excluded, spawned
 * scene objects are included. BSP geometry is not exported.
 */
class FLIGHTFORGECORE_API FWorldMeshExtractor
{
public:
	/** SceneBounds is in the scene frame (metres, right-handed); an invalid box disables filtering. */
	static bool Extract(UWorld* World, const FVector& WorldOrigin, const FBox& SceneBounds, int32 MaxTriangles, FWorldMeshResult& Out);
};

/* //} */
