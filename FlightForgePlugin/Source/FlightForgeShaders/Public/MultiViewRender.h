#pragma once

#include "CoreMinimal.h"

class FSceneViewFamily;
class FTextureRenderTargetResource;
class ISceneRenderBuilder;

/* FMultiViewRender //{ */

struct FMultiViewTile
{
	FIntRect                      Rect;
	FTextureRenderTargetResource* Target = nullptr;
};

// renders a family of several views into Atlas as one scene renderer, then copies each tile into its own target
class FLIGHTFORGESHADERS_API FMultiViewRender
{
public:
	static void Add(ISceneRenderBuilder& Builder, FSceneViewFamily& Family, FString&& Name, FTextureRenderTargetResource* Atlas, TArray<FMultiViewTile>&& Tiles);
};

/* //} */
