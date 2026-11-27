#include "ObjectLibrary.h"

#include "Algo/AllOf.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#include "yaml.h"

#include "YamlUtils.h"

namespace
{

constexpr const TCHAR* ProjectLibrary = TEXT("project");

FString ReadString(const YAML::Node& Node, const char* Key)
{
  return Node[Key] ? ToFString(Node[Key].as<std::string>("")) : FString();
}

double ReadDouble(const YAML::Node& Node, const char* Key, const double Default)
{
  return Node[Key] ? Node[Key].as<double>(Default) : Default;
}

// a relative path starts at the library's root, an object path stays as it is
FString Rooted(const FString& Path, const FString& Root)
{
  return Path.IsEmpty() || Path.StartsWith(TEXT("/")) ? Path : FPaths::Combine(Root, Path);
}

}  // namespace

/* Load() //{ */

bool FObjectLibrary::Load(const FString& Path, int32& OutCount, FString& OutError)
{
  OutCount = 0;
  OutError.Empty();

  TArray<FLibraryObject> Loaded;

  if (FPaths::DirectoryExists(Path))
  {
    const FString Root    = Path.EndsWith(TEXT("/")) ? Path.LeftChop(1) : Path;
    const FString Library = FPaths::GetCleanFilename(Root);

    TArray<FString> Files;
    IFileManager::Get().FindFilesRecursive(Files, *Root, TEXT("*.glb"), true, false);
    IFileManager::Get().FindFilesRecursive(Files, *Root, TEXT("*.gltf"), true, false, false);

    for (const FString& File : Files)
    {
      FString Relative = File;
      FPaths::MakePathRelativeTo(Relative, *(Root + TEXT("/")));

      FLibraryObject Object;
      Object.Library = Library;
      Object.Id      = FPaths::Combine(FPaths::GetPath(Relative), FPaths::GetBaseFilename(Relative));
      Object.Asset   = File;
      Object.Class   = FPaths::GetCleanFilename(FPaths::GetPath(Relative));
      Loaded.Add(MoveTemp(Object));
    }
  }
  else if (!LoadManifest(Path, Loaded, OutError))
  {
    return false;
  }

  if (Loaded.IsEmpty())
  {
    OutError = FString::Printf(TEXT("no objects in %s"), *Path);
    return false;
  }

  OutCount = Loaded.Num();

  const FString Library = Loaded[0].Library;
  Replace(Library, MoveTemp(Loaded));

  UE_LOG(LogTemp, Log, TEXT("[ObjectLibrary] %s: %d objects from %s"), *Library, OutCount, *Path);

  return true;
}

/* //} */

/* LoadManifest() //{ */

bool FObjectLibrary::LoadManifest(const FString& FilePath, TArray<FLibraryObject>& OutObjects, FString& OutError) const
{
  FString Text;

  if (!FFileHelper::LoadFileToString(Text, *FilePath))
  {
    OutError = FString::Printf(TEXT("could not read %s"), *FilePath);
    return false;
  }

  try
  {
    const YAML::Node Root = YAML::Load(TCHAR_TO_UTF8(*Text));

    if (!Root || !Root.IsMap() || !Root["objects"] || !Root["objects"].IsSequence())
    {
      OutError = FString::Printf(TEXT("%s has no list of objects"), *FilePath);
      return false;
    }

    const FString Name = Root["name"] ? ReadString(Root, "name") : FPaths::GetBaseFilename(FilePath);
    const FString Base = Root["root"] ? ReadString(Root, "root") : FPaths::GetPath(FilePath);

    for (const auto& Node : Root["objects"])
    {
      FLibraryObject Object;

      Object.Library     = Name;
      Object.Id          = ReadString(Node, "id");
      Object.Asset       = Rooted(ReadString(Node, "asset"), Base);
      Object.Class       = ReadString(Node, "class");
      Object.Description = ReadString(Node, "description");
      Object.License     = ReadString(Node, "license");
      Object.Thumbnail   = Rooted(ReadString(Node, "thumbnail"), Base);
      Object.Scale       = ReadDouble(Node, "scale", 1.0);
      Object.HeightM     = ReadDouble(Node, "height", 0.0);
      Object.BaseZ       = ReadDouble(Node, "base_z", 0.0);

      if (Object.Id.IsEmpty() || Object.Asset.IsEmpty())
      {
        OutError = FString::Printf(TEXT("%s: entry %d needs an id and an asset"), *FilePath, OutObjects.Num());
        return false;
      }

      OutObjects.Add(MoveTemp(Object));
    }
  }
  catch (const std::exception& Exception)
  {
    OutError = FString::Printf(TEXT("%s: %s"), *FilePath, UTF8_TO_TCHAR(Exception.what()));
    return false;
  }

  return true;
}

