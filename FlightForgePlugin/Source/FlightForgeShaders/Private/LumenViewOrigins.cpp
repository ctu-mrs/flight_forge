#include "LumenViewOrigins.h"

#include "HAL/IConsoleManager.h"
#include "RenderingThread.h"
#include "SceneView.h"
#include "ScenePrivate.h"

/* console variables //{ */

namespace
{

TAutoConsoleVariable<int32> CVarOrigins(TEXT("FlightForge.Lumen.AllViewOrigins"), 1,
                                        TEXT("1: every family's Lumen scene update keeps surface cache cards around every view that rendered Lumen recently (viewport and cameras); 0: only the family's own views"));

TAutoConsoleVariable<int32> CVarOriginFrames(TEXT("FlightForge.Lumen.AllViewOrigins.Frames"), 90,
                                             TEXT("frames a view's origin stays in the list after it last rendered with Lumen"));

TAutoConsoleVariable<int32> CVarMaxOrigins(TEXT("FlightForge.Lumen.AllViewOrigins.Max"), 8, TEXT("most origins handed to one family, nearest first after the viewport"));

TAutoConsoleVariable<int32> CVarGdfCatchUp(TEXT("FlightForge.Lumen.DistanceFieldCatchUp"), 1,
                                            TEXT("1: a view's global distance field that skipped frames takes the primitive changes it missed; 0: the engine's own, which only a rendering view sees"));

constexpr int32 GdfHistoryFrames = 600;
static_assert(GDF_Num == 2, "FGdfFrame::Bounds holds one array per distance field cache type");

bool UsesLumen(const FSceneView& View)
{
	const FFinalPostProcessSettings& Settings = View.FinalPostProcessSettings;
	return Settings.DynamicGlobalIlluminationMethod == EDynamicGlobalIlluminationMethod::Lumen || Settings.ReflectionMethod == EReflectionMethod::Lumen;
}

}  // namespace

/* //} */

/* FLumenViewOrigins //{ */

FLumenViewOrigins::FLumenViewOrigins(const FAutoRegister& AutoRegister) : FSceneViewExtensionBase(AutoRegister)
{
}

void FLumenViewOrigins::Register()
{ //{
	static TSharedPtr<FLumenViewOrigins, ESPMode::ThreadSafe> Extension;
	if (!Extension.IsValid())
	{
		Extension = FSceneViewExtensions::NewExtension<FLumenViewOrigins>();
	}
} //}

bool FLumenViewOrigins::IsActiveThisFrame_Internal(const FSceneViewExtensionContext& Context) const
{
	return CVarOrigins.GetValueOnAnyThread() != 0 || CVarGdfCatchUp.GetValueOnAnyThread() != 0;
}

void FLumenViewOrigins::PostRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily)
{ //{
	if (CVarGdfCatchUp.GetValueOnRenderThread() != 0 && InViewFamily.Scene && InViewFamily.Scene->GetRenderScene())
	{
		FScopeLock Scope(&Lock);
		RecordModifiedBounds(*InViewFamily.Scene->GetRenderScene());
	}
} //}

void FLumenViewOrigins::RecordModifiedBounds(const FScene& Scene)
{ //{
	const uint64 Frame = GFrameCounterRenderThread;
	const auto&  Bounds = Scene.DistanceFieldSceneData.PrimitiveModifiedBounds;
	bool         bAny   = false;
	for (const TArray<FBox>& Type : Bounds)
	{
		bAny |= !Type.IsEmpty();
	}
	if (!bAny)
	{
		return;
	}
	if (GdfHistory.IsEmpty() || GdfHistory.Last().Frame != Frame)
	{
		GdfHistory.AddDefaulted_GetRef().Frame = Frame;
	}
	for (int32 Type = 0; Type < GDF_Num; ++Type)
	{
		GdfHistory.Last().Bounds[Type] = Bounds[Type];
	}
	while (GdfHistory.Num() > 1 && GdfHistory[0].Frame + GdfHistoryFrames < Frame)
	{
		GdfCoveredFrom = GdfHistory[0].Frame + 1;
		GdfHistory.RemoveAt(0);
	}
} //}

