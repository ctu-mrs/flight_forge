#include "LidarRayTrace.h"

#include "DataDrivenShaderPlatformInfo.h"
#include "Misc/CoreDelegates.h"
#include "GlobalShader.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
#include "RenderUtils.h"
#include "SceneInterface.h"
#include "SceneView.h"
#include "ShaderParameterStruct.h"

#if RHI_RAYTRACING
#include "DeferredShadingRenderer.h"
#include "PrimitiveSceneInfo.h"
#include "RayTracing/RayTracingScene.h"
#include "SceneRendering.h"
#include "ScenePrivate.h"
#endif

/* shader //{ */

#if RHI_RAYTRACING
class FLidarRayTraceCS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FLidarRayTraceCS);
	SHADER_USE_PARAMETER_STRUCT(FLidarRayTraceCS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)
		SHADER_PARAMETER_RDG_UNIFORM_BUFFER(FSceneUniformParameters, Scene)
		SHADER_PARAMETER_RDG_BUFFER_SRV(RaytracingAccelerationStructure, TLAS)
		SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<float4>, Rays)
		SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<uint2>, PrimitiveInfo)
		SHADER_PARAMETER(uint32, NumPrimitives)
		SHADER_PARAMETER(uint32, NumRays)
		SHADER_PARAMETER(uint32, OutWidth)
		SHADER_PARAMETER(FVector3f, StartOrigin)
		SHADER_PARAMETER(FVector3f, EndOrigin)
		SHADER_PARAMETER(FVector4f, StartRotation)
		SHADER_PARAMETER(FVector4f, EndRotation)
		SHADER_PARAMETER(float, MinRange)
		SHADER_PARAMETER(float, MaxRange)
		SHADER_PARAMETER(uint32, InstanceMask)
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, Out)
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsRayTracingEnabledForProject(Parameters.Platform) && RHISupportsRayTracing(Parameters.Platform) && RHISupportsInlineRayTracing(Parameters.Platform);
	}

	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
		// inline ray queries need one wave per group, and wave32
		OutEnvironment.CompilerFlags.Add(CFLAG_Wave32);
		OutEnvironment.CompilerFlags.Add(CFLAG_InlineRayTracing);
		OutEnvironment.SetDefine(TEXT("VF_SUPPORTS_PRIMITIVE_SCENE_DATA"), 1);
	}
};

IMPLEMENT_GLOBAL_SHADER(FLidarRayTraceCS, "/FlightForgePlugin/LidarRayTrace.usf", "Main", SF_Compute);

// the stock engine exports neither FViewInfo::GetPrimaryView, which the handle's accessor calls, nor a way around it
using FRayTracingViewHandleMember = FRayTracingScene::FViewHandle FViewInfo::*;
FRayTracingViewHandleMember RayTracingViewHandleMember();
template <FRayTracingViewHandleMember Member>
struct TRayTracingViewHandleAccess
{
	friend FRayTracingViewHandleMember RayTracingViewHandleMember() { return Member; }
};
template struct TRayTracingViewHandleAccess<&FViewInfo::RayTracingSceneViewHandle>;
#endif

/* //} */

/* FLidarRayTraceViewExtension //{ */

static TAutoConsoleVariable<int32> CVarLidarInstanceMask(TEXT("FlightForge.Lidar.InstanceMask"), 1,
                                                         TEXT("ray-tracing instance mask the GPU lidar traces against; 1 opaque and alpha-tested, 255 everything"), ECVF_RenderThreadSafe);
static TAutoConsoleVariable<int32> CVarLidarGpuProfile(TEXT("FlightForge.Lidar.GpuProfile"), 0,
                                                       TEXT("log the GPU time of each GPU lidar scan: the lidar capture's whole render and the trace alone"), ECVF_RenderThreadSafe);

FLidarRayTraceViewExtension::FLidarRayTraceViewExtension(const FAutoRegister& AutoRegister) : FSceneViewExtensionBase(AutoRegister)
{
}

