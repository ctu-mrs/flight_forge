#pragma once

#include <atomic>

#include "CoreMinimal.h"
#include "SceneViewExtension.h"

#include "CameraReadback.h"

class FRenderTarget;

/* FLidarGpuJob //{ */

struct FLidarGpuJob
{
	// xyz unit direction in the sensor frame, w the ray's place in the scan: 0 at StartPose, 1 at EndPose
	TSharedPtr<const TArray<FVector4f>, ESPMode::ThreadSafe> Rays;

	FTransform StartPose;  // world, cm
	FTransform EndPose;    // the frame the points come back in

	float MinRangeCm = 0.0f;
	float MaxRangeCm = 0.0f;

	TArray<FPrimitiveComponentId> Skip;  // traced through
	TArray<FPrimitiveComponentId> Drop;  // a hit is no return

	// Float4Pixels, one per ray in ray order: xyz cm in EndPose, w the hit component's id bits; NaN xyz for no return
	TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Slot;
};

/* //} */

/* FLidarRayTraceViewExtension //{ */

// traces lidar rays in the TLAS the renderer builds for the lidar's own capture (plugin GI method), read back like a camera
class FLIGHTFORGESHADERS_API FLidarRayTraceViewExtension : public FSceneViewExtensionBase
{
public:
	FLidarRayTraceViewExtension(const FAutoRegister& AutoRegister);

	static constexpr int32 OutputWidth = 2048;

	// hardware ray tracing in this process, now: r.RayTracing.Enable can turn it off at runtime
	static bool IsRayTracingAvailable();

	virtual ESceneViewExtensionFlags GetFlags() const override { return ESceneViewExtensionFlags::SubscribesToPostTLASBuild; }

	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override {}
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override {}
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override {}
	virtual bool IsActiveThisFrame_Internal(const FSceneViewExtensionContext& Context) const override;
	virtual void PostTLASBuild_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView) override;
	virtual void PreRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily) override;
	virtual void PostRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily) override;

	// game thread; the next render of Target's family traces it, or completes the slot empty
	void SubmitJob(const FRenderTarget* Target, FLidarGpuJob&& Job);

	static TSharedPtr<FLidarRayTraceViewExtension, ESPMode::ThreadSafe> Get();

private:
	struct FPendingJob
	{
		FLidarGpuJob Job;
		uint32       FrameNumber = 0;
	};

	static void Complete(const TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>& Slot);

	// FlightForge.Lidar.GpuProfile: timestamps around the lidar's whole render and around the trace
	struct FProfile
	{
		FRenderQueryRHIRef Queries[4];
		double             RenderThreadFrom = 0.0;
		double             RenderThreadMs   = 0.0;
		int32              NumRays          = 0;
	};
	static void AddTimestamp(FRDGBuilder& GraphBuilder, const FRenderQueryRHIRef& Query);
	void        PollProfiles_RenderThread();

	TMap<const FRenderTarget*, TSharedPtr<FProfile>> OpenProfiles;
	TArray<TSharedPtr<FProfile>>                     ClosedProfiles;
	FDelegateHandle                                  ProfilePoll;

	// render thread only
	TMap<const FRenderTarget*, FPendingJob> Jobs;

	std::atomic<int32> PendingJobs{0};
};

/* //} */
