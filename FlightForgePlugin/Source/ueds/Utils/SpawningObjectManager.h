// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ueds/uedsGameModeBase.h"
#include "SpawningObjectManager.generated.h"



UCLASS()
class UEDS_API ASpawningObjectManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawningObjectManager();

	UPROPERTY(EditAnywhere)
	FString GatesYamlConfigPath = "gate_config_abu_january_full.yaml";

	UPROPERTY(EditAnywhere)
	FString FullPathToActorsFolder_ = "/Game/Worlds/Warehouse/Blueprints/";

	UPROPERTY(EditAnywhere)
	FString SlimGateName = "BP_a2rl_gate_slim";

	UPROPERTY(EditAnywhere)
	bool bCollisionFree = false;

	UPROPERTY()
	AuedsGameModeBase* GameMode;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SpawnActorByName(const FString& NameOfActor, const FString& FullPathToActorsFolder, const FTransform& Transform, int32 StencilValue);
};
