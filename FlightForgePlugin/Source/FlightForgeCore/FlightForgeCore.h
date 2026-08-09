// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/* FFlightForgeCoreModule //{ */

class FFlightForgeCoreModule : public IModuleInterface
{
public:
	// maps /FlightForgePlugin to the plugin's Shaders directory (SegId pass shaders)
	virtual void StartupModule() override;
};

/* //} */
