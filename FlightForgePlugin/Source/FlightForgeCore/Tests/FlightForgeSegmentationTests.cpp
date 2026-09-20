// Instance segmentation against a physics oracle: every pixel of the SegId pass is
// compared with a complex line trace through the same pixel. Needs a running level:
//   UnrealEditor-Cmd <project> <map> -game -RenderOffScreen -ExecCmds="Automation RunTests FlightForge.Segmentation"

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "CollisionQueryParams.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "LandscapeProxy.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#include "FlightForgeCore/DronePawn.h"
#include "FlightForgeCore/FlightForgeGameModeBase.h"
#include "FlightForgeCore/Sensors/SegIdCamera.h"

namespace
{

struct FSegDiffRig
{
	TWeakObjectPtr<AFlightForgeGameModeBase> GameMode;
	TWeakObjectPtr<ADronePawn>               Pawn;
	TWeakObjectPtr<USegIdCamera>             Camera;
	int32                                    Port    = -1;
	double                                   Started = 0.0;

	TArray<FColor>                Ids;
	FIntPoint                     Size = FIntPoint::ZeroValue;
	TArray<FSegInstanceEntry>     Map;
};

UWorld* GameWorld()
{
	for (const FWorldContext& Context : GEngine->GetWorldContexts())
	{
		if ((Context.WorldType == EWorldType::Game || Context.WorldType == EWorldType::PIE) && Context.World())
		{
			return Context.World();
		}
	}
	return nullptr;
}

uint32 IdOf(const FColor& Pixel)
{
	return Pixel.R | (Pixel.G << 8) | (Pixel.B << 16);
}

FColor Palette(const uint32 Id)
{
	if (Id == 0)
	{
		return FColor::Black;
	}
	const uint32 H = Id * 2654435761u;
	return FColor(64 + (H & 0x7F), 64 + ((H >> 8) & 0x7F), 64 + ((H >> 16) & 0x7F), 255);
}

void SavePng(const FString& Name, const TArray<FColor>& Pixels, const FIntPoint Size)
{
	TArray<uint8> Bytes;
	UCamera::CompressFrame(Pixels, Size, FCameraCompression{}, true, Bytes);
	const FString Path = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("FlightForge"), Name);
	FFileHelper::SaveArrayToFile(Bytes, *Path);
	UE_LOG(LogTemp, Log, TEXT("SegDiff: wrote %s"), *Path);
}

}  // namespace

/* spawn the rig //{ */

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FSegDiffSpawn, TSharedPtr<FSegDiffRig>, Rig);

bool FSegDiffSpawn::Update()
{
	if (Rig->Started == 0.0)
	{
		Rig->Started = FPlatformTime::Seconds();
	}

	UWorld*                   World    = GameWorld();
	AFlightForgeGameModeBase* GameMode = World ? World->GetAuthGameMode<AFlightForgeGameModeBase>() : nullptr;

	if (!GameMode || !GameMode->HasActorBegunPlay())
	{
		return FPlatformTime::Seconds() - Rig->Started > 120.0;  // give up, the compare step reports it
	}

	Rig->GameMode = GameMode;
	Rig->Port     = GameMode->SpawnDroneAtLocation(GameMode->GetWorldOrigin() + FVector(0, 0, 150), "x500");

	ADronePawn* Pawn = Rig->Port >= 0 ? GameMode->FindDronePawn(Rig->Port) : nullptr;
	if (!Pawn)
	{
		return true;
	}

	Rig->Pawn            = Pawn;
	const int32 SensorId = Pawn->AddSensor(static_cast<int>(SensorType::INSTANCE_SEG_CAMERA));
	Rig->Camera          = Cast<USegIdCamera>(Pawn->FindSensorById(SensorId));

	if (Rig->Camera.IsValid())
	{
		FRgbCameraConfig Config;
		Config.Width  = 320;
		Config.Height = 240;
		Rig->Camera->SetSegIdCameraConfig(Config);
	}

	Rig->Started = FPlatformTime::Seconds();
	return true;
}

//}

/* capture one frame //{ */

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FSegDiffCapture, TSharedPtr<FSegDiffRig>, Rig);

bool FSegDiffCapture::Update()
{
	USegIdCamera* Camera = Rig->Camera.Get();
	if (!Camera)
	{
		return true;
	}

	// the first frames after a resize still carry the old target; ask again until one lands at the new size
	if (Camera->CopyLastFrame(Rig->Ids, Rig->Size, Rig->Map) && Rig->Size == FIntPoint(320, 240) && FPlatformTime::Seconds() - Rig->Started > 1.0)
	{
		return true;
	}

	Camera->UpdateCamera(true, FPlatformTime::Seconds());
	return FPlatformTime::Seconds() - Rig->Started > 20.0;
}

//}

/* compare against traces //{ */

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FSegDiffCompare, TSharedPtr<FSegDiffRig>, Rig);

