#include "PerlinWorm.h"

FPerlinWorm::FPerlinWorm()
{
	Noise = std::make_unique<FastNoiseLite>();
	Noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	Noise->SetFrequency(0.03);
	Noise->SetFractalType(FastNoiseLite::FractalType_FBm);
	Noise->SetFractalOctaves(3);
	Noise->SetSeed(1975);
}

FPerlinWorm::FPerlinWorm(UWorld* UnrealWorld, bool bDebug, int MinRadius, int MaxRadius, int Seed)
: MinRadius(MinRadius), MaxRadius(MaxRadius), bDebug(bDebug), UnrealWorld(UnrealWorld)
{
	
	Noise = std::make_unique<FastNoiseLite>();
	Noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	Noise->SetFrequency(0.03);
	Noise->SetFractalType(FastNoiseLite::FractalType_FBm);
	Noise->SetFractalOctaves(3);
	Noise->SetSeed(Seed);
}

void FPerlinWorm::MakeWorm(TMap<FIntVector, AMarchingRender*>& Chunks, FIntVector CurrChunkCoord, FVector CurrLocation,
	FVector DestLocation)
{
	
	//DrawDebugSphere(UnrealWorld, DestLocation, 30, 10,FColor::Blue, true, -1, 0, 3);
	const int ChunkSize = Chunks[FIntVector::ZeroValue]->Size;
	while(true)//for(int i=0; ; i++)
	{
		WormsPathPoints.Add(CurrLocation);
		FVector ChunkLocation = FVector(CurrChunkCoord) * ChunkSize * 100;
		//UE_LOG(LogTemp, Warning, TEXT("%d Worm Position is: %s"), i,*(CurrLocation-ChunkLocation).ToString());
		//UE_LOG(LogTemp, Warning, TEXT("%d Worm Position[int] is: %s"), i,*FIntVector(currPos.X, currPos.Y, currPos.Z).ToString());
		
		FIntVector HoleCenterInt = FIntVector((CurrLocation-ChunkLocation)/100); //Round to Int
		int HoleRadius = FMath::RandRange(MinRadius,MaxRadius);

		//MAKE HOLE! If center is near to border of chunk, dig hole also in next chunk
		DigHole(Chunks, CurrChunkCoord, ChunkSize, HoleCenterInt, HoleRadius);
		
		float NoiseValueXY = (Noise->GetNoise(CurrLocation.X/100, CurrLocation.Y/100, CurrLocation.Z/100) + 1) / 2;
		float NoiseValueXZ = (Noise->GetNoise(CurrLocation.Y/100, CurrLocation.X/100, CurrLocation.Z/100) + 1) / 2;
		
		FVector RandomDirection = FVector(FMath::Cos(NoiseValueXY * 2.0f * PI),FMath::Sin(NoiseValueXY * 2.0f * PI), FMath::Sin(NoiseValueXZ * 2.0f * PI));
		
		FVector DesireDir = DestLocation - CurrLocation; // FVector::One();
		DesireDir.Normalize();
		RandomDirection.Normalize();
		// DrawDebugLine(GetWorld(), currPos, currPos + 100*DesireDir,FColor::Blue, true, -1, 0, 3);
		// DrawDebugLine(GetWorld(), currPos, currPos + 100*Offset,FColor::Green, true, -1, 0, 3);
		RandomDirection = DesireDir * 0.6 + RandomDirection * 0.4;
		
		RandomDirection.Normalize();
		RandomDirection *= 100;

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("%d Noise Offset is: %s"), i,*Offset.ToString()));
		//UE_LOG(LogTemp, Warning, TEXT("%d Noise Offset is: %s"), i,*Offset.ToString());

		if(bDebug)
		{
			DrawDebugSphere(UnrealWorld, CurrLocation, 10, 10,FColor::Red, true, -1, 0, 25);
			DrawDebugLine(UnrealWorld, CurrLocation, CurrLocation + RandomDirection,FColor::Yellow, true, -1, 0, 25);
		}
		CurrLocation = CurrLocation + RandomDirection;

		//UE_LOG(LogTemp, Warning, TEXT("Distace to Desire Position is: %f"), FVector::Dist(CurrLocation, DestLocation));
		if(FVector::Dist(CurrLocation, DestLocation) <= 100.0)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Find Desire Position"));
			break;
		}

		// Check if next step is inside the world
		FIntVector NewHoleCenterInt = FIntVector((CurrLocation-ChunkLocation)/100);
		if(NewHoleCenterInt.X > ChunkSize)
		{
			CurrChunkCoord.X += 1;
		}
		else if(NewHoleCenterInt.X < 0)
		{
			CurrChunkCoord.X -= 1;
		}
		if(NewHoleCenterInt.Y > ChunkSize)
		{
			CurrChunkCoord.Y += 1;
		}
		else if(NewHoleCenterInt.Y < 0)
		{
			CurrChunkCoord.Y -= 1;
		}
		if(NewHoleCenterInt.Z > ChunkSize)
		{
			CurrChunkCoord.Z += 1;
		}
		else if(NewHoleCenterInt.Z < 0)
		{
			CurrChunkCoord.Z -= 1;
		}

		if(!Chunks.Find(CurrChunkCoord))
		{
			break;
		}
	}
}

