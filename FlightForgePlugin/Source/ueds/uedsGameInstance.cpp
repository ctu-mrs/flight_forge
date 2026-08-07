
#include "uedsGameInstance.h"

#include "Misc/CommandLine.h"
#include "Misc/Parse.h"


void UuedsGameInstance::Init()
{
	Super::Init();

	// Game-mode server port is configurable via "-FlightForgeGameModePort=<port>" (default 8551).
	int32 GameModePort = 8551;
	FParse::Value(FCommandLine::Get(), TEXT("FlightForgeGameModePort="), GameModePort);

	UE_LOG(LogTemp, Warning, TEXT("FlightForge: game-mode server port %d"), GameModePort);

	Server = std::make_shared<UedsGameModeServer>(GameModePort, nullptr);
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
