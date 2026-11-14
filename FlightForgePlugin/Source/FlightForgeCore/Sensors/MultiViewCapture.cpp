#include "MultiViewCapture.h"

#include "Camera/CameraTypes.h"
#include "Engine/Engine.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/World.h"
#include "GameFramework/WorldSettings.h"
#include "HAL/IConsoleManager.h"
#include "LegacyScreenPercentageDriver.h"
#include "SceneInterface.h"
#include "SceneRenderBuilderInterface.h"
#include "SceneView.h"
#include "SceneViewExtension.h"
#include "TextureResource.h"

#include "MultiViewRender.h"

/* console variables //{ */

namespace
{

TAutoConsoleVariable<int32> CVarMultiView(TEXT("FlightForge.Camera.MultiView"), 1,
                                          TEXT("1: a multi-view camera (stereo, fisheye) renders its views as one view family; 0: one scene render per view"));

TAutoConsoleVariable<int32> CVarRobotBatch(TEXT("FlightForge.Camera.RobotBatch"), 1,
                                            TEXT("1: a robot's rgb cameras due on the same tick with matching settings render as one view family"));

TAutoConsoleVariable<int32> CVarMultiViewShareLumen(TEXT("FlightForge.Camera.MultiView.ShareLumen"), 0,
                                                    TEXT("1: the views of a family share one Lumen scene (surface cache), as cube captures do"));

constexpr int32 MaxAtlasWidth = 8192;

}  // namespace

//}

/* view setup, after the engine's SceneCaptureRendering.cpp //{ */

namespace
{

FMatrix CaptureViewRotation(const USceneCaptureComponent2D* Capture)
{
	FTransform Transform = Capture->GetComponentToWorld();
	Transform.SetTranslation(FVector::ZeroVector);
	Transform.SetScale3D(FVector::OneVector);
	return Transform.ToInverseMatrixWithScale() * FMatrix(FPlane(0, 0, 1, 0), FPlane(1, 0, 0, 0), FPlane(0, 1, 0, 0), FPlane(0, 0, 0, 1));
}

FMatrix CaptureProjection(const USceneCaptureComponent2D* Capture, const FIntPoint Size)
{
	if (Capture->bUseCustomProjectionMatrix)
	{
		return Capture->CustomProjectionMatrix;
	}
	const float UnscaledFOV = Capture->FOVAngle * (float)PI / 360.0f;
	const float FOV         = FMath::Atan((1.0f + Capture->Overscan) * FMath::Tan(UnscaledFOV));
	const float Near        = Capture->bOverride_CustomNearClippingPlane ? Capture->CustomNearClippingPlane : GNearClippingPlane;
	return FReversedZPerspectiveMatrix(FOV, FOV, 1.0f, Size.X / float(Size.Y), Near, Near);
}

void HiddenAndShowOnly(const USceneCaptureComponent2D* Capture, FSceneView& View)
{
	for (const TWeakObjectPtr<UPrimitiveComponent>& Weak : Capture->HiddenComponents)
	{
		if (const UPrimitiveComponent* Primitive = Weak.Get())
		{
			View.HiddenPrimitives.Add(Primitive->GetPrimitiveSceneId());
		}
	}
	for (const AActor* Actor : Capture->HiddenActors)
	{
		if (Actor)
		{
			for (const UActorComponent* Component : Actor->GetComponents())
			{
				if (const UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(Component))
				{
					View.HiddenPrimitives.Add(Primitive->GetPrimitiveSceneId());
				}
			}
		}
	}
	if (Capture->PrimitiveRenderMode != ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList)
	{
		return;
	}
	View.ShowOnlyPrimitives.Emplace();
	for (const TWeakObjectPtr<UPrimitiveComponent>& Weak : Capture->ShowOnlyComponents)
	{
		if (const UPrimitiveComponent* Primitive = Weak.Get())
		{
			View.ShowOnlyPrimitives->Add(Primitive->GetPrimitiveSceneId());
		}
	}
	for (const AActor* Actor : Capture->ShowOnlyActors)
	{
		if (Actor)
		{
			for (const UActorComponent* Component : Actor->GetComponents())
			{
				if (const UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(Component))
				{
					View.ShowOnlyPrimitives->Add(Primitive->GetPrimitiveSceneId());
				}
			}
		}
	}
}

bool UsesRayTracing(const USceneCaptureComponent2D* Capture)
{
	static const IConsoleVariable* SceneCaptures = IConsoleManager::Get().FindConsoleVariable(TEXT("r.RayTracing.SceneCaptures"));
	const int32 Mode = SceneCaptures ? SceneCaptures->GetInt() : -1;
	return Mode == -1 ? Capture->bUseRayTracingIfEnabled : Mode > 0;
}

}  // namespace

