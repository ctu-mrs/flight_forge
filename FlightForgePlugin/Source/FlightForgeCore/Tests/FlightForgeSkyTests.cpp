// Time of day and weather on a world with no sky actor: the light fallback, hidden lights and the refusals.
//   UnrealEditor-Cmd <project> -ExecCmds="Automation RunTests FlightForge.Api" -unattended -nopause

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "Components/DirectionalLightComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

#include "FlightForgeCore/Utils/SkyControl.h"

/* sky control //{ */

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFlightForgeApiSkyControlTest, "FlightForge.Api.SkyControl",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FFlightForgeApiSkyControlTest::RunTest(const FString& Parameters)
{
	UWorld*        World   = UWorld::CreateWorld(EWorldType::Game, false);
	FWorldContext& Context = GEngine->CreateNewWorldContext(EWorldType::Game);
	Context.SetCurrentWorld(World);

	USkyControl* Sky = World->GetSubsystem<USkyControl>();
	if (TestNotNull(TEXT("every game world has the sky control"), Sky))
	{
		FString Error;
		TestFalse(TEXT("25:00 is refused"), Sky->SetDaytime(25, 0, Error));
		TestTrue(TEXT("naming the time"), Error.Contains(TEXT("25:00")));

		Error.Reset();
		TestFalse(TEXT("an unknown weather is refused"), Sky->SetWeather(99, Error));
		TestTrue(TEXT("as not a weather type"), Error.Contains(TEXT("not a weather type")));

		Error.Reset();
		TestFalse(TEXT("weather without a weather actor fails"), Sky->SetWeather(0, Error));
		TestTrue(TEXT("saying the world has no sky"), Error.Contains(USkyControl::NoSkySystem));

		Error.Reset();
		TestFalse(TEXT("daytime without any light fails"), Sky->SetDaytime(12, 0, Error));
		TestTrue(TEXT("saying the world has no sky"), Error.Contains(USkyControl::NoSkySystem));
	}

	ADirectionalLight*          Light = World->SpawnActor<ADirectionalLight>(FVector::ZeroVector, FRotator(-30.0, 40.0, 0.0));
	UDirectionalLightComponent* Sun   = Light ? Cast<UDirectionalLightComponent>(Light->GetLightComponent()) : nullptr;
	if (Sky && TestNotNull(TEXT("a sun"), Sun))
	{
		Sun->SetMobility(EComponentMobility::Movable);
		Sun->SetIntensity(7.0f);

		// the world's own control already took its baseline, without this light
		USkyControl* Fresh = NewObject<USkyControl>(World);
		FString      Error;

		TestTrue(TEXT("noon through the level's sun"), Fresh->SetDaytime(12, 0, Error));
		TestEqual(TEXT("overhead at noon"), Sun->GetComponentRotation().Pitch, -90.0, 0.01);
		TestEqual(TEXT("at full intensity"), Sun->Intensity, 7.0f);

		TestTrue(TEXT("9:00 through the level's sun"), Fresh->SetDaytime(9, 0, Error));
		TestEqual(TEXT("45 deg up at 9:00"), Sun->GetComponentRotation().Pitch, -45.0, 0.01);
		TestEqual(TEXT("in its own azimuth"), Sun->GetComponentRotation().Yaw, 40.0, 0.01);

		TestTrue(TEXT("midnight through the level's sun"), Fresh->SetDaytime(0, 0, Error));
		TestEqual(TEXT("dark at midnight"), Sun->Intensity, 0.0f);

		TestTrue(TEXT("noon again"), Fresh->SetDaytime(12, 0, Error));
		TestEqual(TEXT("lit again from the first baseline"), Sun->Intensity, 7.0f);

		double  Hours     = 0.0;
		bool    bAnimated = true;
		FString Kind;
		TestTrue(TEXT("the clock reads back"), Fresh->GetDaytime(Hours, bAnimated, Kind, Error));
		TestEqual(TEXT("at what was set"), Hours, 12.0);
		TestFalse(TEXT("standing still"), bAnimated);
		TestEqual(TEXT("from the level's lights"), Kind, FString(TEXT("lights")));
		TestFalse(TEXT("the lights have no day cycle to run"), Fresh->SetDaytimeAnimated(true, Error));
		TestTrue(TEXT("saying so"), Error.Contains(TEXT("no day cycle")));
		TestTrue(TEXT("nor one to stop"), Fresh->SetDaytimeAnimated(false, Error));

		Sun->SetVisibility(false);
		USkyControl* Hidden = NewObject<USkyControl>(World);
		Error.Reset();
		TestFalse(TEXT("a hidden sun lights nothing"), Hidden->SetDaytime(12, 0, Error));
		TestTrue(TEXT("saying the world has no sky"), Error.Contains(USkyControl::NoSkySystem));
		Error.Reset();
		TestFalse(TEXT("and has no clock to read"), Hidden->GetDaytime(Hours, bAnimated, Kind, Error));
		TestTrue(TEXT("for the same reason"), Error.Contains(USkyControl::NoSkySystem));
		Sun->SetVisibility(true);

		Sun->SetMobility(EComponentMobility::Static);
		USkyControl* Static = NewObject<USkyControl>(World);
		TestFalse(TEXT("a static sun cannot change"), Static->SetDaytime(12, 0, Error));
		TestTrue(TEXT("saying the world has no sky"), Error.Contains(USkyControl::NoSkySystem));
	}

	GEngine->DestroyWorldContext(World);
	World->DestroyWorld(false);
	return true;
}

/* //} */

#endif
