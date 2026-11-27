#include "SObjectBrowser.h"

#include "Async/Async.h"
#include "Brushes/SlateDynamicImageBrush.h"
#include "IImageWrapperModule.h"
#include "ImageCore.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Modules/ModuleManager.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "ObjectBrowser"

/* FThumbnailCache //{ */

class FThumbnailCache : public TSharedFromThis<FThumbnailCache>
{
public:
  FThumbnailCache()
  {
    FModuleManager::LoadModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper"));
  }

  /** Null until the image is decoded, and for one that failed. */
  const FSlateBrush* Get(const FString& Path)
  {
    FEntry& Entry   = Entries.FindOrAdd(Path);
    Entry.LastUsed  = GFrameCounter;

    if (!Entry.bRequested)
    {
      Entry.bRequested = true;
      Waiting.Add(Path);
    }

    return Entry.Brush.Get();
  }

  void Tick()
  {
    while (InFlight < MaxInFlight && !Waiting.IsEmpty())
    {
      Load(Waiting.Pop());
    }

    if (Entries.Num() <= MaxEntries)
    {
      return;
    }

    Entries.ValueSort([](const FEntry& A, const FEntry& B) { return A.LastUsed > B.LastUsed; });

    TArray<FString> Stale;
    int32           Index = 0;
    for (const auto& [Path, Entry] : Entries)
    {
      if (Index++ >= MaxEntries && Entry.LastUsed < GFrameCounter && !Entry.bLoading)
      {
        Stale.Add(Path);
      }
    }

    for (const FString& Path : Stale)
    {
      Entries.Remove(Path);
      Waiting.Remove(Path);
    }
  }

private:
  struct FEntry
  {
    TSharedPtr<FSlateDynamicImageBrush> Brush;
    uint64                              LastUsed   = 0;
    bool                                bRequested = false;
    bool                                bLoading   = false;
  };

  void Load(const FString& Path)
  {
    FEntry* Entry = Entries.Find(Path);
    if (!Entry)
    {
      return;
    }

    Entry->bLoading = true;
    InFlight++;

    Async(EAsyncExecution::ThreadPool, [Weak = AsShared().ToWeakPtr(), Path]() {
      TArray<uint8> Bytes;
      FImage        Decoded;
      FImage        Small;

      const bool bDecoded = FFileHelper::LoadFileToArray(Bytes, *Path) &&
                            FModuleManager::GetModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper")).DecompressImage(Bytes.GetData(), Bytes.Num(), Decoded);

      if (bDecoded)
      {
        const double Fit = double(Size) / FMath::Max(Decoded.SizeX, Decoded.SizeY);
        Decoded.ResizeTo(Small, FMath::Max(1, int32(Decoded.SizeX * Fit)), FMath::Max(1, int32(Decoded.SizeY * Fit)), ERawImageFormat::BGRA8, EGammaSpace::sRGB);
      }

      AsyncTask(ENamedThreads::GameThread, [Weak, Path, bDecoded, Small = MoveTemp(Small)]() {
        if (const TSharedPtr<FThumbnailCache> Cache = Weak.Pin())
        {
          Cache->Finish(Path, bDecoded ? &Small : nullptr);
        }
      });
    });
  }

  void Finish(const FString& Path, const FImage* Image)
  {
    InFlight--;

    FEntry* Entry = Entries.Find(Path);
    if (!Entry)
    {
      return;
    }

    Entry->bLoading = false;

    if (Image)
    {
      Entry->Brush = FSlateDynamicImageBrush::CreateWithImageData(FName(*FString::Printf(TEXT("FFThumb_%s"), *Path)), FVector2D(Image->SizeX, Image->SizeY),
                                                                  TArray<uint8>(Image->RawData));
    }
  }

  static constexpr int32 Size        = 128;
  static constexpr int32 MaxInFlight = 8;
  static constexpr int32 MaxEntries  = 400;

  TMap<FString, FEntry> Entries;
  TArray<FString>       Waiting;
  int32                 InFlight = 0;
};

/* //} */

