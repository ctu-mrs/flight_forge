#include "SegIdPass.h"

#include "MaterialDomain.h"
#include "Materials/Material.h"
#include "Materials/MaterialRenderProxy.h"
#include "InstanceCulling/InstanceCullingContext.h"
#include "MeshPassProcessor.h"
#include "MeshPassProcessor.inl"
#include "PostProcess/PostProcessInputs.h"
#include "SimpleMeshDrawCommandPass.h"
#include "PrimitiveSceneInfo.h"
#include "PrimitiveSceneProxy.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
#include "SceneInterface.h"
#include "SceneView.h"
#include "StaticMeshBatch.h"

/* shaders //{ */

class FSegIdShaderElementData : public FMeshMaterialShaderElementData
{
public:
	explicit FSegIdShaderElementData(const FLinearColor& InSegId) : SegId(InSegId)
	{
	}

	FLinearColor SegId;
};

class FSegIdVS : public FMeshMaterialShader
{
	DECLARE_SHADER_TYPE(FSegIdVS, MeshMaterial);

public:
	static bool ShouldCompilePermutation(const FMeshMaterialShaderPermutationParameters& Parameters)
	{
		// only the default-material substitution path is drawn, and Nanite has its own raster
		return IsPCPlatform(Parameters.Platform) && Parameters.MaterialParameters.bIsSpecialEngineMaterial &&
		       !Parameters.VertexFactoryType->SupportsNaniteRendering();
	}

protected:
	FSegIdVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer) : FMeshMaterialShader(Initializer)
	{
	}
	FSegIdVS() = default;
};

IMPLEMENT_MATERIAL_SHADER_TYPE(, FSegIdVS, TEXT("/FlightForgePlugin/SegIdVertexShader.usf"), TEXT("Main"), SF_Vertex);

class FSegIdPS : public FMeshMaterialShader
{
	DECLARE_SHADER_TYPE(FSegIdPS, MeshMaterial);

public:
	static bool ShouldCompilePermutation(const FMeshMaterialShaderPermutationParameters& Parameters)
	{
		return IsPCPlatform(Parameters.Platform) && Parameters.MaterialParameters.bIsSpecialEngineMaterial &&
		       !Parameters.VertexFactoryType->SupportsNaniteRendering();
	}

	FSegIdPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer) : FMeshMaterialShader(Initializer)
	{
		SegId.Bind(Initializer.ParameterMap, TEXT("SegId"), SPF_Optional);
	}
	FSegIdPS() = default;

	void GetShaderBindings(const FScene* Scene, ERHIFeatureLevel::Type FeatureLevel, const FPrimitiveSceneProxy* PrimitiveSceneProxy,
	                       const FMaterialRenderProxy& MaterialRenderProxy, const FMaterial& Material, const FSegIdShaderElementData& ShaderElementData,
	                       FMeshDrawSingleShaderBindings& ShaderBindings) const
	{
		FMeshMaterialShader::GetShaderBindings(Scene, FeatureLevel, PrimitiveSceneProxy, MaterialRenderProxy, Material, ShaderElementData, ShaderBindings);

		ShaderBindings.Add(SegId, ShaderElementData.SegId);
	}

private:
	LAYOUT_FIELD(FShaderParameter, SegId)
};

IMPLEMENT_MATERIAL_SHADER_TYPE(, FSegIdPS, TEXT("/FlightForgePlugin/SegIdPixelShader.usf"), TEXT("Main"), SF_Pixel);

/* //} */

/* FSegIdMeshProcessor //{ */

class FSegIdMeshProcessor : public FMeshPassProcessor
{
public:
	FSegIdMeshProcessor(const FScene* InScene, const FSceneView* InView, FMeshPassDrawListContext* InDrawListContext)
	    : FMeshPassProcessor(TEXT("FlightForgeSegId"), InScene, InView->GetFeatureLevel(), InView, InDrawListContext)
	{
		// read-only test against the scene depth: everything the base pass rendered
		// occludes the ids, including geometry this pass does not draw
		RenderState.SetDepthStencilState(TStaticDepthStencilState<false, CF_DepthNearOrEqual>::GetRHI());
		RenderState.SetBlendState(TStaticBlendState<>::GetRHI());
	}

	FLinearColor CurrentId = FLinearColor::Black;

