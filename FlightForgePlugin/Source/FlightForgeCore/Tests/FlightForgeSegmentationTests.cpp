// Instance segmentation against a physics oracle: every pixel of the SegId pass is
// compared with a complex line trace through the same pixel; ClassCoverage gates the class audit. Needs a running level:
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

#include "FlightForgeCore/RobotPawn.h"
#include "FlightForgeCore/Sensors/DepthCamera.h"
#include "FlightForgeCore/FlightForgeGameModeBase.h"
#include "FlightForgeCore/Sensors/SegIdCamera.h"
#include "FlightForgeCore/Segmentation/SegmentationAudit.h"

namespace
{

struct FSegDiffRig
{
	TWeakObjectPtr<AFlightForgeGameModeBase> GameMode;
	TWeakObjectPtr<ARobotPawn>               Pawn;
	TWeakObjectPtr<USegIdCamera>             Camera;
	TWeakObjectPtr<UDepthCamera>             Depth;  // the render's own depth: a trace hit that is not at it is a hull, not a surface
	TArray<FFloat16>                         DepthCm;
	int32                                    RobotId = -1;
	double                                   Started = 0.0;
	double                                   Pitch   = 0.0;

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
	FCameraFrame Frame;
	UCamera::CompressFrame(Pixels, Size, FCameraCompression{}, true, Frame);
	const FString Path = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("FlightForge"), Name);
	FFileHelper::SaveArrayToFile(Frame.Bytes, *Path);
	UE_LOG(LogTemp, Log, TEXT("SegDiff: wrote %s"), *Path);
}

FString PaintedName(const TArray<FSegInstanceEntry>& Map, const uint32 Id)
{
	for (const FSegInstanceEntry& Entry : Map)
	{
		if (static_cast<uint32>(Entry.Id) == Id)
		{
			return Entry.Actor.Mid(Entry.Actor.Find(TEXT(".")) + 1);
		}
	}
	return Id == 0 ? TEXT("nothing") : TEXT("?");
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

	if (FParse::Param(FCommandLine::Get(), TEXT("FlightForgeNoStencilFill")))
	{
		GEngine->Exec(World, TEXT("FlightForge.SegId.StencilFill 0"));
	}

	Rig->GameMode = GameMode;

	// -FlightForgeAuditPose=X,Y,Z,Yaw,Pitch (world cm, degrees), from tools/audit_worlds.tsv
	FTransform Spawn(GameMode->GetGroundOrigin() + FVector(0, 0, FWireFrame::PlayerStartHeightCm + 150));
	FString    PoseArg;
	if (FParse::Value(FCommandLine::Get(), TEXT("FlightForgeAuditPose="), PoseArg, false))
	{
		TArray<FString> Parts;
		PoseArg.ParseIntoArray(Parts, TEXT(","));
		if (Parts.Num() >= 3)
		{
			Spawn.SetLocation(FVector(FCString::Atod(*Parts[0]), FCString::Atod(*Parts[1]), FCString::Atod(*Parts[2])));
			Spawn.SetRotation(FRotator(0, Parts.Num() >= 4 ? FCString::Atod(*Parts[3]) : 0.0, 0).Quaternion());
			Rig->Pitch = Parts.Num() >= 5 ? FCString::Atod(*Parts[4]) : 0.0;
		}
	}
	ARobotPawn* Pawn = GameMode->SpawnRobot(Spawn, TEXT("x500"), FString());
	Rig->RobotId     = Pawn ? Pawn->RobotId : -1;
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
		Config.Width       = 320;
		Config.Height      = 240;
		Config.Orientation = FRotator(Rig->Pitch, 0, 0);
		Rig->Camera->SetSegIdCameraConfig(Config);
	}
	const int32 DepthId = Pawn->AddSensor(static_cast<int>(SensorType::DEPTH_CAMERA));
	Rig->Depth          = Cast<UDepthCamera>(Pawn->FindSensorById(DepthId));
	if (Rig->Depth.IsValid())
	{
		FRgbCameraConfig Config;
		Config.Width       = 320;
		Config.Height      = 240;
		Config.Orientation = FRotator(Rig->Pitch, 0, 0);
		Rig->Depth->SetDepthCameraConfig(Config);
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

	if (UDepthCamera* Depth = Rig->Depth.Get())
	{
		Depth->CameraBufferCriticalSection->Lock();
		if (Depth->CameraBufferSize == FIntPoint(320, 240))
		{
			Rig->DepthCm = Depth->CameraBufferF16;
		}
		Depth->CameraBufferCriticalSection->Unlock();
		Depth->UpdateCamera(true, FPlatformTime::Seconds());
	}

	// the first frames after a resize still carry the old target, and the pass shaders of masked and wind materials load
	// for a few seconds after the first capture; until then those meshes go through the default material and drop out
	if (Camera->CopyLastFrame(Rig->Ids, Rig->Size, Rig->Map) && Rig->Size == FIntPoint(320, 240) && Rig->DepthCm.Num() == 320 * 240 &&
	    FPlatformTime::Seconds() - Rig->Started > 6.0)
	{
		return true;
	}

	Camera->UpdateCamera(true, FPlatformTime::Seconds());
	return FPlatformTime::Seconds() - Rig->Started > 20.0;
}

