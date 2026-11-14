#include "MultiViewRender.h"

#include "ReadbackPass.h"

#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
#include "SceneRenderBuilderInterface.h"
#include "SceneRendering.h"
#include "TextureResource.h"

/* FMultiViewRender //{ */

void FMultiViewRender::Add(ISceneRenderBuilder& Builder, FSceneViewFamily& Family, FString&& Name, FTextureRenderTargetResource* Atlas, TArray<FMultiViewTile>&& Tiles)
{
	FSceneRenderer* Renderer = Builder.CreateSceneRenderer(&Family);
	const uint32    Frame    = Family.FrameNumber;
	const double    Time     = Family.Time.GetWorldTimeSeconds();

	SCENE_RENDER_GROUP_SCOPE(Builder, MoveTemp(Name), ESceneRenderGroupFlags::None);

	Builder.AddRenderer(Renderer, [Atlas, Tiles = MoveTemp(Tiles), Frame, Time](FRDGBuilder& GraphBuilder, const FSceneRenderFunctionInputs& Inputs) {
		Inputs.Renderer->Render(GraphBuilder, Inputs.SceneUpdateInputs);

		FRDGTextureRef Source = RegisterExternalTexture(GraphBuilder, Atlas->GetRenderTargetTexture(), TEXT("FlightForge.MultiViewAtlas"));
		for (const FMultiViewTile& Tile : Tiles)
		{
			FRHITexture* TargetTexture = Tile.Target->GetRenderTargetTexture();
			if (!TargetTexture)
			{
				continue;
			}
			FRDGTextureRef Dest = RegisterExternalTexture(GraphBuilder, TargetTexture, TEXT("FlightForge.MultiViewTile"));

			FRHICopyTextureInfo Copy;
			Copy.SourcePosition = FIntVector(Tile.Rect.Min.X, Tile.Rect.Min.Y, 0);
			Copy.Size           = FIntVector(Tile.Rect.Width(), Tile.Rect.Height(), 1);
			AddCopyTexturePass(GraphBuilder, Source, Dest, Copy);

			FReadbackViewExtension::Get()->CopyJob_RenderThread(GraphBuilder, Tile.Target, Dest, Frame, Time);
			GraphBuilder.SetTextureAccessFinal(Dest, ERHIAccess::SRVMask);
		}
		GraphBuilder.SetTextureAccessFinal(Source, ERHIAccess::SRVMask);
		return true;
	});
}

/* //} */
