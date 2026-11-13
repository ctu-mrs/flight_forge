#include "ViewportMode.h"

#include "Components/DirectionalLightComponent.h"
#include "Components/SceneCaptureComponent.h"
#include "Components/SkyLightComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/World.h"
#include "HAL/IConsoleManager.h"
#include "SceneInterface.h"
#include "Styling/CoreStyle.h"
#include "UObject/UObjectIterator.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"

#include "FlightForgeCore/RobotPawn.h"
#include "FlightForgeCore/Sensors/Camera.h"

/* console variables //{ */

namespace
{

TAutoConsoleVariable<int32> CVarNoViewport(TEXT("FlightForge.NoViewport"), 0,
                                           TEXT("1: the main viewport draws no world; sensor captures render on their own and the window shows a camera"));

TAutoConsoleVariable<float> CVarSkyRefresh(TEXT("FlightForge.NoViewport.SkyRefresh"), 10.0f,
                                           TEXT("seconds between full frames that recapture a real-time sky light, besides sun changes; 0: only on changes"));

TAutoConsoleVariable<FString> CVarViewportCamera(TEXT("FlightForge.ViewportCamera"), TEXT(""),
                                                 TEXT("camera the window shows without a viewport, \"robot/sensor\" or \"sensor\"; empty: the one read last"));

const TCHAR* TimeSliceVar = TEXT("r.SkyLight.RealTimeReflectionCapture.TimeSlice");

}  // namespace

//}

/* FViewportMode() //{ */

FViewportMode::FViewportMode(const bool bStartOff)
{
	if (bStartOff)
	{
		CVarNoViewport->Set(1, ECVF_SetByCommandline);
		UE_LOG(LogTemp, Display, TEXT("FlightForge: no main viewport, sensors render on their own (FlightForge.NoViewport 0 brings it back)"));
	}

	TickEndHandle = FWorldDelegates::OnWorldTickEnd.AddRaw(this, &FViewportMode::OnWorldTickEnd);
}

FViewportMode::~FViewportMode()
{
	FWorldDelegates::OnWorldTickEnd.Remove(TickEndHandle);
	SetSkyTimeSlicing(false);

	if (Feed.IsValid() && GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(Feed.ToSharedRef());
	}
}

void FViewportMode::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(FeedTarget);
}

//}

/* OnWorldTickEnd() //{ */

// runs after every actor tick and before the viewport draws, paused frames included
void FViewportMode::OnWorldTickEnd(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
	UGameViewportClient* Viewport = GEngine ? GEngine->GameViewport : nullptr;

	if (!World || !World->IsGameWorld() || !World->Scene || !Viewport || Viewport->GetWorld() != World)
	{
		return;
	}

	const bool bOff = CVarNoViewport.GetValueOnGameThread() > 0;

	SetSkyTimeSlicing(bOff);
	UpdateFeed(World, bOff);

	// with the mode off the flag stays whatever UDisableViewportRender made it
	if (!bOff)
	{
		if (bWasOff)
		{
			Viewport->bDisableWorldRendering = false;
			bWasOff = false;
		}
		return;
	}
	bWasOff = true;

	const bool bDrawWorld = SkyRefreshDue(World);

	Viewport->bDisableWorldRendering = !bDrawWorld;

	if (!bDrawWorld)
	{
		RenderCaptures(World);
	}
}

//}

/* RenderCaptures() //{ */

// the frame number goes first: a readback job only matches a render with a newer scene frame
void FViewportMode::RenderCaptures(UWorld* World)
{
	World->SendAllEndOfFrameUpdates();
	World->Scene->IncrementFrameNumber();
	USceneCaptureComponent::UpdateDeferredCaptures(World->Scene);
}

//}

/* SkyRefreshDue() //{ */

// scene captures skip the real-time sky light capture, only the main view runs it
bool FViewportMode::SkyRefreshDue(UWorld* World)
{
	if (SkyWorld.Get() != World)
	{
		SkyWorld = World;
		RealTimeSkyLights.Reset();
		Suns.Reset();

		for (TObjectIterator<USkyLightComponent> It; It; ++It)
		{
			if (It->GetWorld() == World && It->bRealTimeCapture)
			{
				RealTimeSkyLights.Add(*It);
			}
		}

		for (TObjectIterator<UDirectionalLightComponent> It; It; ++It)
		{
			if (It->GetWorld() == World)
			{
				Suns.Add(*It);
			}
		}

		SkyHash       = 0;
		SkyFramesOwed = RealTimeSkyLights.IsEmpty() ? 0 : 2;

		UE_LOG(LogTemp, Display, TEXT("FlightForge: no viewport in %s, %d real-time sky lights recaptured by a viewport frame on sun changes"), *World->GetMapName(),
		       RealTimeSkyLights.Num());
	}

	if (RealTimeSkyLights.IsEmpty())
	{
		return false;
	}

	uint32 Hash = 0;
	for (const TWeakObjectPtr<UDirectionalLightComponent>& Sun : Suns)
	{
		if (const UDirectionalLightComponent* Light = Sun.Get())
		{
			const FVector Direction = Light->GetDirection();
			const FIntVector Quantized(FMath::RoundToInt(Direction.X * 2000.0), FMath::RoundToInt(Direction.Y * 2000.0), FMath::RoundToInt(Direction.Z * 2000.0));
			Hash = HashCombine(Hash, GetTypeHash(Quantized));
			Hash = HashCombine(Hash, GetTypeHash(FMath::RoundToInt(Light->Intensity * 100.0f)));
			Hash = HashCombine(Hash, GetTypeHash(Light->LightColor));
			Hash = HashCombine(Hash, GetTypeHash(Light->IsVisible()));
		}
	}
	for (const TWeakObjectPtr<USkyLightComponent>& Sky : RealTimeSkyLights)
	{
		if (const USkyLightComponent* Light = Sky.Get())
		{
			Hash = HashCombine(Hash, GetTypeHash(FMath::RoundToInt(Light->Intensity * 100.0f)));
		}
	}

	const double Now    = FPlatformTime::Seconds();
	const float  Period = CVarSkyRefresh.GetValueOnGameThread();

	if (Hash != SkyHash || (Period > 0.0f && Now - LastSkyRefresh > Period))
	{
		SkyHash       = Hash;
		SkyFramesOwed = FMath::Max(SkyFramesOwed, 1);
	}

	if (SkyFramesOwed <= 0)
	{
		return false;
	}

	SkyFramesOwed--;
	LastSkyRefresh = Now;
	return true;
}

