#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "SpawnConfig.generated.h"

/* FSpawnObjectEntry //{ */

/**
 * One object to place in the world. Poses are given in a right-handed (ROS) frame,
 * in metres and degrees; they are converted to Unreal's left-handed centimetres by
 * USpawnConfigUtils::ToUnrealTransform.
 */
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

  /** When true, poses are relative to the "UAV" PlayerStart rather than to the world origin. */
  UPROPERTY(BlueprintReadWrite, Category = "FlightForge|Spawn")
  bool bRelativeToWorldOrigin = true;

  /** With a relative frame, z = 0 is the floor under the PlayerStart, as in the ROS node. */
  UPROPERTY(BlueprintReadWrite, Category = "FlightForge|Spawn")
  bool bGroundFrame = false;

  UPROPERTY(BlueprintReadWrite, Category = "FlightForge|Spawn")
  TArray<FSpawnObjectEntry> Objects;
};

/* //} */

/* USpawnConfigUtils //{ */

// the ROS node's offset between the "UAV" PlayerStart and the floor
inline constexpr double PlayerStartHeightCm = 92.12;

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

  /**
   * Right-handed metres/degrees to Unreal's left-handed centimetres. The Y axis and the
   * pitch and yaw senses are flipped; roll is unchanged.
   */
  UFUNCTION(BlueprintCallable, Category = "FlightForge|Spawn")
  static FTransform ToUnrealTransform(const FSpawnObjectEntry& Entry, const FVector& OriginOffset, const FVector& WorldOrigin);

  /** Exact inverse of ToUnrealTransform (with a zero origin offset). */
  UFUNCTION(BlueprintCallable, Category = "FlightForge|Spawn")
  static void FromUnrealTransform(const FTransform& Transform, const FVector& WorldOrigin, FSpawnObjectEntry& OutEntry);

  /** Serializes a config back into the YAML schema ParseSpawnConfig reads. */
  UFUNCTION(BlueprintCallable, Category = "FlightForge|Spawn")
  static FString WriteSpawnConfig(const FSpawnConfig& Config);
};

/* //} */
