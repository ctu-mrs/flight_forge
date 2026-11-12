#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "SkyControl.generated.h"

class UDirectionalLightComponent;
class USkyLightComponent;

/* USkyControl //{ */

/**
 * Time of day and weather for whatever sky the loaded world has, found at runtime by class name and driven
 * through reflection: Ultra Dynamic Sky and Weather, the SunPosition plugin's SunSky, or else the level's own
 * directional and sky lights. Game thread only.
 */
UCLASS()
class FLIGHTFORGECORE_API USkyControl : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// clients match this phrase to tell a world without a sky from a failure
	static constexpr const TCHAR* NoSkySystem = TEXT("has no sky system");

	UFUNCTION(BlueprintCallable, Category = "FlightForge|Environment")
	bool SetDaytime(int32 Hour, int32 Minute, FString& Error);

	// Weather is a flightforge.wire.Weather value
	UFUNCTION(BlueprintCallable, Category = "FlightForge|Environment")
	bool SetWeather(int32 Weather, FString& Error);

private:
	bool SetLightsDaytime(double Hours, FString& Error);

	struct FLightBaseline
	{
		TWeakObjectPtr<UDirectionalLightComponent> Sun;
		FRotator                                   SunRotation;
		float                                      SunIntensity = 0.0f;
		TArray<TPair<TWeakObjectPtr<USkyLightComponent>, float>> SkyLights;
		bool bTaken = false;
	};

	FLightBaseline Baseline;
};

/* //} */
