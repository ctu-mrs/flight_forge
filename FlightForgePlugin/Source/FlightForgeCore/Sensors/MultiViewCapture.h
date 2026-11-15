#pragma once

#include "CoreMinimal.h"
#include "Components/SceneCaptureComponent2D.h"

#include "MultiViewCapture.generated.h"

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