//}

/* UMultiViewCapture //{ */

UMultiViewCapture::UMultiViewCapture()
{
	bCaptureEveryFrame           = false;
	bCaptureOnMovement           = false;
	bAlwaysPersistRenderingState = true;
	PrimaryComponentTick.bCanEverTick = true;
}

bool UMultiViewCapture::Enabled()
{
	return CVarMultiView.GetValueOnGameThread() != 0;
}

void UMultiViewCapture::SetViews(TArrayView<USceneCaptureComponent2D* const> InViews)
{
	Views.Reset();
	Views.Append(InViews.GetData(), InViews.Num());
	ApplyEveryFrame();
}

void UMultiViewCapture::Capture(const bool bDeferred)
{
	if (Enabled())
	{
		bDeferred ? CaptureSceneDeferred() : CaptureScene();
		return;
	}
	for (USceneCaptureComponent2D* View : Views)
	{
		bDeferred ? View->CaptureSceneDeferred() : View->CaptureScene();
	}
}

bool UMultiViewCapture::BatchEnabled()
{
	return CVarRobotBatch.GetValueOnGameThread() != 0;
}

UMultiViewCapture* UMultiViewCapture::RobotBatch(AActor* Robot)
{
	if (!Robot)
	{
		return nullptr;
	}
	static const FName BatchName(TEXT("FlightForgeRobotBatch"));
	for (UActorComponent* Component : Robot->GetComponents())
	{
		if (Component->GetFName() == BatchName)
		{
			return Cast<UMultiViewCapture>(Component);
		}
	}
	UMultiViewCapture* Batch = NewObject<UMultiViewCapture>(Robot, BatchName);
	Batch->SetupAttachment(Robot->GetRootComponent());
	Batch->RegisterComponent();
	return Batch;
}

void UMultiViewCapture::Defer(USceneCaptureComponent2D* View)
{
	Pending.AddUnique(View);
	CaptureSceneDeferred();
}

void UMultiViewCapture::SetCaptureEveryFrame(const bool bEnable)
{
	bEveryFrame = bEnable;
	ApplyEveryFrame();
}

void UMultiViewCapture::ApplyEveryFrame()
{
	bAppliedGroup      = Enabled();
	bCaptureEveryFrame = bAppliedGroup && bEveryFrame;
	for (USceneCaptureComponent2D* View : Views)
	{
		View->bCaptureEveryFrame = !bAppliedGroup && bEveryFrame;
	}
}

void UMultiViewCapture::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (Enabled() != bAppliedGroup)
	{
		ApplyEveryFrame();
	}
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMultiViewCapture::ResetViewStates()
{
	for (int32 Index = ViewStates.Num() - 1; Index >= 0; --Index)
	{
		ViewStates[Index].Destroy();
	}
	ViewStates.Empty();

	for (int32 Index = 0; Index < Views.Num(); ++Index)
	{
		ViewStates.Add(new FSceneViewStateReference());
		if (Index > 0 && SharedLumen)
		{
			ViewStates.Last().ShareOrigin(&ViewStates[0]);
		}
	}
}

// TSR leaves every view after the first black in a multi-view capture family (5.7)
bool UMultiViewCapture::Groupable(const USceneCaptureComponent2D* View)
{
	static const IConsoleVariable* Method = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AntiAliasingMethod"));
	return View && View->TextureTarget && !View->ShouldRenderInMainRenderer() && !(View->ShowFlags.TemporalAA && Method && Method->GetInt() == AAM_TSR);
}

