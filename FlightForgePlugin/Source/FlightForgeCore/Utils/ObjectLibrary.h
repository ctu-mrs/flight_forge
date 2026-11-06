#pragma once

#include "CoreMinimal.h"

/* FLibraryObject //{ */

struct FLibraryObject
{
  FString Library;
  FString Id;

  /** What a spawn config's asset field takes: lib:<library>/<id>, or the object path of a project asset. */
  FString SpawnName;

  /** An object path or an absolute path of a glTF file. */
  FString Asset;

  FString Class;
  FString Description;
  FString License;
  FString Thumbnail;

  double Scale   = 1.0;
  double HeightM = 0.0;

  /** [m] at scale 1, how far the asset's origin sits above its lowest point. */
  double BaseZ = 0.0;

  bool bRuntime = true;
};

/* //} */

/* FObjectLibrary //{ */

/**
 * Everything that can be placed: the project's assets plus runtime libraries, each a manifest
 * or a directory of glTF files.
 */
class FLIGHTFORGECORE_API FObjectLibrary
{
public:
  /** Loads a manifest file or scans a directory, replacing a library of the same name. */
  bool Load(const FString& Path, int32& OutCount, FString& OutError);

  /** Pass the spawnable names of the project, as UAssetResolver lists them. */
  void SetProjectAssets(const TArray<FString>& ObjectPaths);

  const FLibraryObject* Find(const FString& SpawnName) const;

  /** Case-insensitive filter over id, class and description; an empty library or class matches all. */
  TArray<FLibraryObject> Search(const FString& Library, const FString& Class, const FString& Filter, int32 Offset, int32 Limit, int32& OutTotal) const;

  static constexpr const TCHAR* Prefix = TEXT("lib:");

private:
  bool LoadManifest(const FString& FilePath, TArray<FLibraryObject>& OutObjects, FString& OutError) const;

  void Replace(const FString& Library, TArray<FLibraryObject>&& NewObjects);

  TArray<FLibraryObject> Objects;
  TMap<FString, int32>   BySpawnName;
};

/* //} */