namespace
{

FSlateFontInfo Font(const int32 Size, const TCHAR* Style = TEXT("Regular"))
{
  return FCoreStyle::GetDefaultFontStyle(Style, Size);
}

FLinearColor ClassColor(const FString& Class)
{
  return FLinearColor::MakeFromHSV8(uint8(GetTypeHash(Class) & 0xFF), 110, 55);
}

FText Label(const FLibraryObject& Object)
{
  return FText::FromString(Object.Description.IsEmpty() ? FPaths::GetBaseFilename(Object.Id) : Object.Description);
}

FText Tooltip(const FLibraryObject& Object)
{
  TArray<FString> Lines;
  Lines.Add(Object.Class.IsEmpty() ? Object.Library : FString::Printf(TEXT("%s  ·  %s"), *Object.Class, *Object.Library));

  if (Object.HeightM > 0.0)
  {
    Lines.Add(FString::Printf(TEXT("%.2f m tall"), Object.HeightM));
  }

  if (!Object.Description.IsEmpty())
  {
    Lines.Add(Object.Description);
  }

  if (!Object.License.IsEmpty())
  {
    Lines.Add(FString::Printf(TEXT("license: %s"), *Object.License));
  }

  Lines.Add(Object.SpawnName);

  return FText::FromString(FString::Join(Lines, TEXT("\n")));
}

}  // namespace

/* Construct() //{ */

void SObjectBrowser::Construct(const FArguments& InArgs)
{
  GetLibrary = InArgs._Library;
  OnPicked   = InArgs._OnPicked;
  Thumbnails = MakeShared<FThumbnailCache>();

  const FLinearColor Muted(0.55f, 0.58f, 0.62f);

  ChildSlot[
    SNew(SBorder)
    .BorderImage(&PanelBrush)
    .Padding(12.0f)
    [
      SNew(SVerticalBox)

      + SVerticalBox::Slot().AutoHeight()
      [
        SNew(SHorizontalBox)
        + SHorizontalBox::Slot().FillWidth(1.0f).VAlign(VAlign_Center)
        [
          SNew(STextBlock).Font(Font(14, TEXT("Bold"))).Text(LOCTEXT("Title", "Object library"))
        ]
        + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
        [
          SNew(STextBlock).Font(Font(10)).ColorAndOpacity(Muted).Text_Lambda([this]() {
            return FText::Format(LOCTEXT("Count", "{0} {0}|plural(one=object,other=objects)"), Total);
          })
        ]
      ]

      + SVerticalBox::Slot().AutoHeight().Padding(0.0f, 10.0f, 0.0f, 0.0f)
      [
        SAssignNew(SearchBox, SSearchBox)
        .HintText(LOCTEXT("Hint", "Search names, classes and descriptions"))
        .OnTextChanged_Lambda([this](const FText& Text) {
          Filter = Text.ToString().TrimStartAndEnd();
          RefreshItems();
        })
      ]

      + SVerticalBox::Slot().AutoHeight().Padding(0.0f, 6.0f, 0.0f, 0.0f)
      [
        SNew(SHorizontalBox)
        + SHorizontalBox::Slot().FillWidth(1.0f)
        [
          SAssignNew(ClassCombo, SComboBox<FOptionPtr>)
          .OptionsSource(&ClassOptions)
          .OnGenerateWidget(this, &SObjectBrowser::MakeOptionRow)
          .OnSelectionChanged_Lambda([this](FOptionPtr Option, ESelectInfo::Type) {
            if (Option && Option != SelectedClass)
            {
              SelectedClass = Option;
              RefreshItems();
            }
          })
          .Content()[SNew(STextBlock).Font(Font(10)).Text_Lambda([this]() { return SelectedClass ? SelectedClass->Label : FText::GetEmpty(); })]
        ]
        + SHorizontalBox::Slot().AutoWidth().Padding(6.0f, 0.0f, 0.0f, 0.0f)
        [
          SAssignNew(LibraryCombo, SComboBox<FOptionPtr>)
          .Visibility_Lambda([this]() { return LibraryOptions.Num() > 2 ? EVisibility::Visible : EVisibility::Collapsed; })
          .OptionsSource(&LibraryOptions)
          .OnGenerateWidget(this, &SObjectBrowser::MakeOptionRow)
          .OnSelectionChanged_Lambda([this](FOptionPtr Option, ESelectInfo::Type) {
            if (Option && Option != SelectedLibrary)
            {
              SelectedLibrary = Option;
              if (const FObjectLibrary* Source = GetLibrary())
              {
                RefreshOptions(*Source);
              }
              RefreshItems();
            }
          })
          .Content()[SNew(STextBlock).Font(Font(10)).Text_Lambda([this]() { return SelectedLibrary ? SelectedLibrary->Label : FText::GetEmpty(); })]
        ]
      ]

      + SVerticalBox::Slot().FillHeight(1.0f).Padding(0.0f, 10.0f, 0.0f, 0.0f)
      [
        SAssignNew(Tiles, STileView<FItem>)
        .ListItemsSource(&Items)
        .ItemWidth(128.0f)
        .ItemHeight(150.0f)
        .SelectionMode(ESelectionMode::Single)
        .OnGenerateTile(this, &SObjectBrowser::MakeTile)
        .OnSelectionChanged(this, &SObjectBrowser::OnSelectionChanged)
      ]

      + SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 0.0f)
      [
        SNew(STextBlock)
        .Font(Font(10))
        .AutoWrapText(true)
        .ColorAndOpacity(FLinearColor(0.95f, 0.78f, 0.35f))
        .Visibility_Lambda([this]() { return Status.IsEmpty() ? EVisibility::Collapsed : EVisibility::Visible; })
        .Text_Lambda([this]() { return Status; })
      ]

      + SVerticalBox::Slot().AutoHeight().Padding(0.0f, 6.0f, 0.0f, 0.0f)
      [
        SNew(STextBlock)
        .Font(Font(10))
        .AutoWrapText(true)
        .ColorAndOpacity(FLinearColor(0.75f, 0.9f, 1.0f))
        .Text_Lambda([this]() { return Selection; })
      ]

      + SVerticalBox::Slot().AutoHeight().Padding(0.0f, 6.0f, 0.0f, 0.0f)
      [
        SNew(STextBlock)
        .Font(Font(9))
        .AutoWrapText(true)
        .ColorAndOpacity(Muted)
        .Text(LOCTEXT("Help", "Pick an object and click to drop it, shift-click to drop another. Click a placed object to select it: drag it over "
                              "surfaces, along an arrow, or around the ring; the wheel turns it. Del removes, Ctrl+D duplicates, Ctrl+Z and Ctrl+Y "
                              "undo and redo, G snaps, Ctrl+S saves the scene. Hold the right button to look, WASD and Q/E to fly, Tab hides this panel."))
      ]
    ]
  ];
}

