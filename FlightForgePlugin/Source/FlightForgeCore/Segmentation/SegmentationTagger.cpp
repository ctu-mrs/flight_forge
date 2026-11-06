#include "SegmentationTagger.h"

#include "HAL/IConsoleManager.h"

#include "SegIdPass.h"

#include "Components/MeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "LandscapeComponent.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#include "yaml.h"
#include "FlightForgeCore/Utils/YamlUtils.h"

/* OnWorldBeginPlay() / Deinitialize() //{ */

void USegmentationTagger::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	LoadClassMap();
	if (Rules.Num() == 0 && ClassifiedRoots.Num() == 0)
	{
		return;
	}

	int32 Tagged = 0;
	for (TActorIterator<AActor> It(&InWorld); It; ++It)
	{
		TagActor(*It);
		++Tagged;
	}

	UE_LOG(LogTemp, Log, TEXT("SegmentationTagger: %d rules, %d roots, %d classes, over %d actors"), Rules.Num(), ClassifiedRoots.Num(), ClassesById.Num(), Tagged);

	ActorSpawnedHandle = InWorld.AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateUObject(this, &USegmentationTagger::TagActor));
}

void USegmentationTagger::Deinitialize()
{
	if (UWorld* World = GetWorld())
	{
		World->RemoveOnActorSpawnedHandler(ActorSpawnedHandle);
	}

	Super::Deinitialize();
}

//}

/* LoadClassMap() //{ */

// Config/FlightForgeClasses.yaml - a class tree, ids append-only:
//   classes:
//     - {id: 1, path: terrain}
//     - {id: 5, path: vegetation/tree}
//     - {id: 6, path: vegetation/tree/conifer}
//   roots: [/Game/Assets]           # folders below a root name the class path: /Game/Assets/Vegetation/Tree/Conifer/SM_x
//   rules:                          # explicit, first match wins, before the folder derivation
//     - {path: "/Game/EuropeanBeech/*", class: vegetation/tree/broadleaf}
//     - {class_name: "Landscape*", class: terrain}
//     - {tag: forest, class: vegetation/tree}
void USegmentationTagger::LoadClassMap()
{
	const USegmentationTaggerSettings* Settings = GetDefault<USegmentationTaggerSettings>();

	Rules.Reset();
	ClassesById.Reset();
	IdByPath.Reset();
	ClassifiedRoots.Reset();

	auto AddClass = [this](const int32 Id, const FString& Path) {
		FSegClass Class;
		Class.Id   = Id;
		Class.Path = Path.ToLower();
		int32 Slash;
		if (Class.Path.FindLastChar(TEXT('/'), Slash))
		{
			if (const int32* Parent = IdByPath.Find(Class.Path.Left(Slash)))
			{
				Class.ParentId = *Parent;
			}
		}
		ClassesById.Add(Id, Class);
		IdByPath.Add(Class.Path, Id);
	};

	if (Settings)
	{
		Rules = Settings->Rules;
		for (const FSegmentationTagRule& Rule : Rules)
		{
			if (!ClassesById.Contains(Rule.Stencil))
			{
				AddClass(Rule.Stencil, FString::Printf(TEXT("class_%d"), Rule.Stencil));
			}
		}
	}

	const FString Path = Settings && !Settings->ClassMapFile.IsEmpty() ? FPaths::Combine(FPaths::ProjectConfigDir(), Settings->ClassMapFile) : FString();
	FString       Text;
	if (Path.IsEmpty() || !FFileHelper::LoadFileToString(Text, *Path))
	{
		return;
	}

	try
	{
		const YAML::Node Root = YAML::Load(TCHAR_TO_UTF8(*Text));

		// parents must exist before children, so shorter paths first
		TArray<TPair<int32, FString>> Declared;
		for (const YAML::Node& Node : Root["classes"])
		{
			if (Node["id"] && Node["path"])
			{
				Declared.Emplace(Node["id"].as<int32>(), ToFString(Node["path"].as<std::string>()));
			}
		}
		Declared.Sort([](const TPair<int32, FString>& A, const TPair<int32, FString>& B) { return A.Value.Len() < B.Value.Len(); });
		for (const auto& Class : Declared)
		{
			AddClass(Class.Key, Class.Value);
		}

		for (const YAML::Node& Node : Root["roots"])
		{
			ClassifiedRoots.Add(ToFString(Node.as<std::string>()).ToLower());
		}

		for (const YAML::Node& Node : Root["rules"])
		{
			FSegmentationTagRule Rule;
			if (Node["path"])
			{
				Rule.MatchType = ESegmentationMatchType::AssetPath;
				Rule.Match     = ToFString(Node["path"].as<std::string>());
			}
			else if (Node["class_name"])
			{
				Rule.MatchType = ESegmentationMatchType::ActorClass;
				Rule.Match     = ToFString(Node["class_name"].as<std::string>());
			}
			else if (Node["tag"])
			{
				Rule.MatchType = ESegmentationMatchType::ActorTag;
				Rule.Match     = ToFString(Node["tag"].as<std::string>());
			}
			else
			{
				continue;
			}

			const int32 Id = Node["class"] && Node["class"].IsScalar() ? ClassIdOf(ToFString(Node["class"].as<std::string>())) : -1;
			if (Id <= 0 || Id > 255)
			{
				UE_LOG(LogTemp, Warning, TEXT("SegmentationTagger: rule '%s' has no usable class (ids are 1..255 while the stencil carries them)"), *Rule.Match);
				continue;
			}
			Rule.Stencil = static_cast<uint8>(Id);
			Rules.Add(Rule);
		}
	}
	catch (const YAML::Exception& Exception)
	{
		UE_LOG(LogTemp, Error, TEXT("SegmentationTagger: %s: %hs"), *Path, Exception.what());
	}
}

