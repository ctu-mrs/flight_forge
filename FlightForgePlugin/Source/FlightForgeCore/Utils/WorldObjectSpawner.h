#pragma once

#include "CoreMinimal.h"

#include "AssetResolver.h"
#include "SpawnConfig.h"

#include "WorldObjectSpawner.generated.h"

/* FSpawnedObjectRecord //{ */

USTRUCT()
struct FSpawnedObjectRecord
{
  GENERATED_BODY()

  UPROPERTY()
  TObjectPtr<AActor> Actor;

  /** The asset name as it appeared in the spawn config, kept for list/export. */
  UPROPERTY()
  FString Asset;

  /** The class id the object carries (from the tagger, or the config's explicit stencil). */
  UPROPERTY()
  int32 Stencil = -1;
};

/* //} */

/* FSpawnedObjectState //{ */

/** A snapshot of one spawned object with its CURRENT pose, back in the config's ROS frame. */
struct FSpawnedObjectState
{
  int32   Id;
  FString Asset;
  int32   Stencil;

  FVector Position;
  FVector Orientation;
  FVector Scale;
};

/* //} */

/* UWorldObjectSpawner //{ */

/**
 * Places the objects named by a spawn configuration into the world and keeps a handle on
 * each one so it can be removed again.
 *
 * An object's segmentation class comes from the tagger like every other actor's; a config
 * entry may still force a stencil (class id) explicitly, which is what export writes back.
 */
UCLASS()
class FLIGHTFORGECORE_API UWorldObjectSpawner : public UObject
{
  GENERATED_BODY()

public:

  /** Spawns every entry, returning the handles of those that were placed. */
  TArray<int32> SpawnFromConfig(UWorld* World, const FSpawnConfig& Config, const FVector& WorldOrigin);

  bool RemoveObject(int32 ObjectId);

  int32 RemoveAllObjects();

  /** Repositions a spawned object; the pose is in the config's ROS frame. */
  bool MoveObject(int32 ObjectId, const FVector& Position, const FVector& Orientation, const FVector& Scale, const FVector& WorldOrigin);

  TArray<FSpawnedObjectState> ListObjects(const FVector& WorldOrigin) const;

  /** Serializes the current objects back into a spawn configuration document. */
  FString ExportScene(const FVector& WorldOrigin) const;

private:
  AActor* SpawnEntry(UWorld* World, const FSpawnObjectEntry& Entry, const FSpawnConfig& Config, const FTransform& Transform);

  static void ApplyStencil(AActor* Actor, int32 StencilValue);

  UPROPERTY()
  TObjectPtr<UAssetResolver> Resolver;

  UPROPERTY()
  TMap<int32, FSpawnedObjectRecord> SpawnedObjects;

  int32 NextObjectId = 0;
};

/* //} */
