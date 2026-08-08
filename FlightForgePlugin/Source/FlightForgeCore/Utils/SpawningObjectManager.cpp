#include "SpawningObjectManager.h"

#include "Kismet/GameplayStatics.h"
#include "Misc/Paths.h"

#include "SpawnConfig.h"
#include "FlightForgeCore/FlightForgeGameModeBase.h"

ASpawningObjectManager::ASpawningObjectManager()
{
  PrimaryActorTick.bCanEverTick = false;
}

/* BeginPlay() //{ */

void ASpawningObjectManager::BeginPlay()
{
  Super::BeginPlay();

  if (!bApplyOnBeginPlay)
  {
    return;
  }

  AFlightForgeGameModeBase* GameMode = Cast<AFlightForgeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
  if (!GameMode)
  {
    UE_LOG(LogTemp, Error, TEXT("[SpawningObjectManager] the game mode is not AFlightForgeGameModeBase, cannot spawn"));
    return;
  }

  FString ConfigPath = SpawnConfigPath;
  if (FPaths::IsRelative(ConfigPath))
  {
    ConfigPath = FPaths::Combine(FPaths::ProjectDir(), ConfigPath);
  }

  // A missing configuration is the normal case for a level that ships no extra objects.
  if (!FPaths::FileExists(ConfigPath))
  {
    UE_LOG(LogTemp, Log, TEXT("[SpawningObjectManager] no spawn configuration at '%s', nothing to place"), *ConfigPath);
    return;
  }

  FSpawnConfig Config;
  FString      Error;
  if (!USpawnConfigUtils::LoadSpawnConfigFile(ConfigPath, Config, Error))
  {
    UE_LOG(LogTemp, Error, TEXT("[SpawningObjectManager] %s"), *Error);
    return;
  }

  GameMode->SpawnObjectsFromConfig(Config);
}

/* //} */