// /Game/Assets/Vegetation/Tree/Conifer/SM_Spruce -> "vegetation/tree/conifer"; the deepest registered prefix wins,
// so an asset in a sub-folder nobody registered yet still gets its parent's class
int32 USegmentationTagger::ClassFromFolder(const FString& AssetPath) const
{
	const FString Lower = AssetPath.ToLower();
	for (const FString& Root : ClassifiedRoots)
	{
		if (!Lower.StartsWith(Root + TEXT("/")))
		{
			continue;
		}
		FString Folder = FPaths::GetPath(Lower.Mid(Root.Len() + 1));
		while (!Folder.IsEmpty())
		{
			if (const int32* Id = IdByPath.Find(Folder))
			{
				return *Id;
			}
			Folder = FPaths::GetPath(Folder);
		}
		UE_LOG(LogTemp, Verbose, TEXT("SegmentationTagger: %s sits under a classified root but no class path matches"), *AssetPath);
		return 0;
	}
	return 0;
}

uint64 USegmentationTagger::ClassMapHash() const
{
	TArray<int32> Ids;
	ClassesById.GetKeys(Ids);
	Ids.Sort();

	uint64 Hash = 14695981039346656037ull;
	for (const int32 Id : Ids)
	{
		for (const TCHAR C : FString::Printf(TEXT("%d=%s;"), Id, *ClassesById[Id].Path))
		{
			Hash ^= static_cast<uint8>(C);
			Hash *= 1099511628211ull;
		}
	}
	return Hash;
}

int32 USegmentationTagger::ClassIdOf(const FString& PathOrId) const
{
	if (const int32* Known = IdByPath.Find(PathOrId.ToLower()))
	{
		return *Known;
	}
	return PathOrId.IsNumeric() ? FCString::Atoi(*PathOrId) : 0;
}

FString USegmentationTagger::ClassPathOf(const int32 Id) const
{
	const FSegClass* Class = ClassesById.Find(Id);
	return Class ? Class->Path : FString();
}

//}

/* FindRule() / ClassOf() / TagActor() //{ */

