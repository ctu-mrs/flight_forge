#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// Loads at PostConfigInit purely so the virtual shader directory is mapped and the
// SegId shader types are registered before the engine compiles anything.
class FFlightForgeShadersModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
};
