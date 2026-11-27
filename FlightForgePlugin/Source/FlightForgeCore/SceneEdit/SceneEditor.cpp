#include "SceneEditor.h"

#include "Camera/PlayerCameraManager.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "Framework/Application/IInputProcessor.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/PlayerController.h"
#include "HAL/PlatformTime.h"
#include "Misc/DateTime.h"
#include "Misc/FileHelper.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SOverlay.h"

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

  virtual bool HandleMouseButtonDoubleClickEvent(FSlateApplication& SlateApp, const FPointerEvent& Event) override
  {
    USceneEditor* Pinned = Editor.Get();
    return Pinned && Pinned->HandleMouseDown(Event);
  }

  virtual bool HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& Event) override
  {
    USceneEditor* Pinned = Editor.Get();
    return Pinned && Pinned->HandleMouseUp(Event);
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

constexpr double YawStepDeg   = 15.0;
constexpr double GridCm       = 25.0;
constexpr double AxisPixels   = 110.0;
constexpr double PickPixels   = 9.0;
constexpr double PivotPixels  = 24.0;
constexpr int32  RingSegments = 48;
constexpr int32  MaxEdits     = 200;

// wheel notches on one object within this many seconds undo as one edit
constexpr double WheelMergeS = 0.8;

// ROS axes, so the green arrow is +y of the spawn configs
const FVector AxisDirections[3] = {FVector::ForwardVector, -FVector::RightVector, FVector::UpVector};

const FVector2D Offscreen(NAN, NAN);

double Snapped(const double Value, const double Step, const bool bSnap)
{
  return bSnap ? FMath::GridSnap(Value, Step) : Value;
}

double DistanceToSegment(const FVector2D& Point, const FVector2D& A, const FVector2D& B)
{
  if (FMath::IsNaN(A.X) || FMath::IsNaN(B.X))
  {
    return MAX_dbl;
  }

  const FVector2D Along   = B - A;
  const double    Length2 = Along.SizeSquared();
  const double    T       = Length2 > 0.0 ? FMath::Clamp(FVector2D::DotProduct(Point - A, Along) / Length2, 0.0, 1.0) : 0.0;
  return FVector2D::Distance(Point, A + Along * T);
}

FBox BoundsOf(const AActor& Actor)
{
  return Actor.GetComponentsBoundingBox(true);
}

FVector BaseOf(const FBox& Bounds)
{
  return FVector(Bounds.GetCenter().X, Bounds.GetCenter().Y, Bounds.Min.Z);
}

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

  Gizmo = MakeShared<FGizmoView>();

  Panel = SNew(SOverlay)
      .Visibility(EVisibility::SelfHitTestInvisible)
      + SOverlay::Slot()
      [
        SNew(SSceneGizmo).View(Gizmo).Visibility(EVisibility::HitTestInvisible)
      ]
      + SOverlay::Slot()
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
  BlockLook(true);

  RefreshInfo();
}

/* //} */

/* Close() //{ */

void USceneEditor::Close()
{
  EndPlacing(false);
  EndDrag(true);
  SelectedId = INDEX_NONE;

  if (!Panel)
  {
    return;
  }

  UWorld* World = GetWorld();
  if (UGameViewportClient* Viewport = World ? World->GetGameViewport() : nullptr)
  {
    Viewport->RemoveViewportWidgetContent(Panel.ToSharedRef());

    BlockLook(false);

    if (APlayerController* Player = GetPlayer())
    {
      Player->SetInputMode(FInputModeGameOnly());
      Player->SetShowMouseCursor(false);
    }

    Viewport->SetMouseCaptureMode(SavedCaptureMode);
  }

  Panel.Reset();
  Browser.Reset();
  Gizmo.Reset();
}

/* //} */

/* placement //{ */

