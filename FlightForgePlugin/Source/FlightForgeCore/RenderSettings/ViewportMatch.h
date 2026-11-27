#pragma once

#include <atomic>
#include <memory>

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "ViewportMatch.generated.h"

class ACameraActor;
class AActor;
class USceneCaptureComponent2D;

/* UViewportMatch //{ */

struct FViewportShot
{
	std::atomic<bool> bDone{false};
	TArray<FColor>    Pixels;
	FIntPoint         Size = FIntPoint::ZeroValue;
	double            Stamp = 0.0;
	double            Fov   = 0.0;
};

// player 0's view on a camera's exact view and image size, so the two renders can be compared; game thread only
UCLASS()
class FLIGHTFORGECORE_API UViewportMatch : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	bool Match(USceneCaptureComponent2D* Capture, FString& Error);
	void Release();

	// fills Shot after the next viewport frame drawn at the matched size
	bool RequestShot(const std::shared_ptr<FViewportShot>& Shot, FString& Error);

	virtual void Deinitialize() override;

private:
	void OnScreenshot(int32 Width, int32 Height, const TArray<FColor>& Pixels);

	UPROPERTY()
	TObjectPtr<ACameraActor> Probe = nullptr;

	TWeakObjectPtr<AActor> SavedViewTarget;
	FIntPoint              SavedResolution = FIntPoint::ZeroValue;
	FIntPoint              WantedSize      = FIntPoint::ZeroValue;

	std::shared_ptr<FViewportShot> PendingShot;
	FDelegateHandle                ScreenshotHandle;
};

/* //} */
