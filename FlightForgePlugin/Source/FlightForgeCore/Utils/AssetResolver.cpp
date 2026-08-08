#include "AssetResolver.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/Blueprint.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/Actor.h"
#include "HAL/FileManager.h"
#include "Misc/PackageName.h"
#include "Modules/ModuleManager.h"
#include "Misc/Paths.h"

#include "glTFRuntimeAsset.h"
#include "glTFRuntimeFunctionLibrary.h"

namespace
{

/* path helpers //{ */

// Probing deliberately loads quietly: most candidates are expected to miss, and the
// engine would otherwise log an error for every search path that does not hold the asset.
constexpr uint32 ProbeLoadFlags = LOAD_NoWarn | LOAD_Quiet;

FString ToObjectPath(const FString& PackagePath)
{
  if (PackagePath.Contains(TEXT(".")))
  {
    return PackagePath;
  }

  return PackagePath + TEXT(".") + FPackageName::GetShortName(PackagePath);
}

TArray<FString> CandidatePackagePaths(const FString& AssetName, const TArray<FString>& SearchPaths)
{
  TArray<FString> Candidates;

  if (AssetName.StartsWith(TEXT("/")))
  {
    Candidates.Add(AssetName);
    return Candidates;
  }

  for (const FString& SearchPath : SearchPaths)
  {
    if (SearchPath.IsEmpty())
    {
      continue;
    }

    FString Base = SearchPath;
    while (Base.EndsWith(TEXT("/")))
    {
      Base.LeftChopInline(1);
    }

    Candidates.Add(Base + TEXT("/") + AssetName);
  }

  return Candidates;
}

bool IsGltfPath(const FString& Path)
{
  const FString Extension = FPaths::GetExtension(Path, false);
  return Extension.Equals(TEXT("glb"), ESearchCase::IgnoreCase) || Extension.Equals(TEXT("gltf"), ESearchCase::IgnoreCase);
}

/* //} */

}  // namespace

/* FindModelFile() //{ */

bool UAssetResolver::FindModelFile(const FString& AssetName, const TArray<FString>& ModelPaths, FString& OutFilePath)
{
  const auto Accept = [&OutFilePath](const FString& Candidate) {
    if (FPaths::FileExists(Candidate))
    {
      OutFilePath = Candidate;
      return true;
    }
    return false;
  };

  if (IsGltfPath(AssetName))
  {
    if (!FPaths::IsRelative(AssetName) && Accept(AssetName))
    {
      return true;
    }

    for (const FString& ModelPath : ModelPaths)
    {
      if (Accept(FPaths::Combine(ModelPath, AssetName)))
      {
        return true;
      }
    }

    return false;
  }

  for (const FString& ModelPath : ModelPaths)
  {
    for (const TCHAR* Extension : {TEXT(".glb"), TEXT(".gltf")})
    {
      if (Accept(FPaths::Combine(ModelPath, AssetName + Extension)) ||
          Accept(FPaths::Combine(ModelPath, AssetName, AssetName + Extension)))
      {
        return true;
      }
    }
  }

  return false;
}

/* //} */

/* ImportGltf() //{ */

UStaticMesh* UAssetResolver::ImportGltf(const FString& FilePath)
{
  if (const TObjectPtr<UStaticMesh>* Cached = ImportedMeshes.Find(FilePath))
  {
    return *Cached;
  }

  const FglTFRuntimeConfig LoaderConfig;

  UglTFRuntimeAsset* Asset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromFilename(FilePath, false, LoaderConfig);
  if (!Asset)
  {
    UE_LOG(LogTemp, Error, TEXT("[AssetResolver] '%s' could not be parsed as glTF"), *FilePath);
    return nullptr;
  }

  // An empty node name merges the whole first scene into one mesh, so a model split
  // across several nodes still arrives as a single spawnable asset.
  const FglTFRuntimeStaticMeshConfig StaticMeshConfig;

  UStaticMesh* Mesh = Asset->LoadStaticMeshRecursive(FString(), TArray<FString>(), StaticMeshConfig);
  if (!Mesh)
  {
    UE_LOG(LogTemp, Error, TEXT("[AssetResolver] no static mesh could be built from '%s'"), *FilePath);
    return nullptr;
  }

  ImportedMeshes.Add(FilePath, Mesh);

  return Mesh;
}