void USceneEditor::BeginPlacing(const FLibraryObject& Object)
{
  EndPlacing(false);
  EndDrag(true);
  Select(INDEX_NONE);

  if (!TraceCursor(PlacingPoint))
  {
    PlacingPoint = FVector::ZeroVector;
  }

  const int32 Id    = SpawnAt(Object.SpawnName, PlacingPoint);
  AActor*     Ghost = Id == INDEX_NONE ? nullptr : GetSpawner()->GetActor(Id);

  if (!Ghost)
  {
    Browser->SetStatus(FText::Format(LOCTEXT("SpawnFailed", "{0} could not be placed"), FText::FromString(Object.SpawnName)));
    Browser->ClearPicked();
    return;
  }

  Ghost->SetActorEnableCollision(false);

  Placing   = Object;
  PlacingId = Id;
  Browser->SetStatus(FText::GetEmpty());

  Update();
}

void USceneEditor::EndPlacing(const bool bKeep)
{
  if (!IsPlacing())
  {
    return;
  }

  if (UWorldObjectSpawner* Spawner = GetSpawner())
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

      FEdit Edit;
      Edit.Kind  = EEdit::Placed;
      Edit.Id    = PlacingId;
      Edit.Asset = Placing.SpawnName;
      Record(MoveTemp(Edit));
    }
  }

  PlacingId = INDEX_NONE;

  if (Browser)
  {
    Browser->ClearPicked();
  }
}

bool USceneEditor::PlaceOn(AActor& Actor, const FVector& Point, const double FallbackLiftCm)
{
  const UStaticMeshComponent* Mesh = Actor.FindComponentByClass<UStaticMeshComponent>();

  if (!Mesh || !Mesh->GetStaticMesh())
  {
    Actor.SetActorLocation(Point + FVector(0.0, 0.0, FallbackLiftCm));
    return false;
  }

  // a model's origin can sit metres away from its geometry, so the bounds go on the point, not the origin
  Actor.SetActorLocation(Point + Actor.GetActorLocation() - BaseOf(BoundsOf(Actor)));
  return true;
}

void USceneEditor::TurnAbout(AActor& Actor, const FTransform& From, const double DeltaDeg, const FVector& Pivot)
{
  const FQuat Turn(FVector::UpVector, FMath::DegreesToRadians(DeltaDeg));

  FTransform Turned = From;
  Turned.SetLocation(Pivot + Turn.RotateVector(From.GetLocation() - Pivot));
  Turned.SetRotation(Turn * From.GetRotation());
  Actor.SetActorTransform(Turned, false, nullptr, ETeleportType::TeleportPhysics);
}

/* //} */

/* Update() //{ */

void USceneEditor::Update()
{
  UWorldObjectSpawner* Spawner = GetSpawner();
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

  if (Gizmo)
  {
    Gizmo->bVisible = false;
  }

  if (IsPlacing())
  {
    AActor* Ghost = Spawner->GetActor(PlacingId);
    if (!Ghost)
    {
      PlacingId = INDEX_NONE;
      Browser->ClearPicked();
      Browser->SetStatus(FText::Format(LOCTEXT("ImportFailed", "{0} did not import"), FText::FromString(Placing.SpawnName)));
      return;
    }

    if (TraceCursor(PlacingPoint))
    {
      Ghost->SetActorRotation(FRotator(0.0, PlacingYaw, 0.0));
      PlaceOn(*Ghost, PlacingPoint, Placing.BaseZ * Ghost->GetActorScale3D().Z * 100.0);
    }

    return;
  }

  if (SelectedId == INDEX_NONE || !Gizmo)
  {
    return;
  }

  AActor* Selected = Spawner->GetActor(SelectedId);
  if (!Selected)
  {
    EndDrag(false);
    Select(INDEX_NONE);
    return;
  }

  FVector2D Mouse;
  const bool bMouse = GetMouse(Mouse);

  if (Drag != EDrag::None && bMouse)
  {
    UpdateDrag(*Selected, Mouse);
  }

  UpdateGizmo(*Selected);

  Gizmo->Active  = Drag == EDrag::None ? EGizmoHandle::None : DragHandle;
  Gizmo->Hovered = bMouse ? HandleUnder(Mouse) : EGizmoHandle::None;
}

/* //} */

