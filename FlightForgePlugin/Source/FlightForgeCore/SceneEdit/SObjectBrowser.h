#pragma once

#include "CoreMinimal.h"
#include "Brushes/SlateRoundedBoxBrush.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Views/STileView.h"

#include "FlightForgeCore/Utils/ObjectLibrary.h"

class FThumbnailCache;
class SSearchBox;

DECLARE_DELEGATE_OneParam(FOnLibraryObjectPicked, const FLibraryObject&);

/* SObjectBrowser //{ */

/** The library as a searchable grid of tiles; picking a tile hands the object to the placement tool. */
class SObjectBrowser : public SCompoundWidget
{
public:
  SLATE_BEGIN_ARGS(SObjectBrowser) {}
  SLATE_ARGUMENT(TFunction<const FObjectLibrary*()>, Library)
  SLATE_EVENT(FOnLibraryObjectPicked, OnPicked)
  SLATE_END_ARGS()

  void Construct(const FArguments& InArgs);

  virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

  TSharedPtr<SWidget> GetSearchBox() const;

  void ClearPicked();

  void SetStatus(const FText& InStatus);

private:
  using FItem = TSharedPtr<FLibraryObject>;

  struct FOption
  {
    FString Value;
    FText   Label;
  };

  using FOptionPtr = TSharedPtr<FOption>;

  void RefreshOptions(const FObjectLibrary& Source);

  void RefreshItems();

  TSharedRef<ITableRow> MakeTile(FItem Item, const TSharedRef<STableViewBase>& Owner);

  TSharedRef<SWidget> MakeThumbnail(const FItem& Item) const;

  TSharedRef<SWidget> MakeOptionRow(FOptionPtr Option) const;

  void OnSelectionChanged(FItem Item, ESelectInfo::Type How);

  TFunction<const FObjectLibrary*()> GetLibrary;
  FOnLibraryObjectPicked             OnPicked;

  TArray<FItem>      Items;
  TArray<FOptionPtr> LibraryOptions;
  TArray<FOptionPtr> ClassOptions;
  FOptionPtr         SelectedLibrary;
  FOptionPtr         SelectedClass;
  FString            Filter;
  int32              Total    = 0;
  int32              Revision = -1;
  FText              Status;

  TSharedPtr<STileView<FItem>>       Tiles;
  TSharedPtr<SSearchBox>             SearchBox;
  TSharedPtr<SComboBox<FOptionPtr>>  LibraryCombo;
  TSharedPtr<SComboBox<FOptionPtr>>  ClassCombo;
  TSharedPtr<FThumbnailCache>        Thumbnails;

  FSlateRoundedBoxBrush PanelBrush{FLinearColor(0.012f, 0.014f, 0.018f, 0.92f), 8.0f};
  FSlateRoundedBoxBrush TileBrush{FLinearColor(0.06f, 0.065f, 0.075f, 1.0f), 4.0f};
  FSlateRoundedBoxBrush PlaceholderBrush{FLinearColor::White, 4.0f};
};

/* //} */
