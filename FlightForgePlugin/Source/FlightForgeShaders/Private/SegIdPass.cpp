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
#include "SceneRendering.h"
#include "ScenePrivate.h"
#include "SystemTextures.h"
#include "Nanite/NaniteCullRaster.h"
#include "Nanite/NaniteShared.h"
#include "Rendering/NaniteStreamingManager.h"
#include "SceneRendererInterface.h"
#include "SceneManagement.h"
#include "SceneView.h"
#include "HAL/IConsoleManager.h"
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
		// the default material stands in for everything opaque and rigid; materials that move vertices (wind) or cut holes
		// (masked leaves) must run their own code or the pass's depth no longer matches the scene's
		const FMaterialShaderParameters& M = Parameters.MaterialParameters;
		return IsPCPlatform(Parameters.Platform) && (M.bIsSpecialEngineMaterial || M.bMaterialMayModifyMeshPosition || !M.bWritesEveryPixel) &&
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
		const FMaterialShaderParameters& M = Parameters.MaterialParameters;
		return IsPCPlatform(Parameters.Platform) && (M.bIsSpecialEngineMaterial || M.bMaterialMayModifyMeshPosition || !M.bWritesEveryPixel) &&
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
	int32        NumOwn = 0, NumDefault = 0, NumTranslucent = 0, NumCompiling = 0;

	virtual void AddMeshBatch(const FMeshBatch& RESTRICT MeshBatch, uint64 BatchElementMask, const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy,
	                          int32 StaticMeshId = -1) override final
	{
		if (!MeshBatch.bUseForMaterial)
		{
			return;
		}

		const FMaterialRenderProxy* DefaultProxy    = UMaterial::GetDefaultMaterial(MD_Surface)->GetRenderProxy();
		const FMaterial*            DefaultMaterial = DefaultProxy ? DefaultProxy->GetMaterialNoFallback(FeatureLevel) : nullptr;
		const FMaterialRenderProxy* OwnProxy        = MeshBatch.MaterialRenderProxy;
		const FMaterial*            OwnMaterial     = OwnProxy ? OwnProxy->GetMaterialNoFallback(FeatureLevel) : nullptr;  // null while its shaders compile
		if (OwnMaterial && IsTranslucentBlendMode(*OwnMaterial))
		{
			++NumTranslucent;
			return;  // glass, particles: not a surface the id image should claim
		}
		if (!OwnMaterial)
		{
			++NumCompiling;
		}

		const bool bNeedsOwn = OwnMaterial && (OwnMaterial->MaterialModifiesMeshPosition_RenderThread() || !OwnMaterial->WritesEveryPixel());
		if (bNeedsOwn) { ++NumOwn; } else { ++NumDefault; }

		const FMaterialRenderProxy* MaterialRenderProxy = bNeedsOwn ? OwnProxy : DefaultProxy;
		const FMaterial*            Material            = bNeedsOwn ? OwnMaterial : DefaultMaterial;

		if (!Material || !Material->GetRenderingThreadShaderMap())
		{
			return;
		}

		Process(MeshBatch, BatchElementMask, StaticMeshId, PrimitiveSceneProxy, *MaterialRenderProxy, *Material, OwnMaterial ? *OwnMaterial : *Material);
	}

private:
	bool Process(const FMeshBatch& MeshBatch, const uint64 BatchElementMask, const int32 StaticMeshId,
	             const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy, const FMaterialRenderProxy& RESTRICT MaterialRenderProxy,
	             const FMaterial& RESTRICT MaterialResource, const FMaterial& RESTRICT OwnMaterial)
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
		const ERasterizerCullMode                MeshCullMode     = ComputeMeshCullMode(OwnMaterial, OverrideSettings);  // two-sided cards stay two-sided

		BuildMeshDrawCommands(MeshBatch, BatchElementMask, PrimitiveSceneProxy, MaterialRenderProxy, MaterialResource, RenderState, PassShaders, MeshFillMode,
		                      MeshCullMode, FMeshDrawCommandSortKey::Default, EMeshPassFeatures::Default, ShaderElementData);

		return true;
	}

	FMeshPassProcessorRenderState RenderState;
};

/* //} */

/* Nanite ids //{ */

