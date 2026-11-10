#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"
#include "Subsystems/WorldSubsystem.h"

#include "FlightForgeCore/Utils/ObjectLibrary.h"

#include "SceneEditor.generated.h"

class AFlightForgeGameModeBase;
class APlayerController;
class FSceneEditInput;
class SObjectBrowser;
class SWidget;
struct FKeyEvent;
struct FPointerEvent;

/* USceneEditor //{ */

/** The in-game scene editor: Tab opens the object library, and a picked object follows the cursor until a click drops it. */
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

  bool HandleWheel(const FPointerEvent& Event);

private:
  AFlightForgeGameModeBase* GetGameMode() const;

  APlayerController* GetPlayer() const;

  bool IsOverPanel(const FPointerEvent& Event) const;

  bool TraceCursor(FVector& OutPoint) const;

  void EndPlacing(bool bKeep);

  /** Grounds the actor's bounds on the point; false while its mesh is still importing. */
  static bool PlaceOn(AActor& Actor, const FVector& Point, double FallbackLiftCm);

  void Close();

  TSharedPtr<FSceneEditInput> Input;
  TSharedPtr<SWidget>         Panel;
  TSharedPtr<SObjectBrowser>  Browser;

  EMouseCaptureMode SavedCaptureMode = EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown;

  FLibraryObject Placing;
  int32          PlacingId  = INDEX_NONE;
  double         PlacingYaw = 0.0;
  FVector        PlacingPoint = FVector::ZeroVector;

  /** Dropped before their meshes landed, so they are grounded once the mesh is there. */
  TMap<int32, FVector> Settling;
};

/* //} */
