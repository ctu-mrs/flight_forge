#include "EventGen.h"

#include "GlobalShader.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
#include "ShaderParameterStruct.h"

/* FEventGenCS //{ */

class FEventGenCS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FEventGenCS);
	SHADER_USE_PARAMETER_STRUCT(FEventGenCS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
	SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, InputTexture)
	SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, StateTexture)
	SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<uint>, LastEventTexture)
	SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<uint2>, EventBuffer)
	SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<uint>, HeaderBuffer)
	SHADER_PARAMETER(FIntPoint, Size)
	SHADER_PARAMETER(float, Dt)
	SHADER_PARAMETER(uint32, BaseUs)
	SHADER_PARAMETER(uint32, DtUs)
	SHADER_PARAMETER(uint32, MaxEvents)
	SHADER_PARAMETER(uint32, FrameIndex)
	SHADER_PARAMETER(uint32, Seed)
	SHADER_PARAMETER(uint32, bSeedState)
	SHADER_PARAMETER(uint32, bCountOnly)
	SHADER_PARAMETER(uint32, Budget)
	SHADER_PARAMETER(float, ThresholdPos)
	SHADER_PARAMETER(float, ThresholdNeg)
	SHADER_PARAMETER(float, ThresholdSigma)
	SHADER_PARAMETER(uint32, RefractoryUs)
	SHADER_PARAMETER(float, CutoffHz)
	SHADER_PARAMETER(float, LeakRateHz)
	SHADER_PARAMETER(float, ShotNoiseRateHz)
	SHADER_PARAMETER(float, HotPixelFraction)
	SHADER_PARAMETER(float, LogEps)
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}
};

IMPLEMENT_GLOBAL_SHADER(FEventGenCS, "/FlightForgePlugin/EventGen.usf", "MainCS", SF_Compute);

//}

/* Dispatch_RenderThread() //{ */

