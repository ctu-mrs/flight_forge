#pragma once

#include "CoreMinimal.h"

#include "AssetResolver.h"
#include "SpawnConfig.h"

#include "WorldObjectSpawner.generated.h"

/* UWorldObjectSpawner //{ */

/**
 * Places the objects named by a spawn configuration into the world and keeps a handle on
 * each one so it can be removed again.
 *
 * Stencil values start at FirstStencilValue so they cannot collide with the values drones
 * assign themselves from their server port.
 */
UCLASS()
class FLIGHTFORGECORE_API UWorldObjectSpawner : public UObject
{
  GENERATED_BODY()

public:
  static constexpr int32 FirstStencilValue = 200;

  /** Spawns every entry, returning the handles of those that were placed. */
  TArray<int32> SpawnFromConfig(UWorld* World, const FSpawnConfig& Config, const FVector& WorldOrigin);

  bool RemoveObject(int32 ObjectId);

  int32 RemoveAllObjects();

private:
  AActor* SpawnEntry(UWorld* World, const FSpawnObjectEntry& Entry, const FSpawnConfig& Config, const FTransform& Transform);

  static void ApplyStencil(AActor* Actor, int32 StencilValue);

  UPROPERTY()
  TObjectPtr<UAssetResolver> Resolver;

  UPROPERTY()
  TMap<int32, TObjectPtr<AActor>> SpawnedObjects;

  int32 NextObjectId = 0;
  int32 NextStencil  = FirstStencilValue;
};

/* //} */