FString USegmentationTagger::MeshAssetPath(const UPrimitiveComponent* Component)
{
	if (const UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(Component))
	{
		return Mesh->GetStaticMesh() ? Mesh->GetStaticMesh()->GetPathName() : FString();
	}
	if (const USkeletalMeshComponent* Skeletal = Cast<USkeletalMeshComponent>(Component))
	{
		return Skeletal->GetSkeletalMeshAsset() ? Skeletal->GetSkeletalMeshAsset()->GetPathName() : FString();
	}
	return FString();
}

const FSegmentationTagRule* USegmentationTagger::FindRule(const AActor* Actor, const UPrimitiveComponent* Component) const
{
	for (const FSegmentationTagRule& Rule : Rules)
	{
		switch (Rule.MatchType)
		{
			case ESegmentationMatchType::AssetPath:
			{
				const FString Path = MeshAssetPath(Component);
				if (!Path.IsEmpty() && Path.MatchesWildcard(Rule.Match))
				{
					return &Rule;
				}
				break;
			}
			case ESegmentationMatchType::ActorClass:
				if (Actor->GetClass()->GetName().MatchesWildcard(Rule.Match))
				{
					return &Rule;
				}
				break;
			case ESegmentationMatchType::ActorTag:
				if (Actor->ActorHasTag(FName(*Rule.Match)))
				{
					return &Rule;
				}
				break;
		}
	}

	return nullptr;
}

// a stencil a level already carries is NOT a class: hand-tagged values predate the tree and mean something else there
int32 USegmentationTagger::ClassOf(const AActor* Actor, const UPrimitiveComponent* Component) const
{
	{
		FReadScopeLock Lock(ExplicitClassesLock);
		if (const int32* Explicit = ExplicitClasses.Find(Actor))
		{
			return *Explicit;
		}
	}
	if (const FSegmentationTagRule* Rule = FindRule(Actor, Component))
	{
		return Rule->Stencil;
	}
	return ClassFromFolder(MeshAssetPath(Component));
}

void USegmentationTagger::SetExplicitClass(const AActor* Actor, const int32 Class)
{
	FWriteScopeLock Lock(ExplicitClassesLock);
	ExplicitClasses.Add(Actor, Class);
}

uint8 USegmentationTagger::GetOrAssignStencilSlot(const AActor* Actor)
{
	if (const uint8* Existing = StencilSlots.Find(Actor))
	{
		return *Existing;
	}
	if (NextStencilSlot == 0)
	{
		return 0;  // 255 Nanite actors already named; the rest stay unlabelled rather than claim another's id
	}
	const uint8 Slot = NextStencilSlot++;
	StencilSlots.Add(Actor, Slot);
	return Slot;
}

FString USegmentationTagger::ClassSourceOf(const AActor* Actor, const UPrimitiveComponent* Component) const
{
	{
		FReadScopeLock Lock(ExplicitClassesLock);
		if (ExplicitClasses.Contains(Actor))
		{
			return TEXT("explicit");
		}
	}
	if (const FSegmentationTagRule* Rule = FindRule(Actor, Component))
	{
		return TEXT("rule:") + Rule->Match;
	}
	if (ClassFromFolder(MeshAssetPath(Component)) > 0)
	{
		return TEXT("folder");
	}
	return FString();
}

void USegmentationTagger::TagActor(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}

	TArray<UPrimitiveComponent*> Primitives;
	Actor->GetComponents<UPrimitiveComponent>(Primitives);

	// the stencil carries the class for the legacy seg camera; folder-derived classes count as well
	for (UPrimitiveComponent* Component : Primitives)
	{
		int32 Class = 0;
		if (const FSegmentationTagRule* Rule = FindRule(Actor, Component))
		{
			Class = Rule->Stencil;
		}
		else
		{
			Class = ClassFromFolder(MeshAssetPath(Component));
		}
		if (Class > 0 && Class <= 255)
		{
			Component->SetRenderCustomDepth(true);
			Component->SetCustomDepthStencilValue(Class);
		}
	}
}

//}

/* instance ids //{ */

uint32 USegmentationTagger::GetOrAssignInstanceId(const AActor* Actor)
{
	FScopeLock Lock(&InstanceIdsLock);

	if (uint32* Existing = InstanceIds.Find(Actor))
	{
		return *Existing;
	}

	const uint32 Id = NextInstanceId++;
	InstanceIds.Add(Actor, Id);

	return Id;
}