TSharedPtr<FLidarRayTraceViewExtension, ESPMode::ThreadSafe> FLidarRayTraceViewExtension::Get()
{
	static TSharedPtr<FLidarRayTraceViewExtension, ESPMode::ThreadSafe> Extension;
	if (!Extension.IsValid())
	{
		Extension = FSceneViewExtensions::NewExtension<FLidarRayTraceViewExtension>();
#if RHI_RAYTRACING
		// only views whose GI method is the plugin ask, and only the lidar's capture sets that
		FGlobalIlluminationPluginDelegates::AnyRayTracingPassEnabled().AddLambda([](bool& bAnyRayTracingPassEnabled) { bAnyRayTracingPassEnabled = true; });
#endif
	}
	return Extension;
}

bool FLidarRayTraceViewExtension::IsRayTracingAvailable()
{
#if RHI_RAYTRACING
	return GIsRHIInitialized && IsRayTracingEnabled() && GRHISupportsInlineRayTracing && GRHISupportsRayTracingShaders &&
	       RHISupportsInlineRayTracing(GMaxRHIShaderPlatform) && IsRayTracingEnabledForProject(GMaxRHIShaderPlatform);
#else
	return false;
#endif
}

bool FLidarRayTraceViewExtension::IsActiveThisFrame_Internal(const FSceneViewExtensionContext& Context) const
{
	return PendingJobs.load() > 0;
}

void FLidarRayTraceViewExtension::Complete(const TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>& Slot)
{
	if (Slot.IsValid() && !Slot->bCopied.exchange(true))
	{
		Slot->Copied->Trigger();
	}
}

void FLidarRayTraceViewExtension::SubmitJob(const FRenderTarget* Target, FLidarGpuJob&& Job)
{
	if (!Target || !Job.Slot.IsValid() || !Job.Rays.IsValid() || Job.Rays->IsEmpty())
	{
		Complete(Job.Slot);
		return;
	}

	PendingJobs++;
	ENQUEUE_RENDER_COMMAND(FlightForgeSubmitLidarJob)(
	    [this, Target, MovedJob = MoveTemp(Job)](FRHICommandListImmediate&) mutable
	    {
		    FPendingJob& Pending = Jobs.FindOrAdd(Target);
		    if (Pending.Job.Slot.IsValid())
		    {
			    Complete(Pending.Job.Slot);
			    PendingJobs--;
		    }
		    Pending.Job         = MoveTemp(MovedJob);
		    Pending.FrameNumber = GFrameNumberRenderThread;
	    });
}

void FLidarRayTraceViewExtension::AddTimestamp(FRDGBuilder& GraphBuilder, const FRenderQueryRHIRef& Query)
{
	GraphBuilder.AddPass(RDG_EVENT_NAME("FlightForgeLidarTimestamp"), ERDGPassFlags::NeverCull, [Query](FRHICommandListImmediate& RHICmdList) { RHICmdList.EndRenderQuery(Query); });
}

void FLidarRayTraceViewExtension::PollProfiles_RenderThread()
{
	for (int32 i = ClosedProfiles.Num() - 1; i >= 0; --i)
	{
		uint64 Us[4] = {};
		bool   bReady = true;
		for (int32 q = 0; q < 4 && bReady; ++q)
		{
			bReady = ClosedProfiles[i]->Queries[q].IsValid() && RHIGetRenderQueryResult(ClosedProfiles[i]->Queries[q], Us[q], false);
		}
		if (bReady)
		{
			UE_LOG(LogTemp, Warning, TEXT("Lidar GPU profile: %d rays, capture render %.3f ms GPU (%.2f ms render thread from its pre-render hook), trace %.3f ms GPU"),
			       ClosedProfiles[i]->NumRays, (Us[3] - Us[0]) / 1000.0, ClosedProfiles[i]->RenderThreadMs, (Us[2] - Us[1]) / 1000.0);
			ClosedProfiles.RemoveAt(i);
		}
	}
}

