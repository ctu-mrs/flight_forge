#pragma once

#include "CoreMinimal.h"
#include "VoxelData.generated.h"

USTRUCT()
struct FVoxelData
{
	GENERATED_BODY()

	//Constructors
	FVoxelData() = default;
	explicit FVoxelData(int Size);

	int Size;
	TArray<float> Data;

	void SetDataAtIndex(int X, int Y, int Z, float Value);
	float GetDataAtIndex(int X, int Y, int Z);
	void InitData(int Size);
	

};

inline FVoxelData::FVoxelData(int Size): Size(Size)
{
	//Data.SetNum(Size * Size * Size);
	Data.Init(1, Size * Size * Size); // All Voxel are Solid
}

inline void FVoxelData::SetDataAtIndex(int X, int Y, int Z, float Value)
{
	Data[Z*Size*Size + Y*Size + X] = Value;
}

inline float FVoxelData::GetDataAtIndex(int X, int Y, int Z)
{
	return Data[Z*Size*Size + Y*Size + X];
}

inline void FVoxelData::InitData(int InSize)
{
	Size = InSize;
	Data.Init(1, Size * Size * Size);
}
