#include "WorldObjectSpawner.h"

#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

/* ApplyStencil() //{ */

void UWorldObjectSpawner::ApplyStencil(AActor* Actor, const int32 StencilValue)
{
  // Every primitive is stencilled, not just the root: Blueprint actors routinely have a
  // bare USceneComponent root with the geometry hanging off it as children.
  TArray<UPrimitiveComponent*> Primitives;
  Actor->GetComponents<UPrimitiveComponent>(Primitives);

  for (UPrimitiveComponent* Primitive : Primitives)
  {
    Primitive->SetRenderCustomDepth(true);
    Primitive->SetCustomDepthStencilValue(StencilValue);
  }
}

/* //} */

/* SpawnEntry() //{ */

AActor* UWorldObjectSpawner::SpawnEntry(UWorld* World, const FSpawnObjectEntry& Entry, const FSpawnConfig& Config, const FTransform& Transform)
{
  FResolvedAsset Resolved;
  if (!Resolver->Resolve(Entry.Asset, Config.SearchPaths, Config.ModelPaths, Resolved))
  {
    return nullptr;
  }

  FActorSpawnParameters SpawnParameters;
  SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

  if (Resolved.Kind == EResolvedAssetKind::ActorClass)
  {
    return World->SpawnActor<AActor>(Resolved.ActorClass, Transform, SpawnParameters);
  }

  AStaticMeshActor* MeshActor = World->SpawnActorDeferred<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Transform, nullptr, nullptr,
                                                                           ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
  if (!MeshActor)
  {
    return nullptr;
  }

  // Static mobility is the default and would reject the transform we spawned it with.
  UStaticMeshComponent* MeshComponent = MeshActor->GetStaticMeshComponent();
  MeshComponent->SetMobility(EComponentMobility::Movable);
  MeshComponent->SetStaticMesh(Resolved.Mesh);

  MeshActor->FinishSpawning(Transform);

  return MeshActor;
}

/* //} */

/* SpawnFromConfig() //{ */

TArray<int32> UWorldObjectSpawner::SpawnFromConfig(UWorld* World, const FSpawnConfig& Config, const FVector& WorldOrigin)
{
  TArray<int32> SpawnedIds;

  if (!World)
  {
    UE_LOG(LogTemp, Error, TEXT("[WorldObjectSpawner] no world to spawn into"));
    return SpawnedIds;
  }

  if (!Resolver)
  {
    Resolver = NewObject<UAssetResolver>(this);
  }

  const FVector Origin = Config.bRelativeToWorldOrigin ? WorldOrigin : FVector::ZeroVector;

  for (const FSpawnObjectEntry& Entry : Config.Objects)
  {
    const FTransform Transform = USpawnConfigUtils::ToUnrealTransform(Entry, Config.OriginOffset, Origin);

    AActor* Actor = SpawnEntry(World, Entry, Config, Transform);
    if (!Actor)
    {
      UE_LOG(LogTemp, Warning, TEXT("[WorldObjectSpawner] skipped '%s'"), *Entry.Asset);
      continue;
    }

    ApplyStencil(Actor, Entry.Stencil >= 0 ? Entry.Stencil : NextStencil++);

    const int32 ObjectId = NextObjectId++;
    SpawnedObjects.Add(ObjectId, Actor);
    SpawnedIds.Add(ObjectId);
  }

  UE_LOG(LogTemp, Log, TEXT("[WorldObjectSpawner] spawned %d of %d objects"), SpawnedIds.Num(), Config.Objects.Num());

  return SpawnedIds;
}

/* //} */

/* RemoveObject() //{ */

bool UWorldObjectSpawner::RemoveObject(const int32 ObjectId)
{
  TObjectPtr<AActor> Actor;
  if (!SpawnedObjects.RemoveAndCopyValue(ObjectId, Actor))
  {
    return false;
  }

  if (!IsValid(Actor))
  {
    return false;
  }

  Actor->Destroy();

  return true;
}

/* //} */

/* RemoveAllObjects() //{ */

int32 UWorldObjectSpawner::RemoveAllObjects()
{
  int32 Removed = 0;

  for (const auto& [ObjectId, Actor] : SpawnedObjects)
  {
    if (IsValid(Actor))
    {
      Actor->Destroy();
      ++Removed;
    }
  }

  SpawnedObjects.Empty();

  return Removed;
}

/* //} */
