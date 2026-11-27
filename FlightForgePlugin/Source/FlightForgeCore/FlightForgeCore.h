// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/* FFlightForgeCoreModule //{ */

class FFlightForgeCoreModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	// lets a resolution change through off screen, for get_viewport_image; outside BeginPlay only
	static void ResizeOffScreen(int32 Width, int32 Height);
};

/* //} */
