#pragma once

#include "CoreMinimal.h"
#include "SceneViewExtension.h"

/* FLumenViewOrigins //{ */

class FScene;

// card culling around every Lumen view; a distance field that skipped frames gets the primitive changes the engine kept for one frame only
class FLIGHTFORGESHADERS_API FLumenViewOrigins : public FSceneViewExtensionBase
{
public:
	FLumenViewOrigins(const FAutoRegister& AutoRegister);

	static void Register();

	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override {}
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override {}
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override {}
	virtual void PreRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily) override;
	virtual void PostRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily) override;

protected:
	virtual bool IsActiveThisFrame_Internal(const FSceneViewExtensionContext& Context) const override;

private:
	struct FOrigin
	{
		FVector Location;
		uint64  Frame     = 0;
		bool    bViewport = false;
	};

	struct FGdfFrame
	{
		uint64       Frame = 0;
		TArray<FBox> Bounds[2];  // GDF_Num
	};

	void RecordModifiedBounds(const FScene& Scene);
	void CatchUpDistanceFields(FSceneViewFamily& InViewFamily);

	FCriticalSection             Lock;
	TMap<uint32, FOrigin>        Origins;
	TArray<FGdfFrame>            GdfHistory;
	uint64                       GdfCoveredFrom = 0;
	TMap<const void*, uint64>    GdfLastFrame;
};

/* //} */
