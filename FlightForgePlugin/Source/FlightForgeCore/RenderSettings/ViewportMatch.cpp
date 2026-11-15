#include "ViewportMatch.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/World.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"
#include "Containers/Ticker.h"
#include "HAL/IConsoleManager.h"
#include "UnrealClient.h"

#include "FlightForgeCore/FlightForgeCore.h"

/* UViewportMatch //{ */

bool UViewportMatch::Match(USceneCaptureComponent2D* Capture, FString& Error)
{ //{
	UWorld*            World = GetWorld();
	APlayerController* Player = World ? World->GetFirstPlayerController() : nullptr;
	const IConsoleVariable* NoViewport = IConsoleManager::Get().FindConsoleVariable(TEXT("FlightForge.NoViewport"));
	if (!Player || !GEngine || !GEngine->GameViewport)
	{
		Error = TEXT("no local player viewport");
		return false;
	}
	if (NoViewport && NoViewport->GetInt() != 0)
	{
		Error = TEXT("the main viewport is off (FlightForge.NoViewport 1)");
		return false;
	}
	if (!Capture || !Capture->TextureTarget)
	{
		Error = TEXT("the camera has no render target yet");
		return false;
	}

	if (!Probe)
	{
		FActorSpawnParameters Params;
		Params.ObjectFlags |= RF_Transient;
		Probe = World->SpawnActor<ACameraActor>(Params);
		if (!Probe)
		{
			Error = TEXT("could not spawn the viewport probe camera");
			return false;
		}
		SavedViewTarget = Player->GetViewTarget();
		SavedResolution = FIntPoint(GSystemResolution.ResX, GSystemResolution.ResY);
	}

	UCameraComponent* Camera = Probe->GetCameraComponent();
	Camera->bConstrainAspectRatio = false;
	Camera->FieldOfView           = Capture->FOVAngle;
	Camera->AspectRatio           = Capture->TextureTarget->SizeX / FMath::Max(1.0f, (float)Capture->TextureTarget->SizeY);
	// captures keep the horizontal fov; the project may keep the vertical one
	Camera->bOverrideAspectRatioAxisConstraint = true;
	Camera->AspectRatioAxisConstraint          = AspectRatio_MaintainXFOV;
	Probe->AttachToComponent(Capture, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Player->SetViewTarget(Probe);

	WantedSize = FIntPoint(Capture->TextureTarget->SizeX, Capture->TextureTarget->SizeY);
	if (GEngine->GameViewport->Viewport && GEngine->GameViewport->Viewport->GetSizeXY() != WantedSize)
	{
		FFlightForgeCoreModule::ResizeOffScreen(WantedSize.X, WantedSize.Y);
	}
	return true;
} //}

void UViewportMatch::Release()
{ //{
	if (!Probe)
	{
		return;
	}
	if (APlayerController* Player = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr)
	{
		Player->SetViewTarget(SavedViewTarget.IsValid() ? SavedViewTarget.Get() : Player);
	}
	Probe->Destroy();
	Probe      = nullptr;
	WantedSize = FIntPoint::ZeroValue;
	if (SavedResolution.X > 0 && SavedResolution.Y > 0)
	{
		FFlightForgeCoreModule::ResizeOffScreen(SavedResolution.X, SavedResolution.Y);
	}
} //}

bool UViewportMatch::RequestShot(const std::shared_ptr<FViewportShot>& Shot, FString& Error)
{ //{
	if (!GEngine || !GEngine->GameViewport)
	{
		Error = TEXT("no game viewport");
		return false;
	}
	if (!ScreenshotHandle.IsValid())
	{
		ScreenshotHandle = UGameViewportClient::OnScreenshotCaptured().AddUObject(this, &UViewportMatch::OnScreenshot);
	}
	PendingShot = Shot;
	FScreenshotRequest::RequestScreenshot(false);
	return true;
} //}

void UViewportMatch::OnScreenshot(const int32 Width, const int32 Height, const TArray<FColor>& Pixels)
{ //{
	if (!PendingShot)
	{
		return;
	}
	// a resize lands a frame or two after it is asked for; the engine clears requests made inside this callback
	if (WantedSize.X > 0 && FIntPoint(Width, Height) != WantedSize)
	{
		FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([](float) {
			FScreenshotRequest::RequestScreenshot(false);
			return false;
		}));
		return;
	}
	const std::shared_ptr<FViewportShot> Shot = MoveTemp(PendingShot);
	Shot->Pixels = Pixels;
	Shot->Size   = FIntPoint(Width, Height);
	Shot->Stamp  = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0;
	const APlayerController* Player = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	Shot->Fov    = Player && Player->PlayerCameraManager ? Player->PlayerCameraManager->GetFOVAngle() : 0.0;
	Shot->bDone  = true;
} //}

void UViewportMatch::Deinitialize()
{ //{
	if (ScreenshotHandle.IsValid())
	{
		UGameViewportClient::OnScreenshotCaptured().Remove(ScreenshotHandle);
	}
	PendingShot.reset();
	Super::Deinitialize();
} //}

/* //} */
