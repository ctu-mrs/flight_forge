#include "SettingsMenuToggle.h"

#include "Blueprint/UserWidget.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"

#include "FlightForgeCore/SceneEdit/SceneEditor.h"

ASettingsMenuToggle::ASettingsMenuToggle()
{
	PrimaryActorTick.bCanEverTick        = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> MenuFinder(TEXT("/FlightForgePlugin/Blueprints/SettingsMenu"));
	if (MenuFinder.Succeeded())
	{
		MenuClass = MenuFinder.Class;
	}
}

/* Tick() //{ */

// the local player may log in after the game mode's BeginPlay
void ASettingsMenuToggle::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (APlayerController* Player = GetWorld()->GetFirstPlayerController())
	{
		Bind(Player);
		SetActorTickEnabled(false);
	}
}

/* //} */

/* Bind() //{ */

void ASettingsMenuToggle::Bind(APlayerController* Player)
{
	EnableInput(Player);

	// lockstep keeps the world paused between steps
	InputComponent->BindKey(EKeys::M, IE_Pressed, this, &ASettingsMenuToggle::Toggle).bExecuteWhenPaused      = true;
	InputComponent->BindKey(EKeys::Escape, IE_Pressed, this, &ASettingsMenuToggle::Toggle).bExecuteWhenPaused = true;

	PlayGame(Player);
}

/* //} */

/* Toggle() //{ */

void ASettingsMenuToggle::Toggle()
{
	APlayerController* Player = GetWorld()->GetFirstPlayerController();
	const USceneEditor* Editor = GetWorld()->GetSubsystem<USceneEditor>();
	if (!Player || IsTyping() || (Editor && Editor->IsOpen()))
	{
		return;
	}

	IsOpen() ? Close(Player) : Open(Player);
}

bool ASettingsMenuToggle::IsOpen() const
{
	return Menu && Menu->IsInViewport();
}

/* //} */

/* Open() / Close() //{ */

void ASettingsMenuToggle::Open(APlayerController* Player)
{
	if (!Menu && MenuClass)
	{
		Menu = CreateWidget<UUserWidget>(Player, MenuClass);
	}
	if (!Menu)
	{
		return;
	}

	bCursorBeforeOpen = Player->ShouldShowMouseCursor();
	Menu->AddToViewport(200);

	FInputModeGameAndUI Mode;
	Mode.SetWidgetToFocus(Menu->TakeWidget());
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	Mode.SetHideCursorDuringCapture(false);
	Player->SetInputMode(Mode);
	Player->SetShowMouseCursor(true);
}

void ASettingsMenuToggle::Close(APlayerController* Player)
{
	Menu->RemoveFromParent();

	// another UI (ModelForge's scene menu) still wants the cursor
	if (bCursorBeforeOpen)
	{
		FInputModeGameAndUI Mode;
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		Mode.SetHideCursorDuringCapture(false);
		Player->SetInputMode(Mode);
		return;
	}

	PlayGame(Player);
}

/* //} */

/* PlayGame() / IsTyping() //{ */

void ASettingsMenuToggle::PlayGame(APlayerController* Player)
{
	Player->SetInputMode(FInputModeGameOnly());
	Player->SetShowMouseCursor(false);
}

// a key a text box leaves unhandled still reaches the game
bool ASettingsMenuToggle::IsTyping()
{
	if (!FSlateApplication::IsInitialized())
	{
		return false;
	}

	const TSharedPtr<SWidget> Focus = FSlateApplication::Get().GetKeyboardFocusedWidget();
	return Focus && (Focus->GetType() == TEXT("SEditableText") || Focus->GetType() == TEXT("SMultiLineEditableText"));
}

/* //} */
