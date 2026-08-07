#include "SegmentationTagger.h"

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
