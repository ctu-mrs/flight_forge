// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawningObjectManager.h"

#include "BlueprintUtils.h"
#include "ueds/uedsGameModeBase.h"


// Sets default values
ASpawningObjectManager::ASpawningObjectManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASpawningObjectManager::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AuedsGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if(!GameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("[ASpawningObjectManager] I have not GameModeBase, but I need it!"));
		return;
	}
	
	FVector WorldOrigin = GameMode->GetWorldOrigin();
	
	TArray<FGateDataYaml> GatesTransform;
	UBlueprintUtils::ReadGatesTransformFromYaml(GatesYamlConfigPath, GatesTransform);

	int32 StencilValue = 10;
	for (auto Gate : GatesTransform)
	{
		FVector Location = FVector(Gate.Position.X, Gate.Position.Y, Gate.Position.Z + 0.1);
		FVector Orientation = FVector(Gate.Orientation.Roll, Gate.Orientation.Pitch, Gate.Orientation.Yaw);
		
		FTransform TransformUE;
		UBlueprintUtils::TransformToUECoord(Location, Orientation, WorldOrigin, TransformUE);

		FString SlimGateName = "BP_a2rl_gate_slim";
		if(Gate.Position.Z > 1.35)
		{
			SlimGateName = "BP_a2rl_gate_slim_core";
		}
		
		this->SpawnActorByName(SlimGateName, FullPathToActorsFolder_, TransformUE, StencilValue++);
	}
	
}

// Called every frame
void ASpawningObjectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawningObjectManager::SpawnActorByName(const FString& NameOfActor, const FString& FullPathToActorsFolder,
                                                             const FTransform& Transform, int32 StencilValue)
{
	FString FullPatchToActor = FullPathToActorsFolder + NameOfActor + "." + NameOfActor + "_C";

	if (UClass* ActorClass = StaticLoadClass(AActor::StaticClass(), nullptr, *FullPatchToActor))
	{
		AActor* Actor = GetWorld()->SpawnActor<AActor>(ActorClass, Transform);
		UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		Primitive->SetCustomDepthStencilValue(StencilValue);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find class: %s"), *FullPatchToActor);
	}
}

