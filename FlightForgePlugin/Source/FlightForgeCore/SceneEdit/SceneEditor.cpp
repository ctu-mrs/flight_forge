#include "SceneEditor.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "Framework/Application/IInputProcessor.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/Layout/SBox.h"

#include "FlightForgeCore/FlightForgeGameModeBase.h"
#include "SObjectBrowser.h"

#define LOCTEXT_NAMESPACE "SceneEditor"

/* FSceneEditInput //{ */

// ahead of the UI, and it keeps ticking while lockstep pauses the world
class FSceneEditInput : public IInputProcessor
{
public:
  explicit FSceneEditInput(USceneEditor* InEditor) : Editor(InEditor) {}

  virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
  {
    if (USceneEditor* Pinned = Editor.Get())
    {
      Pinned->Update();
    }
  }

  virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& Event) override
  {
    USceneEditor* Pinned = Editor.Get();
    return Pinned && Pinned->HandleKeyDown(Event);
  }

  virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& Event) override
  {
    USceneEditor* Pinned = Editor.Get();
    return Pinned && Pinned->HandleMouseDown(Event);
  }

  virtual bool HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp, const FPointerEvent& Event, const FPointerEvent* Gesture) override
  {
    USceneEditor* Pinned = Editor.Get();
    return Pinned && !Gesture && Pinned->HandleWheel(Event);
  }

  virtual const TCHAR* GetDebugName() const override
  {
    return TEXT("FlightForgeSceneEditor");
  }

private:
  TWeakObjectPtr<USceneEditor> Editor;
};

/* //} */

namespace
{

constexpr double YawStepDeg = 15.0;

}  // namespace

/* lifetime //{ */

bool USceneEditor::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
  return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void USceneEditor::OnWorldBeginPlay(UWorld& InWorld)
{
  Super::OnWorldBeginPlay(InWorld);

  if (FSlateApplication::IsInitialized() && InWorld.GetGameViewport())
  {
    Input = MakeShared<FSceneEditInput>(this);
    FSlateApplication::Get().RegisterInputPreProcessor(Input);
  }
}

void USceneEditor::Deinitialize()
{
  Close();

  if (Input && FSlateApplication::IsInitialized())
  {
    FSlateApplication::Get().UnregisterInputPreProcessor(Input);
  }

  Input.Reset();

  Super::Deinitialize();
}

/* //} */

/* Toggle() //{ */

void USceneEditor::Toggle()
{
  if (IsOpen())
  {
    Close();
    return;
  }

  UGameViewportClient* Viewport = GetWorld()->GetGameViewport();
  APlayerController*   Player   = GetPlayer();
  if (!Viewport || !Player || !GetGameMode())
  {
    return;
  }

  TWeakObjectPtr<USceneEditor> Weak(this);

  Panel = SNew(SBox)
      .Visibility(EVisibility::SelfHitTestInvisible)
      .HAlign(HAlign_Left)
      .Padding(12.0f)
      [
        SNew(SBox)
        .WidthOverride(436.0f)
        [
          SAssignNew(Browser, SObjectBrowser)
          .Library([Weak]() -> const FObjectLibrary* {
            AFlightForgeGameModeBase* GameMode = Weak.IsValid() ? Weak->GetGameMode() : nullptr;
            return GameMode ? &GameMode->GetObjectLibrary() : nullptr;
          })
          .OnPicked_Lambda([Weak](const FLibraryObject& Object) {
            if (Weak.IsValid())
            {
              Weak->BeginPlacing(Object);
            }
          })
        ]
      ];

  Viewport->AddViewportWidgetContent(Panel.ToSharedRef(), 100);

  FInputModeGameAndUI Mode;
  Mode.SetWidgetToFocus(Browser->GetSearchBox());
  Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
  Mode.SetHideCursorDuringCapture(true);

  SavedCaptureMode = Viewport->GetMouseCaptureMode();
  Player->SetInputMode(Mode);
  Player->SetShowMouseCursor(true);
  Viewport->SetMouseCaptureMode(EMouseCaptureMode::CaptureDuringRightMouseDown);
}

