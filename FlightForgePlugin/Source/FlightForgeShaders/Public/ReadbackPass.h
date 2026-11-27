#pragma once

#include <atomic>

#include "CoreMinimal.h"
#include "SceneViewExtension.h"
#include "CameraReadback.h"

class FRenderTarget;

/* FReadbackViewExtension //{ */

/**
 * Copies a scene capture's target into a readback slot inside the frame that renders it,
 * so an on-demand request pays the capture's own render and nothing else. Jobs are
 * submitted from any thread and keyed by the capture's render target.
 */
class FLIGHTFORGESHADERS_API FReadbackViewExtension : public FSceneViewExtensionBase
{
public:
	FReadbackViewExtension(const FAutoRegister& AutoRegister);

	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override {}
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override {}
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override {}
	virtual bool IsActiveThisFrame_Internal(const FSceneViewExtensionContext& Context) const override;
	virtual void PostRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily) override;

	// SceneFrame is the scene's frame number at submission, so a render still in flight from an earlier frame cannot take the job
	void SubmitJob(const FRenderTarget* Target, const TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>& Slot, uint32 SceneFrame);

	// render thread: Source holds Target's pixels for the render of FrameNumber
	void CopyJob_RenderThread(FRDGBuilder& GraphBuilder, const FRenderTarget* Target, FRDGTextureRef Source, uint32 FrameNumber, double WorldTime);

	static TSharedPtr<FReadbackViewExtension, ESPMode::ThreadSafe> Get();

private:
	struct FJob
	{
		const FRenderTarget*                             Target = nullptr;
		TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Slot;
		uint64                                           Frame      = 0;
		uint32                                           SceneFrame = 0;
	};

	// game thread, every frame: completes empty the jobs whose frame rendered without them, so no camera waits forever
	void ExpireJobs();

	FCriticalSection   JobsLock;
	TArray<FJob>       Jobs;  // in submission order: the render thread trails the game thread by a frame
	std::atomic<int32> PendingJobs{0};
};

/* //} */
