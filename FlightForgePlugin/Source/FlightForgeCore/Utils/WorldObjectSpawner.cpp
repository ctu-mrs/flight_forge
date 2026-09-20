#include "WorldObjectSpawner.h"

#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "FlightForgeCore/Segmentation/SegmentationTagger.h"

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

  if (!Resolver->Resolve(Entry.Asset, Config.SearchPaths, Config.ModelPaths, Resolved, World))
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

    int32 Stencil = Entry.Stencil;
    if (Stencil >= 0)
    {
      ApplyStencil(Actor, Stencil);
    }
    else if (const USegmentationTagger* Tagger = World->GetSubsystem<USegmentationTagger>())
    {
      // the tagger already classified the actor when it spawned; record what it decided
      TInlineComponentArray<UPrimitiveComponent*> Primitives(Actor);
      Stencil = Primitives.Num() ? Tagger->ClassOf(Actor, Primitives[0]) : 0;
    }

    const int32 ObjectId = NextObjectId++;
    SpawnedObjects.Add(ObjectId, FSpawnedObjectRecord{Actor, Entry.Asset, Stencil});
    SpawnedIds.Add(ObjectId);
  }

  UE_LOG(LogTemp, Log, TEXT("[WorldObjectSpawner] spawned %d of %d objects"), SpawnedIds.Num(), Config.Objects.Num());

  return SpawnedIds;
}

/* //} */

/* RemoveObject() //{ */

bool UWorldObjectSpawner::RemoveObject(const int32 ObjectId)
{
  FSpawnedObjectRecord Record;
  if (!SpawnedObjects.RemoveAndCopyValue(ObjectId, Record))
  {
    return false;
  }

  if (!IsValid(Record.Actor))
  {
    return false;
  }

  Record.Actor->Destroy();

  return true;
}

/* //} */

/* RemoveAllObjects() //{ */

int32 UWorldObjectSpawner::RemoveAllObjects()
{
  int32 Removed = 0;

  for (const auto& [ObjectId, Record] : SpawnedObjects)
  {
    if (IsValid(Record.Actor))
    {
      Record.Actor->Destroy();
      ++Removed;
    }
  }

  SpawnedObjects.Empty();

  return Removed;
}

/* //} */

/* MoveObject() //{ */

bool UWorldObjectSpawner::MoveObject(const int32 ObjectId, const FVector& Position, const FVector& Orientation, const FVector& Scale,
                                     const FVector& WorldOrigin)
{
  const FSpawnedObjectRecord* Record = SpawnedObjects.Find(ObjectId);
  if (!Record || !IsValid(Record->Actor))
  {
    return false;
  }

  FSpawnObjectEntry Entry;
  Entry.Position    = Position;
  Entry.Orientation = Orientation;
  Entry.Scale       = Scale;

  const FTransform Transform = USpawnConfigUtils::ToUnrealTransform(Entry, FVector::ZeroVector, WorldOrigin);

  return Record->Actor->SetActorTransform(Transform, false, nullptr, ETeleportType::TeleportPhysics);
}

/* //} */

/* ListObjects() //{ */

TArray<FSpawnedObjectState> UWorldObjectSpawner::ListObjects(const FVector& WorldOrigin) const
{
  TArray<FSpawnedObjectState> States;

  for (const auto& [ObjectId, Record] : SpawnedObjects)
  {
    if (!IsValid(Record.Actor))
    {
      continue;
    }

    FSpawnObjectEntry Entry;
    USpawnConfigUtils::FromUnrealTransform(Record.Actor->GetActorTransform(), WorldOrigin, Entry);

    FSpawnedObjectState State;
    State.Id          = ObjectId;
    State.Asset       = Record.Asset;
    State.Stencil     = Record.Stencil;
    State.Position    = Entry.Position;
    State.Orientation = Entry.Orientation;
    State.Scale       = Entry.Scale;

    States.Add(State);
  }

  States.Sort([](const FSpawnedObjectState& A, const FSpawnedObjectState& B) { return A.Id < B.Id; });

  return States;
}

/* //} */

/* ExportScene() //{ */

FString UWorldObjectSpawner::ExportScene(const FVector& WorldOrigin) const
{
  FSpawnConfig Config;

  for (const FSpawnedObjectState& State : ListObjects(WorldOrigin))
  {
    FSpawnObjectEntry Entry;
    Entry.Asset       = State.Asset;
    Entry.Position    = State.Position;
    Entry.Orientation = State.Orientation;
    Entry.Scale       = State.Scale;
    Entry.Stencil     = State.Stencil;

    Config.Objects.Add(Entry);
  }

  return USpawnConfigUtils::WriteSpawnConfig(Config);
}

/* //} */
