#include "SpawnConfig.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#include "yaml.h"

#include "YamlUtils.h"
#include "WireFrame.h"

namespace
{

/* yaml readers //{ */

double ReadScalar(const YAML::Node& Node, const char* Key, const double Default)
{
  return (Node && Node[Key]) ? Node[Key].as<double>(Default) : Default;
}

// Accepts either a mapping keyed by the three given names or a three-element sequence,
// so that [x, y, z] and {x: .., y: .., z: ..} are both valid spelling of a vector.
FVector ReadVector(const YAML::Node& Node, const char* A, const char* B, const char* C, const FVector& Default)
{
  if (!Node)
  {
    return Default;
  }

  if (Node.IsSequence() && Node.size() >= 3)
  {
    return FVector(Node[0].as<double>(Default.X), Node[1].as<double>(Default.Y), Node[2].as<double>(Default.Z));
  }

  if (Node.IsMap())
  {
    return FVector(ReadScalar(Node, A, Default.X), ReadScalar(Node, B, Default.Y), ReadScalar(Node, C, Default.Z));
  }

  // A lone scalar is a uniform value, which is what makes "scale: 2" work.
  if (Node.IsScalar())
  {
    const double Uniform = Node.as<double>(Default.X);
    return FVector(Uniform, Uniform, Uniform);
  }

  return Default;
}

TArray<FString> ReadStringArray(const YAML::Node& Node)
{
  TArray<FString> Out;

  if (Node && Node.IsSequence())
  {
    for (const auto& Element : Node)
    {
      Out.Add(ToFString(Element.as<std::string>("")));
    }
  }

  return Out;
}

/* //} */

}  // namespace

/* ParseSpawnConfig() //{ */

bool USpawnConfigUtils::ParseSpawnConfig(const FString& YamlText, FSpawnConfig& OutConfig, FString& OutError)
{
  OutConfig = FSpawnConfig();
  OutError.Empty();

  try
  {
    const YAML::Node Root = YAML::Load(TCHAR_TO_UTF8(*YamlText));

    if (!Root || !Root.IsMap())
    {
      OutError = TEXT("the document root is not a mapping");
      return false;
    }

    if (!Root["objects"] || !Root["objects"].IsSequence())
    {
      OutError = TEXT("no 'objects' sequence found");
      return false;
    }

    OutConfig.SearchPaths  = ReadStringArray(Root["search_paths"]);
    OutConfig.ModelPaths   = ReadStringArray(Root["model_paths"]);
    OutConfig.OriginOffset = ReadVector(Root["origin_offset"], "x", "y", "z", FVector::ZeroVector);

    if (Root["frame"])
    {
      const FString Frame = ToFString(Root["frame"].as<std::string>("ground"));

      // reinterpreting the old frames silently would move a scene by 0.92 m or the whole world origin
      if (Frame.Equals(TEXT("origin"), ESearchCase::IgnoreCase))
      {
        OutError = TEXT("frame 'origin' is no longer accepted: use 'ground' (z = 0 is the floor, 0.9212 m below the UAV PlayerStart) and lower the poses");
        return false;
      }
      if (Frame.Equals(TEXT("world"), ESearchCase::IgnoreCase))
      {
        OutError = TEXT("frame 'world' is no longer accepted: use 'unreal' (absolute Unreal cm and rotator) or 'ground'");
        return false;
      }
      if (!Frame.Equals(TEXT("ground"), ESearchCase::IgnoreCase) && !Frame.Equals(TEXT("unreal"), ESearchCase::IgnoreCase))
      {
        OutError = FString::Printf(TEXT("unknown frame '%s', expected 'ground' or 'unreal'"), *Frame);
        return false;
      }

      OutConfig.bUnrealFrame = Frame.Equals(TEXT("unreal"), ESearchCase::IgnoreCase);
    }

    int32 EntryIndex = 0;
    for (const auto& Node : Root["objects"])
    {
      FSpawnObjectEntry Entry;

      Entry.Asset = ToFString(Node["asset"] ? Node["asset"].as<std::string>("") : "");
      if (Entry.Asset.IsEmpty())
      {
        OutError = FString::Printf(TEXT("object %d has no 'asset'"), EntryIndex);
        return false;
      }

      Entry.Position    = ReadVector(Node["position"], "x", "y", "z", FVector::ZeroVector);
      Entry.Orientation = ReadVector(Node["orientation"], "roll", "pitch", "yaw", FVector::ZeroVector);
      Entry.Scale       = ReadVector(Node["scale"], "x", "y", "z", FVector::OneVector);
      Entry.bHasScale   = bool(Node["scale"]);
      Entry.Stencil     = Node["stencil"] ? Node["stencil"].as<int32>(-1) : -1;
      Entry.Class       = ToFString(Node["class"] ? Node["class"].as<std::string>("") : "");

      OutConfig.Objects.Add(Entry);
      ++EntryIndex;
    }
  }
  catch (const YAML::Exception& Exception)
  {
    OutError = ToFString(Exception.what());
    return false;
  }

  return true;
}

/* //} */

/* LoadSpawnConfigFile() //{ */

