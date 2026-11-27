#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "SettingsMenuToggle.generated.h"

class APlayerController;
class UUserWidget;

// M or Escape opens and closes the settings menu in any world; with it closed the player flies without a click
UCLASS(NotBlueprintable)
class FLIGHTFORGECORE_API ASettingsMenuToggle : public AInfo
{
	GENERATED_BODY()

public:
	ASettingsMenuToggle();

	virtual void Tick(float DeltaSeconds) override;

	void Toggle();

	bool IsOpen() const;

private:
	void Bind(APlayerController* Player);
	void Open(APlayerController* Player);
	void Close(APlayerController* Player);

	static void PlayGame(APlayerController* Player);
	static bool IsTyping();

	UPROPERTY()
	TSubclassOf<UUserWidget> MenuClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> Menu;

	bool bCursorBeforeOpen = false;
};
