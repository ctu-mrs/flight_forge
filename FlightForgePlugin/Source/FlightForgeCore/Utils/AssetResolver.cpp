#include "AssetResolver.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/Blueprint.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/Actor.h"
#include "HAL/FileManager.h"
#include "Misc/PackageName.h"
#include "Modules/ModuleManager.h"
#include "Misc/Paths.h"
#include "Async/Async.h"
#include "PhysicsEngine/BodySetup.h"

#include "glTFRuntimeAsset.h"
#include "glTFRuntimeFunctionLibrary.h"
#include "glTFRuntimeParser.h"

namespace
{

/* path helpers //{ */

// Probing deliberately loads quietly: most candidates are expected to miss, and the
// engine would otherwise log an error for every search path that does not hold the asset.
constexpr uint32 ProbeLoadFlags = LOAD_NoWarn | LOAD_Quiet;

// a safety net: a hung import would otherwise hold its spawn reply
constexpr double ImportTimeoutS = 120.0;

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

/* CombineSceneMeshes() //{ */

// LoadStaticMeshRecursiveAsync, but reporting failure: glTFRuntime's own returns without calling back
const TCHAR* CombineSceneMeshes(FglTFRuntimeParser& Parser, FglTFRuntimeMeshLOD& OutCombined)
{
  FglTFRuntimeScene Scene;
  if (!Parser.LoadScene(0, Scene))
  {
    return TEXT("has no scene");
  }

  TArray<FglTFRuntimeNode> Nodes;
  for (const int32 NodeIndex : Scene.RootNodesIndices)
  {
    if (!Parser.LoadNodesRecursive(NodeIndex, Nodes))
    {
      return TEXT("has a broken node tree");
    }
  }

  for (const FglTFRuntimeNode& Node : Nodes)
  {
    if (Node.MeshIndex == INDEX_NONE)
    {
      continue;
    }

    // a mesh with line or point primitives does not load; the rest of the model still can
    FglTFRuntimeMeshLOD LOD;
    if (!Parser.LoadMeshAsRuntimeLOD(Node.MeshIndex, LOD, FglTFRuntimeMaterialsConfig()))
    {
      continue;
    }

    FTransform       Transform = Node.Transform;
    FglTFRuntimeNode Parent    = Node;
    while (Parent.ParentIndex != INDEX_NONE)
    {
      if (!Parser.LoadNode(Parent.ParentIndex, Parent))
      {
        return TEXT("has a broken node tree");
      }
      Transform *= Parent.Transform;
    }

    for (FglTFRuntimePrimitive& Primitive : LOD.Primitives)
    {
      OutCombined.Primitives.Add(MoveTemp(Primitive));
      OutCombined.AdditionalTransforms.Add(Transform);
    }
  }

  return OutCombined.Primitives.IsEmpty() ? TEXT("has no meshes that load") : nullptr;
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

UStaticMesh* UAssetResolver::ImportGltf(const FString& FilePath, UObject* MeshOuter)
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

  FglTFRuntimeStaticMeshConfig StaticMeshConfig;

  // collision cooks only with CPU access and a game-world Outer; the flag alone is silently ignored
  StaticMeshConfig.bBuildComplexCollision = true;
  StaticMeshConfig.CollisionComplexity    = ECollisionTraceFlag::CTF_UseComplexAsSimple;
  StaticMeshConfig.bAllowCPUAccess        = true;
  StaticMeshConfig.Outer                  = MeshOuter;

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

/* ImportGltfAsync() //{ */

void UAssetResolver::ImportGltfAsync(const FString& FilePath, UObject* MeshOuter, FOnGltfImported Done)
{
  if (const TObjectPtr<UStaticMesh>* Cached = ImportedMeshes.Find(FilePath))
  {
    Done.ExecuteIfBound(*Cached);
    return;
  }

  TObjectPtr<UGltfImport>& Import = Imports.FindOrAdd(FilePath);
  if (!Import)
  {
    Import       = NewObject<UGltfImport>(this);
    Import->Path = FilePath;

    TWeakObjectPtr<UAssetResolver> WeakThis(this);
    Import->OnFinished = [WeakThis](UGltfImport& Finished) {
      UAssetResolver* Self = WeakThis.Get();
      if (!Self)
      {
        return;
      }

      --Self->Running;
      if (Finished.Mesh)
      {
        Self->ImportedMeshes.Add(Finished.Path, Finished.Mesh);
      }
      Self->StartQueued();
    };

    Queued.Emplace(Import, MeshOuter);
  }

  Import->Add(MoveTemp(Done));

  StartQueued();
}

/* //} */

/* StartQueued() / ExpireStalled() //{ */

void UAssetResolver::StartQueued()
{
  const int32 MaxRunning = FMath::Clamp(FPlatformMisc::NumberOfCores() / 2, 2, 8);

  while (Running < MaxRunning && !Queued.IsEmpty())
  {
    const auto [Import, Outer] = Queued[0];
    Queued.RemoveAt(0);

    if (Import.IsValid())
    {
      ++Running;
      Import->Begin(Outer.Get());
    }
  }

  if (Running > 0 && !StallTicker.IsValid())
  {
    StallTicker = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UAssetResolver::ExpireStalled), 1.0f);
  }
}

bool UAssetResolver::ExpireStalled(float)
{
  const double Now = FPlatformTime::Seconds();

  TArray<UGltfImport*> Stalled;
  for (const auto& [Path, Import] : Imports)
  {
    if (Import && Import->IsRunning() && Now - Import->Start > ImportTimeoutS)
    {
      Stalled.Add(Import);
    }
  }

  for (UGltfImport* Import : Stalled)
  {
    Import->Fail(TEXT("timed out"));
  }

  if (Running > 0)
  {
    return true;
  }

  StallTicker.Reset();
  return false;
}

