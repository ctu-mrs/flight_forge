#pragma once

#include "CoreMinimal.h"

#include "AssetResolver.generated.h"

class UStaticMesh;

/* EResolvedAssetKind //{ */

UENUM()
enum class EResolvedAssetKind : uint8
{
  None,
  /** A cooked Blueprint actor class, spawned as-is. */
  ActorClass,
  /** A static mesh, either cooked or imported from disk, spawned on a plain mesh actor. */
  StaticMesh,
};

/* //} */

/* FResolvedAsset //{ */

struct FResolvedAsset
{
  EResolvedAssetKind Kind = EResolvedAssetKind::None;

  UClass*      ActorClass = nullptr;
  UStaticMesh* Mesh       = nullptr;

  bool IsValid() const
  {
    return Kind != EResolvedAssetKind::None;
  }
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
  bool Resolve(const FString& AssetName, const TArray<FString>& SearchPaths, const TArray<FString>& ModelPaths, FResolvedAsset& OutResolved);

  /** Imports a whole glTF scene as a single static mesh. Results are cached by file path. */
  UStaticMesh* ImportGltf(const FString& FilePath);

  /** Locates a .glb/.gltf for an asset name, accepting both <dir>/<name>.glb and <dir>/<name>/<name>.glb. */
  static bool FindModelFile(const FString& AssetName, const TArray<FString>& ModelPaths, FString& OutFilePath);

private:
  UPROPERTY()
  TMap<FString, TObjectPtr<UStaticMesh>> ImportedMeshes;
};

/* //} */
