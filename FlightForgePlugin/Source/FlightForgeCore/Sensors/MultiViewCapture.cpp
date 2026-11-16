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

#include "LumenViewOrigins.h"
#include "MultiViewRender.h"

/* console variables //{ */

namespace
{

TAutoConsoleVariable<int32> CVarMultiView(TEXT("FlightForge.Camera.MultiView"), 1,
                                          TEXT("1: a multi-view camera (stereo, fisheye) renders its views as one view family; 0: one scene render per view"));

TAutoConsoleVariable<int32> CVarRobotBatch(TEXT("FlightForge.Camera.RobotBatch"), 1,
                                            TEXT("1: a robot's rgb cameras due on the same tick with matching settings render as one view family"));

TAutoConsoleVariable<int32> CVarViewportLook(TEXT("FlightForge.Camera.ViewportLook"), 1,
                                             TEXT("1: rgb, stereo and fisheye captures render with the main viewport's post-process chain, Lumen and screen percentage; 0: the engine's scene capture defaults"));

TAutoConsoleVariable<int32> CVarSharedExposure(TEXT("FlightForge.Camera.MultiView.SharedExposure"), 1,
                                               TEXT("1: a stereo pair's or fisheye's views take view 0's exposure; 0: each view meters its own"));

TAutoConsoleVariable<int32> CVarSharedLumenScene(TEXT("FlightForge.Camera.SharedLumenScene"), 1,
                                                 TEXT("1: every camera traces and updates the world's one Lumen scene, the viewport's; 0: each camera (or family) owns a Lumen scene"));

TAutoConsoleVariable<int32> CVarRobotOrigin(TEXT("FlightForge.Camera.RobotViewOrigin"), 1,
                                            TEXT("1: a robot's colour cameras share one view-state origin, so one global distance field; 0: one per camera"));

TAutoConsoleVariable<int32> CVarMultiViewShareLumen(TEXT("FlightForge.Camera.MultiView.ShareLumen"), 1,
                                                    TEXT("1: a family's views share one Lumen scene, any number of views; 0: families with Lumen split into pairs"));

constexpr int32 MaxAtlasWidth = 8192;

// LUMEN_MAX_VIEWS (5.7): a wider family renders without Lumen unless its views are cube-capture views
constexpr int32 MaxLumenViews = 2;

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

bool WantsLumen(const USceneCaptureComponent2D* Capture)
{
	const FPostProcessSettings& Settings = Capture->PostProcessSettings;
	if (Capture->PostProcessBlendWeight > 0.0f && (Settings.bOverride_DynamicGlobalIlluminationMethod || Settings.bOverride_ReflectionMethod))
	{
		return (Settings.bOverride_DynamicGlobalIlluminationMethod && Settings.DynamicGlobalIlluminationMethod == EDynamicGlobalIlluminationMethod::Lumen) ||
		       (Settings.bOverride_ReflectionMethod && Settings.ReflectionMethod == EReflectionMethod::Lumen);
	}
	if (!UMultiViewCapture::HasLook(Capture))
	{
		return false;
	}
	// the viewport's methods: the project's, unless a volume overrides them
	static const IConsoleVariable* GI         = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DynamicGlobalIlluminationMethod"));
	static const IConsoleVariable* Reflection = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ReflectionMethod"));
	return (GI && GI->GetInt() == 1) || (Reflection && Reflection->GetInt() == 1);
}

// show flags can carry padding bits, so a bytewise difference is only a hint
bool SameShowFlags(const FEngineShowFlags& A, const FEngineShowFlags& B)
{
	if (FMemory::Memcmp(&A, &B, sizeof(FEngineShowFlags)) == 0)
	{
		return true;
	}
	struct FSink
	{
		const FEngineShowFlags& A;
		const FEngineShowFlags& B;
		bool                    bSame = true;
		bool OnEngineShowFlag(const uint32 Index, const FString&)
		{
			bSame = A.GetSingleFlag(Index) == B.GetSingleFlag(Index);
			return bSame;
		}
		bool OnCustomShowFlag(const uint32 Index, const FString& Name) { return OnEngineShowFlag(Index, Name); }
	} Sink{A, B};
	FEngineShowFlags::IterateAllFlags(Sink);
	return Sink.bSame;
}

// eye adaptation and motion blur step by the world time since this view last rendered, as the viewport's do every frame
FGameTime LookTime(const UWorld& World, const FSceneViewStateInterface* State)
{
	static TMap<const FSceneViewStateInterface*, double> LastRender;
	const double Now   = World.GetTimeSeconds();
	double&      Last  = LastRender.FindOrAdd(State, Now - World.GetDeltaSeconds());
	const double Delta = FMath::Clamp(Now - Last, 0.0, 1.0);
	Last               = Now;
	return FGameTime::CreateDilated(World.GetRealTimeSeconds(), World.DeltaRealTimeSeconds, Now, Delta);
}

// a capture's first render comes before its target takes the configured size: the next frame must not blur against that projection
bool ProjectionChanged(const FSceneViewStateInterface* State, const FMatrix& Projection)
{
	static TMap<const FSceneViewStateInterface*, FMatrix> Last;
	FMatrix*   Previous = Last.Find(State);
	const bool bChanged = Previous && !Previous->Equals(Projection, 1e-6);
	Last.Add(State, Projection);
	return bChanged;
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

bool UMultiViewCapture::LookEnabled()
{
	return CVarViewportLook.GetValueOnGameThread() != 0;
}

bool UMultiViewCapture::HasLook(const USceneCaptureComponent2D* View)
{
	const ULookCapture* Look = Cast<ULookCapture>(View);
	return Look && Look->bViewportLook && LookEnabled();
}

bool UMultiViewCapture::BatchEnabled()
{
	return CVarRobotBatch.GetValueOnGameThread() != 0;
}

UMultiViewCapture* UMultiViewCapture::FindRobotBatch(const AActor* Robot)
{ //{
	static const FName BatchName(TEXT("FlightForgeRobotBatch"));
	if (Robot)
	{
		for (UActorComponent* Component : Robot->GetComponents())
		{
			if (Component->GetFName() == BatchName)
			{
				return Cast<UMultiViewCapture>(Component);
			}
		}
	}
	return nullptr;
} //}

UMultiViewCapture* UMultiViewCapture::RobotBatch(AActor* Robot)
{ //{
	if (!Robot)
	{
		return nullptr;
	}
	if (UMultiViewCapture* Batch = FindRobotBatch(Robot))
	{
		return Batch;
	}
	UMultiViewCapture* Batch = NewObject<UMultiViewCapture>(Robot, TEXT("FlightForgeRobotBatch"));
	Batch->SetupAttachment(Robot->GetRootComponent());
	Batch->RegisterComponent();
	return Batch;
} //}

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

FSceneViewStateInterface* UMultiViewCapture::FamilyState(USceneCaptureComponent2D* Capture)
{ //{
	if (!GetViewState(0))
	{
		return Capture->GetViewState(0);
	}
	TUniquePtr<FSceneViewStateReference>& State = FamilyStates.FindOrAdd(Capture);
	if (!State)
	{
		State = MakeUnique<FSceneViewStateReference>();
		State->ShareOrigin(&ViewStates[0]);
		State->Allocate(GetScene() ? GetScene()->GetFeatureLevel() : GMaxRHIFeatureLevel);
		Capture->bCameraCutThisFrame = true;
	}
	return State->GetReference();
} //}

FSceneViewStateInterface* UMultiViewCapture::RobotState(USceneCaptureComponent2D* Capture)
{ //{
	UMultiViewCapture* Batch = CVarRobotOrigin.GetValueOnGameThread() != 0 ? FindRobotBatch(Capture->GetOwner()) : nullptr;
	if (!Batch)
	{
		return Capture->GetViewState(0);
	}
	Batch->PruneFamilyStates();
	return Batch->FamilyState(Capture);
} //}

FSceneViewStateInterface* UMultiViewCapture::RobotAnchor(USceneCaptureComponent2D* Capture)
{ //{
	UMultiViewCapture* Batch = CVarRobotOrigin.GetValueOnGameThread() != 0 ? FindRobotBatch(Capture->GetOwner()) : nullptr;
	return Batch ? Batch->GetViewState(0) : nullptr;
} //}

void UMultiViewCapture::OnUnregister()
{ //{
	FamilyStates.Empty();
	Super::OnUnregister();
} //}

bool UMultiViewCapture::Groupable(const USceneCaptureComponent2D* View)
{
	return View && View->TextureTarget && !View->ShouldRenderInMainRenderer();
}

bool UMultiViewCapture::Compatible(const USceneCaptureComponent2D* A, const USceneCaptureComponent2D* B)
{
	const UTextureRenderTarget2D* TargetA = A->TextureTarget.Get();
	const UTextureRenderTarget2D* TargetB = B->TextureTarget.Get();
	return TargetA && TargetB && TargetA->GetFormat() == TargetB->GetFormat() && TargetA->GetDisplayGamma() == TargetB->GetDisplayGamma() && A->CaptureSource == B->CaptureSource && A->IsUnlit() == B->IsUnlit() &&
	       A->UnlitViewmode == B->UnlitViewmode && SameShowFlags(A->ShowFlags, B->ShowFlags);
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
	// a non-sRGB atlas told the members' display gamma: an sRGB one encodes LDR output a second time, about a stop too bright
	const float Gamma = First->GetDisplayGamma();
	if (Atlas->SizeX != Extent.X || Atlas->SizeY != Extent.Y || Atlas->GetFormat() != First->GetFormat() || !Atlas->bForceLinearGamma || Atlas->TargetGamma != Gamma)
	{
		Atlas->RenderTargetFormat = First->RenderTargetFormat;
		Atlas->TargetGamma        = Gamma;
		Atlas->ClearColor         = First->ClearColor;
		Atlas->InitCustomFormat(Extent.X, Extent.Y, First->GetFormat(), true);
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

		int32 Slot = 0;
		while (Remaining.Num() > 0)
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
			RenderFamilies(Scene, SceneRenderBuilder, Group, Slot);
		}
		return;
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
	int32 Slot = 0;
	RenderFamilies(Scene, SceneRenderBuilder, Members, Slot);
}

void UMultiViewCapture::PruneFamilyStates()
{ //{
	for (auto It = FamilyStates.CreateIterator(); It; ++It)
	{
		if (!It.Key().IsValid())
		{
			It.RemoveCurrent();
		}
	}
} //}

void UMultiViewCapture::RenderFamilies(FSceneInterface* Scene, ISceneRenderBuilder& SceneRenderBuilder, TArrayView<USceneCaptureComponent2D* const> Members,
                                       int32& Slot)
{ //{
	PruneFamilyStates();
	const bool  bLumen  = Members.ContainsByPredicate([](const USceneCaptureComponent2D* View) { return View && WantsLumen(View); });
	const bool  bShared = bLumen && CVarMultiViewShareLumen.GetValueOnGameThread() != 0;
	const int32 Size    = bLumen && !bShared ? MaxLumenViews : FMath::Max(Members.Num(), 1);
	for (int32 First = 0; First < Members.Num(); First += Size)
	{
		RenderFamily(Scene, SceneRenderBuilder, Members.Slice(First, FMath::Min(Size, Members.Num() - First)), Slot++, bShared);
	}
} //}

void UMultiViewCapture::RenderFamily(FSceneInterface* Scene, ISceneRenderBuilder& SceneRenderBuilder, TArrayView<USceneCaptureComponent2D* const> Members,
                                     const int32 Slot, const bool bSharedLumen)
{ //{
	FFamilySetup Setup;
	if (Members.Num() < 2 || !PrepareAtlas(Members, Slot, Setup.Tiles))
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

	Setup.Target          = Atlases[Slot]->GameThread_GetRenderTargetResource();
	Setup.bSharedExposure = bSharedExposure && CVarSharedExposure.GetValueOnGameThread() != 0;
	Setup.bCube           = bSharedLumen && Members.Num() > MaxLumenViews;
	if (FSceneViewStateInterface* Anchor = RobotAnchor(Members[0]))
	{
		Setup.LumenAnchor = Anchor;
		Setup.StateFor    = &RobotState;
	}
	else
	{
		Setup.LumenAnchor = bSharedLumen ? GetViewState(0) : nullptr;
		Setup.StateFor    = [this, bSharedLumen](USceneCaptureComponent2D* Capture) { return bSharedLumen ? FamilyState(Capture) : Capture->GetViewState(0); };
	}
	Setup.Name            = GetOwner() ? FString::Printf(TEXT("FlightForgeMultiView %s"), *GetOwner()->GetActorNameOrLabel()) : FString(TEXT("FlightForgeMultiView"));
	RenderViews(Scene, SceneRenderBuilder, Members, MoveTemp(Setup));
} //}

void UMultiViewCapture::RenderSingle(USceneCaptureComponent2D* View, FSceneInterface* Scene, ISceneRenderBuilder& SceneRenderBuilder)
{ //{
	UTextureRenderTarget2D*       Texture = View ? View->TextureTarget.Get() : nullptr;
	FTextureRenderTargetResource* Target  = Texture ? Texture->GameThread_GetRenderTargetResource() : nullptr;
	if (!Target)
	{
		return;
	}
	FFamilySetup Setup;
	Setup.Target      = Target;
	Setup.Tiles       = {FIntRect(0, 0, Texture->SizeX, Texture->SizeY)};
	Setup.bCopyTiles  = false;
	Setup.StateFor    = &RobotState;
	Setup.LumenAnchor = RobotAnchor(View);
	Setup.Name        = View->GetOwner() ? FString::Printf(TEXT("FlightForgeLook %s"), *View->GetOwner()->GetActorNameOrLabel()) : FString(TEXT("FlightForgeLook"));
	RenderViews(Scene, SceneRenderBuilder, MakeArrayView(&View, 1), MoveTemp(Setup));
} //}

void UMultiViewCapture::RenderViews(FSceneInterface* Scene, ISceneRenderBuilder& SceneRenderBuilder, TArrayView<USceneCaptureComponent2D* const> Members,
                                    FFamilySetup&& Setup)
{ //{
	FLumenViewOrigins::Register();

	const USceneCaptureComponent2D* Lead        = Members[0];
	const bool                      bLook       = HasLook(Lead);
	const bool                      bSceneColor = Lead->CaptureSource != SCS_FinalColorLDR && Lead->CaptureSource != SCS_FinalColorHDR && Lead->CaptureSource != SCS_FinalToneCurveHDR;

	FSceneViewFamilyContext Family(FSceneViewFamily::ConstructionValues(Setup.Target, Scene, Lead->ShowFlags).SetResolveScene(!bSceneColor).SetRealtimeUpdate(true));

	FSceneViewExtensionContext ExtensionContext(Scene);
	Family.ViewExtensions = GEngine->ViewExtensions->GatherActiveExtensions(ExtensionContext);
	Family.FrameNumber    = Scene->GetFrameNumber();
	Family.FrameCounter   = GFrameCounter;

	const UWorld* World = Scene->GetWorld();

	TArray<FSceneView*>       FamilyViews;
	TArray<FMultiViewTile>    TileTargets;
	FSceneViewStateInterface* ExposureOwner = nullptr;
	if (bLook && World)
	{
		Family.Time = LookTime(*World, Setup.StateFor(Members[0]));
		// lockstep pauses the world between steps; a paused family freezes the view's previous frame, TSR and Lumen histories
		Family.bWorldIsPaused = false;
	}
	for (int32 Index = 0; Index < Members.Num(); ++Index)
	{
		USceneCaptureComponent2D* Capture = Members[Index];
		const FIntRect            Rect    = Setup.Tiles[Index];
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
		Options.SceneViewStateInterface          = Setup.StateFor(Capture);
		Options.ExposureSceneViewStateInterface  = Setup.bSharedExposure && ExposureOwner ? ExposureOwner : Options.SceneViewStateInterface;
		ExposureOwner                            = ExposureOwner ? ExposureOwner : Options.SceneViewStateInterface;
		// the engine's one-origin Lumen path for wide families; its only other effects are no DFAO history and no water
		Options.bIsSceneCaptureCube              = Setup.bCube;
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

		// the viewport's chain: project defaults, then the volumes at the view, then the camera's own settings
		View->StartFinalPostprocessSettings(Origin);
		if (!bLook)
		{
			View->FinalPostProcessSettings.DynamicGlobalIlluminationMethod = EDynamicGlobalIlluminationMethod::None;
			View->FinalPostProcessSettings.ReflectionMethod                = EReflectionMethod::None;
			View->FinalPostProcessSettings.LumenSurfaceCacheResolution     = 0.5f;
		}
		View->OverridePostProcessSettings(Capture->PostProcessSettings, Capture->PostProcessBlendWeight);
		View->EndFinalPostprocessSettings(Options);
		View->ViewLightingChannelMask = Capture->ViewLightingChannels.GetMaskForStruct();
		View->bCameraCut              = Capture->bCameraCutThisFrame || ProjectionChanged(Options.SceneViewStateInterface, Options.ProjectionMatrix);
		Capture->bCameraCutThisFrame  = false;
		if (Capture->bEnableClipPlane)
		{
			View->GlobalClippingPlane   = FPlane(Capture->ClipPlaneBase, Capture->ClipPlaneNormal.GetSafeNormal());
			View->bAllowTemporalJitter  = false;
		}

		if (FSceneViewStateInterface* State = Setup.LumenAnchor ? Setup.LumenAnchor : Options.SceneViewStateInterface)
		{
			const FFinalPostProcessSettings& Settings = View->FinalPostProcessSettings;
			const bool bLumen = Settings.DynamicGlobalIlluminationMethod == EDynamicGlobalIlluminationMethod::Lumen || Settings.ReflectionMethod == EReflectionMethod::Lumen;
			// a view state without its own Lumen scene resolves to the scene's default one (FScene::FindLumenSceneData)
			if (bLumen && CVarSharedLumenScene.GetValueOnGameThread() == 0)
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

		if (Setup.bCopyTiles)
		{
			TileTargets.Add(FMultiViewTile{Rect, Capture->TextureTarget->GameThread_GetRenderTargetResource()});
		}
	}

	Family.SceneCaptureSource = Lead->CaptureSource;
	if (bLook && !bSceneColor)
	{
		// the viewport's screen percentage, as for a viewport of one view's size
		Family.EngineShowFlags.ScreenPercentage = true;
		FStaticResolutionFractionHeuristic Heuristic;
		Heuristic.Settings.PullRunTimeRenderingSettings(EViewStatusForScreenPercentage::Desktop);
		Heuristic.PullViewFamilyRenderingSettings(Family);
		Heuristic.TotalDisplayedPixelCount = Setup.Tiles[0].Area();
		Family.SetScreenPercentageInterface(new FLegacyScreenPercentageDriver(Family, Heuristic.ResolveResolutionFraction()));
	}
	else
	{
		Family.EngineShowFlags.ScreenPercentage = false;
		Family.SetScreenPercentageInterface(new FLegacyScreenPercentageDriver(Family, 1.0f));
	}
	if (Lead->IsUnlit())
	{
		Family.EngineShowFlags.DisableFeaturesForUnlit(Lead->CaptureSource == SCS_SceneColorHDR || Lead->CaptureSource == SCS_SceneColorHDRNoAlpha ||
		                                               Lead->CaptureSource == SCS_SceneColorSceneDepth);
	}
	Family.EngineShowFlags.SetUnlitViewmode(Lead->UnlitViewmode != ESceneCaptureUnlitViewmode::Disabled);

	if (Lead->ShowFlags.TemporalAA && Setup.bCopyTiles)
	{
		Family.ViewExtensions.Add(FMultiViewRender::OutputExtension());
	}
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

	FMultiViewRender::Add(SceneRenderBuilder, Family, MoveTemp(Setup.Name), Setup.Target, MoveTemp(TileTargets));
} //}

/* //} */

/* ULookCapture //{ */

void ULookCapture::UpdateSceneCaptureContents(FSceneInterface* Scene, ISceneRenderBuilder& SceneRenderBuilder)
{ //{
	if (UMultiViewCapture::HasLook(this) && TextureTarget && !ShouldRenderInMainRenderer())
	{
		UMultiViewCapture::RenderSingle(this, Scene, SceneRenderBuilder);
		return;
	}
	Super::UpdateSceneCaptureContents(Scene, SceneRenderBuilder);
} //}

/* //} */
