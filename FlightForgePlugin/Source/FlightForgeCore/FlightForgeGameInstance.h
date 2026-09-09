
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Server/FlightForgeGameModeServer.h"
#include "Devices/DeviceMeshLibrary.h"
#include "FlightForgeGameInstance.generated.h"

UCLASS()
class FLIGHTFORGECORE_API UFlightForgeGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void Shutdown() override;

	std::shared_ptr<FlightForgeGameModeServer> Server;

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
};
