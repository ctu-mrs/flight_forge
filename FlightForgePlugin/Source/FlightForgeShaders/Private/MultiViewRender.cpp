#include "MultiViewRender.h"

#include "ReadbackPass.h"

#include "RenderGraphBuilder.h"
#include "PostProcess/PostProcessMaterialInputs.h"
#include "RenderGraphUtils.h"
#include "SceneRenderBuilderInterface.h"
#include "SceneRendering.h"
#include "SceneViewExtension.h"
#include "ScreenPass.h"
#include "TextureResource.h"

/* FMultiViewOutputExtension //{ */

namespace
{

class FMultiViewOutputExtension : public ISceneViewExtension
{
public:
	virtual void SubscribeToPostProcessingPass(EPostProcessingPass Pass, const FSceneView& InView, FPostProcessingPassDelegateArray& InOutPassCallbacks,
	                                           bool bIsPassEnabled) override
	{
		if (Pass == EPostProcessingPass::Tonemap)
		{
			InOutPassCallbacks.Add(FAfterPassCallbackDelegate::CreateStatic(&FMultiViewOutputExtension::Output));
		}
	}

private:
	static FScreenPassTexture Output(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessMaterialInputs& Inputs)
	{
		const FScreenPassTextureSlice& Color = Inputs.GetInput(EPostProcessMaterialInput::SceneColor);
		if (!Inputs.OverrideOutput.IsValid())
		{
			return FScreenPassTexture::CopyFromSlice(GraphBuilder, Color);
		}
		AddDrawTexturePass(GraphBuilder, FScreenPassViewInfo(View), Color, Inputs.OverrideOutput);
		return Inputs.OverrideOutput;
	}
};

}  // namespace

/* //} */

/* FMultiViewRender //{ */

void FMultiViewRender::Add(ISceneRenderBuilder& Builder, FSceneViewFamily& Family, FString&& Name, FTextureRenderTargetResource* Atlas, TArray<FMultiViewTile>&& Tiles)
{
	FSceneRenderer* Renderer = Builder.CreateSceneRenderer(&Family);
	const uint32    Frame    = Family.FrameNumber;
	const double    Time     = Family.Time.GetWorldTimeSeconds();

	SCENE_RENDER_GROUP_SCOPE(Builder, MoveTemp(Name), ESceneRenderGroupFlags::None);

	Builder.AddRenderer(Renderer, [Atlas, Tiles = MoveTemp(Tiles), Frame, Time](FRDGBuilder& GraphBuilder, const FSceneRenderFunctionInputs& Inputs) {
		Inputs.Renderer->Render(GraphBuilder, Inputs.SceneUpdateInputs);
		FRHITexture* AtlasTexture = Atlas->GetRenderTargetTexture();
		if (!AtlasTexture)
		{
			return true;
		}

		// the renderer leaves the view family texture in RTV
		FRDGTextureRef Source = RegisterExternalTexture(GraphBuilder, AtlasTexture, TEXT("FlightForge.MultiViewAtlas"));
		GraphBuilder.SetTextureAccessFinal(Source, ERHIAccess::SRVMask);
		for (const FMultiViewTile& Tile : Tiles)
		{
			FRHITexture* TargetTexture = Tile.Target->GetRenderTargetTexture();
			if (!TargetTexture)
			{
				continue;
			}
			FRDGTextureRef Dest = RegisterExternalTexture(GraphBuilder, TargetTexture, TEXT("FlightForge.MultiViewTile"));

			// a target resized after the tiles were laid out is smaller than its rect; an oversized copy hangs the GPU
			const FIntPoint Size = Tile.Rect.Size().ComponentMin(Dest->Desc.Extent).ComponentMin(Source->Desc.Extent - Tile.Rect.Min);
			if (Size != Tile.Rect.Size())
			{
				UE_LOG(LogTemp, Warning, TEXT("FlightForge: multi-view tile %dx%d clamped to %dx%d (target %dx%d)"), Tile.Rect.Width(), Tile.Rect.Height(), Size.X,
				    Size.Y, Dest->Desc.Extent.X, Dest->Desc.Extent.Y);
				if (Size.X <= 0 || Size.Y <= 0)
				{
					continue;
				}
			}

			FRHICopyTextureInfo Copy;
			Copy.SourcePosition = FIntVector(Tile.Rect.Min.X, Tile.Rect.Min.Y, 0);
			Copy.Size           = FIntVector(Size.X, Size.Y, 1);
			AddCopyTexturePass(GraphBuilder, Source, Dest, Copy);

			FReadbackViewExtension::Get()->CopyJob_RenderThread(GraphBuilder, Tile.Target, Dest, Frame, Time);
			GraphBuilder.SetTextureAccessFinal(Dest, ERHIAccess::SRVMask);
		}
		return true;
	});
}

TSharedRef<ISceneViewExtension, ESPMode::ThreadSafe> FMultiViewRender::OutputExtension()
{
	static const TSharedRef<ISceneViewExtension, ESPMode::ThreadSafe> Extension = MakeShared<FMultiViewOutputExtension, ESPMode::ThreadSafe>();
	return Extension;
}

/* //} */
