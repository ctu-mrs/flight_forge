#include "FlightForgeGameInstance.h"

#include "HAL/IConsoleManager.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Engine/Engine.h"
#include "Scalability.h"

#include "Server/SimServer.h"
#include "FlightForgeCore/Utils/WorldCatalog.h"

/* ApplyLaptopPreset() //{ */

namespace
{

// set by code, so the scalability groups set_graphics_settings changes cannot undo them
void ApplyLaptopPreset()
{
	Scalability::FQualityLevels Levels = Scalability::GetQualityLevels();
	Levels.SetFromSingleQualityLevel(0);
	Scalability::SetQualityLevels(Levels);

	const TPair<const TCHAR*, const TCHAR*> Vars[] = {
	    {TEXT("r.ScreenPercentage"), TEXT("25")},
	    {TEXT("FlightForge.Camera.ScreenPercentage"), TEXT("50")},
	    {TEXT("r.VolumetricCloud"), TEXT("0")},
	};

	for (const auto& [Name, Value] : Vars)
	{
		if (IConsoleVariable* Var = IConsoleManager::Get().FindConsoleVariable(Name))
		{
			Var->Set(Value, ECVF_SetByCode);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("FlightForge: laptop preset, scalability low, main viewport at 25 %%, cameras at 50 %%, no volumetric clouds"));
}

}  // namespace

//}

/* Init() //{ */

void UFlightForgeGameInstance::Init()
{
	Super::Init();

	const bool bLaptop = FParse::Param(FCommandLine::Get(), TEXT("FlightForgeLaptop"));
	if (bLaptop)
	{
		ApplyLaptopPreset();
	}

	float CameraScreenPercentage = 0.0f;
	if (FParse::Value(FCommandLine::Get(), TEXT("FlightForgeCameraScreenPercentage="), CameraScreenPercentage))
	{
		// at the preset's priority, after it, so the switch wins over -FlightForgeLaptop
		IConsoleManager::Get().FindConsoleVariable(TEXT("FlightForge.Camera.ScreenPercentage"))->Set(CameraScreenPercentage, ECVF_SetByCode);
	}

	const bool bNoViewport = FParse::Param(FCommandLine::Get(), TEXT("FlightForgeNoViewport")) || FParse::Param(FCommandLine::Get(), TEXT("NoViewport")) ||
	                         (bLaptop && !FParse::Param(FCommandLine::Get(), TEXT("FlightForgeViewport")));
	ViewportMode = std::make_unique<FViewportMode>(bNoViewport);

	int32 MaxFps = bLaptop ? 30 : 0;
	FParse::Value(FCommandLine::Get(), TEXT("FlightForgeMaxFps="), MaxFps);
	if (MaxFps > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("FlightForge: at most %d fps"), MaxFps);

		// every ApplySettings resets t.MaxFPS to the user settings' limit, so the cap is held, not set once
		FrameCapTicker = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([MaxFps](float) {
			static IConsoleVariable* Var = IConsoleManager::Get().FindConsoleVariable(TEXT("t.MaxFPS"));
			if (GEngine && Var && Var->GetInt() != MaxFps)
			{
				GEngine->SetMaxFPS(MaxFps);
			}
			return true;
		}), 0.5f);
	}

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

	// cooked builds fill it from the asset registry, which asserts off the game thread
	FWorldCatalog::Get();

	Server = std::make_shared<FSimServer>(Port, BindAddress);
	Server->Start();

	TravelFailureHandle = GEngine->OnTravelFailure().AddLambda([Weak = std::weak_ptr<FSimServer>(Server)](UWorld*, ETravelFailure::Type, const FString& Error) {
		if (const std::shared_ptr<FSimServer> Locked = Weak.lock())
		{
			Locked->FailWorldSwitch(Error);
		}
	});
}

//}

/* Shutdown() //{ */

void UFlightForgeGameInstance::Shutdown()
{
	FTSTicker::GetCoreTicker().RemoveTicker(FrameCapTicker);
	if (GEngine)
	{
		GEngine->OnTravelFailure().Remove(TravelFailureHandle);
	}
	ViewportMode.reset();

	if (Server)
	{
		Server->Stop();
	}

	Super::Shutdown();
}

//}