// a sliced capture would need several viewport frames to finish
void FViewportMode::SetSkyTimeSlicing(const bool bOff)
{
	IConsoleVariable* Var = IConsoleManager::Get().FindConsoleVariable(TimeSliceVar);

	if (!Var || bOff == SavedTimeSlice.IsSet())
	{
		return;
	}

	if (bOff)
	{
		SavedTimeSlice = Var->GetInt();
		Var->Set(0, ECVF_SetByCode);
	}
	else
	{
		Var->Set(SavedTimeSlice.GetValue(), ECVF_SetByCode);
		SavedTimeSlice.Reset();
	}
}

//}

/* UpdateFeed() //{ */

void FViewportMode::UpdateFeed(UWorld* World, const bool bShow)
{
	UGameViewportClient* Viewport = GEngine->GameViewport;

	if (!bShow)
	{
		if (Feed.IsValid())
		{
			Viewport->RemoveViewportWidgetContent(Feed.ToSharedRef());
			Feed.Reset();
			FeedTarget = nullptr;
			FeedBrush.SetResourceObject(nullptr);
		}
		return;
	}

	if (!Feed.IsValid())
	{
		Feed = SNew(SOverlay)
		       + SOverlay::Slot()
		       [
		           SNew(SBorder).BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush")).BorderBackgroundColor(FLinearColor::Black)
		       ]
		       + SOverlay::Slot()
		       [
		           SNew(SScaleBox).Stretch(EStretch::ScaleToFit)
		           [
		               SNew(SImage).Image(&FeedBrush)
		           ]
		       ]
		       + SOverlay::Slot().HAlign(HAlign_Left).VAlign(VAlign_Bottom).Padding(8.0f)
		       [
		           SNew(STextBlock).Text_Lambda([this]() { return FText::FromString(FeedLabel); }).ColorAndOpacity(FLinearColor(0.8f, 0.8f, 0.8f))
		       ];
	}

	// a level load removes every viewport widget
	if (!Feed->GetParentWidget().IsValid())
	{
		Viewport->AddViewportWidgetContent(Feed.ToSharedRef(), -100);
	}

	const double Now = FPlatformTime::Seconds();
	if (Now < NextFeedPick)
	{
		return;
	}
	NextFeedPick = Now + 0.5;

	UCamera*                Camera = PickFeedCamera(World);
	UTextureRenderTarget2D* Target = Camera ? Camera->RenderTarget2D : nullptr;

	if (Target != FeedTarget)
	{
		FeedTarget = Target;
		FeedBrush.SetResourceObject(Target);
		FeedBrush.ImageSize = Target ? FVector2D(Target->SizeX, Target->SizeY) : FVector2D::ZeroVector;
	}

	if (!Camera)
	{
		FeedLabel = TEXT("no viewport, no camera to show (FlightForge.NoViewport 0 draws the world)");
		return;
	}

	const ARobotPawn* Robot = Cast<ARobotPawn>(Camera->GetOwner());
	FeedLabel = FString::Printf(TEXT("%s/%s%s"), Robot ? *Robot->RobotName : TEXT("?"), *Camera->Name.ToString(),
	                            Camera->camera_last_request_time_.load() > 0.0 ? TEXT("") : TEXT(", not read yet"));
}

UCamera* FViewportMode::PickFeedCamera(UWorld* World) const
{
	const FString Wanted = CVarViewportCamera.GetValueOnGameThread();

	UCamera* Best     = nullptr;
	double   BestRead = -1.0;

	for (TObjectIterator<UCamera> It; It; ++It)
	{
		UCamera* Camera = *It;
		if (Camera->GetWorld() != World || !Camera->RenderTarget2D || !IsValid(Camera))
		{
			continue;
		}

		if (!Wanted.IsEmpty())
		{
			const ARobotPawn* Robot = Cast<ARobotPawn>(Camera->GetOwner());
			const FString     Sensor = Camera->Name.ToString();
			if (Wanted == Sensor || (Robot && Wanted == Robot->RobotName + TEXT("/") + Sensor))
			{
				return Camera;
			}
			continue;
		}

		// an 8-bit target is a picture; depth and event targets are not
		const double Read = Camera->camera_last_request_time_.load() + (Camera->RenderTarget2D->RenderTargetFormat == RTF_RGBA8 ? 1.0e6 : 0.0);
		if (Read > BestRead)
		{
			Best     = Camera;
			BestRead = Read;
		}
	}

	return Best;
}

//}