void FLidarRayTraceViewExtension::PreRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily)
{
	if (CVarLidarGpuProfile.GetValueOnRenderThread() == 0 || !Jobs.Contains(InViewFamily.RenderTarget))
	{
		return;
	}
	TSharedPtr<FProfile> Profile = MakeShared<FProfile>();
	for (FRenderQueryRHIRef& Query : Profile->Queries)
	{
		Query = RHICreateRenderQuery(RQT_AbsoluteTime);
	}
	Profile->RenderThreadFrom = FPlatformTime::Seconds();
	AddTimestamp(GraphBuilder, Profile->Queries[0]);
	OpenProfiles.Add(InViewFamily.RenderTarget, Profile);
	if (!ProfilePoll.IsValid())
	{
		ProfilePoll = FCoreDelegates::OnBeginFrameRT.AddLambda([this]() { PollProfiles_RenderThread(); });
	}
}

// a family that rendered without building its TLAS (ray tracing turned off since the submit) completes empty
void FLidarRayTraceViewExtension::PostRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily)
{
	TSharedPtr<FProfile> Profile;
	if (OpenProfiles.RemoveAndCopyValue(InViewFamily.RenderTarget, Profile))
	{
		AddTimestamp(GraphBuilder, Profile->Queries[3]);
		Profile->RenderThreadMs = 1000.0 * (FPlatformTime::Seconds() - Profile->RenderThreadFrom);
		ClosedProfiles.Add(Profile);
	}

	if (PendingJobs.load() <= 0)
	{
		return;
	}

	for (auto It = Jobs.CreateIterator(); It; ++It)
	{
		if (It->Key == InViewFamily.RenderTarget || GFrameNumberRenderThread - It->Value.FrameNumber > 2)
		{
			Complete(It->Value.Job.Slot);
			It.RemoveCurrent();
			PendingJobs--;
		}
	}
}

