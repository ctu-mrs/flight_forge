#pragma once

#include "CoreMinimal.h"
#include "MeshData.generated.h"

USTRUCT()
struct FMeshData
{
	GENERATED_BODY()

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FLinearColor> Colors;
	TArray<FVector2D> UVs;

	void ClearDataBuffers();
};

inline void FMeshData::ClearDataBuffers()
{
	Vertices.Empty();
	Triangles.Empty();
	Normals.Empty();
	Colors.Empty();
	UVs.Empty();
}
