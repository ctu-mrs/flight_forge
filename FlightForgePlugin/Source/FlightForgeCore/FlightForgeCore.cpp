// Copyright Epic Games, Inc. All Rights Reserved.

#include "FlightForgeCore.h"

#include "HAL/IConsoleManager.h"
#include "Misc/CommandLine.h"
#include "UnrealEngine.h"

/* KeepOffScreenResolution() //{ */

namespace
{

// off screen a resolution change draws a frame on the spot, which crashed mid BeginPlay in SetExposureCompensationCurve
void KeepOffScreenResolution(IConsoleVariable* SetRes)
{
	static bool bRestoring = false;
	if (bRestoring)
	{
		return;
	}

	const TCHAR*  Suffix  = GSystemResolution.WindowMode == EWindowMode::Fullscreen ? TEXT("f") : GSystemResolution.WindowMode == EWindowMode::WindowedFullscreen ? TEXT("wf") : TEXT("w");
	const FString Current = FString::Printf(TEXT("%ux%u%s"), GSystemResolution.ResX, GSystemResolution.ResY, Suffix);
	if (SetRes->GetString() == Current)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("FlightForge: rendering off screen, keeping %s instead of r.SetRes %s"), *Current, *SetRes->GetString());
	TGuardValue<bool> Guard(bRestoring, true);
	SetRes->SetWithCurrentPriority(*Current);
}

}  // namespace

/* //} */

void FFlightForgeCoreModule::StartupModule()
{
	if (FParse::Param(FCommandLine::Get(), TEXT("RenderOffScreen")))
	{
		if (IConsoleVariable* SetRes = IConsoleManager::Get().FindConsoleVariable(TEXT("r.SetRes")))
		{
			SetRes->OnChangedDelegate().AddStatic(&KeepOffScreenResolution);
		}
	}
}

IMPLEMENT_GAME_MODULE(FFlightForgeCoreModule, FlightForgeCore);