void FLumenViewOrigins::CatchUpDistanceFields(FSceneViewFamily& InViewFamily)
{ //{
	const uint64 Frame = GFrameCounterRenderThread;
	for (const FSceneView* View : InViewFamily.Views)
	{
		FSceneViewState* State = View ? static_cast<FSceneViewState*>(View->State) : nullptr;
		FPersistentGlobalDistanceFieldData* Data = State ? State->GlobalDistanceFieldData.GetReference() : nullptr;
		if (!Data)
		{
			continue;
		}
		uint64& Last = GdfLastFrame.FindOrAdd(Data, Frame);
		if (Last + 1 < Frame && Data->bInitializedOrigins)
		{
			if (Last < GdfCoveredFrom)
			{
				Data->bInitializedOrigins = false;
			}
			else
			{
				for (const FGdfFrame& Missed : GdfHistory)
				{
					if (Missed.Frame <= Last || Missed.Frame >= Frame)
					{
						continue;
					}
					for (FGlobalDistanceFieldClipmapState& Clipmap : Data->ClipmapState)
					{
						for (int32 Type = 0; Type < GDF_Num; ++Type)
						{
							Clipmap.Cache[Clipmap.CacheMostlyStaticSeparately ? Type : GDF_Full].PrimitiveModifiedBounds.Append(Missed.Bounds[Type]);
						}
					}
				}
			}
		}
		Last = Frame;
	}
	for (auto It = GdfLastFrame.CreateIterator(); It; ++It)
	{
		if (It.Value() + GdfHistoryFrames < Frame)
		{
			It.RemoveCurrent();
		}
	}
} //}

void FLumenViewOrigins::PreRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily)
{ //{
	if (CVarGdfCatchUp.GetValueOnRenderThread() != 0 && InViewFamily.Scene && InViewFamily.Scene->GetRenderScene())
	{
		FScopeLock Scope(&Lock);
		RecordModifiedBounds(*InViewFamily.Scene->GetRenderScene());
		CatchUpDistanceFields(InViewFamily);
	}
	if (CVarOrigins.GetValueOnRenderThread() == 0)
	{
		return;
	}

	const uint64 Frame  = GFrameCounterRenderThread;
	const uint64 Expiry = (uint64)FMath::Max(CVarOriginFrames.GetValueOnRenderThread(), 1);

	TArray<FVector, TInlineAllocator<4>> Own;
	for (const FSceneView* View : InViewFamily.Views)
	{
		if (!View || !View->State || !UsesLumen(*View) || View->bIsReflectionCapture || View->bIsPlanarReflection)
		{
			continue;
		}
		Own.Add(View->ViewMatrices.GetViewOrigin());
	}
	if (Own.IsEmpty())
	{
		return;
	}

	TArray<FOrigin, TInlineAllocator<16>> Others;
	{
		FScopeLock Scope(&Lock);
		for (const FSceneView* View : InViewFamily.Views)
		{
			if (View && View->State && UsesLumen(*View) && !View->bIsReflectionCapture && !View->bIsPlanarReflection)
			{
				Origins.Add(View->State->GetViewKey(), FOrigin{View->ViewMatrices.GetViewOrigin(), Frame, !View->bIsSceneCapture});
			}
		}
		for (auto It = Origins.CreateIterator(); It; ++It)
		{
			if (Frame > It.Value().Frame + Expiry)
			{
				It.RemoveCurrent();
			}
			else if (!Own.ContainsByPredicate([&It](const FVector& Location) { return FVector::DistSquared(Location, It.Value().Location) < 1.0e4; }))
			{
				Others.Add(It.Value());
			}
		}
	}

	const FVector Anchor = Own[0];
	// a stock engine reads only the first origin beyond its own views (LUMEN_MAX_VIEWS 2): the viewport, else the nearest camera
	Others.Sort([&Anchor](const FOrigin& A, const FOrigin& B) {
		return A.bViewport != B.bViewport ? A.bViewport : FVector::DistSquared(A.Location, Anchor) < FVector::DistSquared(B.Location, Anchor);
	});
	const int32 Max = FMath::Max(CVarMaxOrigins.GetValueOnRenderThread(), 0);
	for (int32 Index = 0; Index < Others.Num() && InViewFamily.StreamingViewOrigins.Num() < Max; ++Index)
	{
		InViewFamily.StreamingViewOrigins.Add(Others[Index].Location);
	}
} //}

/* //} */