static TAutoConsoleVariable<int32> CVarSegIdDisallowNanite(TEXT("FlightForge.SegId.DisallowNanite"), 0,
                                                            TEXT("render Nanite meshes through the classic path so the instance segmentation can draw them"));

void USegmentationTagger::CollectInstanceItems(TArray<FSegIdDrawItem>& OutItems, TArray<FSegInstanceEntry>& OutMap, TArray<FLinearColor>& OutStencilSlots)
{
	OutItems.Reset();
	OutMap.Reset();
	OutStencilSlots.Reset();
	OutStencilSlots.SetNumZeroed(256);

	const bool bDisallowNanite = CVarSegIdDisallowNanite.GetValueOnGameThread() != 0;

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	TSet<uint32> MappedIds;
	int32        SlotlessNanite = 0;

	for (TActorIterator<AActor> It(World); It; ++It)
	{
		const AActor* Actor = *It;

		TInlineComponentArray<UPrimitiveComponent*> Components(Actor);

		bool                       bAnyProxy  = false;
		const UPrimitiveComponent* FirstDrawn = nullptr;

		for (UPrimitiveComponent* Component : Components)
		{
			if (!Component || !Component->SceneProxy || !Component->IsVisible())
			{
				continue;
			}
			UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(Component);
			if (!Component->IsA<UMeshComponent>() && !Component->IsA<ULandscapeComponent>())
			{
				continue;
			}

			// the pass is a classic mesh pass; Nanite geometry only gets an id when pushed onto the classic path
			const bool bNanite = StaticMesh && StaticMesh->GetStaticMesh() && StaticMesh->GetStaticMesh()->HasValidNaniteData() && !StaticMesh->IsDisallowNanite();
			if (bDisallowNanite && bNanite)
			{
				StaticMesh->bDisallowNanite = true;
				StaticMesh->MarkRenderStateDirty();
			}

			const uint32       Id    = GetOrAssignInstanceId(Actor);
			const int32        Class = FMath::Clamp(ClassOf(Actor, Component), 0, 255);
			const FLinearColor IdColor((Id & 0xFF) / 255.0f, ((Id >> 8) & 0xFF) / 255.0f, ((Id >> 16) & 0xFF) / 255.0f, Class / 255.0f);

			if (bNanite && !bDisallowNanite)
			{
				const uint8 Slot = GetOrAssignStencilSlot(Actor);
				if (Slot)
				{
					OutStencilSlots[Slot] = IdColor;
				}
				else
				{
					SlotlessNanite++;
				}
				// a slotless component keeping its class stencil would read back as whichever actor owns that slot
				if (!Component->bRenderCustomDepth || Component->CustomDepthStencilValue != Slot)
				{
					Component->SetRenderCustomDepth(Slot != 0);
					Component->SetCustomDepthStencilValue(Slot);
				}
			}

			FSegIdDrawItem Item;
			Item.ComponentId = Component->GetPrimitiveSceneId();
			Item.IdColor     = IdColor;

			OutItems.Add(Item);
			bAnyProxy  = true;
			FirstDrawn = FirstDrawn ? FirstDrawn : Component;
		}

		if (bAnyProxy)
		{
			const uint32 Id       = GetOrAssignInstanceId(Actor);
			bool         bAlready = false;
			MappedIds.Add(Id, &bAlready);
			if (!bAlready)
			{
				FSegInstanceEntry Entry;
				Entry.Id      = static_cast<int32>(Id);
				Entry.ClassId = ClassOf(Actor, FirstDrawn);
				Entry.Actor   = Actor->GetPathName();
				OutMap.Add(MoveTemp(Entry));
			}
		}
	}

	if (SlotlessNanite > 0 && !bWarnedSlotsOut)
	{
		bWarnedSlotsOut = true;
		UE_LOG(LogTemp, Warning, TEXT("SegId: the 255 Nanite stencil slots ran out, %d Nanite components stay unlabelled"), SlotlessNanite);
	}
}

/* //} */