/* gizmo //{ */

void USceneEditor::UpdateGizmo(const AActor& Selected)
{
  APlayerController* Player = GetPlayer();
  if (!Player || !Player->PlayerCameraManager)
  {
    return;
  }

  const auto Project = [Player](const FVector& World) {
    FVector2D Screen;
    return Player->ProjectWorldLocationToScreen(World, Screen) ? Screen : Offscreen;
  };

  int32 Width  = 0;
  int32 Height = 0;
  Player->GetViewportSize(Width, Height);

  const FBox    Bounds   = BoundsOf(Selected);
  const FVector Center   = Bounds.GetCenter();
  const double  Distance = FVector::Distance(Center, Player->PlayerCameraManager->GetCameraLocation());
  const double  HalfFov  = FMath::DegreesToRadians(Player->PlayerCameraManager->GetFOVAngle() * 0.5);
  const double  Length   = AxisPixels * Distance * FMath::Tan(HalfFov) / FMath::Max(1.0, Width * 0.5);

  Gizmo->bVisible = true;
  Gizmo->Pivot    = Project(Center);

  for (int32 Axis = 0; Axis < 3; Axis++)
  {
    Gizmo->Axes[Axis] = Project(Center + AxisDirections[Axis] * Length);
  }

  for (int32 Corner = 0; Corner < 8; Corner++)
  {
    Gizmo->Box[Corner] = Project(FVector(Corner & 1 ? Bounds.Max.X : Bounds.Min.X, Corner & 2 ? Bounds.Max.Y : Bounds.Min.Y, Corner & 4 ? Bounds.Max.Z : Bounds.Min.Z));
  }

  const double Radius = FMath::Max(FVector2D(Bounds.GetExtent()).Size() * 1.05, Length * 0.7);

  Gizmo->Ring.SetNum(RingSegments);
  for (int32 Index = 0; Index < RingSegments; Index++)
  {
    const double Angle = 2.0 * PI * Index / RingSegments;
    Gizmo->Ring[Index] = Project(FVector(Center.X + Radius * FMath::Cos(Angle), Center.Y + Radius * FMath::Sin(Angle), Bounds.Min.Z + 2.0));
  }
}

EGizmoHandle USceneEditor::HandleUnder(const FVector2D& Mouse) const
{
  if (!Gizmo || !Gizmo->bVisible)
  {
    return EGizmoHandle::None;
  }

  EGizmoHandle Best     = EGizmoHandle::None;
  double       BestDist = PickPixels;

  // the arrows meet at the object's centre, where a press should grab the object itself
  const bool bNearPivot = FVector2D::Distance(Mouse, Gizmo->Pivot) < PivotPixels;

  for (int32 Axis = 0; Axis < 3 && !bNearPivot; Axis++)
  {
    const double Dist = DistanceToSegment(Mouse, Gizmo->Pivot, Gizmo->Axes[Axis]);
    if (Dist < BestDist)
    {
      Best     = EGizmoHandle(int32(EGizmoHandle::X) + Axis);
      BestDist = Dist;
    }
  }

  if (Best != EGizmoHandle::None)
  {
    return Best;
  }

  for (int32 Index = 0; Index < Gizmo->Ring.Num(); Index++)
  {
    if (DistanceToSegment(Mouse, Gizmo->Ring[Index], Gizmo->Ring[(Index + 1) % Gizmo->Ring.Num()]) < PickPixels)
    {
      return EGizmoHandle::Ring;
    }
  }

  return EGizmoHandle::None;
}

/* //} */

/* dragging //{ */

