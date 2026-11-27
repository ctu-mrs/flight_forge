#include "ExternalModelConfig.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#include "yaml.h"

#include "YamlUtils.h"

namespace
{

/* helpers //{ */

FVector ReadTriplet(const YAML::Node& Node, const FVector& Default)
{
  if (!Node || !Node.IsSequence() || Node.size() < 3)
  {
    return Default;
  }

  return FVector(Node[0].as<double>(Default.X), Node[1].as<double>(Default.Y), Node[2].as<double>(Default.Z));
}

// The propeller list is either under a "propellers" key or is the first nested sequence
// of a bare top-level sequence.
YAML::Node FindPropellerList(const YAML::Node& Root)
{
  if (Root.IsMap())
  {
    return Root["propellers"];
  }

  if (Root.IsSequence())
  {
    for (const auto& Element : Root)
    {
      if (Element.IsSequence())
      {
        return Element;
      }
    }
  }

  return YAML::Node(YAML::NodeType::Undefined);
}

/* //} */

}  // namespace

/* Load() //{ */

bool ExternalModelConfig::Load(const FString& YamlPath, FExternalModelConfig& OutConfig, FString& OutError)
{
  OutConfig = FExternalModelConfig();
  OutError.Empty();

  if (!FPaths::FileExists(YamlPath))
  {
    OutError = FString::Printf(TEXT("'%s' does not exist"), *YamlPath);
    return false;
  }

  FString FileContent;
  if (!FFileHelper::LoadFileToString(FileContent, *YamlPath))
  {
    OutError = FString::Printf(TEXT("'%s' could not be read"), *YamlPath);
    return false;
  }

  try
  {
    const YAML::Node Root = YAML::Load(TCHAR_TO_UTF8(*FileContent));

    if (Root.IsMap() && Root["propeller_mesh"] && Root["propeller_mesh"].IsScalar())
    {
      OutConfig.PropellerMeshPath = ToFString(Root["propeller_mesh"].as<std::string>(""));
    }

    const YAML::Node Propellers = FindPropellerList(Root);
    if (!Propellers || !Propellers.IsSequence())
    {
      UE_LOG(LogTemp, Warning, TEXT("[ExternalModelConfig] '%s' describes no propellers"), *YamlPath);
      return true;
    }

    int32 Index = 0;
    for (const auto& Node : Propellers)
    {
      if (!Node.IsMap())
      {
        ++Index;
        continue;
      }

      FExternalPropellerConfig Propeller;

      Propeller.Name = Node["name"] && Node["name"].IsScalar() ? ToFString(Node["name"].as<std::string>(""))
                                                               : FString::Printf(TEXT("Propeller_%d"), Index);

      // Metres to centimetres, with the Y axis flipped into Unreal's left-handed frame.
      const FVector Location = ReadTriplet(Node["location"], FVector::ZeroVector);
      const FVector Scale    = ReadTriplet(Node["scale"], FVector::OneVector);

      // The shipped sidecars disagree on the key name for the same field.
      const YAML::Node RotationNode = Node["orientation"] ? Node["orientation"] : Node["rotation"];
      const FVector    Rotation     = ReadTriplet(RotationNode, FVector::ZeroVector);

      Propeller.RelativeTransform = FTransform(FRotator(Rotation.Y, Rotation.Z, Rotation.X),
                                               FVector(Location.X * 100.0, Location.Y * -100.0, Location.Z * 100.0), Scale);

      OutConfig.Propellers.Add(Propeller);
      ++Index;
    }
  }
  catch (const YAML::Exception& Exception)
  {
    OutError = FString::Printf(TEXT("'%s': %s"), *YamlPath, *ToFString(Exception.what()));
    return false;
  }

  return true;
}

/* //} */
