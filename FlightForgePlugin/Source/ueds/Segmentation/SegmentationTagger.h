#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Subsystems/WorldSubsystem.h"
#include "SegmentationTagger.generated.h"


UENUM()
enum class ESegmentationMatchType : uint8
{
	AssetPath  UMETA(ToolTip = "wildcard against the static mesh asset path"),
	ActorClass UMETA(ToolTip = "wildcard against the actor class name"),
	ActorTag   UMETA(ToolTip = "exact actor tag"),
};

USTRUCT()
struct FSegmentationTagRule
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere, Category = "Segmentation")
	ESegmentationMatchType MatchType = ESegmentationMatchType::AssetPath;

	// wildcard pattern (AssetPath/ActorClass) or tag name (ActorTag)
	UPROPERTY(Config, EditAnywhere, Category = "Segmentation")
	FString Match;

	UPROPERTY(Config, EditAnywhere, Category = "Segmentation")
	uint8 Stencil = 0;
};

/* USegmentationTaggerSettings //{ */

// Project Settings -> Plugins -> FlightForge Segmentation, or DefaultGame.ini
// [/Script/ueds.SegmentationTaggerSettings]. First matching rule wins.
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "FlightForge Segmentation"))
class UEDS_API USegmentationTaggerSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Segmentation")
	TArray<FSegmentationTagRule> Rules;
};

//}

/* USegmentationTagger //{ */

// Assigns CustomDepthStencil segmentation ids from data-driven rules instead
// of per-mesh editor clicks: the whole world at BeginPlay, and every actor
// spawned afterwards (PCG / procedural content included).
UCLASS()
class UEDS_API USegmentationTagger : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;

	void TagActor(AActor* Actor);

private:
	FDelegateHandle ActorSpawnedHandle;

	const FSegmentationTagRule* FindRule(const AActor* Actor, const UPrimitiveComponent* Component) const;
};

//}
