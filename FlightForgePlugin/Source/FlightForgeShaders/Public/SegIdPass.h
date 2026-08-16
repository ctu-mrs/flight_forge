#pragma once

#include <atomic>

#include "CoreMinimal.h"
#include "SceneViewExtension.h"

#include "CameraReadback.h"

class FPrimitiveSceneProxy;
class FRenderTarget;

/* FSegIdDrawItem //{ */

// One primitive to render into the ID pass. The proxy is only valid for the render
// frame it was collected on: a job left unconsumed past that must be dropped, or the
// pass walks proxies the render thread has since freed.
struct FSegIdDrawItem
{
	const FPrimitiveSceneProxy* Proxy = nullptr;

	// 24-bit instance id packed little-endian into RGB, in [0,1] units
	FLinearColor IdColor = FLinearColor::Black;
};

/* //} */

/* FSegIdViewExtension //{ */

/**
 * Renders per-actor instance IDs through a plugin-owned mesh pass.
 *
 * A USegIdCamera submits a one-shot job keyed by its capture's render target;
 * when that view family reaches PrePostProcessPass, the extension draws the
 * job's primitives into a fresh 8-bit target with our SegId shaders,
 * depth-testing read-only against the scene depth (so anything the pass does
 * not draw still occludes correctly), and enqueues the GPU readback straight
 * into the camera's FCameraReadback slot. The capture's own color output is
 * never used, so no tonemapper or gamma can touch the IDs.
 */
class FLIGHTFORGESHADERS_API FSegIdViewExtension : public FSceneViewExtensionBase
{
public:
	FSegIdViewExtension(const FAutoRegister& AutoRegister);

	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override
	{
	}
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override
	{
	}
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override
	{
	}

	virtual bool IsActiveThisFrame_Internal(const FSceneViewExtensionContext& Context) const override;

	virtual void PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& InView, const FPostProcessingInputs& Inputs) override;

	/** One-shot: the next render of TargetFilter's view family produces this job's ID image. */
	void SubmitJob_GameThread(const FRenderTarget* TargetFilter, FIntPoint Size, TArray<FSegIdDrawItem>&& Items,
	                          const TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>& Slot);

	/** Lazily created and globally registered; gating keeps it inert for foreign view families. */
	static TSharedPtr<FSegIdViewExtension, ESPMode::ThreadSafe> Get();

private:
	struct FSegIdJob
	{
		FIntPoint                                        Size = FIntPoint::ZeroValue;
		TArray<FSegIdDrawItem>                           Items;
		TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Slot;

		uint32 FrameNumber = 0;
	};

	void DropStaleJobs_RenderThread();

	// render thread only
	TMap<const FRenderTarget*, FSegIdJob> Jobs;

	std::atomic<int32> PendingJobs{0};
};

/* //} */
