#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SplatLighting.generated.h"

// FlightForge.SplatRelight / FlightForge.SunPitch, applied to ModelForge splats by reflection (no module dependency)
UCLASS()
class FLIGHTFORGECORE_API USplatLighting : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void    OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void    Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool    IsTickable() const override { return bStarted; }

protected:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

private:
	void ApplyRelight(int32 Relight);
	void ApplySunPitch(float Pitch);

	bool  bStarted     = false;
	float AppliedPitch = TNumericLimits<float>::Max();
};
