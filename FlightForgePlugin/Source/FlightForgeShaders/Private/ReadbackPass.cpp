#include "ReadbackPass.h"

#include "Misc/CoreDelegates.h"

#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
#include "RenderTargetPool.h"
#include "SceneView.h"
#include "UnrealClient.h"

/* FReadbackViewExtension //{ */

FReadbackViewExtension::FReadbackViewExtension(const FAutoRegister& AutoRegister) : FSceneViewExtensionBase(AutoRegister)
{
}

TSharedPtr<FReadbackViewExtension, ESPMode::ThreadSafe> FReadbackViewExtension::Get()
{
	static TSharedPtr<FReadbackViewExtension, ESPMode::ThreadSafe> Extension;
	if (!Extension.IsValid())
	{
		Extension = FSceneViewExtensions::NewExtension<FReadbackViewExtension>();
		FCoreDelegates::OnBeginFrame.AddLambda([Weak = TWeakPtr<FReadbackViewExtension, ESPMode::ThreadSafe>(Extension)]() {
			if (const TSharedPtr<FReadbackViewExtension, ESPMode::ThreadSafe> Pinned = Weak.Pin())
			{
				Pinned->ExpireJobs();
			}
		});
	}
	return Extension;
}

bool FReadbackViewExtension::IsActiveThisFrame_Internal(const FSceneViewExtensionContext& Context) const
{
	return PendingJobs.load() > 0;
}

void FReadbackViewExtension::SubmitJob(const FRenderTarget* Target, const TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>& Slot, const uint32 SceneFrame)
{
	if (!Target || !Slot.IsValid())
	{
		return;
	}
	FScopeLock Lock(&JobsLock);
	Jobs.Add(FJob{Target, Slot, GFrameCounter, SceneFrame});
	PendingJobs = Jobs.Num();
}

void FReadbackViewExtension::ExpireJobs()
{
	// a frame's render has finished by the time the game thread is this many frames on
	constexpr uint64 RenderedFrames = 4;

	TArray<TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>> Expired;
	{
		FScopeLock Lock(&JobsLock);
		for (int32 i = Jobs.Num() - 1; i >= 0; --i)
		{
			if (Jobs[i].Frame + RenderedFrames <= GFrameCounter)
			{
				UE_LOG(LogTemp, Warning, TEXT("FlightForge: a camera copy from frame %llu expired, its capture never rendered"), Jobs[i].Frame);
				Expired.Add(Jobs[i].Slot);
				Jobs.RemoveAt(i);
			}
		}
		PendingJobs = Jobs.Num();
	}

	for (const auto& Slot : Expired)
	{
		if (!Slot->bCopied.exchange(true))
		{
			Slot->Copied->Trigger();
		}
	}
}

void FReadbackViewExtension::PostRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily)
{
	if (PendingJobs.load() <= 0 || !InViewFamily.RenderTarget)
	{
		return;
	}
	FRHITexture* Texture = InViewFamily.RenderTarget->GetRenderTargetTexture();
	CopyJob_RenderThread(GraphBuilder, InViewFamily.RenderTarget, Texture ? GraphBuilder.RegisterExternalTexture(CreateRenderTarget(Texture, TEXT("FlightForge.CaptureTarget"))) : nullptr,
	                     InViewFamily.FrameNumber, InViewFamily.Time.GetWorldTimeSeconds());
}

void FReadbackViewExtension::CopyJob_RenderThread(FRDGBuilder& GraphBuilder, const FRenderTarget* Target, FRDGTextureRef Source, const uint32 FrameNumber, const double WorldTime)
{
	if (PendingJobs.load() <= 0)
	{
		return;
	}

	TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Slot;
	{
		FScopeLock Lock(&JobsLock);
		const int32 Index = Jobs.IndexOfByPredicate([Target, FrameNumber](const FJob& Job) { return Job.Target == Target && FrameNumber > Job.SceneFrame; });
		if (Index == INDEX_NONE)
		{
			return;
		}
		Slot = Jobs[Index].Slot;
		Jobs.RemoveAt(Index);
		PendingJobs = Jobs.Num();
	}

	Slot->Stamp = WorldTime;

	if (!Source || !Slot->Readback)
	{
		Slot->bCopied = true;
		Slot->Copied->Trigger();
		return;
	}

	AddEnqueueCopyPass(GraphBuilder, Slot->Readback.Get(), Source, FResolveRect());

	GraphBuilder.AddPass(RDG_EVENT_NAME("FlightForgeReadbackWatch"), ERDGPassFlags::NeverCull, [Slot](FRHICommandList&) { WatchReadback_RenderThread(Slot); });
}

/* //} */
