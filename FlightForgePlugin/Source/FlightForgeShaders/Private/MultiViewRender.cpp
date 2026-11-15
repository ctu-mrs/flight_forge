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
		if (Tiles.IsEmpty())
		{
			return true;
		}

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

TSharedRef<ISceneViewExtension, ESPMode::ThreadSafe> FMultiViewRender::OutputExtension()
{
	static const TSharedRef<ISceneViewExtension, ESPMode::ThreadSafe> Extension = MakeShared<FMultiViewOutputExtension, ESPMode::ThreadSafe>();
	return Extension;
}

/* //} */
