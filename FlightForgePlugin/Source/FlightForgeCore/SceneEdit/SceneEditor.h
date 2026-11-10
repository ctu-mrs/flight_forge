#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"
#include "Subsystems/WorldSubsystem.h"

#include "FlightForgeCore/Utils/ObjectLibrary.h"
#include "SSceneGizmo.h"

#include "SceneEditor.generated.h"

class AFlightForgeGameModeBase;
class APlayerController;
class FSceneEditInput;
class SObjectBrowser;
class SWidget;
class UWorldObjectSpawner;
struct FKeyEvent;
struct FPointerEvent;

/* USceneEditor //{ */

/**
 * The in-game scene editor: Tab opens the object library, a picked object follows the cursor until a click
 * drops it, and a placed object can be selected, moved, turned, removed and the edits undone.
 */
UCLASS()
class FLIGHTFORGECORE_API USceneEditor : public UWorldSubsystem
{
  GENERATED_BODY()

public:
  virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

  virtual void OnWorldBeginPlay(UWorld& InWorld) override;

  virtual void Deinitialize() override;

  bool IsOpen() const { return Panel.IsValid(); }

  bool IsPlacing() const { return PlacingId != INDEX_NONE; }

  void Toggle();

  void BeginPlacing(const FLibraryObject& Object);

  void Update();

  bool HandleKeyDown(const FKeyEvent& Event);

  bool HandleMouseDown(const FPointerEvent& Event);

  bool HandleMouseUp(const FPointerEvent& Event);

  bool HandleWheel(const FPointerEvent& Event);

private:
  enum class EEdit : uint8
  {
    Placed,
    Removed,
    Moved,
  };

  /** Placed and Removed keep the object's pose in After, captured again whenever it is removed. */
  struct FEdit
  {
    EEdit      Kind = EEdit::Moved;
    int32      Id   = INDEX_NONE;
    FString    Asset;
    FTransform Before;
    FTransform After;
    double     Time   = 0.0;
    bool       bWheel = false;
  };

  enum class EDrag : uint8
  {
    None,
    Surface,
    Axis,
    Ring,
  };

  AFlightForgeGameModeBase* GetGameMode() const;

  UWorldObjectSpawner* GetSpawner() const;

  APlayerController* GetPlayer() const;

  bool IsOverPanel(const FPointerEvent& Event) const;

  bool IsTyping() const;

  void BlockLook(bool bBlock);

  bool GetMouse(FVector2D& OutPixels) const;

  bool CursorOnPlane(double Z, FVector& OutPoint) const;

  bool TraceCursor(FVector& OutPoint, const AActor* Ignored = nullptr, AActor** OutHit = nullptr) const;

  /** An exact transform replaces the one the spawn config gives, scale included. */
  int32 SpawnAt(const FString& Asset, const FVector& Location, const TOptional<FTransform>& Exact = {});

  void EndPlacing(bool bKeep);

  /** Grounds the actor's bounds on the point; false while its mesh is still importing. */
  static bool PlaceOn(AActor& Actor, const FVector& Point, double FallbackLiftCm);

  static void TurnAbout(AActor& Actor, const FTransform& From, double DeltaDeg, const FVector& Pivot);

  void Select(int32 ObjectId);

  void UpdateGizmo(const AActor& Selected);

  EGizmoHandle HandleUnder(const FVector2D& Mouse) const;

  void BeginDrag(EDrag Kind, EGizmoHandle Handle, AActor& Selected, const FVector2D& Mouse);

  void UpdateDrag(AActor& Selected, const FVector2D& Mouse);

  void EndDrag(bool bKeep);

  void Record(FEdit&& Edit);

  bool Apply(FEdit& Edit, bool bUndo);

  void Undo();

  void Redo();

  void Remap(int32 From, int32 To);

  void RemoveSelected();

  void DuplicateSelected();

  void SaveScene();

  void RefreshInfo();

  void Close();

  TSharedPtr<FSceneEditInput> Input;
  TSharedPtr<SWidget>         Panel;
  TSharedPtr<SObjectBrowser>  Browser;
  TSharedPtr<FGizmoView>      Gizmo;

  EMouseCaptureMode SavedCaptureMode = EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown;

  FLibraryObject Placing;
  int32          PlacingId    = INDEX_NONE;
  double         PlacingYaw   = 0.0;
  FVector        PlacingPoint = FVector::ZeroVector;

  /** Dropped before their meshes landed, so they are grounded once the mesh is there. */
  TMap<int32, FVector> Settling;

  int32 SelectedId   = INDEX_NONE;
  bool  bSnap        = true;
  bool  bLookBlocked = false;

  EDrag        Drag       = EDrag::None;
  EGizmoHandle DragHandle = EGizmoHandle::None;
  FVector2D    DragMouse  = FVector2D::ZeroVector;
  FTransform   DragStart;
  FVector      DragPivot  = FVector::ZeroVector;
  FVector      DragOffset = FVector::ZeroVector;
  FVector2D    DragScreenAxis  = FVector2D::ZeroVector;
  FVector2D    DragPivotScreen = FVector2D::ZeroVector;
  double       DragAngle       = 0.0;
  double       DragPixelsPerCm = 0.0;

  TArray<FEdit> UndoStack;
  TArray<FEdit> RedoStack;
};

/* //} */