void USceneEditor::BeginDrag(const EDrag Kind, const EGizmoHandle Handle, AActor& Selected, const FVector2D& Mouse)
{
  const FBox Bounds = BoundsOf(Selected);

  Drag       = Kind;
  DragHandle = Handle;
  DragMouse  = Mouse;
  DragStart  = Selected.GetActorTransform();
  DragPivot  = Bounds.GetCenter();
  DragOffset = FVector::ZeroVector;

  APlayerController* Player = GetPlayer();
  if (!Player || !Player->ProjectWorldLocationToScreen(DragPivot, DragPivotScreen))
  {
    Drag = EDrag::None;
    return;
  }

  if (Kind == EDrag::Axis)
  {
    const int32 Axis = int32(Handle) - int32(EGizmoHandle::X);

    FVector2D Moved;
    DragPixelsPerCm = 0.0;
    if (Player->ProjectWorldLocationToScreen(DragPivot + AxisDirections[Axis] * 100.0, Moved))
    {
      DragScreenAxis  = (Moved - DragPivotScreen).GetSafeNormal();
      DragPixelsPerCm = FVector2D::Distance(Moved, DragPivotScreen) / 100.0;
    }

    // an arrow pointing at the camera has no direction to drag along
    if (DragPixelsPerCm < 0.05)
    {
      Drag = EDrag::None;
    }
  }
  else if (Kind == EDrag::Surface)
  {
    // where the cursor ray meets the object's base plane, so the grab does not jump
    const FVector Base = BaseOf(Bounds);
    FVector       Grab;
    if (CursorOnPlane(Base.Z, Grab))
    {
      DragOffset   = Base - Grab;
      DragOffset.Z = 0.0;
    }
  }
  else if (Kind == EDrag::Ring)
  {
    FVector Grab;
    DragOffset = FVector(0.0, 0.0, Bounds.Min.Z);
    DragAngle  = CursorOnPlane(DragOffset.Z, Grab) ? FMath::Atan2(Grab.Y - DragPivot.Y, Grab.X - DragPivot.X) : NAN;
  }
}

void USceneEditor::UpdateDrag(AActor& Selected, const FVector2D& Mouse)
{
  switch (Drag)
  {
    case EDrag::Axis:
    {
      const int32  Axis  = int32(DragHandle) - int32(EGizmoHandle::X);
      const double Delta = Snapped(FVector2D::DotProduct(Mouse - DragMouse, DragScreenAxis) / DragPixelsPerCm, GridCm, bSnap);
      Selected.SetActorLocation(DragStart.GetLocation() + AxisDirections[Axis] * Delta, false, nullptr, ETeleportType::TeleportPhysics);
      break;
    }

    case EDrag::Surface:
    {
      FVector Point;
      if (!TraceCursor(Point, &Selected))
      {
        break;
      }

      const FVector StartBase = DragStart.GetLocation() + (BaseOf(BoundsOf(Selected)) - Selected.GetActorLocation());
      FVector       Target    = Point + DragOffset;
      Target.X                = StartBase.X + Snapped(Target.X - StartBase.X, GridCm, bSnap);
      Target.Y                = StartBase.Y + Snapped(Target.Y - StartBase.Y, GridCm, bSnap);
      PlaceOn(Selected, Target, 0.0);
      break;
    }

    case EDrag::Ring:
    {
      FVector Point;
      if (FMath::IsNaN(DragAngle) || !CursorOnPlane(DragOffset.Z, Point))
      {
        break;
      }

      const double Swept = FMath::RadiansToDegrees(FMath::Atan2(Point.Y - DragPivot.Y, Point.X - DragPivot.X) - DragAngle);
      TurnAbout(Selected, DragStart, Snapped(FRotator::NormalizeAxis(Swept), YawStepDeg, bSnap), DragPivot);
      break;
    }

    default:
      break;
  }
}

void USceneEditor::EndDrag(const bool bKeep)
{
  if (Drag == EDrag::None)
  {
    return;
  }

  Drag = EDrag::None;

  AActor* Selected = GetSpawner() ? GetSpawner()->GetActor(SelectedId) : nullptr;
  if (!Selected)
  {
    return;
  }

  if (!bKeep)
  {
    Selected->SetActorTransform(DragStart, false, nullptr, ETeleportType::TeleportPhysics);
    return;
  }

  if (!Selected->GetActorTransform().Equals(DragStart, 0.01))
  {
    FEdit Edit;
    Edit.Kind   = EEdit::Moved;
    Edit.Id     = SelectedId;
    Edit.Before = DragStart;
    Edit.After  = Selected->GetActorTransform();
    Record(MoveTemp(Edit));
  }
}