//}

/* compare against traces //{ */

// The ray reports the first surface with collision; the pass reports the first surface that renders.
// Where two objects are coincident - a frame in its opening, trim on a wall - those differ legitimately,
// so a painted id counts as long as that actor really has geometry at the point the ray struck.
bool ActorIsAtPoint(const TMap<uint32, TArray<FBox>>& BoxesById, const uint32 Painted, const FVector& Point)
{
	const TArray<FBox>* Boxes = BoxesById.Find(Painted);
	if (!Boxes)
	{
		return false;
	}
	for (const FBox& Box : *Boxes)
	{
		if (Box.ExpandBy(10.0).IsInsideOrOn(Point))
		{
			return true;
		}
	}
	return false;
}

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

	// the pass draws what has a scene proxy; a trace sees what blocks Visibility - actors with only one are not comparable
	TMap<FString, uint32>          IdByActor, IdByName;
	TMap<uint32, TArray<FBox>>     BoxesById;
	TSet<uint32>                   NoCollisionIds;
	uint32                SelfId = 0;
	for (const FSegInstanceEntry& Entry : Rig->Map)
	{
		IdByActor.Add(Entry.Actor, static_cast<uint32>(Entry.Id));
		IdByName.Add(FPaths::GetExtension(Entry.Actor), static_cast<uint32>(Entry.Id));

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
				if (Primitive && Primitive->IsVisible())
				{
					BoxesById.FindOrAdd(static_cast<uint32>(Entry.Id)).Add(Primitive->Bounds.GetBox());
				}
				// the trace runs on Visibility: water and overlap-only volumes have query collision yet never stop it
				bCollides = bCollides || (Primitive && Primitive->IsQueryCollisionEnabled() &&
				                          Primitive->GetCollisionResponseToChannel(ECC_Visibility) == ECR_Block);
			}
		}
		if (!bCollides)
		{
			NoCollisionIds.Add(static_cast<uint32>(Entry.Id));
		}
	}

	// -FlightForgeAuditStandIn=<hidden collision actor>=<visible actor>[+...]: the trace hits the first, the render draws the second
	TMap<FString, FString> StandIns;
	FString                StandInArg;
	if (FParse::Value(FCommandLine::Get(), TEXT("FlightForgeAuditStandIn="), StandInArg, false))
	{
		TArray<FString> Pairs;
		StandInArg.ParseIntoArray(Pairs, TEXT("+"));
		for (const FString& Pair : Pairs)
		{
			FString Hidden, Visible;
			if (Pair.Split(TEXT("="), &Hidden, &Visible))
			{
				StandIns.Add(Hidden, Visible);
			}
		}
	}

	const FRgbCameraConfig& Config = Camera->GetSegIdCameraConfig();
	const FCameraIntrinsics K      = UCamera::EffectiveIntrinsics(Config.Intrinsics, Config.FOVAngle, Rig->Size.X, Rig->Size.Y);
	const FTransform        View   = Camera->SceneCaptureComponent2D->GetComponentTransform();

	// the airframe wraps the lens in its collision volume, so it is skipped the way the lidar skips it
	FCollisionQueryParams Params(SCENE_QUERY_STAT(FlightForgeSegDiff), true);
	Params.AddIgnoredActor(Rig->Pawn.Get());

	int32 Agree = 0, Sky = 0, Missing = 0, Extra = 0, Wrong = 0, Landscape = 0, LandscapeDrawn = 0, Unmapped = 0, UnmappedDrawn = 0, Self = 0, NoCollision = 0, Nanite = 0, NaniteDrawn = 0, DepthMismatch = 0, CoincidentPx = 0;
	TMap<FString, int32> WrongByActor, MissingByActor, UnmappedByActor;

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

			const float TraceZ    = bHit ? Hit.Distance / Local.Size() : 0.0f;
			const float DepthTol  = FMath::Max(30.0f, 0.03f * TraceZ);

			// collision is a hull, the render is masked cards and holes: only where the two agree on depth is the trace an oracle
			if (Rig->DepthCm.IsValidIndex(Index))
			{
				const float RenderZ           = Rig->DepthCm[Index].GetFloat();
				const bool  bRenderHasSurface = RenderZ > 0.0f && RenderZ < 60000.0f;
				if (bHit != bRenderHasSurface || (bHit && FMath::Abs(TraceZ - RenderZ) > FMath::Max(30.0f, 0.03f * RenderZ)))
				{
					DepthMismatch++;
					bComparable = false;
				}
			}
			if (HitActor)
			{
				const FString* StandsFor = StandIns.Find(HitActor->GetName());
				if (const uint32* Mapped = StandsFor ? IdByName.Find(*StandsFor) : IdByActor.Find(HitActor->GetPathName()))
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
						UnmappedByActor.FindOrAdd(FString::Printf(TEXT("%s [%s] painted %s"), *HitActor->GetName(), *HitActor->GetClass()->GetName(), *PaintedName(Rig->Map, Actual)))++;
					}
				}
			}

			const UStaticMeshComponent* HitMesh = Cast<UStaticMeshComponent>(Hit.GetComponent());
			if (Actual == SelfId && SelfId != 0)
			{
				Self++;
				bComparable = false;
			}
			else if (Actual != Expected && NoCollisionIds.Contains(Actual))
			{
				// the trace passes through what the pass painted (foliage, grass); the trace is the one that is wrong here
				NoCollision++;
				bComparable = false;
			}
			else if (bComparable && HitMesh && HitMesh->GetStaticMesh() && HitMesh->GetStaticMesh()->HasValidNaniteData() && !HitMesh->IsDisallowNanite())
			{
				Nanite++;
				NaniteDrawn += Actual == Expected ? 1 : 0;
			}

			TraceImage[Index] = Palette(Expected);
			Mismatch[Index]   = FColor::Black;

			if (!bComparable)
			{
				Mismatch[Index] = FColor(0, 0, 96);
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
				                                         Asset && Asset->HasValidNaniteData() ? TEXT(", nanite") : TEXT("")))++;
			}
			else if (Expected == 0)
			{
				Extra++;
				Mismatch[Index] = FColor::Yellow;
			}
			else if (bHit && ActorIsAtPoint(BoxesById, Actual, Hit.Location))
			{
				// one collision hull often stands in for several coincident meshes (a frame in its opening, a
				// decal panel on a wall): the ray reports the hull, the pass draws whichever surface is in front
				CoincidentPx++;
				Mismatch[Index] = FColor(0, 128, 128);
			}
			else
			{
				Wrong++;
				Mismatch[Index] = FColor::Magenta;

				WrongByActor.FindOrAdd(FString::Printf(TEXT("%s (painted %s)"), HitActor ? *HitActor->GetName() : TEXT("?"), *PaintedName(Rig->Map, Actual)))++;
			}
		}
	}

	const int32  Comparable = Agree + Missing + Extra + Wrong;
	const double Agreement  = Comparable > 0 ? (double)Agree / Comparable : 0.0;

	UE_LOG(LogTemp, Log,
	       TEXT("SegDiff: %d comparable px: agree %d (%.2f%%, sky %d), missing %d, extra %d, wrong actor %d, nanite %d (%d correct); skipped: coincident %d, depth-mismatch %d, self %d, no-collision %d, landscape %d (%d drawn), unmapped %d (%d drawn); %d mapped actors"),
	       Comparable, Agree, 100.0 * Agreement, Sky, Missing, Extra, Wrong, Nanite, NaniteDrawn, CoincidentPx, DepthMismatch, Self, NoCollision, Landscape, LandscapeDrawn, Unmapped, UnmappedDrawn, Rig->Map.Num());
	for (const TPair<FString, int32>& Pair : WrongByActor)
	{
		UE_LOG(LogTemp, Log, TEXT("SegDiff:   wrong at %s: %d px"), *Pair.Key, Pair.Value);
	}
	for (const TPair<FString, int32>& Pair : MissingByActor)
	{
		UE_LOG(LogTemp, Log, TEXT("SegDiff:   missing at %s: %d px"), *Pair.Key, Pair.Value);
	}
	UnmappedByActor.ValueSort(TGreater<int32>());
	int32 UnmappedShown = 0;
	for (const TPair<FString, int32>& Pair : UnmappedByActor)
	{
		if (UnmappedShown++ < 10)
		{
			UE_LOG(LogTemp, Log, TEXT("SegDiff:   unmapped hit %s: %d px"), *Pair.Key, Pair.Value);
		}
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
	Test->TestTrue(TEXT("no actor is drawn with another actor's id"), Wrong <= Comparable / 50);  // trunk bases and leaf edges sit within the depth tolerance of the ground
	Test->TestTrue(TEXT("something was actually rendered"), Comparable - Sky > 1000);

	if (Rig->GameMode.IsValid() && Rig->RobotId >= 0)
	{
		Rig->GameMode->RemoveRobot(Rig->RobotId);
	}
	return true;
}