// Needs the Renderer exports of tools/apply_engine_patch.py; a stock engine falls back to stencil class ids.
#if FLIGHTFORGE_NANITE_IDS
class FSegIdNaniteCS : public FNaniteGlobalShader
{
	DECLARE_GLOBAL_SHADER(FSegIdNaniteCS);
	SHADER_USE_PARAMETER_STRUCT(FSegIdNaniteCS, FNaniteGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_STRUCT_INCLUDE(FViewShaderParameters, View)
		SHADER_PARAMETER_RDG_UNIFORM_BUFFER(FSceneUniformParameters, Scene)
		SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<FPackedView>, InViews)
		SHADER_PARAMETER_RDG_BUFFER_SRV(ByteAddressBuffer, VisibleClustersSWHW)
		SHADER_PARAMETER(FIntVector4, PageConstants)
		SHADER_PARAMETER_RDG_BUFFER_SRV(ByteAddressBuffer, ClusterPageData)
		SHADER_PARAMETER_RDG_BUFFER_SRV(ByteAddressBuffer, HierarchyBuffer)
		SHADER_PARAMETER_RDG_BUFFER_SRV(ByteAddressBuffer, AssemblyTransforms)
		SHADER_PARAMETER_RDG_TEXTURE(Texture2D<UlongType>, VisBuffer64)
		SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float>, SceneDepth)
		SHADER_PARAMETER_RDG_BUFFER_SRV(Buffer<float4>, IdByPrimitive)
		SHADER_PARAMETER(uint32, NumPrimitiveIds)
		SHADER_PARAMETER(FUintVector4, IdRect)
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, OutId)
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return DoesPlatformSupportNanite(Parameters.Platform);
	}
};

IMPLEMENT_GLOBAL_SHADER(FSegIdNaniteCS, "/FlightForgePlugin/SegIdNanite.usf", "Main", SF_Compute);