/* //} */

/* selection //{ */

void USceneEditor::Select(const int32 ObjectId)
{
  SelectedId = ObjectId;
  RefreshInfo();
}

void USceneEditor::RemoveSelected()
{
  UWorldObjectSpawner* Spawner  = GetSpawner();
  AActor*              Selected = Spawner ? Spawner->GetActor(SelectedId) : nullptr;
  if (!Selected)
  {
    return;
  }

  FEdit Edit;
  Edit.Kind  = EEdit::Removed;
  Edit.Id    = SelectedId;
  Edit.Asset = Spawner->GetAsset(SelectedId);
  Edit.After = Selected->GetActorTransform();

  Spawner->RemoveObject(SelectedId);
  Record(MoveTemp(Edit));
  Select(INDEX_NONE);
}

void USceneEditor::DuplicateSelected()
{
  UWorldObjectSpawner* Spawner  = GetSpawner();
  AActor*              Selected = Spawner ? Spawner->GetActor(SelectedId) : nullptr;
  if (!Selected)
  {
    return;
  }

  const FString         Asset = Spawner->GetAsset(SelectedId);
  const FLibraryObject* Known = GetGameMode()->GetObjectLibrary().Find(Asset);

  FLibraryObject Copy;
  if (Known)
  {
    Copy = *Known;
  }
  else
  {
    Copy.SpawnName = Asset;
  }

  PlacingYaw = Selected->GetActorRotation().Yaw;
  BeginPlacing(Copy);
}

/* //} */

/* undo //{ */

void USceneEditor::Record(FEdit&& Edit)
{
  Edit.Time = FPlatformTime::Seconds();

  FEdit* Last = UndoStack.IsEmpty() ? nullptr : &UndoStack.Last();
  if (Last && Edit.bWheel && Last->bWheel && Last->Id == Edit.Id && Edit.Time - Last->Time < WheelMergeS)
  {
    Last->After = Edit.After;
    Last->Time  = Edit.Time;
  }
  else
  {
    UndoStack.Add(MoveTemp(Edit));
  }

  if (UndoStack.Num() > MaxEdits)
  {
    UndoStack.RemoveAt(0);
  }

  RedoStack.Reset();
  RefreshInfo();
}

bool USceneEditor::Apply(FEdit& Edit, const bool bUndo)
{
  UWorldObjectSpawner* Spawner = GetSpawner();
  if (!Spawner)
  {
    return false;
  }

  const bool bRemove = (Edit.Kind == EEdit::Placed) == bUndo;

  if (Edit.Kind == EEdit::Moved)
  {
    AActor* Moved = Spawner->GetActor(Edit.Id);
    return Moved && Moved->SetActorTransform(bUndo ? Edit.Before : Edit.After, false, nullptr, ETeleportType::TeleportPhysics);
  }

  if (bRemove)
  {
    AActor* Removed = Spawner->GetActor(Edit.Id);
    if (!Removed)
    {
      return false;
    }

    Edit.After = Removed->GetActorTransform();
    if (SelectedId == Edit.Id)
    {
      Select(INDEX_NONE);
    }
    return Spawner->RemoveObject(Edit.Id);
  }

  const int32 Respawned = SpawnAt(Edit.Asset, Edit.After.GetLocation(), Edit.After);
  if (Respawned == INDEX_NONE)
  {
    return false;
  }

  Remap(Edit.Id, Respawned);
  Edit.Id = Respawned;
  return true;
}

void USceneEditor::Undo()
{
  EndDrag(true);

  while (!UndoStack.IsEmpty())
  {
    FEdit Edit = UndoStack.Pop();
    if (Apply(Edit, true))
    {
      RedoStack.Add(MoveTemp(Edit));
      break;
    }
  }

  RefreshInfo();
}

