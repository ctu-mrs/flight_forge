#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SpawningObjectManager.generated.h"

/* ASpawningObjectManager //{ */

/**
 * Level-placed entry point for the file-driven half of the spawn tool: on BeginPlay it
 * applies a spawn configuration read from disk. The configuration lives next to the
 * packaged binary so a world can be extended without rebuilding it.
 *
 * The other half of the tool is the spawn_objects_from_yaml API command, which feeds the
 * same spawner; both share one set of object handles.
 */
UCLASS()
class FLIGHTFORGECORE_API ASpawningObjectManager : public AActor
{
  GENERATED_BODY()

public:
  ASpawningObjectManager();

  /** Relative paths resolve against the project directory. */
  UPROPERTY(EditAnywhere, Category = "FlightForge|Spawn")
  FString SpawnConfigPath = TEXT("FlightForge/spawn_config.yaml");

  UPROPERTY(EditAnywhere, Category = "FlightForge|Spawn")
  bool bApplyOnBeginPlay = true;

protected:
  virtual void BeginPlay() override;
};

/* //} */