void FLidarRayTraceViewExtension::PostTLASBuild_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView)
{
#if RHI_RAYTRACING
	if (PendingJobs.load() <= 0 || !InView.Family || !InView.bIsViewInfo)
	{
		return;
	}

	FPendingJob Pending;
	if (!Jobs.RemoveAndCopyValue(InView.Family->RenderTarget, Pending))
	{
		return;
	}
	PendingJobs--;

	FLidarGpuJob& Job = Pending.Job;

	const FViewInfo&       View           = static_cast<const FViewInfo&>(InView);
	const FSceneInterface* SceneInterface = InView.Family->Scene;
	const FScene*          Scene          = SceneInterface ? SceneInterface->GetRenderScene() : nullptr;
	const auto&            Handle         = View.*RayTracingViewHandleMember();
	if (!Scene || !Handle.IsValid() || !Scene->RayTracingScene.IsCreated() || !Scene->RayTracingScene.GetRHIRayTracingScene(ERayTracingSceneLayer::Base, Handle))
	{
		Complete(Job.Slot);
		return;
	}

	RDG_EVENT_SCOPE(GraphBuilder, "FlightForgeLidar");

	TArray<FUintVector2>* Info = GraphBuilder.AllocObject<TArray<FUintVector2>>();
	Info->SetNumZeroed(FMath::Max(1, Scene->GetMaxPersistentPrimitiveIndex()));
	for (const FPrimitiveSceneInfo* Primitive : Scene->Primitives)
	{
		const int32 Index = Primitive ? Primitive->GetPersistentIndex().Index : INDEX_NONE;
		if (Info->IsValidIndex(Index))
		{
			(*Info)[Index].X = Primitive->PrimitiveComponentId.PrimIDValue;
		}
	}
	const auto Flag = [SceneInterface, Info](const TArray<FPrimitiveComponentId>& Ids, const uint32 Value)
	{
		for (const FPrimitiveComponentId& Id : Ids)
		{
			const FPrimitiveSceneInfo* Primitive = SceneInterface->GetPrimitiveSceneInfo(Id);
			const int32                Index     = Primitive ? Primitive->GetPersistentIndex().Index : INDEX_NONE;
			if (Info->IsValidIndex(Index))
			{
				(*Info)[Index].Y = Value;
			}
		}
	};
	Flag(Job.Skip, 1);
	Flag(Job.Drop, 2);

	const int32 NumRays = Job.Rays->Num();
	const FIntPoint Size(FMath::Min(NumRays, OutputWidth), FMath::DivideAndRoundUp(NumRays, OutputWidth));

	auto* Rays = GraphBuilder.AllocObject<TSharedPtr<const TArray<FVector4f>, ESPMode::ThreadSafe>>(Job.Rays);
	FRDGBufferRef RayBuffer = CreateStructuredBuffer(GraphBuilder, TEXT("FlightForge.LidarRays"), sizeof(FVector4f), NumRays, (*Rays)->GetData(),
	                                                 NumRays * sizeof(FVector4f), ERDGInitialDataFlags::NoCopy);
	FRDGBufferRef InfoBuffer = CreateStructuredBuffer(GraphBuilder, TEXT("FlightForge.LidarPrimitives"), sizeof(FUintVector2), Info->Num(), Info->GetData(),
	                                                  Info->Num() * sizeof(FUintVector2), ERDGInitialDataFlags::NoCopy);

	FRDGTextureRef Output = GraphBuilder.CreateTexture(FRDGTextureDesc::Create2D(Size, PF_A32B32G32R32F, FClearValueBinding::None, TexCreate_ShaderResource | TexCreate_UAV),
	                                                   TEXT("FlightForge.LidarHits"));

	// the TLAS is in this view's translated world; the lidar sits near the capture, so the floats stay small
	const FVector Translation = Scene->RayTracingScene.GetPreViewTranslation(Handle);
	const auto    Rotation    = [](const FTransform& Pose) { const FQuat Q = Pose.GetRotation().GetNormalized(); return FVector4f(Q.X, Q.Y, Q.Z, Q.W); };

	FLidarRayTraceCS::FParameters* Parameters = GraphBuilder.AllocParameters<FLidarRayTraceCS::FParameters>();
	Parameters->View          = View.ViewUniformBuffer;
	Parameters->Scene         = View.GetSceneUniforms().GetBuffer(GraphBuilder);
	Parameters->TLAS          = Scene->RayTracingScene.GetLayerView(ERayTracingSceneLayer::Base, Handle);
	Parameters->Rays          = GraphBuilder.CreateSRV(RayBuffer);
	Parameters->PrimitiveInfo = GraphBuilder.CreateSRV(InfoBuffer);
	Parameters->NumPrimitives = Info->Num();
	Parameters->NumRays       = NumRays;
	Parameters->OutWidth      = OutputWidth;
	Parameters->StartOrigin   = FVector3f(Job.StartPose.GetLocation() + Translation);
	Parameters->EndOrigin     = FVector3f(Job.EndPose.GetLocation() + Translation);
	Parameters->StartRotation = Rotation(Job.StartPose);
	Parameters->EndRotation   = Rotation(Job.EndPose);
	Parameters->MinRange      = Job.MinRangeCm;
	Parameters->MaxRange      = Job.MaxRangeCm;
	Parameters->InstanceMask  = CVarLidarInstanceMask.GetValueOnRenderThread() & 0xFF;
	Parameters->Out           = GraphBuilder.CreateUAV(Output);

	const TSharedPtr<FProfile>* Profile = OpenProfiles.Find(InView.Family->RenderTarget);
	if (Profile)
	{
		(*Profile)->NumRays = NumRays;
		AddTimestamp(GraphBuilder, (*Profile)->Queries[1]);
	}

	TShaderMapRef<FLidarRayTraceCS> Shader(GetGlobalShaderMap(View.GetFeatureLevel()));
	FComputeShaderUtils::AddPass(GraphBuilder, RDG_EVENT_NAME("FlightForgeLidarTrace %d rays", NumRays), Shader, Parameters,
	                             FComputeShaderUtils::GetGroupCount(NumRays, 32));

	if (Profile)
	{
		AddTimestamp(GraphBuilder, (*Profile)->Queries[2]);
	}

	Job.Slot->Size        = Size;
	Job.Slot->bRawFloat4  = true;
	AddEnqueueCopyPass(GraphBuilder, Job.Slot->Readback.Get(), Output, FResolveRect());

	TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Slot = Job.Slot;
	GraphBuilder.AddPass(RDG_EVENT_NAME("FlightForgeLidarWatch"), ERDGPassFlags::NeverCull, [Slot](FRHICommandList&) { WatchReadback_RenderThread(Slot); });
#endif
}

/* //} */
