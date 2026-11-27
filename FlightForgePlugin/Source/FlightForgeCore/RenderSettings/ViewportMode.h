#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"
#include "Styling/SlateBrush.h"
#include "UObject/GCObject.h"

class SWidget;
class UCamera;
class UDirectionalLightComponent;
class USkyLightComponent;
class UTextureRenderTarget2D;
class UWorld;

/* FViewportMode //{ */

// FlightForge.NoViewport 1: no main viewport render, sensors capture on their own and the window shows a camera
class FLIGHTFORGECORE_API FViewportMode : public FGCObject
{
public:
	explicit FViewportMode(bool bStartOff);
	virtual ~FViewportMode() override;

	virtual void    AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override { return TEXT("FViewportMode"); }

private:
	void OnWorldTickEnd(UWorld* World, ELevelTick TickType, float DeltaSeconds);
	void RenderCaptures(UWorld* World);
	bool SkyRefreshDue(UWorld* World);
	void SetSkyTimeSlicing(bool bOff);
	void UpdateFeed(UWorld* World, bool bShow);
	UCamera* PickFeedCamera(UWorld* World) const;

	FDelegateHandle TickEndHandle;
	bool            bWasOff = false;

	/* sky light //{ */

	TWeakObjectPtr<UWorld>                            SkyWorld;
	TArray<TWeakObjectPtr<USkyLightComponent>>         RealTimeSkyLights;
	TArray<TWeakObjectPtr<UDirectionalLightComponent>> Suns;
	uint32                                            SkyHash        = 0;
	int32                                             SkyFramesOwed  = 0;
	double                                            LastSkyRefresh = 0.0;
	TOptional<int32>                                  SavedTimeSlice;

	//}

	/* camera feed //{ */

	TSharedPtr<SWidget>      Feed;
	FSlateBrush              FeedBrush;
	TObjectPtr<UTextureRenderTarget2D> FeedTarget = nullptr;
	FString                  FeedLabel;
	double                   NextFeedPick = 0.0;

	//}
};

//}
