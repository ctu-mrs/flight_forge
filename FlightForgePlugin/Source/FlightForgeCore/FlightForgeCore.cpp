// Copyright Epic Games, Inc. All Rights Reserved.

#include "FlightForgeCore.h"

// The shader directory mapping moved to FlightForgeShaders, which loads at PostConfigInit;
// mapping it here would be too late for the shader types registered in that module.
void FFlightForgeCoreModule::StartupModule()
{
}

IMPLEMENT_GAME_MODULE(FFlightForgeCoreModule, FlightForgeCore);
