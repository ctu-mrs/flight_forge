#include "SSceneGizmo.h"

#include "Rendering/DrawElements.h"
#include "Rendering/DrawElementTypes.h"

namespace
{

const FLinearColor AxisColors[3] = {FLinearColor(0.95f, 0.22f, 0.2f), FLinearColor(0.3f, 0.85f, 0.25f), FLinearColor(0.25f, 0.5f, 1.0f)};
const FLinearColor BoxColor(1.0f, 0.78f, 0.25f, 0.9f);
const FLinearColor RingColor(0.3f, 0.9f, 0.95f, 0.9f);
const FLinearColor HotColor(1.0f, 1.0f, 0.55f);

bool IsOnScreen(const FVector2D& Point)
{
  return !FMath::IsNaN(Point.X);
}

}  // namespace

void SSceneGizmo::Construct(const FArguments& InArgs)
{
  View = InArgs._View;
}

FVector2D SSceneGizmo::ComputeDesiredSize(float) const
{
  return FVector2D::ZeroVector;
}

/* OnPaint() //{ */

int32 SSceneGizmo::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
                           int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
  if (!View || !View->bVisible || !IsOnScreen(View->Pivot))
  {
    return LayerId;
  }

  // the view is in viewport pixels, this widget lays out in DPI-scaled units
  const float Scale = AllottedGeometry.Scale;

  const auto Line = [&](const FVector2D& A, const FVector2D& B, const FLinearColor& Color, const float Thickness) {
    if (IsOnScreen(A) && IsOnScreen(B))
    {
      FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(), TArray<FVector2D>{A / Scale, B / Scale}, ESlateDrawEffect::None, Color, true, Thickness);
    }
  };

  const auto Hot = [this](const EGizmoHandle Handle) { return View->Active == Handle || (View->Active == EGizmoHandle::None && View->Hovered == Handle); };

  for (int32 Corner = 0; Corner < 8; Corner++)
  {
    for (int32 Bit = 1; Bit < 8; Bit <<= 1)
    {
      if (!(Corner & Bit))
      {
        Line(View->Box[Corner], View->Box[Corner | Bit], BoxColor, 1.5f);
      }
    }
  }

  const bool bRingHot = Hot(EGizmoHandle::Ring);
  for (int32 Index = 0; Index < View->Ring.Num(); Index++)
  {
    Line(View->Ring[Index], View->Ring[(Index + 1) % View->Ring.Num()], bRingHot ? HotColor : RingColor, bRingHot ? 4.0f : 2.5f);
  }

  for (int32 Axis = 0; Axis < 3; Axis++)
  {
    const FVector2D     End   = View->Axes[Axis];
    const bool          bHot  = Hot(EGizmoHandle(int32(EGizmoHandle::X) + Axis));
    const FLinearColor& Color = bHot ? HotColor : AxisColors[Axis];

    if (!IsOnScreen(End) || FVector2D::Distance(End, View->Pivot) < 4.0)
    {
      continue;
    }

    const FVector2D Along = (End - View->Pivot).GetSafeNormal();
    const FVector2D Across(-Along.Y, Along.X);

    Line(View->Pivot, End, Color, bHot ? 5.0f : 3.5f);
    Line(End, End - Along * 14.0 + Across * 7.0, Color, bHot ? 5.0f : 3.5f);
    Line(End, End - Along * 14.0 - Across * 7.0, Color, bHot ? 5.0f : 3.5f);
  }

  return LayerId + 1;
}

/* //} */