bool UMultiViewCapture::Compatible(const USceneCaptureComponent2D* A, const USceneCaptureComponent2D* B)
{
	const UTextureRenderTarget2D* TargetA = A->TextureTarget.Get();
	const UTextureRenderTarget2D* TargetB = B->TextureTarget.Get();
	return TargetA && TargetB && TargetA->GetFormat() == TargetB->GetFormat() && TargetA->bForceLinearGamma == TargetB->bForceLinearGamma &&
	       TargetA->TargetGamma == TargetB->TargetGamma && A->CaptureSource == B->CaptureSource && A->IsUnlit() == B->IsUnlit() &&
	       A->UnlitViewmode == B->UnlitViewmode && FMemory::Memcmp(&A->ShowFlags, &B->ShowFlags, sizeof(FEngineShowFlags)) == 0;
}

bool UMultiViewCapture::PrepareAtlas(TArrayView<USceneCaptureComponent2D* const> Members, const int32 Slot, TArray<FIntRect>& OutTiles)
{
	if (Members.Num() == 0 || !Members[0] || !Members[0]->TextureTarget)
	{
		return false;
	}
	const UTextureRenderTarget2D* First = Members[0]->TextureTarget;

	FIntPoint Cursor(0, 0);
	FIntPoint Extent(0, 0);
	int32     RowHeight = 0;
	OutTiles.Reset();
	for (USceneCaptureComponent2D* View : Members)
	{
		UTextureRenderTarget2D* Target = View ? View->TextureTarget.Get() : nullptr;
		if (!Target || !Target->GameThread_GetRenderTargetResource() || !Groupable(View) || !Compatible(View, Members[0]))
		{
			return false;
		}
		const FIntPoint Size(Target->SizeX, Target->SizeY);
		if (Cursor.X > 0 && Cursor.X + Size.X > MaxAtlasWidth)
		{
			Cursor    = FIntPoint(0, Cursor.Y + RowHeight);
			RowHeight = 0;
		}
		OutTiles.Add(FIntRect(Cursor, Cursor + Size));
		Cursor.X += Size.X;
		RowHeight = FMath::Max(RowHeight, Size.Y);
		Extent    = Extent.ComponentMax(Cursor + FIntPoint(0, Size.Y));
	}

	while (Atlases.Num() <= Slot)
	{
		Atlases.Add(NewObject<UTextureRenderTarget2D>(this));
	}
	UTextureRenderTarget2D* Atlas = Atlases[Slot];
	if (Atlas->SizeX != Extent.X || Atlas->SizeY != Extent.Y || Atlas->GetFormat() != First->GetFormat() || Atlas->bForceLinearGamma != First->bForceLinearGamma ||
	    Atlas->TargetGamma != First->TargetGamma)
	{
		Atlas->RenderTargetFormat = First->RenderTargetFormat;
		Atlas->TargetGamma        = First->TargetGamma;
		Atlas->ClearColor         = First->ClearColor;
		Atlas->InitCustomFormat(Extent.X, Extent.Y, First->GetFormat(), First->bForceLinearGamma);
	}
	return Atlas->GameThread_GetRenderTargetResource() != nullptr;
}

void UMultiViewCapture::UpdateSceneCaptureContents(FSceneInterface* Scene, ISceneRenderBuilder& SceneRenderBuilder)
{
	if (Pending.Num() > 0)
	{
		TArray<USceneCaptureComponent2D*> Remaining;
		for (const TWeakObjectPtr<USceneCaptureComponent2D>& Weak : Pending)
		{
			if (USceneCaptureComponent2D* View = Weak.Get())
			{
				Remaining.Add(View);
			}
		}
		Pending.Reset();

		for (int32 Slot = 0; Remaining.Num() > 0; ++Slot)
		{
			const USceneCaptureComponent2D*   Key = Remaining[0];
			TArray<USceneCaptureComponent2D*> Group;
			for (int32 Index = Remaining.Num() - 1; Index >= 0; --Index)
			{
				if (Remaining[Index] == Key || (Groupable(Key) && Groupable(Remaining[Index]) && Compatible(Remaining[Index], Key)))
				{
					Group.Insert(Remaining[Index], 0);
					Remaining.RemoveAt(Index);
				}
			}
			RenderFamily(Scene, SceneRenderBuilder, Group, Slot, false);
		}
		return;
	}

	const int32 Share = CVarMultiViewShareLumen.GetValueOnGameThread();
	const bool  bReset = Share && (ViewStates.Num() != Views.Num() || SharedLumen != Share);
	SharedLumen        = Share;
	if (bReset)
	{
		ResetViewStates();
	}

	TArray<USceneCaptureComponent2D*> Members;
	for (USceneCaptureComponent2D* View : Views)
	{
		Members.Add(View);
	}
	if (!Enabled())
	{
		for (USceneCaptureComponent2D* View : Members)
		{
			if (View)
			{
				View->UpdateSceneCaptureContents(Scene, SceneRenderBuilder);
			}
		}
		return;
	}
	RenderFamily(Scene, SceneRenderBuilder, Members, 0, SharedLumen != 0);
}

