// Fill out your copyright notice in the Description page of Project Settings.


#include "CaveManager.h"

#include <chrono>
#include <iostream>

#include "PerlinWorm.h"
#include "Field/FieldSystemNoiseAlgo.h"


// Sets default values
ACaveManager::ACaveManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACaveManager::BeginPlay()
{
	Super::BeginPlay();
	PerlinWorm = std::make_unique<FPerlinWorm>(GetWorld(), bDebug, MinHoleRadius, MaxHoleRadius, Seed);

	// typedef std::chrono::system_clock Time;
	// typedef std::chrono::milliseconds ms;
	// typedef std::chrono::duration<float> fsec;
	// //initialization of first 9 chunks
	// auto t0 = Time::now();
	// auto start = std::chrono::system_clock::now();

	InitCaveWorld();
	BuildTunnels(FIntVector(0,0,ChunksCountInDirectionXYZ.Z-1));
	
	RenderCaveWorld();
	DecorateCave();
	
	// auto end = std::chrono::system_clock::now();
	// auto t1 = Time::now();
	//
	// fsec fs = t1 - t0;
	// ms d = std::chrono::duration_cast<ms>(fs);
	// UE_LOG(LogTemp, Warning, TEXT("Cave Generation TIME: %lld ms"), d.count());
	//
	// auto elapsed = end - start;
	// double elapsed_s = elapsed.count() / 1000000000.0;
	// UE_LOG(LogTemp, Warning, TEXT("Cave Generation TIME: %lld s"), std::chrono::duration_cast<std::chrono::seconds>(elapsed).count());
	// UE_LOG(LogTemp, Warning, TEXT("Cave Generation TIME: %lf s"), elapsed_s);
}

// Called every frame
void ACaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACaveManager::InitCaveWorld()
{
	for(int X=0; X<ChunksCountInDirectionXYZ.X; ++X)
	{
		for(int Y=0; Y<ChunksCountInDirectionXYZ.Y; ++Y)
		{
			for(int Z=0; Z<ChunksCountInDirectionXYZ.Z; ++Z)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Spawn Chunk at [x: %d][y: %d][z: %d]"), X,Y,Z);
				FIntVector ChunkCoord = FIntVector(X,Y,Z);
				FVector ChunkLocation = FVector(ChunkCoord.X ,ChunkCoord.Y ,ChunkCoord.Z) * ChunkSize * 100;
				
				AMarchingRender* Chunk = GetWorld()->SpawnActor<AMarchingRender>(ChunkBlueprint, ChunkLocation,FVector::Zero().ToOrientationRotator());
				Chunk->bSmoothMesh = this->bSmoothMesh;
				Chunk->ResizeVoxelData(ChunkSize);
				
				Chunks.Add(ChunkCoord, Chunk);
			}
		}
	}
}

