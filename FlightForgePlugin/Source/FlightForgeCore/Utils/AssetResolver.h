#pragma once

#include "CoreMinimal.h"
#include "Containers/Ticker.h"

#include "AssetResolver.generated.h"

class FglTFRuntimeParser;
class UStaticMesh;

DECLARE_DELEGATE_OneParam(FOnGltfImported, UStaticMesh*);

/* EResolvedAssetKind //{ */

UENUM()
enum class EResolvedAssetKind : uint8
{
  None,
  /** A cooked Blueprint actor class, spawned as-is. */
  ActorClass,
  /** A cooked static mesh, spawned on a plain mesh actor. */
  StaticMesh,
  /** An on-disk glTF, imported with ImportGltfAsync. */
  ModelFile,
};

/* //} */

/* FResolvedAsset //{ */

struct FResolvedAsset
{
  EResolvedAssetKind Kind = EResolvedAssetKind::None;

  UClass*      ActorClass = nullptr;
  UStaticMesh* Mesh       = nullptr;
  FString      FilePath;

  bool IsValid() const
  {
    return Kind != EResolvedAssetKind::None;
  }
};

/* //} */

/* UGltfImport //{ */

// one on-disk model on its way in: parsed, built and collision-cooked off the game thread
UCLASS()
class UGltfImport : public UObject
{
  GENERATED_BODY()

public:
  virtual void BeginDestroy() override;

  void Begin(UObject* MeshOuter);
  void Add(FOnGltfImported Done);
  void Fail(const TCHAR* Reason);

  bool IsRunning() const { return Start > 0.0 && !bDone; }
  bool IsDone() const { return bDone; }

  FString Path;
  double  Start = 0.0;

  TFunction<void(UGltfImport&)> OnFinished;

  UPROPERTY()
  TObjectPtr<UStaticMesh> Mesh;

private:
  UFUNCTION()
  void OnMesh(UStaticMesh* StaticMesh);

  void OnCooked(bool bSuccess);
  void Finish(UStaticMesh* Result);

  bool bDone = false;

  TSharedPtr<FglTFRuntimeParser> Parser;

  TArray<FOnGltfImported> Waiters;
};

/* //} */

/* UAssetResolver //{ */

/**
 * Turns an asset name from a spawn configuration into something spawnable, deciding
 * whether the asset is packed into the build or has to be loaded from disk. This is what
 * lets a packaged binary place content that was never cooked into it.
 *
 * Resolution order is: cooked Blueprint class, then cooked static mesh, then an on-disk
 * glTF imported through glTFRuntime. Cooked assets win so that a shipped asset is never
 * silently shadowed by a stray file on disk.
 */
UCLASS()
class FLIGHTFORGECORE_API UAssetResolver : public UObject
{
  GENERATED_BODY()

public:
  virtual void BeginDestroy() override;

  bool Resolve(const FString& AssetName, const TArray<FString>& SearchPaths, const TArray<FString>& ModelPaths, FResolvedAsset& OutResolved);

  /** Imports a whole glTF scene as a single static mesh. Results are cached by file path. */
  UStaticMesh* ImportGltf(const FString& FilePath, UObject* MeshOuter);

  /** ImportGltf without stalling the game thread; Done runs on the game thread, with nullptr on failure. */
  void ImportGltfAsync(const FString& FilePath, UObject* MeshOuter, FOnGltfImported Done);

  /** Locates a .glb/.gltf for an asset name, accepting both <dir>/<name>.glb and <dir>/<name>/<name>.glb. */
  static bool FindModelFile(const FString& AssetName, const TArray<FString>& ModelPaths, FString& OutFilePath);

  /**
   * Enumerates what the spawn tool could place: cooked static meshes and Blueprints under
   * PathPrefix (object paths), plus the bare names of .glb/.gltf files under ModelPaths.
   * NameFilter is a case-insensitive substring match; bOutTruncated is set when MaxResults
   * cut the listing short.
   */
  static TArray<FString> ListSpawnableAssets(const FString& PathPrefix, const FString& NameFilter, const TArray<FString>& ModelPaths, int32 MaxResults,
                                             bool& bOutTruncated);

private:
  void StartQueued();
  bool ExpireStalled(float DeltaTime);

  UPROPERTY()
  TMap<FString, TObjectPtr<UStaticMesh>> ImportedMeshes;

  UPROPERTY()
  TMap<FString, TObjectPtr<UGltfImport>> Imports;

  TArray<TPair<TWeakObjectPtr<UGltfImport>, TWeakObjectPtr<UObject>>> Queued;

  int32 Running = 0;

  FTSTicker::FDelegateHandle StallTicker;
};

/* //} */
