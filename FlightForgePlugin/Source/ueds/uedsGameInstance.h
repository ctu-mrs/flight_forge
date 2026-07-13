
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Server/UedsGameModeServer.h"
#include "uedsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UEDS_API UuedsGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void Shutdown() override;
	
	std::shared_ptr<UedsGameModeServer> Server;
};