void USceneEditor::Redo()
{
  EndDrag(true);

  while (!RedoStack.IsEmpty())
  {
    FEdit Edit = RedoStack.Pop();
    if (Apply(Edit, false))
    {
      UndoStack.Add(MoveTemp(Edit));
      break;
    }
  }

  RefreshInfo();
}

void USceneEditor::Remap(const int32 From, const int32 To)
{
  for (TArray<FEdit>* Stack : {&UndoStack, &RedoStack})
  {
    for (FEdit& Edit : *Stack)
    {
      Edit.Id = Edit.Id == From ? To : Edit.Id;
    }
  }

  SelectedId = SelectedId == From ? To : SelectedId;
}

/* //} */

/* SaveScene() //{ */

void USceneEditor::SaveScene()
{
  AFlightForgeGameModeBase* GameMode = GetGameMode();
  if (!GameMode || !Browser)
  {
    return;
  }

  const FString Path = FPaths::ConvertRelativePathToFull(
      FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("FlightForge"), TEXT("Scenes"), FDateTime::Now().ToString(TEXT("scene_%Y%m%d_%H%M%S.yaml"))));

  const int32 Count = GameMode->ListSpawnedObjects().Num();

  if (FFileHelper::SaveStringToFile(GameMode->ExportSceneYaml(), *Path, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM))
  {
    Browser->SetStatus(FText::Format(LOCTEXT("Saved", "Saved {0} {0}|plural(one=object,other=objects) to {1}"), Count, FText::FromString(Path)));
  }
  else
  {
    Browser->SetStatus(FText::Format(LOCTEXT("SaveFailed", "Could not write {0}"), FText::FromString(Path)));
  }
}

/* //} */

/* RefreshInfo() //{ */

void USceneEditor::RefreshInfo()
{
  if (!Browser)
  {
    return;
  }

  const FText Snap = bSnap ? FText::Format(LOCTEXT("SnapOn", "snap {0} m, {1}°"), GridCm / 100.0, YawStepDeg) : LOCTEXT("SnapOff", "snap off");

  UWorldObjectSpawner* Spawner = GetSpawner();
  if (SelectedId == INDEX_NONE || !Spawner)
  {
    Browser->SetSelection(FText::Format(LOCTEXT("NoSelection", "Nothing selected  ·  {0}  ·  {1} {1}|plural(one=edit,other=edits) to undo"), Snap, UndoStack.Num()));
    return;
  }

  const FString         Asset = Spawner->GetAsset(SelectedId);
  const FLibraryObject* Known = GetGameMode()->GetObjectLibrary().Find(Asset);
  const FString         Name  = Known && !Known->Class.IsEmpty() ? Known->Class.Replace(TEXT("_"), TEXT(" ")) : FPaths::GetBaseFilename(Asset);

  Browser->SetSelection(FText::Format(LOCTEXT("Selection", "Selected {0} (id {1})  ·  {2}"), FText::FromString(Name), SelectedId, Snap));
}

/* //} */

/* input //{ */

bool USceneEditor::HandleKeyDown(const FKeyEvent& Event)
{
  if (Event.IsRepeat())
  {
    return false;
  }

  const FKey Key = Event.GetKey();

  if (Key == EKeys::Tab && !Event.IsControlDown() && !Event.IsAltDown())
  {
    Toggle();
    return GetGameMode() != nullptr;
  }

  if (!IsOpen())
  {
    return false;
  }

  // the default fly pawn sinks on LeftControl; the shortcuts read the modifier from the platform
  if (Key == EKeys::LeftControl || Key == EKeys::RightControl)
  {
    return true;
  }

  if (Key == EKeys::Escape)
  {
    if (IsPlacing())
    {
      EndPlacing(false);
    }
    else if (Drag != EDrag::None)
    {
      EndDrag(false);
    }
    else if (SelectedId != INDEX_NONE)
    {
      Select(INDEX_NONE);
    }
    else
    {
      Close();
    }
    return true;
  }

  if (Event.IsControlDown() && Key == EKeys::S)
  {
    SaveScene();
    return true;
  }

  if (IsTyping() || Drag != EDrag::None)
  {
    return false;
  }

  if (Event.IsControlDown() && (Key == EKeys::Y || (Key == EKeys::Z && Event.IsShiftDown())))
  {
    Redo();
    return true;
  }

  if (Event.IsControlDown() && Key == EKeys::Z)
  {
    Undo();
    return true;
  }

  if (Event.IsControlDown() && Key == EKeys::D)
  {
    DuplicateSelected();
    return true;
  }

  if (Key == EKeys::Delete && SelectedId != INDEX_NONE)
  {
    RemoveSelected();
    return true;
  }

  if (Key == EKeys::G && !Event.IsControlDown())
  {
    bSnap = !bSnap;
    RefreshInfo();
    return true;
  }

  return false;
}

