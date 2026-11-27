// FlightForge.AuditClasses: how much of a world the class tree covers, and which assets to
// write rules for next. Console command in PIE/-game, or headless:
//   UnrealEditor-Cmd <project> <map> -game -RenderOffScreen -ExecCmds="FlightForge.AuditClasses"
// Writes Saved/FlightForge/classes_<map>.csv and logs the summary. See docs/asset_classification.md.

#include "SegmentationAudit.h"
#include "SegmentationTagger.h"

#include "Components/MeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "HAL/IConsoleManager.h"
#include "LandscapeComponent.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

namespace
{

/* helpers //{ */

struct FAuditRow
{
	FString Asset;   // mesh asset path, or the component class for landscape and the like
	int32   ClassId = 0;
	FString Source;  // rule:<match> | folder | stencil | (empty)
	TSet<FString> ActorClasses;
	int32   Components = 0;
	double  AreaM2     = 0.0;  // bounding-box surface, a stand-in for how much of a view it can cover
	TSet<FString> Actors;
};

// name keywords worth suggesting as rules for what stays unclassified; the human decides
const TPair<const TCHAR*, const TCHAR*> KeywordHints[] = {
    {TEXT("*Tree*"), TEXT("vegetation/tree")},     {TEXT("*Bush*"), TEXT("vegetation/bush")},     {TEXT("*Grass*"), TEXT("terrain/grass")},
    {TEXT("*Rock*"), TEXT("terrain/rock")},        {TEXT("*Cliff*"), TEXT("terrain/rock")},       {TEXT("*Road*"), TEXT("terrain/road")},
    {TEXT("*Fence*"), TEXT("structure/fence")},    {TEXT("*Wall*"), TEXT("structure/wall")},      {TEXT("*Roof*"), TEXT("structure/building/roof")},
    {TEXT("*Pole*"), TEXT("structure/pole")},      {TEXT("*Lamp*"), TEXT("structure/pole")},      {TEXT("*Tower*"), TEXT("structure/tower")},
    {TEXT("*Pylon*"), TEXT("structure/tower")},    {TEXT("*Sign*"), TEXT("structure/sign")},      {TEXT("*Door*"), TEXT("structure/door")},
    {TEXT("*Window*"), TEXT("structure/window")},  {TEXT("*Stair*"), TEXT("structure/stairs")},   {TEXT("*Pipe*"), TEXT("structure/pipe")},
    {TEXT("*Cable*"), TEXT("structure/power_line")}, {TEXT("*Wire*"), TEXT("structure/power_line")}, {TEXT("*Car*"), TEXT("vehicle/car")},
    {TEXT("*Truck*"), TEXT("vehicle/truck")},      {TEXT("*Bus*"), TEXT("vehicle/truck")},        {TEXT("*Airplane*"), TEXT("vehicle/aircraft")},
    {TEXT("*Heli*"), TEXT("vehicle/aircraft")},    {TEXT("*Container*"), TEXT("prop/container")}, {TEXT("*Barrel*"), TEXT("prop/container")},
    {TEXT("*Tank*"), TEXT("prop/container")},      {TEXT("*Crate*"), TEXT("prop/box")},           {TEXT("*Box*"), TEXT("prop/box")},
    {TEXT("*Pallet*"), TEXT("prop/box")},          {TEXT("*Table*"), TEXT("prop/furniture")},     {TEXT("*Chair*"), TEXT("prop/furniture")},
    {TEXT("*Shelf*"), TEXT("prop/furniture")},     {TEXT("*Debris*"), TEXT("prop/debris")},       {TEXT("*Garbage*"), TEXT("prop/debris")},
    {TEXT("*Rubble*"), TEXT("prop/debris")},       {TEXT("*Gate*"), TEXT("prop/gate_target")},    {TEXT("*Sky*"), TEXT("sky")},
    {TEXT("*Water*"), TEXT("terrain/water")},      {TEXT("*Building*"), TEXT("structure/building")}, {TEXT("*House*"), TEXT("structure/building")},
};

FString SuggestRule(const FString& Asset)
{
	const FString Name = FPaths::GetBaseFilename(Asset);
	for (const auto& Hint : KeywordHints)
	{
		if (Name.MatchesWildcard(Hint.Key))
		{
			return FString::Printf(TEXT("{path: \"*%s*\", class: %s}"), Hint.Key + 1, Hint.Value).Replace(TEXT("**"), TEXT("*"));
		}
	}
	return FString();
}

FString Csv(const FString& S)
{
	return TEXT("\"") + S.Replace(TEXT("\""), TEXT("\"\"")) + TEXT("\"");
}

//}

}  // namespace

/* AuditWorldClasses() //{ */

double FClassAuditSummary::ClassifiedShare() const
{
	if (AreaM2 > 0.0)
	{
		return ClassifiedAreaM2 / AreaM2;
	}
	return Components > 0 ? static_cast<double>(Classified) / Components : 1.0;
}

