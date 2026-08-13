// Copyright Epic Games, Inc. All Rights Reserved.

#include "FlightForgeShaders.h"

#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "ShaderCore.h"

void FFlightForgeShadersModule::StartupModule()
{
	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("FlightForgePlugin"));

	if (Plugin.IsValid())
	{
		const FString ShaderDirectory = FPaths::Combine(Plugin->GetBaseDir(), TEXT("Shaders"));
		AddShaderSourceDirectoryMapping(TEXT("/FlightForgePlugin"), ShaderDirectory);
	}
}

IMPLEMENT_MODULE(FFlightForgeShadersModule, FlightForgeShaders);
