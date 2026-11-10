#include "WorldObjectSpawner.h"

#include "ObjectLibrary.h"

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

AActor* UWorldObjectSpawner::SpawnEntry(UWorld* World, const FSpawnObjectEntry& Entry, const FSpawnConfig& Config, const FTransform& Transform,
                                        FString& OutModelFile)
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

  OutModelFile = Resolved.FilePath;

  return MeshActor;
}

/* //} */

/* OnModelImported() / TaggedStencil() //{ */

void UWorldObjectSpawner::OnModelImported(const int32 ObjectId, UStaticMesh* Mesh, const bool bExplicitStencil)
{
  FSpawnedObjectRecord* Record = SpawnedObjects.Find(ObjectId);
  if (!Record || !IsValid(Record->Actor))
  {
    return;
  }

  AStaticMeshActor* MeshActor = Cast<AStaticMeshActor>(Record->Actor);

  if (!Mesh || !MeshActor)
  {
    UE_LOG(LogTemp, Warning, TEXT("[WorldObjectSpawner] removed '%s': its model did not import"), *Record->Asset);
    Record->Actor->Destroy();
    SpawnedObjects.Remove(ObjectId);
    return;
  }

  MeshActor->GetStaticMeshComponent()->SetStaticMesh(Mesh);

  if (!bExplicitStencil)
  {
    if (USegmentationTagger* Tagger = MeshActor->GetWorld()->GetSubsystem<USegmentationTagger>())
    {
      Tagger->TagActor(MeshActor);
    }
    Record->Stencil = TaggedStencil(MeshActor);
  }
}

int32 UWorldObjectSpawner::TaggedStencil(const AActor* Actor)
{
  const USegmentationTagger* Tagger = Actor->GetWorld()->GetSubsystem<USegmentationTagger>();
  if (!Tagger)
  {
    return -1;
  }

  TInlineComponentArray<UPrimitiveComponent*> Primitives(Actor);
  return Primitives.Num() ? Tagger->ClassOf(Actor, Primitives[0]) : 0;
}

/* //} */

/* SpawnFromConfig() //{ */

TArray<int32> UWorldObjectSpawner::SpawnFromConfig(UWorld* World, const FSpawnConfig& Config, const FVector& WorldOrigin, const FSpawnBatchPtr& Batch)
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

  FVector Origin = Config.bRelativeToWorldOrigin ? WorldOrigin : FVector::ZeroVector;

  if (Config.bRelativeToWorldOrigin && Config.bGroundFrame)
  {
    Origin.Z -= PlayerStartHeightCm;
  }

  for (const FSpawnObjectEntry& Requested : Config.Objects)
  {
    FSpawnObjectEntry Entry = Requested;
    double            BaseZ = 0.0;

    if (const FLibraryObject* Object = Library ? Library->Find(Requested.Asset) : nullptr)
    {
      Entry.Asset = Object->Asset;
      BaseZ       = Object->BaseZ;

      if (!Requested.bHasScale)
      {
        Entry.Scale = FVector(Object->Scale);
      }

      Entry.Position.Z += BaseZ * Entry.Scale.Z;
    }
    else if (Requested.Asset.StartsWith(FObjectLibrary::Prefix))
    {
      UE_LOG(LogTemp, Warning, TEXT("[WorldObjectSpawner] no library object '%s'"), *Requested.Asset);
      continue;
    }

    const FTransform Transform = USpawnConfigUtils::ToUnrealTransform(Entry, Config.OriginOffset, Origin);

    FString ModelFile;
    AActor* Actor = SpawnEntry(World, Entry, Config, Transform, ModelFile);
    if (!Actor)
    {
      UE_LOG(LogTemp, Warning, TEXT("[WorldObjectSpawner] skipped '%s'"), *Entry.Asset);
      continue;
    }

    USegmentationTagger* Tagger = World->GetSubsystem<USegmentationTagger>();

    int32 Stencil = Entry.Stencil;
    if (Stencil < 0 && Tagger && !Entry.Class.IsEmpty())
    {
      Stencil = Tagger->ClassIdOf(Entry.Class);
      if (Stencil <= 0 || Stencil > 255)
      {
        UE_LOG(LogTemp, Warning, TEXT("[WorldObjectSpawner] '%s': class '%s' is not in the class tree"), *Entry.Asset, *Entry.Class);
        Stencil = -1;
      }
    }

    const bool bExplicitStencil = Stencil >= 0;
    if (bExplicitStencil)
    {
      ApplyStencil(Actor, Stencil);
      if (Tagger)
      {
        Tagger->SetExplicitClass(Actor, Stencil);
      }
    }
    else
    {
      Stencil = TaggedStencil(Actor);
    }

    const int32 ObjectId = NextObjectId++;
    SpawnedObjects.Add(ObjectId, FSpawnedObjectRecord{Actor, Requested.Asset, Stencil, BaseZ});
    SpawnedIds.Add(ObjectId);

    if (!ModelFile.IsEmpty())
    {
      if (Batch)
      {
        ++Batch->Pending;
      }

      TWeakObjectPtr<UWorldObjectSpawner> WeakThis(this);

      Resolver->ImportGltfAsync(ModelFile, World, FOnGltfImported::CreateLambda([WeakThis, ObjectId, Batch, bExplicitStencil](UStaticMesh* Mesh) {
                                  if (UWorldObjectSpawner* Self = WeakThis.Get())
                                  {
                                    Self->OnModelImported(ObjectId, Mesh, bExplicitStencil);
                                  }
                                  if (Batch)
                                  {
                                    --Batch->Pending;
                                  }
                                }));
    }
  }

  // a model already known to be broken fails inside ImportGltfAsync
  SpawnedIds.RemoveAll([this](const int32 Id) { return !HasObject(Id); });

  UE_LOG(LogTemp, Log, TEXT("[WorldObjectSpawner] spawned %d of %d objects"), SpawnedIds.Num(), Config.Objects.Num());

  return SpawnedIds;
}

/* //} */

/* HasObject() //{ */

bool UWorldObjectSpawner::HasObject(const int32 ObjectId) const
{
  const FSpawnedObjectRecord* Record = SpawnedObjects.Find(ObjectId);
  return Record && IsValid(Record->Actor);
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

  Entry.Position.Z += Record->BaseZ * Scale.Z;

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
    State.Position    = Entry.Position - FVector(0, 0, Record.BaseZ * Entry.Scale.Z);
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

  Config.bGroundFrame = true;

  const USegmentationTagger* Tagger = nullptr;
  for (const auto& [ObjectId, Record] : SpawnedObjects)
  {
    if (IsValid(Record.Actor) && Record.Actor->GetWorld())
    {
      Tagger = Record.Actor->GetWorld()->GetSubsystem<USegmentationTagger>();
      break;
    }
  }

  for (const FSpawnedObjectState& State : ListObjects(WorldOrigin - FVector(0, 0, PlayerStartHeightCm)))
  {
    FSpawnObjectEntry Entry;
    Entry.Asset       = State.Asset;
    Entry.Position    = State.Position;
    Entry.Orientation = State.Orientation;
    Entry.Scale       = State.Scale;
    Entry.Stencil     = State.Stencil;
    Entry.Class       = Tagger && State.Stencil > 0 ? Tagger->ClassPathOf(State.Stencil) : FString();

    Config.Objects.Add(Entry);
  }

  return USpawnConfigUtils::WriteSpawnConfig(Config);
}

/* //} */