static void AddNaniteIds(FRDGBuilder& GraphBuilder, const FScene* Scene, const FSceneView& InView, const FPostProcessingInputs& Inputs,
                         const TArray<FSegIdDrawItem>& Items, FRDGTextureRef IdTexture)
{
	if (!Scene || !UseNanite(Scene->GetShaderPlatform()) || !Inputs.SceneTextures)
	{
		return;
	}

	// by persistent index, as InstanceData.PrimitiveId is; zero means "not one of ours"
	TArray<FVector4f> Lookup;
	Lookup.SetNumZeroed(Scene->GetMaxPersistentPrimitiveIndex());
	bool bAny = false;
	for (const FSegIdDrawItem& Item : Items)
	{
		const FPrimitiveSceneInfo* SceneInfo = Scene->GetPrimitiveSceneInfo(Item.ComponentId);
		const int32                Index     = SceneInfo ? SceneInfo->GetPersistentIndex().Index : INDEX_NONE;
		if (SceneInfo && SceneInfo->Proxy && SceneInfo->Proxy->IsNaniteMesh() && Lookup.IsValidIndex(Index))
		{
			Lookup[Index] = FVector4f(Item.IdColor.R, Item.IdColor.G, Item.IdColor.B, Item.IdColor.A);
			bAny          = true;
		}
	}
	if (!bAny)
	{
		return;
	}

	FViewInfo&             View       = const_cast<FViewInfo&>(static_cast<const FViewInfo&>(InView));
	const FViewFamilyInfo& ViewFamily = static_cast<const FViewFamilyInfo&>(*InView.Family);
	const FIntPoint        Size       = IdTexture->Desc.Extent;

	RDG_EVENT_SCOPE(GraphBuilder, "FlightForgeSegIdNanite");

	Nanite::FSharedContext SharedContext{};
	SharedContext.FeatureLevel = Scene->GetFeatureLevel();
	SharedContext.ShaderMap    = GetGlobalShaderMap(SharedContext.FeatureLevel);
	SharedContext.Pipeline     = Nanite::EPipeline::Primary;

	Nanite::FRasterContext RasterContext = Nanite::InitRasterContext(GraphBuilder, SharedContext, ViewFamily, Size, FIntRect(0, 0, Size.X, Size.Y),
	                                                                 Nanite::EOutputBufferMode::VisBuffer, true, false);

	Nanite::FConfiguration CullingConfig = {0};
	CullingConfig.SetViewFlags(View);
	CullingConfig.bTwoPassOcclusion = false;
	CullingConfig.bUpdateStreaming  = false;

	Nanite::FPackedViewArray::ArrayType PackedViews;
	PackedViews.Add(Nanite::CreatePackedViewFromViewInfo(View, Size, NANITE_VIEW_FLAG_NEAR_CLIP));
	Nanite::FPackedViewArray* ViewArray = Nanite::FPackedViewArray::Create(GraphBuilder, 1, MoveTemp(PackedViews));

	TUniquePtr<Nanite::IRenderer> Renderer =
	    Nanite::IRenderer::Create(GraphBuilder, *Scene, View, View.GetSceneUniforms(), SharedContext, RasterContext, CullingConfig, View.ViewRect, nullptr);
	Renderer->DrawGeometry(const_cast<FScene*>(Scene)->NaniteRasterPipelines[ENaniteMeshPass::BasePass], nullptr, *ViewArray);

	Nanite::FRasterResults RasterResults;
	Renderer->ExtractResults(RasterResults);
	if (!RasterResults.VisBuffer64 || !RasterResults.VisibleClustersSWHW)
	{
		return;
	}

	FRDGBufferRef LookupBuffer = CreateVertexBuffer(GraphBuilder, TEXT("FlightForge.SegIdByPrimitive"), FRDGBufferDesc::CreateBufferDesc(sizeof(FVector4f), Lookup.Num()),
	                                                Lookup.GetData(), Lookup.Num() * sizeof(FVector4f));

	FSegIdNaniteCS::FParameters* Parameters = GraphBuilder.AllocParameters<FSegIdNaniteCS::FParameters>();
	Parameters->View                = View.GetShaderParameters();
	Parameters->Scene               = View.GetSceneUniforms().GetBuffer(GraphBuilder);
	Parameters->InViews             = GraphBuilder.CreateSRV(RasterResults.ViewsBuffer);
	Parameters->VisibleClustersSWHW = GraphBuilder.CreateSRV(RasterResults.VisibleClustersSWHW);
	Parameters->PageConstants       = RasterResults.PageConstants;
	Parameters->ClusterPageData     = Nanite::GStreamingManager.GetClusterPageDataSRV(GraphBuilder);
	Parameters->HierarchyBuffer     = Nanite::GStreamingManager.GetHierarchySRV(GraphBuilder);
	Parameters->AssemblyTransforms  = GraphBuilder.CreateSRV(RasterResults.AssemblyTransforms ? RasterResults.AssemblyTransforms
	                                                                                           : GSystemTextures.GetDefaultByteAddressBuffer(GraphBuilder, 48u));
	Parameters->VisBuffer64         = RasterResults.VisBuffer64;
	Parameters->SceneDepth          = (*Inputs.SceneTextures)->SceneDepthTexture;
	Parameters->IdByPrimitive       = GraphBuilder.CreateSRV(LookupBuffer, PF_A32B32G32R32F);
	Parameters->NumPrimitiveIds     = Lookup.Num();
	Parameters->IdRect              = FUintVector4(View.ViewRect.Min.X, View.ViewRect.Min.Y, Size.X, Size.Y);
	Parameters->OutId               = GraphBuilder.CreateUAV(IdTexture);

	TShaderMapRef<FSegIdNaniteCS> Shader(SharedContext.ShaderMap);
	FComputeShaderUtils::AddPass(GraphBuilder, RDG_EVENT_NAME("FlightForgeSegIdNaniteDecode"), Shader, Parameters, FComputeShaderUtils::GetGroupCount(Size, 8));
}
#endif

class FSegIdStencilFillCS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FSegIdStencilFillCS);
	SHADER_USE_PARAMETER_STRUCT(FSegIdStencilFillCS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2D<uint2>, CustomStencil)
		SHADER_PARAMETER_RDG_BUFFER_SRV(Buffer<float4>, IdBySlot)
		SHADER_PARAMETER(FUintVector4, IdRect)
		SHADER_PARAMETER(uint32, bOverwrite)
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, OutId)
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsPCPlatform(Parameters.Platform);
	}
};

IMPLEMENT_GLOBAL_SHADER(FSegIdStencilFillCS, "/FlightForgePlugin/SegIdStencilFill.usf", "Main", SF_Compute);

static TAutoConsoleVariable<int32> CVarSegIdStencilFill(TEXT("FlightForge.SegId.StencilFill"), 1,
                                                        TEXT("resolve Nanite ids from the custom stencil slots; 0 leaves Nanite pixels to the decode or unlabelled, 2 paints every raw stencil value over everything"));

