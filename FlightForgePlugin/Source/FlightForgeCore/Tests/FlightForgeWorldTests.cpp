// World names: list_worlds' naming and switch_world's resolution, offline.
//   UnrealEditor-Cmd <project> -ExecCmds="Automation RunTests FlightForge.Api" -unattended -nopause

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "FlightForgeCore/Utils/WorldCatalog.h"

/* world catalog //{ */

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFlightForgeApiWorldCatalogTest, "FlightForge.Api.WorldCatalog",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FFlightForgeApiWorldCatalogTest::RunTest(const FString& Parameters)
{
	const TArray<FWorldEntry> Worlds = FWorldCatalog::FromPackages({
	    TEXT("/Game/Worlds/Warehouse/Maps/Warehouse"),
	    TEXT("/Game/Worlds/IndustrialWarehouse/Maps/industrial_warehouse"),
	    TEXT("/Game/Worlds/Warehouse/Scene_Warehouse/Maps/Industrial_Warehouse"),
	    TEXT("/Game/Worlds/IndustrialWarehouse/Maps/SprindLab"),
	    TEXT("/Game/Worlds/Warehouse/Maps/SprindLab"),
	    TEXT("/Game/MWDeadSpruceForest/Maps/dead_spruce_forest"),
	});

	const auto NameOf = [&Worlds](const FString& Package) {
		const FWorldEntry* Entry = Worlds.FindByPredicate([&](const FWorldEntry& World) { return World.Package == Package; });
		return Entry ? Entry->Name : FString();
	};
	TestEqual(TEXT("six worlds"), Worlds.Num(), 6);
	TestEqual(TEXT("a unique map goes by its short name"), NameOf(TEXT("/Game/Worlds/Warehouse/Maps/Warehouse")), FString(TEXT("Warehouse")));
	TestEqual(TEXT("names differing in case stay short"), NameOf(TEXT("/Game/Worlds/IndustrialWarehouse/Maps/industrial_warehouse")),
	          FString(TEXT("industrial_warehouse")));
	TestEqual(TEXT("a shared short name goes by its package"), NameOf(TEXT("/Game/Worlds/Warehouse/Maps/SprindLab")),
	          FString(TEXT("/Game/Worlds/Warehouse/Maps/SprindLab")));
	TestEqual(TEXT("display name"), Worlds.FindByPredicate([](const FWorldEntry& W) { return W.Name == TEXT("dead_spruce_forest"); })->DisplayName,
	          FString(TEXT("Dead Spruce Forest")));

	const auto Resolves = [&Worlds](const FString& Level) {
		FString Package;
		FString Error;
		return FWorldCatalog::Resolve(Worlds, Level, Package, Error) ? Package : TEXT("error: ") + Error;
	};
	TestEqual(TEXT("exact short name"), Resolves(TEXT("industrial_warehouse")), FString(TEXT("/Game/Worlds/IndustrialWarehouse/Maps/industrial_warehouse")));
	TestEqual(TEXT("other case of the pair"), Resolves(TEXT("Industrial_Warehouse")), FString(TEXT("/Game/Worlds/Warehouse/Scene_Warehouse/Maps/Industrial_Warehouse")));
	TestEqual(TEXT("unique ignoring case"), Resolves(TEXT("warehouse")), FString(TEXT("/Game/Worlds/Warehouse/Maps/Warehouse")));
	TestEqual(TEXT("package path, any case, object path"), Resolves(TEXT("/game/worlds/warehouse/maps/warehouse.Warehouse")),
	          FString(TEXT("/Game/Worlds/Warehouse/Maps/Warehouse")));
	TestTrue(TEXT("ambiguous"), Resolves(TEXT("SprindLab")).StartsWith(TEXT("error: world \"SprindLab\" is ambiguous")));
	TestTrue(TEXT("unknown"), Resolves(TEXT("CaveTunnel")).StartsWith(TEXT("error: unknown world")));
	TestTrue(TEXT("unknown package"), Resolves(TEXT("/Game/Nowhere")).StartsWith(TEXT("error: no map")));
	TestTrue(TEXT("empty"), Resolves(TEXT("")).StartsWith(TEXT("error: switch_world needs a level")));
	return true;
}

/* //} */

#endif
