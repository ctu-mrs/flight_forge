#pragma once

#include "CoreMinimal.h"

class FSceneViewFamily;
class FTextureRenderTargetResource;
class ISceneRenderBuilder;
class ISceneViewExtension;

/* FMultiViewRender //{ */

struct FMultiViewTile
{
	FIntRect                      Rect;
	FTextureRenderTargetResource* Target = nullptr;
};

// renders a family of views into Atlas as one scene renderer, then copies each tile into its own target; no tiles: Atlas is the views' own target
class FLIGHTFORGESHADERS_API FMultiViewRender
{
public:
	static void Add(ISceneRenderBuilder& Builder, FSceneViewFamily& Family, FString&& Name, FTextureRenderTargetResource* Atlas, TArray<FMultiViewTile>&& Tiles);

	// last after-tonemap pass of a TSR family: the engine's untouched-colour copy ignores the destination rect, so every view would land on tile 0
	static TSharedRef<ISceneViewExtension, ESPMode::ThreadSafe> OutputExtension();
};

/* //} */
