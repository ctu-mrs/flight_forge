// Editor-only ground truth for segmentation: dumps a per-actor instance mask rendered
// through the engine's hit-proxy pass, which is pixel-exact and unlimited in instance
// count. Used to validate the stencil-based segmentation (255-value cap, collisions)
// and, later, the custom-mesh-pass implementation. The hit-proxy renderer only exists
// WITH_EDITOR, so this is a prototype tool, not a wire-facing sensor.

#if WITH_EDITOR

#include "Components/PrimitiveComponent.h"
#include "Editor.h"
#include "EngineUtils.h"
#include "HAL/IConsoleManager.h"
#include "ImageUtils.h"
#include "LevelEditorViewport.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

namespace
{

/* DumpSegGroundTruth() //{ */

int32 ActorStencil(const AActor* Actor)
{
	TArray<UPrimitiveComponent*> Primitives;
	Actor->GetComponents<UPrimitiveComponent>(Primitives);

	for (const UPrimitiveComponent* Primitive : Primitives)
	{
		if (Primitive->bRenderCustomDepth)
		{
			return Primitive->CustomDepthStencilValue;
		}
	}

	return -1;
}

void DumpSegGroundTruth(const TArray<FString>& Args)
{
	FLevelEditorViewportClient* Client = nullptr;
	for (FLevelEditorViewportClient* Candidate : GEditor->GetLevelViewportClients())
	{
		if (Candidate && Candidate->IsPerspective() && Candidate->Viewport)
		{
			Client = Candidate;
			break;
		}
	}

	if (!Client)
	{
		UE_LOG(LogTemp, Error, TEXT("[SegGroundTruth] no perspective level viewport; run in the editor or Simulate-in-Editor"));
		return;
	}

	FViewport*      Viewport = Client->Viewport;
	const FIntPoint Size     = Viewport->GetSizeXY();

	if (Size.X <= 0 || Size.Y <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("[SegGroundTruth] viewport has no size"));
		return;
	}

	// Renders the hit-proxy pass on demand when the cached map is stale.
	TArray<HHitProxy*> ProxyMap;
	Viewport->GetHitProxyMap(FIntRect(0, 0, Size.X, Size.Y), ProxyMap);

	if (ProxyMap.Num() != Size.X * Size.Y)
	{
		UE_LOG(LogTemp, Error, TEXT("[SegGroundTruth] hit proxy map is %d entries for a %dx%d viewport"), ProxyMap.Num(), Size.X, Size.Y);
		return;
	}

	// Compact per-actor instance ids, encoded 24-bit little-endian in RGB. Id 0 is
	// reserved for "no actor" so unlabelled pixels are pure black.
	TMap<const AActor*, int32> ActorIds;
	TArray<const AActor*>      Actors;

	TArray<FColor> Mask;
	Mask.Init(FColor(0, 0, 0, 255), ProxyMap.Num());

	for (int32 Index = 0; Index < ProxyMap.Num(); ++Index)
	{
		HHitProxy* Proxy = ProxyMap[Index];
		if (!Proxy || !Proxy->IsA(HActor::StaticGetType()))
		{
			continue;
		}

		const AActor* Actor = static_cast<HActor*>(Proxy)->Actor;
		if (!Actor)
		{
			continue;
		}

		int32* Found = ActorIds.Find(Actor);
		int32  Id;
		if (Found)
		{
			Id = *Found;
		}
		else
		{
			Id = Actors.Add(Actor) + 1;
			ActorIds.Add(Actor, Id);
		}

		Mask[Index] = FColor(Id & 0xFF, (Id >> 8) & 0xFF, (Id >> 16) & 0xFF, 255);
	}

	const FString Directory = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("FlightForge"));
	const FString BaseName  = FString::Printf(TEXT("seg_groundtruth_%s"), *FDateTime::Now().ToString(TEXT("%H%M%S")));

	TArray64<uint8> Png;
	FImageUtils::PNGCompressImageArray(Size.X, Size.Y, Mask, Png);
	const FString PngPath = FPaths::Combine(Directory, BaseName + TEXT(".png"));
	FFileHelper::SaveArrayToFile(Png, *PngPath);

	// id -> actor mapping plus each actor's stencil
	TSet<int32> UsedStencils;
	int32       StencilCollisions = 0;

	FString Json = TEXT("{\n");
	for (int32 Index = 0; Index < Actors.Num(); ++Index)
	{
		const AActor* Actor   = Actors[Index];
		const int32   Stencil = ActorStencil(Actor);

		if (Stencil >= 0)
		{
			bool bAlreadyUsed = false;
			UsedStencils.Add(Stencil, &bAlreadyUsed);
			StencilCollisions += bAlreadyUsed ? 1 : 0;
		}

		Json += FString::Printf(TEXT("  \"%d\": {\"actor\": \"%s\", \"class\": \"%s\", \"stencil\": %d}%s\n"), Index + 1, *Actor->GetPathName(),
		                        *Actor->GetClass()->GetName(), Stencil, Index + 1 < Actors.Num() ? TEXT(",") : TEXT(""));
	}
	Json += TEXT("}\n");

	const FString JsonPath = FPaths::Combine(Directory, BaseName + TEXT(".json"));
	FFileHelper::SaveStringToFile(Json, *JsonPath);

	UE_LOG(LogTemp, Log, TEXT("[SegGroundTruth] %d instances (%d with a stencil, %d stencil collisions) -> %s + .json"), Actors.Num(), UsedStencils.Num(),
	       StencilCollisions, *PngPath);
}

/* //} */

FAutoConsoleCommand GDumpSegGroundTruthCommand(
    TEXT("FlightForge.DumpSegGroundTruth"),
    TEXT("Renders the level viewport through the hit-proxy pass and saves a 24-bit per-actor instance mask plus a JSON id->actor/stencil mapping to "
         "Saved/FlightForge/. Editor and Simulate-in-Editor only."),
    FConsoleCommandWithArgsDelegate::CreateStatic(&DumpSegGroundTruth));

}  // namespace

#endif  // WITH_EDITOR