bool USceneEditor::HandleMouseDown(const FPointerEvent& Event)
{
  if (IsOpen() && Event.GetEffectingButton() == EKeys::RightMouseButton)
  {
    BlockLook(false);
    return false;
  }

  if (!IsOpen() || Event.GetEffectingButton() != EKeys::LeftMouseButton || IsOverPanel(Event))
  {
    return false;
  }

  if (IsPlacing())
  {
    const FLibraryObject Again = Placing;
    EndPlacing(true);

    if (Event.IsShiftDown())
    {
      BeginPlacing(Again);
    }

    return true;
  }

  FVector2D            Mouse;
  UWorldObjectSpawner* Spawner = GetSpawner();
  if (!Spawner || !GetMouse(Mouse))
  {
    return false;
  }

  AActor* Selected = Spawner->GetActor(SelectedId);

  if (const EGizmoHandle Handle = Selected ? HandleUnder(Mouse) : EGizmoHandle::None; Handle != EGizmoHandle::None)
  {
    BeginDrag(Handle == EGizmoHandle::Ring ? EDrag::Ring : EDrag::Axis, Handle, *Selected, Mouse);
    return true;
  }

  FVector Point;
  AActor* Hit = nullptr;
  TraceCursor(Point, nullptr, &Hit);

  const int32 HitId = Spawner->FindObject(Hit);
  Select(HitId);

  // keys go to the viewport from here, so WASD flies instead of typing into the search
  FSlateApplication::Get().SetAllUserFocusToGameViewport();

  if (HitId != INDEX_NONE)
  {
    BeginDrag(EDrag::Surface, EGizmoHandle::None, *Hit, Mouse);
  }

  return true;
}

bool USceneEditor::HandleMouseUp(const FPointerEvent& Event)
{
  if (IsOpen() && Event.GetEffectingButton() == EKeys::RightMouseButton)
  {
    BlockLook(true);
    return false;
  }

  if (Drag == EDrag::None || Event.GetEffectingButton() != EKeys::LeftMouseButton)
  {
    return false;
  }

  EndDrag(true);
  return true;
}

bool USceneEditor::HandleWheel(const FPointerEvent& Event)
{
  if (!IsOpen() || IsOverPanel(Event) || Drag != EDrag::None)
  {
    return false;
  }

  const double Step = FMath::Sign(Event.GetWheelDelta()) * YawStepDeg;

  if (IsPlacing())
  {
    PlacingYaw = FRotator::NormalizeAxis(PlacingYaw + Step);
    Update();
    return true;
  }

  AActor* Selected = GetSpawner() ? GetSpawner()->GetActor(SelectedId) : nullptr;
  if (!Selected)
  {
    return false;
  }

  FEdit Edit;
  Edit.Kind   = EEdit::Moved;
  Edit.Id     = SelectedId;
  Edit.Before = Selected->GetActorTransform();
  Edit.bWheel = true;

  TurnAbout(*Selected, Edit.Before, Step, BoundsOf(*Selected).GetCenter());

  Edit.After = Selected->GetActorTransform();
  Record(MoveTemp(Edit));
  return true;
}

/* //} */

/* helpers //{ */

