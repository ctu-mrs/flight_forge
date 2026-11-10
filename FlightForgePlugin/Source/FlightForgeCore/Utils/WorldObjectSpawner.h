#pragma once

#include <atomic>

#include "CoreMinimal.h"

#include "AssetResolver.h"
#include "SpawnConfig.h"

#include "WorldObjectSpawner.generated.h"

class FObjectLibrary;

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

  /** [m] at scale 1, from a library; poses of such an object name its base, not its origin. */
  UPROPERTY()
  double BaseZ = 0.0;
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

/* FSpawnBatch //{ */

/** Counts one spawn request's on-disk models still importing; the server waits on it off the game thread. */
struct FSpawnBatch
{
  std::atomic<int32> Pending{0};
};

using FSpawnBatchPtr = TSharedPtr<FSpawnBatch, ESPMode::ThreadSafe>;

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

  void SetLibrary(const FObjectLibrary* InLibrary) { Library = InLibrary; }

  /** Spawns every entry, returning the handles of those that were placed; an on-disk model gets its mesh once Batch drains. */
  TArray<int32> SpawnFromConfig(UWorld* World, const FSpawnConfig& Config, const FVector& WorldOrigin, const FSpawnBatchPtr& Batch = nullptr);

  bool HasObject(int32 ObjectId) const;

  AActor* GetActor(int32 ObjectId) const;

  bool RemoveObject(int32 ObjectId);

  int32 RemoveAllObjects();

  /** Repositions a spawned object; the pose is in the config's ROS frame. */
  bool MoveObject(int32 ObjectId, const FVector& Position, const FVector& Orientation, const FVector& Scale, const FVector& WorldOrigin);

  TArray<FSpawnedObjectState> ListObjects(const FVector& WorldOrigin) const;

  /** Serializes the current objects back into a spawn configuration document. */
  FString ExportScene(const FVector& WorldOrigin) const;

private:
  AActor* SpawnEntry(UWorld* World, const FSpawnObjectEntry& Entry, const FSpawnConfig& Config, const FTransform& Transform, FString& OutModelFile);

  void OnModelImported(int32 ObjectId, UStaticMesh* Mesh, bool bExplicitStencil);

  static int32 TaggedStencil(const AActor* Actor);

  static void ApplyStencil(AActor* Actor, int32 StencilValue);

  UPROPERTY()
  TObjectPtr<UAssetResolver> Resolver;

  UPROPERTY()
  TMap<int32, FSpawnedObjectRecord> SpawnedObjects;

  int32 NextObjectId = 0;

  const FObjectLibrary* Library = nullptr;
};

/* //} */
