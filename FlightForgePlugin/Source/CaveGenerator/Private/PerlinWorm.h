#pragma once

#include "MarchingRender.h"
#include "CaveGenerator/VoxelData.h"

class FPerlinWorm
{
public:
	FPerlinWorm();
	FPerlinWorm(UWorld* UnrealWorld, bool bDebug, int MinRadius, int MaxRadius, int Seed);
	int MinRadius = 1;
	int MaxRadius = 10;
	bool bDebug = false;
private:
	UWorld* UnrealWorld = nullptr;
	std::unique_ptr<FastNoiseLite> Noise;

	TArray<FVector> WormsPathPoints;

public:
	TArray<FVector> GetWormsPathPoints() const
	{
		return WormsPathPoints;
	}
	
	void MakeWorm(TMap<FIntVector, AMarchingRender*> &Chunks, FIntVector CurrChunkCoord, FVector CurrLocation, FVector DestLocation);
	float GetNoise(float X, float Y, float Z) const;
	
	void DebugSphere();
private:
	void DigHole(TMap<FIntVector, AMarchingRender*>& Chunks, FIntVector CurrChunkCoord, const int ChunkSize, FIntVector HoleCenterInt,
	int HoleRadius);
	void DigHoleOneChunk(FVoxelData &VoxelData, FIntVector Center, int Radius);
};