/* //} */

/* Close() //{ */

void USceneEditor::Close()
{
  EndPlacing(false);

  if (!Panel)
  {
    return;
  }

  UWorld* World = GetWorld();
  if (UGameViewportClient* Viewport = World ? World->GetGameViewport() : nullptr)
  {
    Viewport->RemoveViewportWidgetContent(Panel.ToSharedRef());

    if (APlayerController* Player = GetPlayer())
    {
      Player->SetInputMode(FInputModeGameOnly());
      Player->SetShowMouseCursor(false);
    }

    Viewport->SetMouseCaptureMode(SavedCaptureMode);
  }

  Panel.Reset();
  Browser.Reset();
}

/* //} */

/* BeginPlacing() //{ */

void USceneEditor::BeginPlacing(const FLibraryObject& Object)
{
  EndPlacing(false);

  AFlightForgeGameModeBase* GameMode = GetGameMode();
  if (!GameMode)
  {
    return;
  }

  if (!TraceCursor(PlacingPoint))
  {
    PlacingPoint = FVector::ZeroVector;
  }

  FSpawnObjectEntry Entry;
  USpawnConfigUtils::FromUnrealTransform(FTransform(PlacingPoint), GameMode->GetWorldOrigin(), Entry);
  Entry.Asset       = Object.SpawnName;
  Entry.Orientation = FVector(0.0, 0.0, -PlacingYaw);
  Entry.Scale       = FVector::OneVector;

  FSpawnConfig Config;
  Config.ModelPaths.Add(FPaths::Combine(FPaths::ProjectDir(), TEXT("FlightForge")));
  Config.Objects.Add(Entry);

  const TArray<int32> Ids = GameMode->SpawnObjectsFromConfig(Config);
  AActor*             Ghost = Ids.IsEmpty() ? nullptr : GameMode->GetObjectSpawner()->GetActor(Ids[0]);

  if (!Ghost)
  {
    Browser->SetStatus(FText::Format(LOCTEXT("SpawnFailed", "{0} could not be placed"), FText::FromString(Object.SpawnName)));
    Browser->ClearPicked();
    return;
  }

  Ghost->SetActorEnableCollision(false);

  Placing   = Object;
  PlacingId = Ids[0];
  Browser->SetStatus(FText::GetEmpty());

  Update();
}

/* //} */

/* EndPlacing() //{ */

void USceneEditor::EndPlacing(const bool bKeep)
{
  if (!IsPlacing())
  {
    return;
  }

  AFlightForgeGameModeBase* GameMode = GetGameMode();
  UWorldObjectSpawner*      Spawner  = GameMode ? GameMode->GetObjectSpawner() : nullptr;

  if (Spawner)
  {
    if (!bKeep)
    {
      Spawner->RemoveObject(PlacingId);
    }
    else if (AActor* Placed = Spawner->GetActor(PlacingId))
    {
      Placed->SetActorEnableCollision(true);

      if (!PlaceOn(*Placed, PlacingPoint, Placing.BaseZ * Placed->GetActorScale3D().Z * 100.0))
      {
        Settling.Add(PlacingId, PlacingPoint);
      }
    }
  }

  PlacingId = INDEX_NONE;

  if (Browser)
  {
    Browser->ClearPicked();
  }
}

/* //} */

/* Update() //{ */

void USceneEditor::Update()
{
  AFlightForgeGameModeBase* GameMode = GetGameMode();
  UWorldObjectSpawner*      Spawner  = GameMode ? GameMode->GetObjectSpawner() : nullptr;
  if (!Spawner)
  {
    return;
  }

  for (auto It = Settling.CreateIterator(); It; ++It)
  {
    AActor* Placed = Spawner->GetActor(It.Key());
    if (!Placed || PlaceOn(*Placed, It.Value(), 0.0))
    {
      It.RemoveCurrent();
    }
  }

  if (!IsPlacing())
  {
    return;
  }

  AActor* Ghost = Spawner->GetActor(PlacingId);
  if (!Ghost)
  {
    PlacingId = INDEX_NONE;
    Browser->ClearPicked();
    Browser->SetStatus(FText::Format(LOCTEXT("ImportFailed", "{0} did not import"), FText::FromString(Placing.SpawnName)));
    return;
  }

  if (!TraceCursor(PlacingPoint))
  {
    return;
  }

  Ghost->SetActorRotation(FRotator(0.0, PlacingYaw, 0.0));
  PlaceOn(*Ghost, PlacingPoint, Placing.BaseZ * Ghost->GetActorScale3D().Z * 100.0);
}