/* //} */

/* Tick() //{ */

void SObjectBrowser::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
  SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

  Thumbnails->Tick();

  const FObjectLibrary* Source = GetLibrary();
  if (Source && Source->GetRevision() != Revision)
  {
    Revision = Source->GetRevision();
    RefreshOptions(*Source);
    RefreshItems();
  }
}

/* //} */

/* RefreshOptions() //{ */

void SObjectBrowser::RefreshOptions(const FObjectLibrary& Source)
{
  const FString Library = SelectedLibrary ? SelectedLibrary->Value : FString();
  const FString Class   = SelectedClass ? SelectedClass->Value : FString();

  LibraryOptions.Reset();
  LibraryOptions.Add(MakeShared<FOption>(FOption{FString(), LOCTEXT("AllLibraries", "All libraries")}));
  SelectedLibrary = LibraryOptions[0];

  for (const FString& Name : Source.Libraries())
  {
    LibraryOptions.Add(MakeShared<FOption>(FOption{Name, FText::FromString(Name)}));
    if (Name == Library)
    {
      SelectedLibrary = LibraryOptions.Last();
    }
  }

  ClassOptions.Reset();
  ClassOptions.Add(MakeShared<FOption>(FOption{FString(), LOCTEXT("AllClasses", "All classes")}));
  SelectedClass = ClassOptions[0];

  for (const auto& [Name, Count] : Source.Classes(SelectedLibrary->Value))
  {
    ClassOptions.Add(MakeShared<FOption>(FOption{Name, FText::Format(LOCTEXT("ClassCount", "{0}  ({1})"), FText::FromString(Name), FText::AsNumber(Count))}));
    if (Name == Class)
    {
      SelectedClass = ClassOptions.Last();
    }
  }

  LibraryCombo->RefreshOptions();
  ClassCombo->RefreshOptions();
}

