
#include "uedsGameInstance.h"


void UuedsGameInstance::Init()
{
	Super::Init();
	
	Server = std::make_shared<UedsGameModeServer>(8551, nullptr);
	Server->Run();
}

void UuedsGameInstance::Shutdown()
{
	if (Server)
	{
		Server->Stop();
	}

	Super::Shutdown();
}