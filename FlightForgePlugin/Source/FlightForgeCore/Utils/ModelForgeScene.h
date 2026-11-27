#pragma once

#include "CoreMinimal.h"

class UWorld;

// ModelForgeSplatScene's scene loader by reflection, so FlightForge builds without the plugin
namespace ModelForgeScene
{

// the export folder a loader in the world is loading or showing; empty when there is none or it failed
FString ActiveFolder(UWorld* World);

bool Load(UWorld* World, const FString& Folder);

// the ground point of the export's player start, once the loader has put the player there
bool PlacedStart(UWorld* World, FVector& OutGround);

}  // namespace ModelForgeScene