/// <summary>
/// 3D noise at given position using current settings
/// </summary>
/// <returns>
/// Noise output bounded between 0...1
/// </returns>
float FPerlinWorm::GetNoise(const float X, const float Y, const float Z) const
{
	return (Noise->GetNoise(X,Y,Z) + 1) / 2;
}

void FPerlinWorm::DigHoleOneChunk(FVoxelData& VoxelData, FIntVector Center, int Radius)
{
	int minX = (Center.X - Radius) <= 0 ? 0 : Center.X - Radius;
	int maxX = (Center.X + Radius) >= VoxelData.Size-1 ? VoxelData.Size-1 : Center.X + Radius;

	int minY = (Center.Y - Radius) <= 0 ? 0 : Center.Y - Radius;
	int maxY = (Center.Y + Radius) >= VoxelData.Size-1 ? VoxelData.Size-1 : Center.Y + Radius;

	int minZ = (Center.Z - Radius) <= 0 ? 0 : Center.Z - Radius;
	int maxZ = (Center.Z + Radius) >= VoxelData.Size-1 ? VoxelData.Size-1 : Center.Z + Radius;

	for (int x = minX; x <= maxX; x++)
	{
		for (int y = minY; y <= maxY; y++)
		{
			for(int z = minZ; z <= maxZ; z++)
			{
				//UE_LOG(LogTemp, Warning, TEXT("The voxel %f at [x: %d][y: %d][z: %d]"), VoxelData->GetDataAtIndex(x,y,z),x,y,z);
				
				//sphere
				//If voxel is solid and we are inside shape, make a hole.
				if(VoxelData.GetDataAtIndex(x,y,z) == 1 && (x-Center.X)*(x-Center.X)+(y-Center.Y)*(y-Center.Y)+(z-Center.Z)*(z  -Center.Z) <= Radius*Radius)
				{
					VoxelData.SetDataAtIndex(x,y,z, -1); // Empty voxel
				}
			}
		}
	}
}

void FPerlinWorm::DigHole(TMap<FIntVector, AMarchingRender*>& Chunks, FIntVector CurrChunkCoord, const int ChunkSize, FIntVector HoleCenterInt,
	int HoleRadius)
{
	for(int X=-1; X<=1; ++X)
	{
		for(int Y=-1; Y<=1; ++Y)
		{
			for(int Z=-1; Z<=1; ++Z)
			{
				FIntVector NeighborDir = FIntVector(X,Y,Z);
					
				FIntVector NewHoleCenter = HoleCenterInt;
				FIntVector NextChunkCoord = CurrChunkCoord+NeighborDir;
				if(auto AChunkPtr = Chunks.Find(NextChunkCoord))
				{
					if(NeighborDir.X == 1)
					{
						NewHoleCenter.X -= ChunkSize;
					}
					else if(NeighborDir.X == -1)
					{
						NewHoleCenter.X += ChunkSize;
					}
					if(NeighborDir.Y == 1)
					{
						NewHoleCenter.Y -= ChunkSize;
					}
					else if(NeighborDir.Y == -1)
					{
						NewHoleCenter.Y += ChunkSize;
					}
					if(NeighborDir.Z == 1)
					{
						NewHoleCenter.Z -= ChunkSize;
					}
					else if(NeighborDir.Z == -1)
					{
						NewHoleCenter.Z += ChunkSize;
					}
					DigHoleOneChunk(*((*AChunkPtr)->VoxelData),NewHoleCenter, HoleRadius);
				}
			}
		}
	}
}

void FPerlinWorm::DebugSphere()
{
	UE_LOG(LogTemp, Warning, TEXT("Debug Sphere at [x: 0][y: 0][z: 0]"));
	DrawDebugSphere(UnrealWorld, FVector::Zero(), 30, 10,FColor::Purple, true, -1, 0, 3);
}