FClassAuditSummary AuditWorldClasses(UWorld* World)
{
	FClassAuditSummary Summary;
	if (!World)
	{
		return Summary;
	}
	USegmentationTagger* Tagger = World->GetSubsystem<USegmentationTagger>();
	if (!Tagger)
	{
		UE_LOG(LogTemp, Error, TEXT("[AuditClasses] no segmentation tagger in this world"));
		return Summary;
	}

	TMap<FString, FAuditRow> Rows;

	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor*                      Actor = *It;
		TArray<UPrimitiveComponent*> Primitives;
		Actor->GetComponents<UPrimitiveComponent>(Primitives);

		for (const UPrimitiveComponent* Component : Primitives)
		{
			const bool bMesh = Component->IsA<UMeshComponent>() || Component->IsA<ULandscapeComponent>();
			if (!bMesh || !Component->IsVisible() || Component->bHiddenInGame)
			{
				continue;
			}

			FString Asset = USegmentationTagger::MeshAssetPath(Component);
			if (Asset.IsEmpty())
			{
				Asset = TEXT("<") + Component->GetClass()->GetName() + TEXT(">");
			}

			// sky spheres and backdrop planes would swamp the area statistic; they count as one component, no area
			const FVector E = Component->Bounds.BoxExtent / 100.0;
			const double  A = Component->Bounds.SphereRadius < 20000.0f ? 8.0 * (E.X * E.Y + E.Y * E.Z + E.X * E.Z) : 0.0;
			const int32   Cls  = Tagger->ClassOf(Actor, Component);

			FAuditRow& Row = Rows.FindOrAdd(Asset);
			Row.Asset      = Asset;
			Row.ClassId    = Cls;
			Row.Source     = Tagger->ClassSourceOf(Actor, Component);
			Row.Components += 1;
			Row.AreaM2 += A;
			Row.Actors.Add(Actor->GetName());
			Row.ActorClasses.Add(Actor->GetClass()->GetName());

			Summary.Components += 1;
			Summary.AreaM2 += A;
			if (Cls > 0)
			{
				Summary.Classified += 1;
				Summary.ClassifiedAreaM2 += A;
			}
		}
	}

	TArray<FAuditRow> Sorted;
	Rows.GenerateValueArray(Sorted);
	Sorted.Sort([](const FAuditRow& A, const FAuditRow& B) { return A.AreaM2 > B.AreaM2; });

	const TMap<int32, FSegClass>& Classes = Tagger->Classes();
	auto                          ClassPath = [&Classes](int32 Id) { return Classes.Contains(Id) ? Classes[Id].Path : (Id > 0 ? FString::Printf(TEXT("class_%d"), Id) : FString()); };

	FString Out = TEXT("asset,class_id,class_path,source,components,actors,actor_class,bounds_area_m2,suggested_rule\n");
	for (const FAuditRow& Row : Sorted)
	{
		Out += FString::Printf(TEXT("%s,%d,%s,%s,%d,%d,%s,%.1f,%s\n"), *Csv(Row.Asset), Row.ClassId, *Csv(ClassPath(Row.ClassId)), *Csv(Row.Source), Row.Components,
		                       Row.Actors.Num(), *Csv(FString::Join(Row.ActorClasses.Array(), TEXT(" "))), Row.AreaM2,
		                       *Csv(Row.ClassId > 0 ? FString() : SuggestRule(Row.Asset)));
	}

	const FString MapName = World->GetMapName().Replace(TEXT("UEDPIE_0_"), TEXT(""));
	const FString Path    = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("FlightForge"), FString::Printf(TEXT("classes_%s.csv"), *MapName));
	FFileHelper::SaveStringToFile(Out, *Path);

	const double PctComponents = Summary.Components ? 100.0 * Summary.Classified / Summary.Components : 0.0;
	const double PctArea       = Summary.AreaM2 > 0.0 ? 100.0 * Summary.ClassifiedAreaM2 / Summary.AreaM2 : 0.0;
	UE_LOG(LogTemp, Warning, TEXT("[AuditClasses] %s: %d mesh components in %d assets; classified %.1f%% of components, %.1f%% of bounds area; %s"), *MapName,
	       Summary.Components, Rows.Num(), PctComponents, PctArea, *Path);

	int32 Shown = 0;
	for (const FAuditRow& Row : Sorted)
	{
		if (Row.ClassId > 0 || Shown >= 20)
		{
			continue;
		}
		++Shown;
		UE_LOG(LogTemp, Warning, TEXT("[AuditClasses]   unclassified %8.0f m2 x%-4d %s  %s"), Row.AreaM2, Row.Components, *Row.Asset, *SuggestRule(Row.Asset));
	}
	return Summary;
}

static FAutoConsoleCommandWithWorldAndArgs GAuditClassesCommand(TEXT("FlightForge.AuditClasses"),
                                                                TEXT("Reports how much of this world the segmentation class tree covers and writes Saved/FlightForge/classes_<map>.csv"),
                                                                FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>&, UWorld* World) { AuditWorldClasses(World); }));

//}