// DFS with Perlin`s random order of neighbor
void ACaveManager::BuildTunnels(FIntVector ChunkCoord)
{
	bool &bVisited = (*Chunks.Find(ChunkCoord))->bVisited;
	if(bVisited)
	{
		return;
	}
	TArray<FIntVector> Neighbors;
	for(int X=-1; X<=1; ++X)
	{
		for(int Y=-1; Y<=1; ++Y)
		{
			for(int Z=-1; Z<=0; ++Z)
			{
				if(X==0 && Y==0 && Z==0)
				{
					continue;
				}
				FIntVector NeighborDir = FIntVector(X,Y,Z);
				FIntVector NeighborChunkCoord = ChunkCoord+NeighborDir;
				
				// FVector Location = FVector(NeighborChunkCoord+ChunkCoord) * ChunkSize * 100 + ChunkSize * 100 / 2;
				// float Perlin = PerlinWorm->GetNoise(Location.X/10, Location.Y/10, Location.Z/10);
				
				if(Chunks.Find(NeighborChunkCoord) && !(*Chunks.Find(NeighborChunkCoord))->bVisited)
				{
					//UE_LOG(LogTemp, Warning, TEXT("noise perlin = %f at [x: %f][y: %f][z: %f]"), Perlin, Location.X,Location.Y,Location.Z);
					//UE_LOG(LogTemp, Warning, TEXT("The boolean value is %s"), ( (*Chunks.Find(NeighborChunkCoord))->bVisited ? TEXT("true") : TEXT("false") ));
					Neighbors.Add(NeighborChunkCoord);
				}
			}
		}
	}
	
	bVisited = true;

	Neighbors.Sort([ChunkCoord,this](const FIntVector& ACoord,const FIntVector& BCoord)
	{
		FVector LocationA = FVector(ACoord+ChunkCoord) * ChunkSize * 100 + ChunkSize * 100 / 2;
		FVector LocationB = FVector(BCoord+ChunkCoord) * ChunkSize * 100 + ChunkSize * 100 / 2;

		return PerlinWorm->GetNoise(LocationA.X/10, LocationA.Y/10, LocationA.Z/10)
				< PerlinWorm->GetNoise(LocationB.X/10, LocationB.Y/10, LocationB.Z/10);
	});

	//test sort
	// for (auto Neighbor : Neighbors)
	// {
	// 	FVector LocationA = FVector(Neighbor+ChunkCoord) * ChunkSize * 100 + ChunkSize * 100 / 2;
	// 	float noise = PerlinWorm->GetNoise(LocationA.X/10, LocationA.Y/10, LocationA.Z/10);
	// 	UE_LOG(LogTemp, Warning, TEXT("SORT noise perlin = %f at [x: %f][y: %f][z: %f]"), noise, LocationA.X,LocationA.Y,LocationA.Z);
	// }
	
	for(const auto Neighbor : Neighbors)
	{
		if((*Chunks.Find(Neighbor))->bVisited)
		{
			continue;
		}
		const FVector LocationFrom = FVector(ChunkCoord) * ChunkSize * 100 + FVector::One() * ChunkSize/2 * 100;
		const FVector LocationTo = FVector(Neighbor) * ChunkSize * 100 + FVector::One() * ChunkSize/2 * 100;
		PerlinWorm->MakeWorm(Chunks, ChunkCoord, LocationFrom, LocationTo);
		BuildTunnels(Neighbor);
	}
}

void ACaveManager::RenderCaveWorld()
{
	for (const auto Chunk : Chunks)
	{
		Chunk.Value->CreateMeshData();
		Chunk.Value->UpdateProceduralMesh();
	}
}

void ACaveManager::DecorateCave()
{
	TArray<FVector> WormsPathPoints = PerlinWorm->GetWormsPathPoints();

	int StepsBetweenStalagmites = FMath::RandRange(MINStepsBetweenStalagmites, MAXStepsBetweenStalagmites);
	int StepsBetweenLights = FMath::RandRange(MINStepsBetweenLights, MAXStepsBetweenLights);
	for(int i=0; i < WormsPathPoints.Num(); i++)
	{
		//UE_LOG(LogTemp, Warning, TEXT("i= %d i modulo 50 = %d"), i,i%50);
		if(i%StepsBetweenStalagmites == 0)
		{
			StalagmitesGroupSpawner(WormsPathPoints[i]);
			StepsBetweenStalagmites = FMath::RandRange(MINStepsBetweenStalagmites, MAXStepsBetweenStalagmites);
		}
		
		if(bSpawnLights && i%StepsBetweenLights == 0)
		{
			LightSpawner(WormsPathPoints[i]);
			StepsBetweenLights = FMath::RandRange(MINStepsBetweenLights, MAXStepsBetweenLights);
		}
		
	}
}

void ACaveManager::SpawnRandomStalagmite(const FVector& Location, const FRotator& Orientation)
{
	if(UWorld* World = GetWorld())
	{
		FActorSpawnParameters ActorSpawnParameters = FActorSpawnParameters();
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		int StalagIdx = FMath::RandRange(0,StalagmiteBlueprintArray.Num()-1);
		AActor* Actor = World->SpawnActor<AActor>(StalagmiteBlueprintArray[StalagIdx], Location,Orientation, ActorSpawnParameters);
		float Scale = FMath::RandRange(MINStalagmitesScale,MAXStalagmitesScale);
		Actor->SetActorScale3D(Scale*FVector::One());
	}
}