void FlightForgeEventGen::Dispatch_RenderThread(FRHICommandListImmediate& RHICmdList, FRHITexture* Input, FEventGpuState& State, FEventGenParams Params,
                                                const TSharedPtr<FEventReadback, ESPMode::ThreadSafe>& Slot)
{
	if (!Input || Params.Size.X <= 0 || Params.Size.Y <= 0 || Params.MaxEvents == 0)
	{
		Slot->bCopied = true;
		return;
	}

	FRDGBuilder GraphBuilder(RHICmdList);

	const bool bRecreate = State.Size != Params.Size || !State.State || !State.LastEvent;
	Params.bSeedState |= bRecreate;

	FRDGTextureRef InputTexture = RegisterExternalTexture(GraphBuilder, Input, TEXT("FlightForge.EventInput"));

	FRDGTextureRef StateTexture;
	FRDGTextureRef LastEventTexture;

	if (bRecreate)
	{
		const ETextureCreateFlags Flags = TexCreate_ShaderResource | TexCreate_UAV;
		StateTexture     = GraphBuilder.CreateTexture(FRDGTextureDesc::Create2D(Params.Size, PF_A32B32G32R32F, FClearValueBinding::None, Flags), TEXT("FlightForge.EventState"));
		LastEventTexture = GraphBuilder.CreateTexture(FRDGTextureDesc::Create2D(Params.Size, PF_R32_UINT, FClearValueBinding::None, Flags), TEXT("FlightForge.EventLastEvent"));

		State.Size = Params.Size;
	}
	else
	{
		StateTexture     = GraphBuilder.RegisterExternalTexture(State.State, TEXT("FlightForge.EventState"));
		LastEventTexture = GraphBuilder.RegisterExternalTexture(State.LastEvent, TEXT("FlightForge.EventLastEvent"));
	}

	// one buffer per frame in flight, so a frame that outgrew its first readback can still be copied again
	FRDGBufferRef EventBuffer = GraphBuilder.CreateBuffer(FRDGBufferDesc::CreateStructuredDesc(sizeof(uint32) * 2, Params.MaxEvents), TEXT("FlightForge.EventBuffer"));

	FRDGBufferRef HeaderBuffer = GraphBuilder.CreateBuffer(FRDGBufferDesc::CreateStructuredDesc(sizeof(uint32), 4), TEXT("FlightForge.EventHeader"));
	AddClearUAVPass(GraphBuilder, GraphBuilder.CreateUAV(HeaderBuffer), 0u);

	auto* PassParameters             = GraphBuilder.AllocParameters<FEventGenCS::FParameters>();
	PassParameters->InputTexture     = InputTexture;
	PassParameters->StateTexture     = GraphBuilder.CreateUAV(StateTexture);
	PassParameters->LastEventTexture = GraphBuilder.CreateUAV(LastEventTexture);
	PassParameters->EventBuffer      = GraphBuilder.CreateUAV(EventBuffer);
	PassParameters->HeaderBuffer     = GraphBuilder.CreateUAV(HeaderBuffer);
	PassParameters->Size             = Params.Size;
	PassParameters->Dt               = Params.Dt;
	PassParameters->BaseUs           = Params.BaseUs;
	PassParameters->DtUs             = Params.DtUs;
	PassParameters->MaxEvents        = Params.MaxEvents;
	PassParameters->FrameIndex       = Params.FrameIndex;
	PassParameters->Seed             = Params.Seed;
	PassParameters->bSeedState       = Params.bSeedState ? 1u : 0u;
	PassParameters->ThresholdPos     = Params.ThresholdPos;
	PassParameters->ThresholdNeg     = Params.ThresholdNeg;
	PassParameters->ThresholdSigma   = Params.ThresholdSigma;
	PassParameters->RefractoryUs     = Params.RefractoryUs;
	PassParameters->CutoffHz         = Params.CutoffHz;
	PassParameters->LeakRateHz       = Params.LeakRateHz;
	PassParameters->ShotNoiseRateHz  = Params.ShotNoiseRateHz;
	PassParameters->HotPixelFraction = Params.HotPixelFraction;
	PassParameters->LogEps           = Params.LogEps;
	PassParameters->Budget           = Params.Budget;
	PassParameters->bCountOnly       = 0u;

	auto* CountParameters       = GraphBuilder.AllocParameters<FEventGenCS::FParameters>();
	*CountParameters            = *PassParameters;
	CountParameters->bCountOnly = 1u;

	TShaderMapRef<FEventGenCS> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
	const FIntVector           Groups = FComputeShaderUtils::GetGroupCount(Params.Size, FIntPoint(8, 8));
	FComputeShaderUtils::AddPass(GraphBuilder, RDG_EVENT_NAME("FlightForgeEventCount"), ComputeShader, CountParameters, Groups);
	FComputeShaderUtils::AddPass(GraphBuilder, RDG_EVENT_NAME("FlightForgeEventGen"), ComputeShader, PassParameters, Groups);

	const uint32 CopyEvents = FMath::Clamp(Params.CopyEvents, 1u, Params.MaxEvents);
	Slot->Capacity          = Params.MaxEvents;
	Slot->CopiedCapacity    = CopyEvents;

	if (!Slot->Header)
	{
		Slot->Header = MakeUnique<FRHIGPUBufferReadback>(TEXT("FlightForgeEventHeader"));
	}
	if (!Slot->Events)
	{
		Slot->Events = MakeUnique<FRHIGPUBufferReadback>(TEXT("FlightForgeEvents"));
	}

	AddEnqueueCopyPass(GraphBuilder, Slot->Header.Get(), HeaderBuffer, sizeof(uint32) * 4);
	AddEnqueueCopyPass(GraphBuilder, Slot->Events.Get(), EventBuffer, CopyEvents * sizeof(uint32) * 2);

	if (bRecreate)
	{
		GraphBuilder.QueueTextureExtraction(StateTexture, &State.State);
		GraphBuilder.QueueTextureExtraction(LastEventTexture, &State.LastEvent);
	}
	GraphBuilder.QueueBufferExtraction(EventBuffer, &Slot->Buffer, ERHIAccess::CopySrc);

	GraphBuilder.Execute();
}

//}

/* Resolve_RenderThread() //{ */

void FlightForgeEventGen::Resolve_RenderThread(FRHICommandListImmediate& RHICmdList, const TSharedPtr<FEventReadback, ESPMode::ThreadSafe>& Slot)
{
	if (Slot->Header && Slot->Events)
	{
		if (!Slot->bHeaderRead)
		{
			if (const uint32* Header = static_cast<const uint32*>(Slot->Header->Lock(sizeof(uint32) * 4)))
			{
				Slot->Count        = Header[0];
				Slot->MaxLogChange = *reinterpret_cast<const float*>(&Header[1]);
				Slot->Kept         = Header[2];
				Slot->Header->Unlock();
			}
			Slot->bHeaderRead = true;
		}

		const uint32 Available = FMath::Min(Slot->Kept, Slot->Capacity);

		if (Available > Slot->CopiedCapacity && Slot->Buffer)
		{
			Slot->Events = MakeUnique<FRHIGPUBufferReadback>(TEXT("FlightForgeEvents"));
			Slot->Events->EnqueueCopy(RHICmdList, Slot->Buffer->GetRHI(), Available * sizeof(uint32) * 2);
			Slot->CopiedCapacity = Available;
			Slot->bResolveIssued = false;
			return;
		}

		if (Available > 0)
		{
			if (const uint32* Events = static_cast<const uint32*>(Slot->Events->Lock(Available * sizeof(uint32) * 2)))
			{
				Slot->Packed.SetNumUninitialized(Available);
				for (uint32 i = 0; i < Available; ++i)
				{
					Slot->Packed[i] = (static_cast<uint64>(Events[2 * i + 1]) << 32) | Events[2 * i];
				}
				Slot->Events->Unlock();
			}
		}
	}

	Slot->Buffer.SafeRelease();
	Slot->bCopied = true;
}

//}