// with the cursor out, plain mouse motion would turn the camera
void USceneEditor::BlockLook(const bool bBlock)
{
  APlayerController* Player = GetPlayer();
  if (!Player || bLookBlocked == bBlock)
  {
    return;
  }

  bLookBlocked = bBlock;
  Player->SetIgnoreLookInput(bBlock);
}

AFlightForgeGameModeBase* USceneEditor::GetGameMode() const
{
  const UWorld* World = GetWorld();
  return World ? Cast<AFlightForgeGameModeBase>(World->GetAuthGameMode()) : nullptr;
}

UWorldObjectSpawner* USceneEditor::GetSpawner() const
{
  AFlightForgeGameModeBase* GameMode = GetGameMode();
  return GameMode ? GameMode->GetObjectSpawner() : nullptr;
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

bool USceneEditor::IsTyping() const
{
  const TSharedPtr<SWidget> Focused = FSlateApplication::Get().GetKeyboardFocusedWidget();
  return Focused && Focused->GetType().ToString().Contains(TEXT("EditableText"));
}

bool USceneEditor::GetMouse(FVector2D& OutPixels) const
{
  const APlayerController* Player = GetPlayer();
  double                   X      = 0.0;
  double                   Y      = 0.0;
  if (!Player || !Player->GetMousePosition(X, Y))
  {
    return false;
  }

  OutPixels = FVector2D(X, Y);
  return true;
}

bool USceneEditor::CursorOnPlane(const double Z, FVector& OutPoint) const
{
  const APlayerController* Player = GetPlayer();

  FVector Origin;
  FVector Direction;
  if (!Player || !Player->DeprojectMousePositionToWorld(Origin, Direction) || FMath::IsNearlyZero(Direction.Z))
  {
    return false;
  }

  const double T = (Z - Origin.Z) / Direction.Z;
  OutPoint       = Origin + Direction * T;
  return T > 0.0;
}

bool USceneEditor::TraceCursor(FVector& OutPoint, const AActor* Ignored, AActor** OutHit) const
{
  const APlayerController* Player = GetPlayer();

  FVector Origin;
  FVector Direction;
  if (!Player || !Player->DeprojectMousePositionToWorld(Origin, Direction))
  {
    return false;
  }

  FCollisionQueryParams Params(SCENE_QUERY_STAT(SceneEditorPlace), true, Player->GetPawn());
  Params.AddIgnoredActor(Ignored);

  FHitResult Hit;
  if (!GetWorld()->LineTraceSingleByChannel(Hit, Origin, Origin + Direction * 1.0e6, ECC_Visibility, Params))
  {
    return false;
  }

  OutPoint = Hit.ImpactPoint;
  if (OutHit)
  {
    *OutHit = Hit.GetActor();
  }
  return true;
}

int32 USceneEditor::SpawnAt(const FString& Asset, const FVector& Location, const TOptional<FTransform>& Exact)
{
  AFlightForgeGameModeBase* GameMode = GetGameMode();
  if (!GameMode)
  {
    return INDEX_NONE;
  }

  FSpawnObjectEntry Entry;
  USpawnConfigUtils::FromUnrealTransform(FTransform(Location), GameMode->GetGroundOrigin(), Entry);
  Entry.Asset       = Asset;
  Entry.Orientation = FVector::ZeroVector;
  Entry.Scale       = FVector::OneVector;

  FSpawnConfig Config;
  Config.ModelPaths.Add(FPaths::Combine(FPaths::ProjectDir(), TEXT("FlightForge")));
  Config.Objects.Add(Entry);

  const TArray<int32> Ids    = GameMode->SpawnObjectsFromConfig(Config);
  AActor*             Spawned = Ids.IsEmpty() ? nullptr : GameMode->GetObjectSpawner()->GetActor(Ids[0]);
  if (!Spawned)
  {
    return INDEX_NONE;
  }

  if (Exact.IsSet())
  {
    Spawned->SetActorTransform(Exact.GetValue(), false, nullptr, ETeleportType::TeleportPhysics);
  }

  return Ids[0];
}

/* //} */

#undef LOCTEXT_NAMESPACE