void ACaveManager::StalagmitesGroupSpawner(const FVector& RaycastCenter)
{
	if(UWorld* World = GetWorld())
	{
		int NumOfHorStep = 20;
		float HorizontalStep = 360 / NumOfHorStep;
		int NumOfVerStep = 10;
		float VerticalStep = 60 / NumOfVerStep;
			
		FVector Forward = FVector::ForwardVector;
		FVector Right = FVector::RightVector;
		for(int Row = 0; Row < NumOfHorStep; Row++){
			FVector rotatedForward = Forward.RotateAngleAxis(HorizontalStep * Row, FVector::UpVector);
			FVector rotatedRight   = Right.RotateAngleAxis(HorizontalStep * Row, FVector::UpVector);
			for(int Column = 0; Column < NumOfVerStep; Column++){
				float VertAngle = -VerticalStep*Column-VerticalStep-30;
			  	
				FVector raycastDirection = rotatedForward.RotateAngleAxis(VertAngle, rotatedRight);
				raycastDirection *= 2 * MaxHoleRadius * 100;

				FHitResult HitResult;
				if (World->LineTraceSingleByChannel(HitResult, RaycastCenter, RaycastCenter + raycastDirection, ECollisionChannel::ECC_Camera, FCollisionQueryParams::DefaultQueryParam)) {
					//UPPER STALAGMITE
					float ChanceToStalagmite = FMath::RandRange(0.0,1.0);
					if(ChanceToStalagmite <= StalagmitesDensity)
					{
						FVector SpawnLocation;
						if(StalagmiteBlueprintArray.Num() != 0)
						{
							SpawnLocation = FVector(HitResult.Location.X, HitResult.Location.Y, HitResult.Location.Z);
							SpawnRandomStalagmite(SpawnLocation,FRotator(180,0,0)); 
						}
						// if(bDebug)
						// {
						// 	DrawDebugSphere(World, HitResult.Location, 30, 10,FColor::Emerald, true, -1, 0, 3);
						// 	DrawDebugLine(World, RaycastCenter, HitResult.Location,FColor::Emerald, true, -1, 0, 3);
						// }
						//Bottom STALAGMITE
						if (World->LineTraceSingleByChannel(HitResult, HitResult.Location + 100 * FVector::DownVector, HitResult.Location + FVector::DownVector * 2 * MaxHoleRadius * 100, ECollisionChannel::ECC_Camera, FCollisionQueryParams::DefaultQueryParam))
						{
							ChanceToStalagmite = FMath::RandRange(0.0,1.0);
							if(StalagmiteBlueprintArray.Num() != 0 && ChanceToStalagmite <= BottomStalagmitesDensity)
							{
								SpawnLocation = FVector(HitResult.Location.X, HitResult.Location.Y, HitResult.Location.Z);
								SpawnRandomStalagmite(SpawnLocation,FRotator(0,0,0)); 
							}
						}
					}
				}
			}
		}
	}
}

void ACaveManager::LightSpawner(const FVector& RaycastCenter)
{
	 if(UWorld* World = GetWorld())
	 {
	 	
	 	FActorSpawnParameters ActorSpawnParameters = FActorSpawnParameters();
	 	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	 	FVector SpawnLocation;
	 	
	 	FHitResult HitResult;
	 	//FVector Start = RaycastCenter;
	 	FVector End = RaycastCenter + FVector::DownVector * 2 * MaxHoleRadius * 100;
	 	if(World->LineTraceSingleByChannel(HitResult, RaycastCenter, End, ECollisionChannel::ECC_Camera, FCollisionQueryParams::DefaultQueryParam))
	 	{
	 		if(bDebug)
	 		{
	 			DrawDebugSphere(World, HitResult.Location, 30, 10,FColor::Emerald, true, -1, 0, 3);
	 			DrawDebugLine(World, RaycastCenter, End,FColor::Emerald, true, -1, 0, 3);
	 		}
	
	 		if(LightBlueprint)
	 		{
	 			SpawnLocation = FVector(HitResult.Location.X, HitResult.Location.Y, HitResult.Location.Z);
	 			World->SpawnActor<AActor>(LightBlueprint, SpawnLocation,FRotator(0,FMath::RandRange(0.0,360.0),0), ActorSpawnParameters);
	 		}
	 	}
			
	}
}