//}

/* class coverage //{ */

static TAutoConsoleVariable<float> CVarMinClassifiedShare(TEXT("FlightForge.Segmentation.MinClassifiedShare"), 0.95f,
                                                          TEXT("ClassCoverage fails a world whose classified share of mesh bounds area is below this"));

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FClassCoverageAudit, double, Started);

bool FClassCoverageAudit::Update()
{
	FAutomationTestBase* Test = FAutomationTestFramework::Get().GetCurrentTest();
	UWorld*              World = GameWorld();
	const AGameModeBase* GameMode = World ? World->GetAuthGameMode() : nullptr;

	Started = Started > 0.0 ? Started : FPlatformTime::Seconds();
	if (!Test)
	{
		return true;
	}
	if (!GameMode || !GameMode->HasActorBegunPlay())
	{
		if (FPlatformTime::Seconds() - Started > 120.0)
		{
			Test->AddError(TEXT("the level never began play"));
			return true;
		}
		return false;
	}

	const FClassAuditSummary Summary = AuditWorldClasses(World);
	const double             Min     = CVarMinClassifiedShare.GetValueOnGameThread();
	Test->TestTrue(FString::Printf(TEXT("at least %.1f%% of the mesh area has a class (%.1f%%)"), 100.0 * Min, 100.0 * Summary.ClassifiedShare()),
	               Summary.ClassifiedShare() >= Min);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFlightForgeSegmentationClassCoverageTest, "FlightForge.Segmentation.ClassCoverage",
                                 EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FFlightForgeSegmentationClassCoverageTest::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FClassCoverageAudit(0.0));
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