/* //} */

/* RefreshItems() //{ */

void SObjectBrowser::RefreshItems()
{
  Items.Reset();
  Total = 0;

  if (const FObjectLibrary* Source = GetLibrary())
  {
    for (FLibraryObject& Object :
         Source->Search(SelectedLibrary ? SelectedLibrary->Value : FString(), SelectedClass ? SelectedClass->Value : FString(), Filter, 0, MAX_int32, Total))
    {
      Items.Add(MakeShared<FLibraryObject>(MoveTemp(Object)));
    }
  }

  Tiles->RequestListRefresh();
  Tiles->ScrollToTop();
}

/* //} */

/* MakeTile() //{ */

TSharedRef<ITableRow> SObjectBrowser::MakeTile(FItem Item, const TSharedRef<STableViewBase>& Owner)
{
  return SNew(STableRow<FItem>, Owner)
      .Padding(4.0f)
      .ToolTipText(Tooltip(*Item))
      [
        SNew(SVerticalBox)
        + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center)
        [
          SNew(SBox).WidthOverride(112.0f).HeightOverride(112.0f)[MakeThumbnail(Item)]
        ]
        + SVerticalBox::Slot().AutoHeight().Padding(2.0f, 4.0f, 2.0f, 0.0f)
        [
          SNew(STextBlock).Font(Font(9)).OverflowPolicy(ETextOverflowPolicy::Ellipsis).Text(Label(*Item))
        ]
      ];
}

/* //} */

/* MakeThumbnail() //{ */

TSharedRef<SWidget> SObjectBrowser::MakeThumbnail(const FItem& Item) const
{
  const FString Path = Item->Thumbnail;

  TSharedRef<SWidget> Placeholder = SNew(SBorder)
      .BorderImage(&PlaceholderBrush)
      .BorderBackgroundColor(ClassColor(Item->Class))
      .HAlign(HAlign_Center)
      .VAlign(VAlign_Center)
      .Padding(6.0f)
      [
        SNew(STextBlock).Font(Font(11, TEXT("Bold"))).Justification(ETextJustify::Center).AutoWrapText(true)
        .Text(FText::FromString(Item->Class.IsEmpty() ? Item->Library : Item->Class.Replace(TEXT("_"), TEXT(" "))))
      ];

  if (Path.IsEmpty())
  {
    return Placeholder;
  }

  TWeakPtr<FThumbnailCache> Cache = Thumbnails;

  return SNew(SBorder)
      .BorderImage(&TileBrush)
      .Padding(0.0f)
      [
        SNew(SImage).Image_Lambda([Cache, Path]() -> const FSlateBrush* {
          const TSharedPtr<FThumbnailCache> Pinned = Cache.Pin();
          return Pinned ? Pinned->Get(Path) : nullptr;
        })
      ];
}

/* //} */

/* MakeOptionRow() //{ */

TSharedRef<SWidget> SObjectBrowser::MakeOptionRow(FOptionPtr Option) const
{
  return SNew(STextBlock).Font(Font(10)).Text(Option->Label);
}

/* //} */

/* OnSelectionChanged() //{ */

void SObjectBrowser::OnSelectionChanged(FItem Item, ESelectInfo::Type How)
{
  if (Item && How != ESelectInfo::Direct)
  {
    OnPicked.ExecuteIfBound(*Item);
  }
}

/* //} */

TSharedPtr<SWidget> SObjectBrowser::GetSearchBox() const
{
  return SearchBox;
}

void SObjectBrowser::ClearPicked()
{
  Tiles->ClearSelection();
}

void SObjectBrowser::SetStatus(const FText& InStatus)
{
  Status = InStatus;
}

void SObjectBrowser::SetSelection(const FText& InSelection)
{
  Selection = InSelection;
}

#undef LOCTEXT_NAMESPACE