/* //} */

/* Resolve() //{ */

bool UAssetResolver::Resolve(const FString& AssetName, const TArray<FString>& SearchPaths, const TArray<FString>& ModelPaths,
                             FResolvedAsset& OutResolved)
{
  OutResolved = FResolvedAsset();

  if (AssetName.IsEmpty())
  {
    return false;
  }

  const TArray<FString> Candidates = CandidatePackagePaths(AssetName, SearchPaths);

  if (!IsGltfPath(AssetName))
  {
    for (const FString& Candidate : Candidates)
    {
      const FString ObjectPath = ToObjectPath(Candidate);

      if (UClass* ActorClass = StaticLoadClass(AActor::StaticClass(), nullptr, *(ObjectPath + TEXT("_C")), nullptr, ProbeLoadFlags))
      {
        OutResolved.Kind       = EResolvedAssetKind::ActorClass;
        OutResolved.ActorClass = ActorClass;
        return true;
      }
    }

    for (const FString& Candidate : Candidates)
    {
      if (UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, *ToObjectPath(Candidate), nullptr, ProbeLoadFlags))
      {
        OutResolved.Kind = EResolvedAssetKind::StaticMesh;
        OutResolved.Mesh = Mesh;
        return true;
      }
    }
  }

  FString ModelFilePath;
  if (FindModelFile(AssetName, ModelPaths, ModelFilePath))
  {
    if (UStaticMesh* Mesh = ImportGltf(ModelFilePath))
    {
      OutResolved.Kind = EResolvedAssetKind::StaticMesh;
      OutResolved.Mesh = Mesh;
      return true;
    }
  }

  UE_LOG(LogTemp, Error, TEXT("[AssetResolver] could not resolve '%s'; tried packages [%s] and model dirs [%s]"), *AssetName,
         *FString::Join(Candidates, TEXT(", ")), *FString::Join(ModelPaths, TEXT(", ")));

  return false;
}

/* //} */

/* ListSpawnableAssets() //{ */

TArray<FString> UAssetResolver::ListSpawnableAssets(const FString& PathPrefix, const FString& NameFilter, const TArray<FString>& ModelPaths,
                                                    const int32 MaxResults, bool& bOutTruncated)
{
  TArray<FString> Names;
  bOutTruncated = false;

  const auto Matches = [&NameFilter](const FString& Name) { return NameFilter.IsEmpty() || Name.Contains(NameFilter, ESearchCase::IgnoreCase); };

  const auto Add = [&Names, &bOutTruncated, MaxResults](const FString& Name) {
    if (Names.Num() >= MaxResults)
    {
      bOutTruncated = true;
      return false;
    }
    Names.Add(Name);
    return true;
  };

  const IAssetRegistry& Registry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();

  FARFilter Filter;
  Filter.PackagePaths.Add(PathPrefix.IsEmpty() ? FName(TEXT("/Game")) : FName(*PathPrefix));
  Filter.bRecursivePaths = true;
  Filter.ClassPaths.Add(UStaticMesh::StaticClass()->GetClassPathName());
  Filter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());

  TArray<FAssetData> Assets;
  Registry.GetAssets(Filter, Assets);

  for (const FAssetData& Asset : Assets)
  {
    if (!Matches(Asset.AssetName.ToString()))
    {
      continue;
    }

    if (!Add(Asset.GetSoftObjectPath().ToString()))
    {
      return Names;
    }
  }

  // On-disk glTF models are listed by their bare name, exactly as the 'asset' field takes them.
  for (const FString& ModelPath : ModelPaths)
  {
    TArray<FString> Files;
    IFileManager::Get().FindFilesRecursive(Files, *ModelPath, TEXT("*.glb"), true, false);
    IFileManager::Get().FindFilesRecursive(Files, *ModelPath, TEXT("*.gltf"), true, false, false);

    for (const FString& File : Files)
    {
      const FString Name = FPaths::GetBaseFilename(File);
      if (!Matches(Name))
      {
        continue;
      }

      if (!Add(Name))
      {
        return Names;
      }
    }
  }

  return Names;
}

/* //} */
