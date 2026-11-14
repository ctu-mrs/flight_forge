#pragma once

#include <atomic>

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

	/** An explicit class (scene editor), else the rules', else the folder's, else 0. Any thread. */
	int32 ClassOf(const AActor* Actor, const UPrimitiveComponent* Component) const;

	/** A class the spawn config named for this actor; beats every rule. */
	void SetExplicitClass(const AActor* Actor, int32 Class);

	/** 1..255 for a Nanite actor, 0 once they run out. */
	uint8 GetOrAssignStencilSlot(const AActor* Actor);

	/** Where ClassOf's answer came from: "explicit", "rule:<match>", "folder" or "" for unclassified. For the audit. */
	FString ClassSourceOf(const AActor* Actor, const UPrimitiveComponent* Component) const;

	static FString MeshAssetPath(const UPrimitiveComponent* Component);

	/** every class the world knows (the yaml tree plus any ini rule id), by id, and a hash over them */
	const TMap<int32, FSegClass>& Classes() const { return ClassesById; }
	uint64 ClassMapHash() const;

	/** a tree path ("prop/box", any case) or a bare id ("62"); 0 when neither names a class */
	int32 ClassIdOf(const FString& PathOrId) const;
	FString ClassPathOf(int32 Id) const;

	/**
	 * Snapshot for one ID-pass render: every static mesh component's proxy with its
	 * owner's id and class packed as a colour, plus the id -> class/actor map that pairs with the image.
	 * OutStencilSlots is the 1..255 custom-stencil slot of each Nanite actor mapped to the same colour:
	 * Nanite never enters a mesh pass, but it does write custom depth/stencil, so the slot is the only
	 * id that survives the render (see docs/asset_classification.md).
	 */
	void CollectInstanceItems(TArray<struct FSegIdDrawItem>& OutItems, TArray<FSegInstanceEntry>& OutMap, TArray<FLinearColor>& OutStencilSlots);

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

	// 1..255, handed out once per Nanite actor and written to its components' custom stencil
	TMap<TWeakObjectPtr<const AActor>, uint8> StencilSlots;
	uint8 NextStencilSlot = 1;
	bool  bWarnedSlotsOut = false;

	TMap<TWeakObjectPtr<const AActor>, int32> ExplicitClasses;
	mutable FRWLock                           ExplicitClassesLock;  // the lidar reads classes on its server thread

	FCriticalSection InstanceIdsLock;  // the lidar resolves ids on its server thread

	const FSegmentationTagRule* FindRule(const AActor* Actor, const UPrimitiveComponent* Component) const;

	// game thread: ClassOf and actor paths per capture, recomputed only when the mesh, tags or class rules change
	struct FCachedClass
	{
		const UObject* Mesh       = nullptr;
		uint32         TagsHash   = 0;
		uint32         Generation = 0;
		int32          Class      = 0;
	};
	int32          CachedClassOf(const AActor* Actor, const UPrimitiveComponent* Component);
	const FString& CachedPath(const AActor* Actor);

	TMap<TWeakObjectPtr<const UPrimitiveComponent>, FCachedClass> ClassCache;
	TMap<TWeakObjectPtr<const AActor>, FString>                   PathCache;
	std::atomic<uint32>                                           ClassGeneration{1};
	uint32                                                        CollectCalls = 0;
};

//}