/* //} */

/* PlaceOn() //{ */

bool USceneEditor::PlaceOn(AActor& Actor, const FVector& Point, const double FallbackLiftCm)
{
  const UStaticMeshComponent* Mesh = Actor.FindComponentByClass<UStaticMeshComponent>();

  if (!Mesh || !Mesh->GetStaticMesh())
  {
    Actor.SetActorLocation(Point + FVector(0.0, 0.0, FallbackLiftCm));
    return false;
  }

  // a model's origin can sit metres away from its geometry, so the bounds go on the point, not the origin
  const FBox    Bounds = Actor.GetComponentsBoundingBox(true);
  const FVector Base(Bounds.GetCenter().X, Bounds.GetCenter().Y, Bounds.Min.Z);
  Actor.SetActorLocation(Point + Actor.GetActorLocation() - Base);
  return true;
}

/* //} */

/* input //{ */

bool USceneEditor::HandleKeyDown(const FKeyEvent& Event)
{
  if (Event.IsRepeat())
  {
    return false;
  }

  if (Event.GetKey() == EKeys::Tab && !Event.IsControlDown() && !Event.IsAltDown())
  {
    Toggle();
    return GetGameMode() != nullptr;
  }

  if (Event.GetKey() == EKeys::Escape && IsOpen())
  {
    IsPlacing() ? EndPlacing(false) : Close();
    return true;
  }

  return false;
}

bool USceneEditor::HandleMouseDown(const FPointerEvent& Event)
{
  if (!IsPlacing() || Event.GetEffectingButton() != EKeys::LeftMouseButton || IsOverPanel(Event))
  {
    return false;
  }

  const FLibraryObject Again = Placing;
  EndPlacing(true);

  if (Event.IsShiftDown())
  {
    BeginPlacing(Again);
  }

  return true;
}

bool USceneEditor::HandleWheel(const FPointerEvent& Event)
{
  if (!IsPlacing() || IsOverPanel(Event))
  {
    return false;
  }

  PlacingYaw = FRotator::NormalizeAxis(PlacingYaw + FMath::Sign(Event.GetWheelDelta()) * YawStepDeg);
  Update();
  return true;
}

/* //} */

/* helpers //{ */

AFlightForgeGameModeBase* USceneEditor::GetGameMode() const
{
  const UWorld* World = GetWorld();
  return World ? Cast<AFlightForgeGameModeBase>(World->GetAuthGameMode()) : nullptr;
}

APlayerController* USceneEditor::GetPlayer() const
{
  UWorld* World = GetWorld();
  return World && GEngine ? GEngine->GetFirstLocalPlayerController(World) : nullptr;
}

bool USceneEditor::IsOverPanel(const FPointerEvent& Event) const
{
  return Browser && Browser->GetTickSpaceGeometry().IsUnderLocation(Event.GetScreenSpacePosition());
}

bool USceneEditor::TraceCursor(FVector& OutPoint) const
{
  const APlayerController* Player = GetPlayer();

  FVector Origin;
  FVector Direction;
  if (!Player || !Player->DeprojectMousePositionToWorld(Origin, Direction))
  {
    return false;
  }

  FCollisionQueryParams Params(SCENE_QUERY_STAT(SceneEditorPlace), true, Player->GetPawn());

  FHitResult Hit;
  if (!GetWorld()->LineTraceSingleByChannel(Hit, Origin, Origin + Direction * 1.0e6, ECC_Visibility, Params))
  {
    return false;
  }

  OutPoint = Hit.ImpactPoint;
  return true;
}

/* //} */

#undef LOCTEXT_NAMESPACE
