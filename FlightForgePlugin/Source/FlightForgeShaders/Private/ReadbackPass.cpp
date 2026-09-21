#include "ReadbackPass.h"

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
	}
	return Extension;
}

bool FReadbackViewExtension::IsActiveThisFrame_Internal(const FSceneViewExtensionContext& Context) const
{
	return PendingJobs.load() > 0;
}

void FReadbackViewExtension::SubmitJob(const FRenderTarget* Target, const TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>& Slot)
{
	if (!Target || !Slot.IsValid())
	{
		return;
	}
	FScopeLock Lock(&JobsLock);
	if (!Jobs.Contains(Target))
	{
		PendingJobs++;
	}
	Jobs.Add(Target, Slot);
}

void FReadbackViewExtension::PostRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily)
{
	if (PendingJobs.load() <= 0 || !InViewFamily.RenderTarget)
	{
		return;
	}

	TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Slot;
	{
		FScopeLock Lock(&JobsLock);
		if (!Jobs.RemoveAndCopyValue(InViewFamily.RenderTarget, Slot))
		{
			return;
		}
		PendingJobs--;
	}

	FRHITexture* Texture = InViewFamily.RenderTarget->GetRenderTargetTexture();
	if (!Texture || !Slot->Readback)
	{
		Slot->bCopied = true;
		Slot->Copied->Trigger();
		return;
	}

	FRDGTextureRef Source = GraphBuilder.RegisterExternalTexture(CreateRenderTarget(Texture, TEXT("FlightForge.CaptureTarget")));
	AddEnqueueCopyPass(GraphBuilder, Slot->Readback.Get(), Source, FResolveRect());

	GraphBuilder.AddPass(RDG_EVENT_NAME("FlightForgeReadbackWatch"), ERDGPassFlags::NeverCull, [Slot](FRHICommandList&) { WatchReadback_RenderThread(Slot); });
}

/* //} */