/* //} */

/* SetProjectAssets() //{ */

void FObjectLibrary::SetProjectAssets(const TArray<FString>& ObjectPaths)
{
  TArray<FLibraryObject> Project;

  for (const FString& Path : ObjectPaths)
  {
    if (!Path.StartsWith(TEXT("/")))
    {
      continue;
    }

    FLibraryObject Object;
    Object.Library   = ProjectLibrary;
    Object.Id        = Path;
    Object.SpawnName = Path;
    Object.Asset     = Path;
    Object.Class     = FPaths::GetCleanFilename(FPaths::GetPath(Path));
    Object.bRuntime  = false;
    Project.Add(MoveTemp(Object));
  }

  Replace(ProjectLibrary, MoveTemp(Project));
}

/* //} */

/* Replace() //{ */

void FObjectLibrary::Replace(const FString& Library, TArray<FLibraryObject>&& NewObjects)
{
  Objects.RemoveAll([&Library](const FLibraryObject& Object) { return Object.Library == Library; });

  for (FLibraryObject& Object : NewObjects)
  {
    if (Object.SpawnName.IsEmpty())
    {
      Object.SpawnName = FString(Prefix) + Object.Library + TEXT("/") + Object.Id;
    }
  }

  Objects.Append(MoveTemp(NewObjects));

  BySpawnName.Reset();

  for (int32 Index = 0; Index < Objects.Num(); Index++)
  {
    BySpawnName.Add(Objects[Index].SpawnName, Index);
  }

  Revision++;
}

/* //} */

/* Find() //{ */

const FLibraryObject* FObjectLibrary::Find(const FString& SpawnName) const
{
  const int32* Index = BySpawnName.Find(SpawnName);

  return Index ? &Objects[*Index] : nullptr;
}

/* //} */

/* Search() //{ */

TArray<FLibraryObject> FObjectLibrary::Search(const FString& Library, const FString& Class, const FString& Filter, const int32 Offset, const int32 Limit,
                                              int32& OutTotal) const
{
  TArray<FLibraryObject> Found;

  OutTotal = 0;

  TArray<FString> Words;
  Filter.ParseIntoArrayWS(Words);

  for (const FLibraryObject& Object : Objects)
  {
    if ((!Library.IsEmpty() && Object.Library != Library) || (!Class.IsEmpty() && !Object.Class.Equals(Class, ESearchCase::IgnoreCase)))
    {
      continue;
    }

    const bool bMatches = Algo::AllOf(Words, [&Object](const FString& Word) {
      return Object.Id.Contains(Word) || Object.Class.Contains(Word) || Object.Description.Contains(Word);
    });

    if (!bMatches)
    {
      continue;
    }

    if (OutTotal >= Offset && Found.Num() < Limit)
    {
      Found.Add(Object);
    }

    OutTotal++;
  }

  return Found;
}

/* //} */

/* Libraries() //{ */

TArray<FString> FObjectLibrary::Libraries() const
{
  TArray<FString> Names;

  for (const FLibraryObject& Object : Objects)
  {
    Names.AddUnique(Object.Library);
  }

  return Names;
}

/* //} */

/* Classes() //{ */

TArray<TPair<FString, int32>> FObjectLibrary::Classes(const FString& Library) const
{
  TMap<FString, int32> Counts;

  for (const FLibraryObject& Object : Objects)
  {
    if (Library.IsEmpty() || Object.Library == Library)
    {
      Counts.FindOrAdd(Object.Class)++;
    }
  }

  Counts.KeySort([](const FString& A, const FString& B) { return A < B; });

  return Counts.Array();
}

/* //} */