void UAssetResolver::BeginDestroy()
{
  if (StallTicker.IsValid())
  {
    FTSTicker::GetCoreTicker().RemoveTicker(StallTicker);
    StallTicker.Reset();
  }

  Super::BeginDestroy();
}

/* //} */

/* UGltfImport //{ */

void UGltfImport::BeginDestroy()
{
  Finish(nullptr);

  Super::BeginDestroy();
}

void UGltfImport::Begin(UObject* MeshOuter)
{
  Start = FPlatformTime::Seconds();

  TWeakObjectPtr<UGltfImport> WeakThis(this);
  TWeakObjectPtr<UObject>     WeakOuter(MeshOuter);
  const FglTFRuntimeConfig    LoaderConfig;

  Async(EAsyncExecution::Thread, [WeakThis, WeakOuter, FilePath = Path, LoaderConfig]() {
    TSharedPtr<FglTFRuntimeParser> Parser = FglTFRuntimeParser::FromFilename(FilePath, LoaderConfig);
    FglTFRuntimeMeshLOD            Combined;
    const TCHAR*                   Error = Parser.IsValid() ? CombineSceneMeshes(*Parser, Combined) : TEXT("could not be parsed as glTF");

    AsyncTask(ENamedThreads::GameThread, [WeakThis, WeakOuter, Parser, Combined = MoveTemp(Combined), Error]() {
      UGltfImport* Self = WeakThis.Get();
      if (!Self || Self->bDone)
      {
        return;
      }

      if (Error)
      {
        Self->Fail(Error);
        return;
      }

      Self->Parser = Parser;

      // collision is cooked in OnMesh, off the game thread; glTFRuntime would cook it synchronously
      FglTFRuntimeStaticMeshConfig StaticMeshConfig;
      StaticMeshConfig.bAllowCPUAccess = true;
      StaticMeshConfig.Outer           = WeakOuter.Get();

      FglTFRuntimeStaticMeshAsync Done;
      Done.BindDynamic(Self, &UGltfImport::OnMesh);
      Parser->LoadStaticMeshFromRuntimeLODsAsync({Combined}, Done, StaticMeshConfig);
    });
  });
}

void UGltfImport::Add(FOnGltfImported Done)
{
  if (bDone)
  {
    Done.ExecuteIfBound(Mesh);
    return;
  }

  Waiters.Add(MoveTemp(Done));
}

void UGltfImport::OnMesh(UStaticMesh* StaticMesh)
{
  if (bDone)
  {
    return;
  }

  if (!StaticMesh)
  {
    Fail(TEXT("built no static mesh"));
    return;
  }

  Mesh = StaticMesh;

  UBodySetup* BodySetup = Mesh->GetBodySetup();
  if (!BodySetup)
  {
    Finish(Mesh);
    return;
  }

  BodySetup->CollisionTraceFlag             = ECollisionTraceFlag::CTF_UseComplexAsSimple;
  BodySetup->bNeverNeedsCookedCollisionData = false;
  BodySetup->InvalidatePhysicsData();
  BodySetup->CreatePhysicsMeshesAsync(FOnAsyncPhysicsCookFinished::CreateUObject(this, &UGltfImport::OnCooked));
}

void UGltfImport::OnCooked(const bool bSuccess)
{
  if (bDone)
  {
    return;
  }

  if (!bSuccess)
  {
    UE_LOG(LogTemp, Warning, TEXT("[AssetResolver] '%s' has no collision: cooking failed"), *Path);
  }

  Finish(Mesh);
}

void UGltfImport::Fail(const TCHAR* Reason)
{
  UE_LOG(LogTemp, Error, TEXT("[AssetResolver] '%s' %s"), *Path, Reason);
  Mesh = nullptr;
  Finish(nullptr);
}

void UGltfImport::Finish(UStaticMesh* Result)
{
  if (bDone)
  {
    return;
  }

  bDone = true;

  if (Result)
  {
    UE_LOG(LogTemp, Log, TEXT("[AssetResolver] %s imported in %.2f s"), *FPaths::GetCleanFilename(Path), FPlatformTime::Seconds() - Start);
  }

  for (FOnGltfImported& Done : Waiters)
  {
    Done.ExecuteIfBound(Result);
  }
  Waiters.Empty();

  if (OnFinished)
  {
    OnFinished(*this);
  }
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

  if (FindModelFile(AssetName, ModelPaths, OutResolved.FilePath))
  {
    OutResolved.Kind = EResolvedAssetKind::ModelFile;
    return true;
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
  Filter.ClassPaths.Add(UBlueprintGeneratedClass::StaticClass()->GetClassPathName());  // a cooked Blueprint has only this

  TArray<FAssetData> Assets;
  Registry.GetAssets(Filter, Assets);

  for (const FAssetData& Asset : Assets)
  {
    // a cooked level script is a BlueprintGeneratedClass too
    if (!Matches(Asset.AssetName.ToString()) || Asset.HasAnyPackageFlags(PKG_ContainsMap))
    {
      continue;
    }

    // Resolve appends the _C itself
    FString Path = Asset.GetSoftObjectPath().ToString();
    Path.RemoveFromEnd(TEXT("_C"), ESearchCase::CaseSensitive);

    if (!Add(Path))
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
