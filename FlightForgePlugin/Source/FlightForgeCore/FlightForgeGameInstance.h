
#pragma once

#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "Engine/GameInstance.h"
#include <memory>

#include "Devices/DeviceMeshLibrary.h"
#include "RenderSettings/ViewportMode.h"
#include "FlightForgeGameInstance.generated.h"

class FSimServer;

UCLASS()
class FLIGHTFORGECORE_API UFlightForgeGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void Shutdown() override;

	std::shared_ptr<FSimServer> Server;

	UDeviceMeshLibrary* GetDeviceMeshes()
	{
		if (!DeviceMeshes)
		{
			DeviceMeshes = NewObject<UDeviceMeshLibrary>(this);
		}
		return DeviceMeshes;
	}

private:
	UPROPERTY()
	TObjectPtr<UDeviceMeshLibrary> DeviceMeshes;

	FTSTicker::FDelegateHandle FrameCapTicker;

	std::unique_ptr<FViewportMode> ViewportMode;
};