static void AddStencilClassFill(FRDGBuilder& GraphBuilder, const FSceneView& InView, const FPostProcessingInputs& Inputs,
                                const TArray<FLinearColor>& StencilSlots, FRDGTextureRef IdTexture)
{
	const int32 Mode = CVarSegIdStencilFill.GetValueOnRenderThread();
	if (Mode == 0 || !Inputs.SceneTextures || !(*Inputs.SceneTextures)->CustomStencilTexture || StencilSlots.Num() != 256)
	{
		return;
	}
	const FIntPoint Size = IdTexture->Desc.Extent;

	TArray<FLinearColor> RawRamp;
	if (Mode == 2)
	{
		RawRamp.SetNumUninitialized(256);
		for (int32 Value = 0; Value < 256; ++Value)
		{
			RawRamp[Value] = FLinearColor(Value / 255.0f, 1.0f, 0.0f, 0.0f);
		}
	}
	const TArray<FLinearColor>& Table = Mode == 2 ? RawRamp : StencilSlots;

	FRDGBufferRef SlotBuffer = CreateVertexBuffer(GraphBuilder, TEXT("FlightForge.SegIdBySlot"), FRDGBufferDesc::CreateBufferDesc(sizeof(FLinearColor), 256),
	                                              Table.GetData(), 256 * sizeof(FLinearColor));

	FSegIdStencilFillCS::FParameters* Parameters = GraphBuilder.AllocParameters<FSegIdStencilFillCS::FParameters>();
	Parameters->CustomStencil = (*Inputs.SceneTextures)->CustomStencilTexture;
	Parameters->IdBySlot      = GraphBuilder.CreateSRV(SlotBuffer, PF_A32B32G32R32F);
	Parameters->IdRect        = FUintVector4(InView.UnscaledViewRect.Min.X, InView.UnscaledViewRect.Min.Y, Size.X, Size.Y);
	Parameters->bOverwrite    = Mode == 2 ? 1 : 0;
	Parameters->OutId         = GraphBuilder.CreateUAV(IdTexture);

	TShaderMapRef<FSegIdStencilFillCS> Shader(GetGlobalShaderMap(InView.GetFeatureLevel()));
	FComputeShaderUtils::AddPass(GraphBuilder, RDG_EVENT_NAME("FlightForgeSegIdStencilFill"), Shader, Parameters, FComputeShaderUtils::GetGroupCount(Size, 8));
}

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
		// a deferred capture renders the frame after the submit command ran; older than that the capture never came
		if (GFrameNumberRenderThread - It->Value.FrameNumber > 2)
		{
			UE_LOG(LogTemp, Verbose, TEXT("SegId: dropping a job left from frame %u"), It->Value.FrameNumber);
			It.RemoveCurrent();
			PendingJobs--;
		}
	}
}

