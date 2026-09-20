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

	// the class id; written to the stencil as well when it fits in 8 bits, for the legacy seg camera
	UPROPERTY(Config, EditAnywhere, Category = "Segmentation")
	uint8 Stencil = 0;
};

// one instance the pass drew: its id, class and actor path
struct FSegInstanceEntry
{
	int32   Id      = 0;
	int32   ClassId = 0;
	FString Actor;
};

/* USegmentationTaggerSettings //{ */

// Project Settings -> Plugins -> FlightForge Segmentation, or DefaultGame.ini
// [/Script/FlightForgeCore.SegmentationTaggerSettings]. First matching rule wins.
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "FlightForge Segmentation"))
class FLIGHTFORGECORE_API USegmentationTaggerSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Segmentation")
	TArray<FSegmentationTagRule> Rules;

	// YAML class tree under the project's Config dir (classes, roots, rules - see docs/asset_classification.md);
	// its rules run after the ini rules above
	UPROPERTY(Config, EditAnywhere, Category = "Segmentation")
	FString ClassMapFile = TEXT("FlightForgeClasses.yaml");
};

// one node of the class tree
struct FSegClass
{
	int32   Id       = 0;
	int32   ParentId = 0;
	FString Path;  // "vegetation/tree/conifer"
};

//}

/* USegmentationTagger //{ */

// Assigns CustomDepthStencil segmentation ids from data-driven rules instead
// of per-mesh editor clicks: the whole world at BeginPlay, and every actor
// spawned afterwards (PCG / procedural content included).
UCLASS()
class FLIGHTFORGECORE_API USegmentationTagger : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;

	void TagActor(AActor* Actor);

	/** Stable per-actor instance id (starting at 1) shared by the SegId pass and the lidar labels; 0 stays "background". Any thread. */
	uint32 GetOrAssignInstanceId(const AActor* Actor);

	/** The class the rules give this component, else the stencil it already carries, else 0. */
	int32 ClassOf(const AActor* Actor, const UPrimitiveComponent* Component) const;

	/** every class the world knows (the yaml tree plus any ini rule id), by id, and a hash over them */
	const TMap<int32, FSegClass>& Classes() const { return ClassesById; }
	uint64 ClassMapHash() const;

	/**
	 * Snapshot for one ID-pass render: every static mesh component's proxy with its
	 * owner's id and class packed as a colour, plus the id -> class/actor map that pairs with the image.
	 */
	void CollectInstanceItems(TArray<struct FSegIdDrawItem>& OutItems, TArray<FSegInstanceEntry>& OutMap);

private:
	FDelegateHandle ActorSpawnedHandle;

	void LoadClassMap();

	TArray<FSegmentationTagRule> Rules;  // ini rules first, then the yaml's
	TMap<int32, FSegClass>       ClassesById;
	TMap<FString, int32>         IdByPath;
	TArray<FString>              ClassifiedRoots;  // content folders whose sub-folders name the class path

	int32 ClassFromFolder(const FString& AssetPath) const;

	TMap<TWeakObjectPtr<const AActor>, uint32> InstanceIds;
	uint32 NextInstanceId = 1;
	FCriticalSection InstanceIdsLock;  // the lidar resolves ids on its server thread

	const FSegmentationTagRule* FindRule(const AActor* Actor, const UPrimitiveComponent* Component) const;
	static FString              MeshAssetPath(const UPrimitiveComponent* Component);
};

//}
