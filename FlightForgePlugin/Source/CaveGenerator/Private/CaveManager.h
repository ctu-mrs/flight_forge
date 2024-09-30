// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MarchingRender.h"
#include "PerlinWorm.h"
#include "GameFramework/Actor.h"
#include "CaveManager.generated.h"

UCLASS()
class CAVEGENERATOR_API ACaveManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACaveManager();

	UPROPERTY(EditAnywhere)
	int Seed = 1975;

	UPROPERTY(EditAnywhere)
	FIntVector ChunksCountInDirectionXYZ = FIntVector(2,2,2);
	
	UPROPERTY(EditAnywhere)
	int ChunkSize = 32;

	UPROPERTY(EditAnywhere)
	int MinHoleRadius = 5;

	UPROPERTY(EditAnywhere)
	int MaxHoleRadius = 5;

	UPROPERTY(EditAnywhere)
	bool bSmoothMesh = true;

	UPROPERTY(EditAnywhere)
	bool bSpawnLights = true;
	
	UPROPERTY(EditAnywhere)
	int MINStepsBetweenLights = 20;
	
	UPROPERTY(EditAnywhere)
	int MAXStepsBetweenLights = 50;

	UPROPERTY(EditAnywhere)
	int MINStepsBetweenStalagmites = 10;
	
	UPROPERTY(EditAnywhere)
	int MAXStepsBetweenStalagmites = 20;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float StalagmitesDensity = 0.3;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float BottomStalagmitesDensity = 0.6;

	UPROPERTY(EditAnywhere)
	float MINStalagmitesScale = 0.5;

	UPROPERTY(EditAnywhere)
	float MAXStalagmitesScale = 1.5;
	
	UPROPERTY(EditAnywhere)
	TArray<UClass*> StalagmiteBlueprintArray;
	
	UPROPERTY(EditAnywhere)
	UClass* LightBlueprint = nullptr;
	
	UPROPERTY(EditAnywhere)
	UClass* ChunkBlueprint = nullptr;

	UPROPERTY(EditAnywhere)
	bool bDebug = false;

private:
	UPROPERTY()
	TMap<FIntVector, AMarchingRender*> Chunks;

	std::unique_ptr<FPerlinWorm> PerlinWorm;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	void InitCaveWorld();
	void BuildTunnels(FIntVector ChunkCoord);
	void RenderCaveWorld();
	void DecorateCave();

	void SpawnRandomStalagmite(const FVector& Location, const FRotator& Orientation);
	void StalagmitesGroupSpawner(const FVector& RaycastCenter);

	void LightSpawner(const FVector& RaycastCenter);
};
