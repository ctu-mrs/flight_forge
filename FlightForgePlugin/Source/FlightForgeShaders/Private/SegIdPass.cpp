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
#include "SceneRendererInterface.h"
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

// Kept as an off-switch: this pass renders through engine internals, so a hardware or engine
// regression here should be silenceable without a rebuild.
static TAutoConsoleVariable<int32> CVarSegIdEnable(TEXT("FlightForge.SegId.Enable"), 1,
                                                   TEXT("Enable the instance-segmentation mesh pass."), ECVF_RenderThreadSafe);

// A job holds raw scene proxies, which the render thread may free once the frame that
// collected them is over. A capture that never renders (component asleep, sensor removed)
// would otherwise leave one parked in the map until some later view happens to match it.
void FSegIdViewExtension::DropStaleJobs_RenderThread()
{
	for (auto It = Jobs.CreateIterator(); It; ++It)
	{
		if (It->Value.FrameNumber != GFrameNumberRenderThread)
		{
			UE_LOG(LogTemp, Verbose, TEXT("SegId: dropping a job left from frame %u"), It->Value.FrameNumber);
			It.RemoveCurrent();
			PendingJobs--;
		}
	}
}

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
		    Extension->DropStaleJobs_RenderThread();

		    FSegIdJob& Job   = Extension->Jobs.FindOrAdd(TargetFilter);
		    Job.Size         = Size;
		    Job.Items        = MoveTemp(MovedItems);
		    Job.Slot         = Slot;
		    Job.FrameNumber  = GFrameNumberRenderThread;
	    });
}

// Mesh material shaders read View and Scene (GPU Scene primitive data) as well as the
// InstanceCulling buffer AddSimpleMeshPass fills; leaving any of them off the struct means
// nothing binds them, and the bindless descriptor update then walks unbound state.
BEGIN_SHADER_PARAMETER_STRUCT(FSegIdPassParameters, )
SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)
SHADER_PARAMETER_RDG_UNIFORM_BUFFER(FSceneUniformParameters, Scene)
SHADER_PARAMETER_STRUCT_INCLUDE(FInstanceCullingDrawParams, InstanceCullingDrawParams)
RENDER_TARGET_BINDING_SLOTS()
END_SHADER_PARAMETER_STRUCT()

void FSegIdViewExtension::PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& InView, const FPostProcessingInputs& Inputs)
{
	if (PendingJobs.load() <= 0 || !InView.Family || !InView.Family->RenderTarget)
	{
		return;
	}

	DropStaleJobs_RenderThread();

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
	PassParameters->View             = InView.ViewUniformBuffer;
	PassParameters->Scene            = GetSceneUniformBufferRef(GraphBuilder, InView);
	PassParameters->RenderTargets[0] = FRenderTargetBinding(IdTexture, ERenderTargetLoadAction::EClear);

	if (SceneDepth)
	{
		PassParameters->RenderTargets.DepthStencil =
		    FDepthStencilBinding(SceneDepth, ERenderTargetLoadAction::ELoad, ERenderTargetLoadAction::ELoad, FExclusiveDepthStencil::DepthRead_StencilRead);
	}

	const FIntPoint Size = Job.Size;

	FSceneInterface* SceneInterface = InView.Family->Scene;
	const FScene*    Scene          = SceneInterface ? SceneInterface->GetRenderScene() : nullptr;

	AddSimpleMeshPass(GraphBuilder, PassParameters, Scene, InView, nullptr, RDG_EVENT_NAME("FlightForgeSegId"), FIntRect(0, 0, Size.X, Size.Y),
	                  [ItemsOnPass, Scene, SceneInterface, &InView](FDynamicPassMeshDrawListContext* DrawListContext)
	                  {
		                  FSegIdMeshProcessor Processor(Scene, &InView, DrawListContext);

		                  for (const FSegIdDrawItem& Item : *ItemsOnPass)
		                  {
			                  // resolved here, on the render thread, against the scene as it
			                  // stands now: a primitive destroyed since collection is absent
			                  // rather than a pointer into freed memory
			                  const FPrimitiveSceneInfo* SceneInfo = SceneInterface ? SceneInterface->GetPrimitiveSceneInfo(Item.ComponentId) : nullptr;
			                  if (!SceneInfo || !SceneInfo->Proxy)
			                  {
				                  continue;
			                  }

			                  Processor.CurrentId = Item.IdColor;

			                  // the LOD the proxy picks for this view is the one the scene depth was rendered with;
			                  // drawing LOD0 against it loses the depth test wherever the two surfaces differ
			                  int32 LOD = SceneInfo->Proxy->GetLOD(&InView);
			                  if (LOD == INDEX_NONE)
			                  {
				                  LOD = 0;
			                  }
			                  bool bAny = false;
			                  for (const FStaticMeshBatch& Mesh : SceneInfo->StaticMeshes)
			                  {
				                  if (Mesh.LODIndex == LOD)
				                  {
					                  Processor.AddMeshBatch(Mesh, ~0ull, SceneInfo->Proxy);
					                  bAny = true;
				                  }
			                  }
			                  if (!bAny)
			                  {
				                  for (const FStaticMeshBatch& Mesh : SceneInfo->StaticMeshes)
				                  {
					                  if (Mesh.LODIndex == 0)
					                  {
						                  Processor.AddMeshBatch(Mesh, ~0ull, SceneInfo->Proxy);
					                  }
				                  }
			                  }
		                  }
	                  });

	AddEnqueueCopyPass(GraphBuilder, Job.Slot->Readback.Get(), IdTexture, FResolveRect());
}

/* //} */