	virtual void AddMeshBatch(const FMeshBatch& RESTRICT MeshBatch, uint64 BatchElementMask, const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy,
	                          int32 StaticMeshId = -1) override final
	{
		if (!MeshBatch.bUseForMaterial)
		{
			return;
		}

		// always substitute the default material - the ids do not depend on the
		// object's material, and the permutation space stays tiny
		const FMaterialRenderProxy* MaterialRenderProxy = UMaterial::GetDefaultMaterial(MD_Surface)->GetRenderProxy();
		const FMaterial*            Material            = MaterialRenderProxy ? MaterialRenderProxy->GetMaterialNoFallback(FeatureLevel) : nullptr;

		if (!Material || !Material->GetRenderingThreadShaderMap())
		{
			return;
		}

		Process(MeshBatch, BatchElementMask, StaticMeshId, PrimitiveSceneProxy, *MaterialRenderProxy, *Material);
	}

private:
	bool Process(const FMeshBatch& MeshBatch, const uint64 BatchElementMask, const int32 StaticMeshId,
	             const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy, const FMaterialRenderProxy& RESTRICT MaterialRenderProxy,
	             const FMaterial& RESTRICT MaterialResource)
	{
		FMaterialShaderTypes ShaderTypes;
		ShaderTypes.AddShaderType<FSegIdVS>();
		ShaderTypes.AddShaderType<FSegIdPS>();

		FMaterialShaders Shaders;
		if (!MaterialResource.TryGetShaders(ShaderTypes, MeshBatch.VertexFactory->GetType(), Shaders))
		{
			return false;
		}

		TMeshProcessorShaders<FSegIdVS, FSegIdPS> PassShaders;
		Shaders.TryGetVertexShader(PassShaders.VertexShader);
		Shaders.TryGetPixelShader(PassShaders.PixelShader);

		FSegIdShaderElementData ShaderElementData(CurrentId);
		ShaderElementData.InitializeMeshMaterialData(ViewIfDynamicMeshCommand, PrimitiveSceneProxy, MeshBatch, StaticMeshId, false);

		const FMeshDrawingPolicyOverrideSettings OverrideSettings = ComputeMeshOverrideSettings(MeshBatch);
		const ERasterizerFillMode                MeshFillMode     = ComputeMeshFillMode(MaterialResource, OverrideSettings);
		const ERasterizerCullMode                MeshCullMode     = ComputeMeshCullMode(MaterialResource, OverrideSettings);

		BuildMeshDrawCommands(MeshBatch, BatchElementMask, PrimitiveSceneProxy, MaterialRenderProxy, MaterialResource, RenderState, PassShaders, MeshFillMode,
		                      MeshCullMode, FMeshDrawCommandSortKey::Default, EMeshPassFeatures::Default, ShaderElementData);

		return true;
	}

	FMeshPassProcessorRenderState RenderState;
};

/* //} */

/* FSegIdViewExtension //{ */

FSegIdViewExtension::FSegIdViewExtension(const FAutoRegister& AutoRegister) : FSceneViewExtensionBase(AutoRegister)
{
}

TSharedPtr<FSegIdViewExtension, ESPMode::ThreadSafe> FSegIdViewExtension::Get()
{
	static TSharedPtr<FSegIdViewExtension, ESPMode::ThreadSafe> Extension;

	if (!Extension.IsValid())
	{
		Extension = FSceneViewExtensions::NewExtension<FSegIdViewExtension>();
	}

	return Extension;
}

bool FSegIdViewExtension::IsActiveThisFrame_Internal(const FSceneViewExtensionContext& Context) const
{
	return PendingJobs.load() > 0;
}

// Off by default: the pass draws, but the draw crashes in the Vulkan bindless descriptor path
// (UpdateBindlessDescriptors), so a client asking for instance segmentation would kill the sim.
static TAutoConsoleVariable<int32> CVarSegIdEnable(TEXT("FlightForge.SegId.Enable"), 0,
                                                   TEXT("Enable the experimental instance-segmentation mesh pass (unstable)."), ECVF_RenderThreadSafe);

