#pragma once

#include "CoreMinimal.h"

#include "ExternalModelConfig.generated.h"

/* FExternalPropellerConfig //{ */

USTRUCT(BlueprintType)
struct FLIGHTFORGECORE_API FExternalPropellerConfig
{
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite, Category = "FlightForge|External Model")
  FTransform RelativeTransform = FTransform::Identity;

  UPROPERTY(BlueprintReadWrite, Category = "FlightForge|External Model")
  FString Name = TEXT("Propeller");
};

/* //} */

/* FExternalModelConfig //{ */

/** The sidecar that sits next to a user model and describes where its propellers go. */
USTRUCT(BlueprintType)
struct FLIGHTFORGECORE_API FExternalModelConfig
{
  GENERATED_BODY()

  /** Optional override of the propeller mesh, as a UE object path. */
  UPROPERTY(BlueprintReadWrite, Category = "FlightForge|External Model")
  FString PropellerMeshPath;

  UPROPERTY(BlueprintReadWrite, Category = "FlightForge|External Model")
  TArray<FExternalPropellerConfig> Propellers;
};

/* //} */

namespace ExternalModelConfig
{
/**
 * Reads a propeller sidecar. Two spellings are accepted, because both were shipped: a
 * mapping with a "propellers" key, and a bare sequence whose first nested sequence is the
 * propeller list. Within an entry, "orientation" and "rotation" are interchangeable and
 * every field is optional.
 *
 * Returns false only when the file is missing, unreadable, or not valid YAML; a file that
 * simply describes no propellers is a success with an empty list.
 */
FLIGHTFORGECORE_API bool Load(const FString& YamlPath, FExternalModelConfig& OutConfig, FString& OutError);
}  // namespace ExternalModelConfig