void FSegIdViewExtension::SubmitJob_GameThread(const FRenderTarget* TargetFilter, const FIntPoint Size, TArray<FSegIdDrawItem>&& Items,
                                               TArray<FLinearColor>&& StencilSlots, const TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>& Slot)
{
	if (!TargetFilter || !Slot.IsValid() || CVarSegIdEnable.GetValueOnGameThread() == 0)
	{
		return;
	}

	PendingJobs++;

	FSegIdViewExtension* Extension = this;

	ENQUEUE_RENDER_COMMAND(FlightForgeSubmitSegIdJob)(
	    [Extension, TargetFilter, Size, MovedItems = MoveTemp(Items), MovedSlots = MoveTemp(StencilSlots), Slot](FRHICommandListImmediate&) mutable
	    {
		    Extension->DropStaleJobs_RenderThread();

		    FSegIdJob& Job   = Extension->Jobs.FindOrAdd(TargetFilter);
		    Job.Size         = Size;
		    Job.Items        = MoveTemp(MovedItems);
		    Job.StencilSlots = MoveTemp(MovedSlots);
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
	    FRDGTextureDesc::Create2D(Job.Size, PF_B8G8R8A8, FClearValueBinding::Black, TexCreate_RenderTargetable | TexCreate_ShaderResource | TexCreate_UAV);
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

	// the same scale the scene's visibility pass used when it picked each primitive's LOD
	static const auto* CVarLODDistanceScale = IConsoleManager::Get().FindConsoleVariable(TEXT("r.StaticMeshLODDistanceScale"));
	const float        LODScale             = (CVarLODDistanceScale ? CVarLODDistanceScale->GetFloat() : 1.0f) * InView.LODDistanceFactor;

	AddSimpleMeshPass(GraphBuilder, PassParameters, Scene, InView, nullptr, RDG_EVENT_NAME("FlightForgeSegId"), FIntRect(0, 0, Size.X, Size.Y),
	                  [ItemsOnPass, Scene, SceneInterface, &InView, LODScale](FDynamicPassMeshDrawListContext* DrawListContext)
	                  {
		                  FSegIdMeshProcessor Processor(Scene, &InView, DrawListContext);

		                  // instanced and skeletal meshes have no static batches: their per-view ones (instance runs, LOD) are what the
		                  // scene renderer already collected for this capture
		                  TMap<FPrimitiveComponentId, FLinearColor> IdByComponent;
		                  for (const FSegIdDrawItem& Item : *ItemsOnPass)
		                  {
			                  IdByComponent.Add(Item.ComponentId, Item.IdColor);
		                  }
		                  const FViewInfo& ViewInfo = static_cast<const FViewInfo&>(InView);
		                  int32 DynamicDrawn = 0, DynamicSkipped = 0;
		                  for (const FMeshBatchAndRelevance& Dynamic : ViewInfo.DynamicMeshElements)
		                  {
			                  if (!Dynamic.Mesh || !Dynamic.PrimitiveSceneProxy)
			                  {
				                  continue;
			                  }
			                  if (const FLinearColor* Id = IdByComponent.Find(Dynamic.PrimitiveSceneProxy->GetPrimitiveComponentId()))
			                  {
				                  Processor.CurrentId = *Id;
				                  Processor.AddMeshBatch(*Dynamic.Mesh, ~0ull, Dynamic.PrimitiveSceneProxy);
				                  ++DynamicDrawn;
			                  }
			                  else
			                  {
				                  ++DynamicSkipped;
			                  }
		                  }
		                  UE_LOG(LogTemp, Verbose, TEXT("SegIdPass: %d items, %d dynamic elements (%d ours, %d not), own-material %d, default %d, translucent %d, compiling %d"),
		                         ItemsOnPass->Num(), ViewInfo.DynamicMeshElements.Num(), DynamicDrawn, DynamicSkipped, Processor.NumOwn, Processor.NumDefault,
		                         Processor.NumTranslucent, Processor.NumCompiling);

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

			                  // the LOD the scene depth was rendered with: the proxy's own choice, else the engine's
			                  // screen-size selection over its batches (landscape, instanced meshes); drawing LOD0
			                  // against a different LOD's depth loses the test wherever the two surfaces differ
			                  FLODMask    LODMask;
			                  const int32 ProxyLOD = SceneInfo->Proxy->GetLOD(&InView);
			                  if (ProxyLOD != INDEX_NONE)
			                  {
				                  LODMask.SetLOD(ProxyLOD);
			                  }
			                  else
			                  {
				                  const FBoxSphereBounds& Bounds = SceneInfo->Proxy->GetBounds();
				                  float                   ScreenRadiusSquared = 0.0f;
				                  LODMask = ComputeLODForMeshes(SceneInfo->StaticMeshRelevances, InView, Bounds.Origin, Bounds.SphereRadius, -1, ScreenRadiusSquared,
				                                                SceneInfo->Proxy->GetCurrentFirstLODIdx_RenderThread(), LODScale, false);
			                  }
			                  bool bAny = false;
			                  for (const FStaticMeshBatch& Mesh : SceneInfo->StaticMeshes)
			                  {
				                  if (LODMask.ContainsLOD(Mesh.LODIndex))
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

	AddStencilClassFill(GraphBuilder, InView, Inputs, Job.StencilSlots, IdTexture);
#if FLIGHTFORGE_NANITE_IDS
	AddNaniteIds(GraphBuilder, Scene, InView, Inputs, *ItemsOnPass, IdTexture);
#endif

	AddEnqueueCopyPass(GraphBuilder, Job.Slot->Readback.Get(), IdTexture, FResolveRect());

	TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Slot = Job.Slot;
	GraphBuilder.AddPass(RDG_EVENT_NAME("FlightForgeSegIdWatch"), ERDGPassFlags::NeverCull, [Slot](FRHICommandList&) { WatchReadback_RenderThread(Slot); });
}

/* //} */
