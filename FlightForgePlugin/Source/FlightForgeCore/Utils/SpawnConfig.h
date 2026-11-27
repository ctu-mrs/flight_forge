#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "SpawnConfig.generated.h"

/* FSpawnObjectEntry //{ */

/** One object to place, in the ground frame (m, roll/pitch/yaw deg) or Unreal's (cm, rotator). */
USTRUCT(BlueprintType)
struct FLIGHTFORGECORE_API FSpawnObjectEntry
{
  GENERATED_BODY()

  /** Bare asset name, a /Game or /FlightForgePlugin object path, or a path to a .glb/.gltf file. */
  UPROPERTY(BlueprintReadWrite, Category = "FlightForge|Spawn")
  FString Asset;

  UPROPERTY(BlueprintReadWrite, Category = "FlightForge|Spawn")
  FVector Position = FVector::ZeroVector;

  /** X = roll, Y = pitch, Z = yaw. */
  UPROPERTY(BlueprintReadWrite, Category = "FlightForge|Spawn")
  FVector Orientation = FVector::ZeroVector;

  UPROPERTY(BlueprintReadWrite, Category = "FlightForge|Spawn")
  FVector Scale = FVector::OneVector;

  /** Whether the config set a scale; a library object keeps its own otherwise. */
  UPROPERTY(BlueprintReadWrite, Category = "FlightForge|Spawn")
  bool bHasScale = false;

  /** Custom depth stencil used for the segmentation output; negative means auto-assign. */
  UPROPERTY(BlueprintReadWrite, Category = "FlightForge|Spawn")
  int32 Stencil = -1;

  /** Class tree path ("prop/box"); stands in for the stencil when that is absent. */
  UPROPERTY(BlueprintReadWrite, Category = "FlightForge|Spawn")
  FString Class;
};

/* //} */

/* FSpawnConfig //{ */

USTRUCT(BlueprintType)
struct FLIGHTFORGECORE_API FSpawnConfig
{
  GENERATED_BODY()

  /** Package paths searched when an asset is given as a bare name. */
  UPROPERTY(BlueprintReadWrite, Category = "FlightForge|Spawn")
  TArray<FString> SearchPaths;

  /** Filesystem directories searched for .glb/.gltf files that are not cooked into the build. */
  UPROPERTY(BlueprintReadWrite, Category = "FlightForge|Spawn")
  TArray<FString> ModelPaths;

  /** Added to every position, in the same right-handed metres as the entries themselves. */
  UPROPERTY(BlueprintReadWrite, Category = "FlightForge|Spawn")
  FVector OriginOffset = FVector::ZeroVector;

  /** frame: unreal */
  UPROPERTY(BlueprintReadWrite, Category = "FlightForge|Spawn")
  bool bUnrealFrame = false;

  UPROPERTY(BlueprintReadWrite, Category = "FlightForge|Spawn")
  TArray<FSpawnObjectEntry> Objects;
};

/* //} */

/* USpawnConfigUtils //{ */

UCLASS()
class FLIGHTFORGECORE_API USpawnConfigUtils : public UBlueprintFunctionLibrary
{
  GENERATED_BODY()

public:
  /** Parses a spawn configuration from a YAML document held in memory. */
  UFUNCTION(BlueprintCallable, Category = "FlightForge|Spawn")
  static bool ParseSpawnConfig(const FString& YamlText, FSpawnConfig& OutConfig, FString& OutError);

  UFUNCTION(BlueprintCallable, Category = "FlightForge|Spawn")
  static bool LoadSpawnConfigFile(const FString& FilePath, FSpawnConfig& OutConfig, FString& OutError);

  /** GroundOrigin in Unreal cm. */
  UFUNCTION(BlueprintCallable, Category = "FlightForge|Spawn")
  static FTransform ToUnrealTransform(const FSpawnObjectEntry& Entry, const FVector& OriginOffset, bool bUnrealFrame, const FVector& GroundOrigin);

  /** Exact inverse of ToUnrealTransform into the ground frame (with a zero origin offset). */
  UFUNCTION(BlueprintCallable, Category = "FlightForge|Spawn")
  static void FromUnrealTransform(const FTransform& Transform, const FVector& GroundOrigin, FSpawnObjectEntry& OutEntry);

  /** Serializes a config back into the YAML schema ParseSpawnConfig reads. */
  UFUNCTION(BlueprintCallable, Category = "FlightForge|Spawn")
  static FString WriteSpawnConfig(const FSpawnConfig& Config);
};

/* //} */
