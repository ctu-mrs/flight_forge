// The surface-height traces against one cube in an empty world.
//   UnrealEditor-Cmd <project> -ExecCmds="Automation RunTests FlightForge.Api" -unattended -nopause

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#if WITH_EDITOR
#include "StaticMeshCompiler.h"
#endif

#include "FlightForgeCore/Utils/SurfaceHeightQuery.h"

/* surface height //{ */

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFlightForgeApiSurfaceHeightTest, "FlightForge.Api.SurfaceHeight",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FFlightForgeApiSurfaceHeightTest::RunTest(const FString& Parameters)
{
	UWorld*        World   = UWorld::CreateWorld(EWorldType::Game, false);
	FWorldContext& Context = GEngine->CreateNewWorldContext(EWorldType::Game);
	Context.SetCurrentWorld(World);

	UStaticMesh*      Cube  = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	AStaticMeshActor* Block = World->SpawnActor<AStaticMeshActor>(FVector(0.0, 0.0, 50.0), FRotator::ZeroRotator);
	if (TestNotNull(TEXT("the engine cube"), Cube) && TestNotNull(TEXT("a block"), Block))
	{
#if WITH_EDITOR
		FStaticMeshCompilingManager::Get().FinishCompilation({Cube});  // a mesh still compiling gets no physics state
#endif
		Block->GetStaticMeshComponent()->SetStaticMesh(Cube);
		Block->GetStaticMeshComponent()->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
		World->Tick(LEVELTICK_All, 0.01f);
		TestTrue(TEXT("the block has a body"), Block->GetStaticMeshComponent()->GetBodyInstance()->IsValidBodyInstance());

		const double   Nan = std::numeric_limits<double>::quiet_NaN();
		TArray<double> Heights;
		FSurfaceHeightQuery::Batch(World, FVector::ZeroVector, {FVector(0.0, 0.0, Nan), FVector(0.0, 0.0, 0.5), FVector(0.0, 0.0, -0.1), FVector(3.0, 3.0, Nan)},
		                           true, Heights);

		if (TestEqual(TEXT("one height per point"), Heights.Num(), 4))
		{
			TestEqual(TEXT("the block's top from the top of the world"), Heights[0], 1.0, 0.001);
			TestTrue(TEXT("nothing below a start inside the block"), FMath::IsNaN(Heights[1]));
			TestTrue(TEXT("nothing below a start under the block"), FMath::IsNaN(Heights[2]));
			TestTrue(TEXT("nothing beside the block"), FMath::IsNaN(Heights[3]));
		}

		const TArray<const AActor*> Own = {Block, nullptr};
		FSurfaceHeightQuery::Batch(World, FVector::ZeroVector, {FVector(0.0, 0.0, Nan), FVector(0.0, 0.0, Nan)}, true, Heights, &Own);
		if (TestEqual(TEXT("one height per point, per-point ignores"), Heights.Num(), 2))
		{
			TestTrue(TEXT("a point leaves out its own actor"), FMath::IsNaN(Heights[0]));
			TestEqual(TEXT("the next point still sees it"), Heights[1], 1.0, 0.001);
		}
	}

	GEngine->DestroyWorldContext(World);
	World->DestroyWorld(false);
	return true;
}

/* //} */

#endif