bool USpawnConfigUtils::LoadSpawnConfigFile(const FString& FilePath, FSpawnConfig& OutConfig, FString& OutError)
{
  if (!FPaths::FileExists(FilePath))
  {
    OutError = FString::Printf(TEXT("'%s' does not exist"), *FilePath);
    return false;
  }

  FString FileContent;
  if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
  {
    OutError = FString::Printf(TEXT("'%s' could not be read"), *FilePath);
    return false;
  }

  if (!ParseSpawnConfig(FileContent, OutConfig, OutError))
  {
    OutError = FString::Printf(TEXT("'%s': %s"), *FilePath, *OutError);
    return false;
  }

  // Relative model directories are resolved against the config file, so that a config
  // and the models it names can be moved around together.
  const FString ConfigDir = FPaths::GetPath(FilePath);
  for (FString& ModelPath : OutConfig.ModelPaths)
  {
    if (FPaths::IsRelative(ModelPath))
    {
      ModelPath = FPaths::Combine(ConfigDir, ModelPath);
    }
  }

  if (OutConfig.ModelPaths.IsEmpty())
  {
    OutConfig.ModelPaths.Add(ConfigDir);
  }

  return true;
}

/* //} */

/* ToUnrealTransform() //{ */

FTransform USpawnConfigUtils::ToUnrealTransform(const FSpawnObjectEntry& Entry, const FVector& OriginOffset, const bool bUnrealFrame, const FVector& GroundOrigin)
{
  const FVector Position = Entry.Position + OriginOffset;

  if (bUnrealFrame)
  {
    return FTransform(FRotator(Entry.Orientation.Y, Entry.Orientation.Z, Entry.Orientation.X).Quaternion(), Position, Entry.Scale);
  }

  const FRotator Rotation(-Entry.Orientation.Y, -Entry.Orientation.Z, Entry.Orientation.X);

  return FTransform(Rotation.Quaternion(), FWireFrame::FromGround(Position, GroundOrigin), Entry.Scale);
}

/* //} */

/* FromUnrealTransform() //{ */

void USpawnConfigUtils::FromUnrealTransform(const FTransform& Transform, const FVector& GroundOrigin, FSpawnObjectEntry& OutEntry)
{
  OutEntry.Position = FWireFrame::ToGround(Transform.GetLocation(), GroundOrigin);

  const FRotator Rotation = Transform.GetRotation().Rotator();

  OutEntry.Orientation = FVector(Rotation.Roll, -Rotation.Pitch, -Rotation.Yaw);

  OutEntry.Scale = Transform.GetScale3D();
}

/* //} */

/* WriteSpawnConfig() //{ */

FString USpawnConfigUtils::WriteSpawnConfig(const FSpawnConfig& Config)
{
  YAML::Emitter Out;

  const auto EmitVector = [&Out](const char* Key, const FVector& Vector) {
    Out << YAML::EmitterManip::Key << Key << YAML::EmitterManip::Value << YAML::EmitterManip::Flow << YAML::EmitterManip::BeginSeq << Vector.X << Vector.Y << Vector.Z << YAML::EmitterManip::EndSeq;
  };

  Out << YAML::EmitterManip::BeginMap;

  if (!Config.SearchPaths.IsEmpty())
  {
    Out << YAML::EmitterManip::Key << "search_paths" << YAML::EmitterManip::Value << YAML::EmitterManip::BeginSeq;
    for (const FString& Path : Config.SearchPaths)
    {
      Out << std::string(TCHAR_TO_UTF8(*Path));
    }
    Out << YAML::EmitterManip::EndSeq;
  }

  if (!Config.ModelPaths.IsEmpty())
  {
    Out << YAML::EmitterManip::Key << "model_paths" << YAML::EmitterManip::Value << YAML::EmitterManip::BeginSeq;
    for (const FString& Path : Config.ModelPaths)
    {
      Out << std::string(TCHAR_TO_UTF8(*Path));
    }
    Out << YAML::EmitterManip::EndSeq;
  }

  if (!Config.OriginOffset.IsNearlyZero())
  {
    EmitVector("origin_offset", Config.OriginOffset);
  }

  Out << YAML::EmitterManip::Key << "frame" << YAML::EmitterManip::Value << (Config.bUnrealFrame ? "unreal" : "ground");

  Out << YAML::EmitterManip::Key << "objects" << YAML::EmitterManip::Value << YAML::EmitterManip::BeginSeq;

  for (const FSpawnObjectEntry& Entry : Config.Objects)
  {
    Out << YAML::EmitterManip::BeginMap;
    Out << YAML::EmitterManip::Key << "asset" << YAML::EmitterManip::Value << std::string(TCHAR_TO_UTF8(*Entry.Asset));

    EmitVector("position", Entry.Position);
    EmitVector("orientation", Entry.Orientation);

    if (!Entry.Scale.Equals(FVector::OneVector))
    {
      EmitVector("scale", Entry.Scale);
    }

    if (Entry.Stencil >= 0)
    {
      Out << YAML::EmitterManip::Key << "stencil" << YAML::EmitterManip::Value << Entry.Stencil;
    }

    if (!Entry.Class.IsEmpty())
    {
      Out << YAML::EmitterManip::Key << "class" << YAML::EmitterManip::Value << std::string(TCHAR_TO_UTF8(*Entry.Class));
    }

    Out << YAML::EmitterManip::EndMap;
  }

  Out << YAML::EmitterManip::EndSeq;
  Out << YAML::EmitterManip::EndMap;

  return FString(UTF8_TO_TCHAR(Out.c_str()));
}

/* //} */
