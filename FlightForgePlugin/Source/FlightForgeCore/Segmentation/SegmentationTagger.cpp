#include "SegmentationTagger.h"

#include "SegIdPass.h"

#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "EngineUtils.h"

/* OnWorldBeginPlay() / Deinitialize() //{ */

void USegmentationTagger::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	const USegmentationTaggerSettings* Settings = GetDefault<USegmentationTaggerSettings>();

	if (!Settings || Settings->Rules.Num() == 0)
	{
		return;
	}

	int32 Tagged = 0;
	for (TActorIterator<AActor> It(&InWorld); It; ++It)
	{
		TagActor(*It);
		++Tagged;
	}

	UE_LOG(LogTemp, Log, TEXT("SegmentationTagger: %d rules over %d actors"), Settings->Rules.Num(), Tagged);

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

/* FindRule() / TagActor() //{ */

const FSegmentationTagRule* USegmentationTagger::FindRule(const AActor* Actor, const UPrimitiveComponent* Component) const
{
	const USegmentationTaggerSettings* Settings = GetDefault<USegmentationTaggerSettings>();

	for (const FSegmentationTagRule& Rule : Settings->Rules)
	{
		switch (Rule.MatchType)
		{
			case ESegmentationMatchType::AssetPath:
			{
				const UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(Component);
				if (MeshComp && MeshComp->GetStaticMesh() && MeshComp->GetStaticMesh()->GetPathName().MatchesWildcard(Rule.Match))
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

void USegmentationTagger::TagActor(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}

	TArray<UPrimitiveComponent*> Primitives;
	Actor->GetComponents<UPrimitiveComponent>(Primitives);

	for (UPrimitiveComponent* Component : Primitives)
	{
		if (const FSegmentationTagRule* Rule = FindRule(Actor, Component))
		{
			Component->SetRenderCustomDepth(true);
			Component->SetCustomDepthStencilValue(Rule->Stencil);
		}
	}
}

//}

/* instance ids //{ */

uint32 USegmentationTagger::GetOrAssignInstanceId(const AActor* Actor)
{
	if (uint32* Existing = InstanceIds.Find(Actor))
	{
		return *Existing;
	}

	const uint32 Id = NextInstanceId++;
	InstanceIds.Add(Actor, Id);

	return Id;
}

void USegmentationTagger::CollectInstanceItems(TArray<FSegIdDrawItem>& OutItems, TArray<TPair<int32, FString>>& OutMap)
{
	OutItems.Reset();
	OutMap.Reset();

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	TSet<uint32> MappedIds;

	for (TActorIterator<AActor> It(World); It; ++It)
	{
		const AActor* Actor = *It;

		TInlineComponentArray<UStaticMeshComponent*> Components(Actor);

		bool bAnyProxy = false;

		for (const UStaticMeshComponent* Component : Components)
		{
			if (!Component || !Component->SceneProxy || !Component->IsVisible())
			{
				continue;
			}

			const uint32 Id = GetOrAssignInstanceId(Actor);

			FSegIdDrawItem Item;
			Item.Proxy   = Component->SceneProxy;
			Item.IdColor = FLinearColor((Id & 0xFF) / 255.0f, ((Id >> 8) & 0xFF) / 255.0f, ((Id >> 16) & 0xFF) / 255.0f, 1.0f);

			OutItems.Add(Item);
			bAnyProxy = true;
		}

		if (bAnyProxy)
		{
			const uint32 Id = GetOrAssignInstanceId(Actor);
			bool         bAlready = false;
			MappedIds.Add(Id, &bAlready);
			if (!bAlready)
			{
				OutMap.Emplace(static_cast<int32>(Id), Actor->GetPathName());
			}
		}
	}
}

/* //} */
