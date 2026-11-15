#pragma once

#include "CoreMinimal.h"
#include "Components/SceneCaptureComponent2D.h"

#include "MultiViewCapture.generated.h"

class FSceneViewStateInterface;
class FTextureRenderTargetResource;
class UTextureRenderTarget2D;

/* UMultiViewCapture //{ */

// renders its members as the views of one view family; they must share format, capture source and show flags
UCLASS()
class FLIGHTFORGECORE_API UMultiViewCapture : public USceneCaptureComponent2D
{
	GENERATED_BODY()

public:
	UMultiViewCapture();

	static bool Enabled();

	static bool BatchEnabled();

	// FlightForge.Camera.ViewportLook: colour captures render with the main viewport's post-process, Lumen and screen percentage
	static bool LookEnabled();
	static bool HasLook(const USceneCaptureComponent2D* View);

	// one capture as a family of one, with the viewport's look
	static void RenderSingle(USceneCaptureComponent2D* View, FSceneInterface* Scene, ISceneRenderBuilder& SceneRenderBuilder);

	// the other views take view 0's exposure, as one camera would; no seams between fisheye faces
	bool bSharedExposure = false;

	// the robot's batch, made on first use
	static UMultiViewCapture* RobotBatch(AActor* Robot);

	// joins this frame's batch instead of rendering alone
	void Defer(USceneCaptureComponent2D* View);

	void SetViews(TArrayView<USceneCaptureComponent2D* const> InViews);

	// renders the members together when enabled, else each member as before
	void Capture(bool bDeferred);
	void SetCaptureEveryFrame(bool bEnable);
	bool CapturesEveryFrame() const { return bEveryFrame; }

	virtual void UpdateSceneCaptureContents(FSceneInterface* Scene, ISceneRenderBuilder& SceneRenderBuilder) override;
	virtual void OnUnregister() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	static bool Groupable(const USceneCaptureComponent2D* View);
	static bool Compatible(const USceneCaptureComponent2D* A, const USceneCaptureComponent2D* B);
	bool        PrepareAtlas(TArrayView<USceneCaptureComponent2D* const> Members, int32 Slot, TArray<FIntRect>& OutTiles);
	void        RenderFamilies(FSceneInterface* Scene, ISceneRenderBuilder& SceneRenderBuilder, TArrayView<USceneCaptureComponent2D* const> Members, int32& Slot);
	void        RenderFamily(FSceneInterface* Scene, ISceneRenderBuilder& SceneRenderBuilder, TArrayView<USceneCaptureComponent2D* const> Members, int32 Slot,
	                         bool bSharedLumen);

	struct FFamilySetup
	{
		FTextureRenderTargetResource*                                   Target = nullptr;
		TArray<FIntRect>                                                Tiles;
		bool                                                            bCopyTiles = true;
		bool                                                            bSharedExposure = false;
		bool                                                            bCube = false;
		FSceneViewStateInterface*                                       LumenAnchor = nullptr;
		TFunction<FSceneViewStateInterface*(USceneCaptureComponent2D*)> StateFor;
		FString                                                         Name;
	};
	static void RenderViews(FSceneInterface* Scene, ISceneRenderBuilder& SceneRenderBuilder, TArrayView<USceneCaptureComponent2D* const> Members,
	                        FFamilySetup&& Setup);
	void ApplyEveryFrame();
	// a member's state inside a shared-Lumen family: shares this component's origin, so its Lumen scene; the member's own state stays untouched
	FSceneViewStateInterface* FamilyState(USceneCaptureComponent2D* Capture);
	void                      PruneFamilyStates();

	UPROPERTY()
	TArray<TObjectPtr<USceneCaptureComponent2D>> Views;

	UPROPERTY()
	TArray<TObjectPtr<UTextureRenderTarget2D>> Atlases;

	TArray<TWeakObjectPtr<USceneCaptureComponent2D>> Pending;

	// destroyed before ViewStates[0], their share origin
	TMap<TWeakObjectPtr<USceneCaptureComponent2D>, TUniquePtr<FSceneViewStateReference>> FamilyStates;

	bool bEveryFrame   = false;
	bool bAppliedGroup = false;
};

/* //} */

/* ULookCapture //{ */

// a colour camera's capture: with the viewport look it renders through UMultiViewCapture's view setup, not the engine's scene capture defaults
UCLASS()
class FLIGHTFORGECORE_API ULookCapture : public USceneCaptureComponent2D
{
	GENERATED_BODY()

public:
	bool bViewportLook = false;

	virtual void UpdateSceneCaptureContents(FSceneInterface* Scene, ISceneRenderBuilder& SceneRenderBuilder) override;
};

/* //} */