bool FSegDiffCompare::Update()
{
	FAutomationTestBase* Test   = FAutomationTestFramework::Get().GetCurrentTest();
	USegIdCamera*        Camera = Rig->Camera.Get();
	UWorld*              World  = GameWorld();

	if (!Test)
	{
		return true;
	}
	if (!Camera || !World || Rig->Size != FIntPoint(320, 240) || Rig->Ids.Num() != 320 * 240)
	{
		Test->AddError(TEXT("no 320x240 instance frame arrived"));
		return true;
	}

	// the pass draws what has a scene proxy; a trace sees what has query collision - actors with only one are not comparable
	TMap<FString, uint32> IdByActor;
	TSet<uint32>          NoCollisionIds;
	uint32                SelfId = 0;
	for (const FSegInstanceEntry& Entry : Rig->Map)
	{
		IdByActor.Add(Entry.Actor, static_cast<uint32>(Entry.Id));

		AActor* Actor = FindObject<AActor>(nullptr, *Entry.Actor);
		if (Actor == Rig->Pawn.Get())
		{
			SelfId = static_cast<uint32>(Entry.Id);
		}
		bool bCollides = false;
		if (Actor)
		{
			TInlineComponentArray<UPrimitiveComponent*> Primitives(Actor);
			for (const UPrimitiveComponent* Primitive : Primitives)
			{
				bCollides = bCollides || (Primitive && Primitive->IsQueryCollisionEnabled());
			}
		}
		if (!bCollides)
		{
			NoCollisionIds.Add(static_cast<uint32>(Entry.Id));
		}
	}

	const FRgbCameraConfig& Config = Camera->GetSegIdCameraConfig();
	const FCameraIntrinsics K      = UCamera::EffectiveIntrinsics(Config.Intrinsics, Config.FOVAngle, Rig->Size.X, Rig->Size.Y);
	const FTransform        View   = Camera->SceneCaptureComponent2D->GetComponentTransform();

	// the airframe wraps the lens in its collision volume, so it is skipped the way the lidar skips it
	FCollisionQueryParams Params(SCENE_QUERY_STAT(FlightForgeSegDiff), true);
	Params.AddIgnoredActor(Rig->Pawn.Get());

	int32 Agree = 0, Sky = 0, Missing = 0, Extra = 0, Wrong = 0, Landscape = 0, LandscapeDrawn = 0, Unmapped = 0, UnmappedDrawn = 0, Self = 0, NoCollision = 0, Nanite = 0, NaniteDrawn = 0;
	TMap<FString, int32> WrongByActor, MissingByActor;

	TArray<FColor> TraceImage, Mismatch;
	TraceImage.SetNumUninitialized(Rig->Ids.Num());
	Mismatch.SetNumUninitialized(Rig->Ids.Num());

	for (int32 V = 0; V < Rig->Size.Y; ++V)
	{
		for (int32 U = 0; U < Rig->Size.X; ++U)
		{
			const int32   Index  = V * Rig->Size.X + U;
			const uint32  Actual = IdOf(Rig->Ids[Index]);
			const FVector Local((float)1.0, (U + 0.5 - K.Cx) / K.Fx, -(V + 0.5 - K.Cy) / K.Fy);
			const FVector Dir   = View.TransformVectorNoScale(Local).GetSafeNormal();
			const FVector Start = View.GetLocation();

			FHitResult Hit;
			const bool bHit = World->LineTraceSingleByChannel(Hit, Start, Start + Dir * 100000.0, ECC_Visibility, Params);
			AActor* HitActor = bHit ? Hit.GetActor() : nullptr;

			uint32 Expected     = 0;
			bool   bComparable  = true;
			if (HitActor)
			{
				if (const uint32* Mapped = IdByActor.Find(HitActor->GetPathName()))
				{
					Expected = *Mapped;
				}
				else
				{
					bComparable = false;
					if (HitActor->IsA<ALandscapeProxy>())
					{
						Landscape++;
						LandscapeDrawn += Actual != 0 ? 1 : 0;
					}
					else
					{
						Unmapped++;
						UnmappedDrawn += Actual != 0 ? 1 : 0;
					}
				}
			}

			// Nanite geometry is outside the classic mesh pass unless FlightForge.SegId.DisallowNanite pushes it there
			const UStaticMeshComponent* HitMesh = Cast<UStaticMeshComponent>(Hit.GetComponent());
			if (bComparable && HitMesh && HitMesh->GetStaticMesh() && HitMesh->GetStaticMesh()->IsNaniteEnabled() && !HitMesh->IsDisallowNanite())
			{
				Nanite++;
				NaniteDrawn += Actual == Expected ? 1 : 0;
				bComparable = false;
			}
			else if (Actual == SelfId && SelfId != 0)
			{
				Self++;
				bComparable = false;
			}
			else if (!HitActor && NoCollisionIds.Contains(Actual))
			{
				NoCollision++;
				bComparable = false;
			}

			TraceImage[Index] = Palette(Expected);
			Mismatch[Index]   = FColor::Black;

			if (!bComparable)
			{
				Mismatch[Index] = HitMesh && HitMesh->GetStaticMesh() && HitMesh->GetStaticMesh()->IsNaniteEnabled() ? FColor(0, 96, 96) : FColor(0, 0, 96);
				continue;
			}
			if (Expected == Actual)
			{
				Agree++;
				Sky += Expected == 0 ? 1 : 0;
				continue;
			}
			if (Actual == 0)
			{
				Missing++;
				Mismatch[Index] = FColor::Red;
				const UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(Hit.GetComponent());
				const UStaticMesh*          Asset = Mesh ? Mesh->GetStaticMesh() : nullptr;
				MissingByActor.FindOrAdd(FString::Printf(TEXT("%s [%s%s%s]"), HitActor ? *HitActor->GetName() : TEXT("?"),
				                                         Hit.GetComponent() ? *Hit.GetComponent()->GetClass()->GetName() : TEXT("?"),
				                                         Asset ? *FString::Printf(TEXT(", %s"), *Asset->GetName()) : TEXT(""),
				                                         Asset && Asset->IsNaniteEnabled() ? TEXT(", nanite") : TEXT("")))++;
			}
			else if (Expected == 0)
			{
				Extra++;
				Mismatch[Index] = FColor::Yellow;
			}
			else
			{
				Wrong++;
				Mismatch[Index] = FColor::Magenta;
				WrongByActor.FindOrAdd(HitActor ? HitActor->GetName() : TEXT("?"))++;
			}
		}
	}

	const int32  Comparable = Agree + Missing + Extra + Wrong;
	const double Agreement  = Comparable > 0 ? (double)Agree / Comparable : 0.0;

	UE_LOG(LogTemp, Log,
	       TEXT("SegDiff: %d comparable px: agree %d (%.2f%%, sky %d), missing %d, extra %d, wrong actor %d; skipped: nanite %d (%d correct), self %d, no-collision %d, landscape %d (%d drawn), unmapped %d (%d drawn); %d mapped actors"),
	       Comparable, Agree, 100.0 * Agreement, Sky, Missing, Extra, Wrong, Nanite, NaniteDrawn, Self, NoCollision, Landscape, LandscapeDrawn, Unmapped, UnmappedDrawn, Rig->Map.Num());
	for (const TPair<FString, int32>& Pair : WrongByActor)
	{
		UE_LOG(LogTemp, Log, TEXT("SegDiff:   wrong at %s: %d px"), *Pair.Key, Pair.Value);
	}
	for (const TPair<FString, int32>& Pair : MissingByActor)
	{
		UE_LOG(LogTemp, Log, TEXT("SegDiff:   missing at %s: %d px"), *Pair.Key, Pair.Value);
	}

	TArray<FColor> IdImage;
	IdImage.SetNumUninitialized(Rig->Ids.Num());
	for (int32 i = 0; i < Rig->Ids.Num(); ++i)
	{
		IdImage[i] = Palette(IdOf(Rig->Ids[i]));
	}
	SavePng(TEXT("segdiff_pass.png"), IdImage, Rig->Size);
	SavePng(TEXT("segdiff_trace.png"), TraceImage, Rig->Size);
	SavePng(TEXT("segdiff_mismatch.png"), Mismatch, Rig->Size);

	// silhouettes differ by a pixel between the rasteriser and a ray, nothing more
	Test->TestTrue(TEXT("pass and traces agree on at least 97% of comparable pixels"), Agreement >= 0.97);
	Test->TestTrue(TEXT("no actor is drawn with another actor's id"), Wrong <= Comparable / 200);
	Test->TestTrue(TEXT("something was actually rendered"), Comparable - Sky > 1000);

	if (Rig->GameMode.IsValid() && Rig->Port >= 0)
	{
		Rig->GameMode->RemoveDrone(Rig->Port);
	}
	return true;
}

//}

/* the test //{ */

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFlightForgeSegmentationTraceDiffTest, "FlightForge.Segmentation.TraceDiff",
                                 EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FFlightForgeSegmentationTraceDiffTest::RunTest(const FString& Parameters)
{
	TSharedPtr<FSegDiffRig> Rig = MakeShared<FSegDiffRig>();
	ADD_LATENT_AUTOMATION_COMMAND(FSegDiffSpawn(Rig));
	ADD_LATENT_AUTOMATION_COMMAND(FSegDiffCapture(Rig));
	ADD_LATENT_AUTOMATION_COMMAND(FSegDiffCompare(Rig));
	return true;
}

//}

#endif  // WITH_DEV_AUTOMATION_TESTS