void UMultiViewCapture::RenderFamily(FSceneInterface* Scene, ISceneRenderBuilder& SceneRenderBuilder, TArrayView<USceneCaptureComponent2D* const> Members,
                                     const int32 Slot, const bool bOwnStates)
{
	TArray<FIntRect> Tiles;
	if (Members.Num() < 2 || !PrepareAtlas(Members, Slot, Tiles))
	{
		for (USceneCaptureComponent2D* View : Members)
		{
			if (View)
			{
				View->UpdateSceneCaptureContents(Scene, SceneRenderBuilder);
			}
		}
		return;
	}

	FTextureRenderTargetResource* AtlasResource = Atlases[Slot]->GameThread_GetRenderTargetResource();
	const USceneCaptureComponent2D* Lead = Members[0];
	const bool bSceneColor = Lead->CaptureSource != SCS_FinalColorLDR && Lead->CaptureSource != SCS_FinalColorHDR && Lead->CaptureSource != SCS_FinalToneCurveHDR;

	FSceneViewFamilyContext       Family(FSceneViewFamily::ConstructionValues(AtlasResource, Scene, Lead->ShowFlags).SetResolveScene(!bSceneColor).SetRealtimeUpdate(true));

	FSceneViewExtensionContext ExtensionContext(Scene);
	Family.ViewExtensions = GEngine->ViewExtensions->GatherActiveExtensions(ExtensionContext);
	Family.FrameNumber    = Scene->GetFrameNumber();
	Family.FrameCounter   = GFrameCounter;

	const UWorld* World = Scene->GetWorld();

	TArray<FSceneView*>     FamilyViews;
	TArray<FMultiViewTile>  TileTargets;
	for (int32 Index = 0; Index < Members.Num(); ++Index)
	{
		USceneCaptureComponent2D* Capture = Members[Index];
		const FIntRect            Rect    = Tiles[Index];
		const FVector             Origin  = Capture->GetComponentLocation();

		FSceneViewInitOptions Options;
		Options.SetViewRectangle(Rect);
		Options.ViewFamily                       = &Family;
		Options.ViewActor                        = Capture->GetViewOwner();
		Options.ViewLocation                     = Origin;
		Options.ViewOrigin                       = Origin;
		Options.ViewRotationMatrix               = CaptureViewRotation(Capture);
		Options.ProjectionMatrix                 = CaptureProjection(Capture, Rect.Size());
		Options.BackgroundColor                  = FLinearColor::Black;
		Options.OverrideFarClippingPlaneDistance = Capture->MaxViewDistanceOverride;
		Options.StereoPass                       = EStereoscopicPass::eSSP_FULL;
		Options.StereoViewIndex                  = INDEX_NONE;
		Options.bIsSceneCapture                  = true;
		Options.FOV                              = Capture->FOVAngle;
		Options.DesiredFOV                       = Capture->FOVAngle;
		// a member's own state keeps its TAA and exposure history across switches
		Options.SceneViewStateInterface          = bOwnStates ? GetViewState(Index) : Capture->GetViewState(0);
		Options.LODDistanceFactor                = FMath::Clamp(Capture->LODDistanceFactor, .01f, 100.0f);
		Options.bSceneCaptureUsesRayTracing      = UsesRayTracing(Capture);
		Options.bExcludeFromSceneTextureExtents  = Capture->bExcludeFromSceneTextureExtents;
		if (World && World->GetWorldSettings())
		{
			Options.WorldToMetersScale = World->GetWorldSettings()->WorldToMeters;
		}
		if (bSceneColor)
		{
			Family.EngineShowFlags.PostProcessing = 0;
			Options.OverlayColor                  = FLinearColor::Black;
		}
		FMinimalViewInfo CameraView;
		Capture->GetCameraView(0.0f, CameraView);
		Options.FirstPersonParams = FFirstPersonParameters(CameraView.CalculateFirstPersonFOVCorrectionFactor(), CameraView.FirstPersonScale, CameraView.bUseFirstPersonParameters);

		FSceneView* View = new FSceneView(Options);
		HiddenAndShowOnly(Capture, *View);
		Family.Views.Add(View);
		FamilyViews.Add(View);

		View->StartFinalPostprocessSettings(Origin);
		View->FinalPostProcessSettings.DynamicGlobalIlluminationMethod = EDynamicGlobalIlluminationMethod::None;
		View->FinalPostProcessSettings.ReflectionMethod                = EReflectionMethod::None;
		View->FinalPostProcessSettings.LumenSurfaceCacheResolution     = 0.5f;
		View->OverridePostProcessSettings(Capture->PostProcessSettings, Capture->PostProcessBlendWeight);
		View->EndFinalPostprocessSettings(Options);
		View->ViewLightingChannelMask = Capture->ViewLightingChannels.GetMaskForStruct();
		View->bCameraCut              = Capture->bCameraCutThisFrame;
		Capture->bCameraCutThisFrame  = false;
		if (Capture->bEnableClipPlane)
		{
			View->GlobalClippingPlane   = FPlane(Capture->ClipPlaneBase, Capture->ClipPlaneNormal.GetSafeNormal());
			View->bAllowTemporalJitter  = false;
		}

		if (FSceneViewStateInterface* State = Options.SceneViewStateInterface)
		{
			const FFinalPostProcessSettings& Settings = View->FinalPostProcessSettings;
			if (Settings.DynamicGlobalIlluminationMethod == EDynamicGlobalIlluminationMethod::Lumen || Settings.ReflectionMethod == EReflectionMethod::Lumen)
			{
				State->AddLumenSceneData(Scene, Settings.LumenSurfaceCacheResolution);
			}
			else
			{
				State->RemoveLumenSceneData(Scene);
			}
		}

		for (int32 Ext = 0; Ext < Capture->SceneViewExtensions.Num(); ++Ext)
		{
			const TSharedPtr<ISceneViewExtension, ESPMode::ThreadSafe> Extension = Capture->SceneViewExtensions[Ext].Pin();
			if (Extension.IsValid() && Extension->IsActiveThisFrame(ExtensionContext) && !Family.ViewExtensions.Contains(Extension.ToSharedRef()))
			{
				Family.ViewExtensions.Add(Extension.ToSharedRef());
			}
		}

		TileTargets.Add(FMultiViewTile{Rect, Capture->TextureTarget->GameThread_GetRenderTargetResource()});
	}

	Family.SceneCaptureSource              = Lead->CaptureSource;
	Family.EngineShowFlags.ScreenPercentage = false;
	Family.SetScreenPercentageInterface(new FLegacyScreenPercentageDriver(Family, 1.0f));
	if (Lead->IsUnlit())
	{
		Family.EngineShowFlags.DisableFeaturesForUnlit(Lead->CaptureSource == SCS_SceneColorHDR || Lead->CaptureSource == SCS_SceneColorHDRNoAlpha ||
		                                               Lead->CaptureSource == SCS_SceneColorSceneDepth);
	}
	Family.EngineShowFlags.SetUnlitViewmode(Lead->UnlitViewmode != ESceneCaptureUnlitViewmode::Disabled);

	for (const FSceneViewExtensionRef& Extension : Family.ViewExtensions)
	{
		Extension->SetupViewFamily(Family);
	}
	for (FSceneView* View : FamilyViews)
	{
		for (const FSceneViewExtensionRef& Extension : Family.ViewExtensions)
		{
			Extension->SetupView(Family, *View);
		}
	}

	FString Name = GetOwner() ? FString::Printf(TEXT("FlightForgeMultiView %s"), *GetOwner()->GetActorNameOrLabel()) : FString(TEXT("FlightForgeMultiView"));
	FMultiViewRender::Add(SceneRenderBuilder, Family, MoveTemp(Name), AtlasResource, MoveTemp(TileTargets));
}

/* //} */
