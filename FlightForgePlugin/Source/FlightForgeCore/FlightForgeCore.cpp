// Copyright Epic Games, Inc. All Rights Reserved.

#include "FlightForgeCore.h"

#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "ShaderCore.h"

void FFlightForgeCoreModule::StartupModule()
{
	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("FlightForgePlugin"));

	if (Plugin.IsValid())
	{
		const FString ShaderDirectory = FPaths::Combine(Plugin->GetBaseDir(), TEXT("Shaders"));
		AddShaderSourceDirectoryMapping(TEXT("/FlightForgePlugin"), ShaderDirectory);
	}
}

IMPLEMENT_GAME_MODULE(FFlightForgeCoreModule, FlightForgeCore);
