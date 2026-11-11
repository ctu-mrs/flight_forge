#include "FlightForgeGameInstance.h"

#include "Misc/CommandLine.h"
#include "Misc/Parse.h"

#include "Server/SimServer.h"

void UFlightForgeGameInstance::Init()
{
	Super::Init();

	// -FlightForgePort= wins over the older -FlightForgeGameModePort=
	int32 Port = 8600;
	if (!FParse::Value(FCommandLine::Get(), TEXT("FlightForgePort="), Port))
	{
		FParse::Value(FCommandLine::Get(), TEXT("FlightForgeGameModePort="), Port);
	}

	FString BindAddress = TEXT("127.0.0.1");
	FParse::Value(FCommandLine::Get(), TEXT("FlightForgeBindAddress="), BindAddress);

	int32 DroneStartPort = 0;
	if (FParse::Value(FCommandLine::Get(), TEXT("FlightForgeDroneStartPort="), DroneStartPort))
	{
		UE_LOG(LogTemp, Warning, TEXT("FlightForge: -FlightForgeDroneStartPort is ignored, robots are addressed by id on port %d"), Port);
	}

	Server = std::make_shared<FSimServer>(Port, BindAddress);
	Server->Start();
}

void UFlightForgeGameInstance::Shutdown()
{
	if (Server)
	{
		Server->Stop();
	}

	Super::Shutdown();
}