void FSegIdViewExtension::SubmitJob_GameThread(const FRenderTarget* TargetFilter, const FIntPoint Size, TArray<FSegIdDrawItem>&& Items,
                                               const TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>& Slot)
{
	if (!TargetFilter || !Slot.IsValid() || CVarSegIdEnable.GetValueOnGameThread() == 0)
	{
		return;
	}

	PendingJobs++;

	FSegIdViewExtension* Extension = this;

	ENQUEUE_RENDER_COMMAND(FlightForgeSubmitSegIdJob)(
	    [Extension, TargetFilter, Size, MovedItems = MoveTemp(Items), Slot](FRHICommandListImmediate&) mutable
	    {
		    FSegIdJob& Job = Extension->Jobs.FindOrAdd(TargetFilter);
		    Job.Size       = Size;
		    Job.Items      = MoveTemp(MovedItems);
		    Job.Slot       = Slot;
	    });
}

// AddSimpleMeshPass fills InstanceCullingDrawParams; the mesh shaders read the InstanceCulling
// uniform buffer through it, and the RHI fatals if the pass omits it.
BEGIN_SHADER_PARAMETER_STRUCT(FSegIdPassParameters, )
SHADER_PARAMETER_STRUCT_INCLUDE(FInstanceCullingDrawParams, InstanceCullingDrawParams)
RENDER_TARGET_BINDING_SLOTS()
END_SHADER_PARAMETER_STRUCT()

void FSegIdViewExtension::PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& InView, const FPostProcessingInputs& Inputs)
{
	if (PendingJobs.load() <= 0 || !InView.Family || !InView.Family->RenderTarget)
	{
		return;
	}

	FSegIdJob Job;
	if (!Jobs.RemoveAndCopyValue(InView.Family->RenderTarget, Job))
	{
		return;
	}
	PendingJobs--;

	if (!Job.Slot.IsValid() || Job.Size.X <= 0 || Job.Size.Y <= 0)
	{
		return;
	}

	const FRDGTextureDesc Desc =
	    FRDGTextureDesc::Create2D(Job.Size, PF_B8G8R8A8, FClearValueBinding::Black, TexCreate_RenderTargetable | TexCreate_ShaderResource);
	FRDGTextureRef IdTexture = GraphBuilder.CreateTexture(Desc, TEXT("FlightForge.SegId"));

	FRDGTextureRef SceneDepth = nullptr;
	if (Inputs.SceneTextures)
	{
		SceneDepth = Inputs.SceneTextures->GetParameters()->SceneDepthTexture;
	}

	TArray<FSegIdDrawItem>* ItemsOnPass = GraphBuilder.AllocObject<TArray<FSegIdDrawItem>>(MoveTemp(Job.Items));

	auto* PassParameters             = GraphBuilder.AllocParameters<FSegIdPassParameters>();
	PassParameters->RenderTargets[0] = FRenderTargetBinding(IdTexture, ERenderTargetLoadAction::EClear);

	if (SceneDepth)
	{
		PassParameters->RenderTargets.DepthStencil =
		    FDepthStencilBinding(SceneDepth, ERenderTargetLoadAction::ELoad, ERenderTargetLoadAction::ELoad, FExclusiveDepthStencil::DepthRead_StencilRead);
	}

	const FIntPoint Size = Job.Size;

	const FScene* Scene = InView.Family->Scene ? InView.Family->Scene->GetRenderScene() : nullptr;

	AddSimpleMeshPass(GraphBuilder, PassParameters, Scene, InView, nullptr, RDG_EVENT_NAME("FlightForgeSegId"), FIntRect(0, 0, Size.X, Size.Y),
	                  [ItemsOnPass, Scene, &InView](FDynamicPassMeshDrawListContext* DrawListContext)
	                  {
		                  FSegIdMeshProcessor Processor(Scene, &InView, DrawListContext);

		                  for (const FSegIdDrawItem& Item : *ItemsOnPass)
		                  {
			                  if (!Item.Proxy)
			                  {
				                  continue;
			                  }

			                  const FPrimitiveSceneInfo* SceneInfo = Item.Proxy->GetPrimitiveSceneInfo();
			                  if (!SceneInfo)
			                  {
				                  continue;
			                  }

			                  Processor.CurrentId = Item.IdColor;

			                  for (const FStaticMeshBatch& Mesh : SceneInfo->StaticMeshes)
			                  {
				                  if (Mesh.LODIndex == 0)
				                  {
					                  Processor.AddMeshBatch(Mesh, ~0ull, Item.Proxy);
				                  }
			                  }
		                  }
	                  });

	AddEnqueueCopyPass(GraphBuilder, Job.Slot->Readback.Get(), IdTexture, FResolveRect());
}

/* //} */
