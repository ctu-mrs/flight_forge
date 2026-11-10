#pragma once

#include "CoreMinimal.h"
#include "Widgets/SLeafWidget.h"

enum class EGizmoHandle : uint8
{
  None,
  X,
  Y,
  Z,
  Ring,
};

/* FGizmoView //{ */

/** The selection's handles projected to viewport pixels; a point behind the camera is NaN. */
struct FGizmoView
{
  bool bVisible = false;

  FVector2D         Pivot;
  FVector2D         Axes[3];
  FVector2D         Box[8];
  TArray<FVector2D> Ring;

  EGizmoHandle Hovered = EGizmoHandle::None;
  EGizmoHandle Active  = EGizmoHandle::None;
};

/* //} */

/* SSceneGizmo //{ */

/** Paints the selection's box, arrows and yaw ring; USceneEditor takes the input. */
class SSceneGizmo : public SLeafWidget
{
public:
  SLATE_BEGIN_ARGS(SSceneGizmo) {}
  SLATE_ARGUMENT(TSharedPtr<const FGizmoView>, View)
  SLATE_END_ARGS()

  void Construct(const FArguments& InArgs);

  virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
                        int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

  virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;

private:
  TSharedPtr<const FGizmoView> View;
};

/* //} */
