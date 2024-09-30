// Fill out your copyright notice in the Description page of Project Settings.


#include "MarchingRender.h"

#include "KismetProceduralMeshLibrary.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMarchingRender::AMarchingRender()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = ProceduralMeshComponent;

	ProceduralMeshComponent->SetCastShadow(true);
	ProceduralMeshComponent->SetCastContactShadow(true);
	ProceduralMeshComponent->SetCastHiddenShadow(true);
	ProceduralMeshComponent->SetCastInsetShadow(true);
	
	Noise = std::make_unique<FastNoiseLite>();
	Noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	Noise->SetFrequency(0.03);
	Noise->SetFractalType(FastNoiseLite::FractalType_FBm);
	Noise->SetFractalOctaves(3);
	Noise->SetSeed(1975);
	
	//VoxelData = std::make_unique<FVoxelData>(Size+1);
}

void AMarchingRender::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
}

// Called when the game starts or when spawned
void AMarchingRender::BeginPlay()
{
	Super::BeginPlay();

	VoxelData = std::make_shared<FVoxelData>(Size+1);
	
	//PerlinWorm();
	
	//GenerateVoxelNoise3D();
	//GenerateVoxelNoise2D();
	
	// CreateMeshData();
	//
	// UpdateProceduralMesh();
}

// Called every frame
void AMarchingRender::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMarchingRender::GenerateVoxelNoise3D()
{
	const FVector Position = GetActorLocation() / 100;
	for (int x = 0; x < VoxelData->Size; x++)
	{
		for (int y = 0; y < VoxelData->Size; y++)
		{
			for(int z = 0; z < VoxelData->Size; z++)
			{
				// UE_LOG(LogTemp, Warning, TEXT("The noise %f at [x: %d][y: %d][z: %d]"), VoxelData->GetDataAtIndex(x,y,z),x,y,z);
				//VoxelData->SetDataAtIndex(x, y, z, Noise->GetNoise(Position.X + x, Position.Y + y, Position.Z + z));
				
				//sphere
				const FVector Center = FVector(Size,Size,Size)/2;
				if((x-Center.X)*(x-Center.X)+(y-Center.Y)*(y-Center.Y)+(z-Center.Z)*(z  -Center.Z) <= (Size/3)*(Size/3))
				{
					VoxelData->SetDataAtIndex(x,y,z, -1);
				}
				else
				{
					VoxelData->SetDataAtIndex(x,y,z, 1);
				}
				
			}
		}
	}
}

void AMarchingRender::GenerateVoxelNoise2D()
{
	const FVector Position = GetActorLocation() / 100;
	for (int x = 0; x <= Size; x++)
	{
		for (int y = 0; y <= Size; y++)
		{
			const float Xpos = x + Position.X;
			const float ypos = y + Position.Y;
			
			const int Height = FMath::Clamp(FMath::RoundToInt((Noise->GetNoise(Xpos, ypos) + 1) * Size / 2), 0, Size);

			for (int z = 0; z < Height; z++)
			{
				VoxelData->SetDataAtIndex(x,y,z, 1);
			}

			for (int z = Height; z < Size; z++)
			{
				VoxelData->SetDataAtIndex(x,y,z, -1);
			}
		}
	}
}

float AMarchingRender::GetInterpolationOffset(const float Val1, const float Val2) const
{
	return (Val2-Val1)==0 ? 0.0f : (IsoLevel - Val1) / (Val2 - Val1);
}

void AMarchingRender::Polygonise(const float MarchCube[8], int X, int Y, int Z)
{
	int CubeIndex = 0; //bitmask for vertices, which are below isoLevel
	for(int i = 0; i < 8; i++)
	{
		if(MarchCube[i] <= IsoLevel) CubeIndex |= (1 << i);
	}
	
	int EdgeMask = EdgeTable[CubeIndex]; //bitmask for edges, which are intersect by surface
	
	/* Cube is entirely in/out of the surface */
	if(EdgeMask == 0) return;
	
	FVector EdgeVertices[12];
	for(int i=0; i < 12; i++)
	{
		if( (EdgeMask & (1 << i)) != 0)
		{
			const float Offset = bInterpolate ? GetInterpolationOffset(MarchCube[EdgeConnection[i][0]],MarchCube[EdgeConnection[i][1]]) : 0.5f;
	
			EdgeVertices[i].X = X + VertexOffset[EdgeConnection[i][0]][0] + Offset * EdgeDirection[i][0];
			EdgeVertices[i].Y = Y + VertexOffset[EdgeConnection[i][0]][1] + Offset * EdgeDirection[i][1];
			EdgeVertices[i].Z = Z + VertexOffset[EdgeConnection[i][0]][2] + Offset * EdgeDirection[i][2];
		}
	}
	
	/* Create the triangle */
	for(int i = 0; TriangleTable[CubeIndex][i] != -1; i += 3)
	{
		const float VertexLocationScale = 100;
		auto V1 = EdgeVertices[TriangleTable[CubeIndex][i]] * VertexLocationScale;
		auto V2 = EdgeVertices[TriangleTable[CubeIndex][i + 1]] * VertexLocationScale;
		auto V3 = EdgeVertices[TriangleTable[CubeIndex][i + 2]] * VertexLocationScale;
	
		auto Normal = FVector::CrossProduct((V2-V1), (V3-V1));
		Normal.Normalize();
	
		MeshData.Vertices.Add(V1);
		MeshData.Vertices.Add(V2);
		MeshData.Vertices.Add(V3);
	
		MeshData.Normals.Add(Normal);
		MeshData.Normals.Add(Normal);
		MeshData.Normals.Add(Normal);

		MeshData.UVs.Add(FVector2D(0, 0));
		MeshData.UVs.Add(FVector2D(0, 1));
		MeshData.UVs.Add(FVector2D(1, 0));
		// if(i % 2 == 0)
		// {
		// 	MeshData.UVs.Add(FVector2D(0, 0));
		// 	MeshData.UVs.Add(FVector2D(0, 1));
		// 	MeshData.UVs.Add(FVector2D(1, 0));
		// }else
		// {
		// 	MeshData.UVs.Add(FVector2D(0, 1));
		// 	MeshData.UVs.Add(FVector2D(1, 0));
		// 	MeshData.UVs.Add(FVector2D(1, 1));
		// }
	
		MeshData.Triangles.Add(VertexCount + TriangleOrder[0]);
		MeshData.Triangles.Add(VertexCount + TriangleOrder[1]);
		MeshData.Triangles.Add(VertexCount + TriangleOrder[2]);
	
		MeshData.Colors.Add(FLinearColor(0,1,0));
		MeshData.Colors.Add(FLinearColor(0,1,0));
		MeshData.Colors.Add(FLinearColor(0,1,0));
	
		VertexCount += 3;
	}
}

void AMarchingRender::CreateMeshData()
{
	float MarchCube[8];

	for (int x = 0; x < Size; x++)
	{
		for (int y = 0; y < Size; y++)
		{
			for(int z = 0; z < Size; z++)
			{
				for(int i = 0; i < 8; i++)
				{
					MarchCube[i] = VoxelData->GetDataAtIndex(x + VertexOffset[i][0],y + VertexOffset[i][1],z + VertexOffset[i][2]);
				}

				Polygonise(MarchCube, x, y, z);
				
			}
		}
	}
	
}

void AMarchingRender::UpdateProceduralMesh()
{
	TArray<FVector> n;
	TArray<FProcMeshTangent> t;
	if(bSmoothMesh)
	{
		UKismetProceduralMeshLibrary::CalculateTangentsForMesh(MeshData.Vertices, MeshData.Triangles, MeshData.UVs, n, t);
	}else
	{
		n = MeshData.Normals;
		t = TArray<FProcMeshTangent>();
	}

	// TArray<FVector> nn;
	// nn.SetNum(n.Num());
	//
	// for (int i = 0; i < n.Num(); i++)
	// {
	// 	nn[i] = -n[i];
	// }
	
	ProceduralMeshComponent->CreateMeshSection_LinearColor(
	0,
	MeshData.Vertices,
	MeshData.Triangles,
	n,
	MeshData.UVs,
	MeshData.Colors,
	t,
	true
	);
	if(Material)
	{
		ProceduralMeshComponent->SetMaterial(0, Material);
	}
}

void AMarchingRender::ResizeVoxelData(int ChunkSize)
{
	VoxelData->InitData(ChunkSize+1);
	this->Size = ChunkSize;
#if WITH_EDITOR
	if (bDrawDebug) DrawDebugBounds();
#endif
}

#if WITH_EDITOR
void AMarchingRender::DrawDebugBounds()
{
	const FVector Start = GetActorLocation();
	int thickness = 25;
	//bottum
	DrawDebugLine(GetWorld(),Start, Start + 100 * Size * FVector::ForwardVector,FColor::Red, true, -1, 0, thickness);
	DrawDebugLine(GetWorld(),Start + 100*Size*FVector::RightVector, Start + 100*Size*(FVector::RightVector + FVector::ForwardVector),FColor::Red, true, -1, 0, thickness);
	DrawDebugLine(GetWorld(),Start, Start + 100 * Size * FVector::RightVector,FColor::Red, true, -1, 0, thickness);
	DrawDebugLine(GetWorld(),Start + 100 * Size * FVector::ForwardVector, Start + 100 * Size * (FVector::ForwardVector + FVector::RightVector),FColor::Red, true, -1, 0, thickness);
	//top
	DrawDebugLine(GetWorld(),Start + 100 * Size * FVector::UpVector, Start + 100 * Size * (FVector::UpVector+FVector::ForwardVector),FColor::Red, true, -1, 0, thickness);
	DrawDebugLine(GetWorld(),Start+ 100 * Size * FVector::UpVector, Start + 100 * Size * (FVector::UpVector+FVector::ForwardVector),FColor::Red, true, -1, 0, thickness);
	DrawDebugLine(GetWorld(),Start+ 100 * Size * (FVector::UpVector+FVector::RightVector), Start + 100 * Size *(FVector::UpVector+FVector::RightVector+FVector::ForwardVector),FColor::Red, true, -1, 0, thickness);
	DrawDebugLine(GetWorld(),Start+ 100 * Size * FVector::UpVector, Start+ 100 * Size *(FVector::UpVector+FVector::RightVector),FColor::Red, true, -1, 0, thickness);
	DrawDebugLine(GetWorld(),Start+ 100 * Size * (FVector::UpVector+FVector::ForwardVector), Start + 100 * Size * (FVector::UpVector+FVector::ForwardVector+FVector::RightVector),FColor::Red, true, -1, 0, thickness);

	DrawDebugLine(GetWorld(),Start + 100 * Size * FVector::ForwardVector, Start + 100 * Size * (FVector::ForwardVector+FVector::UpVector),FColor::Red, true, -1, 0, thickness);
	DrawDebugLine(GetWorld(),Start+ 100 * Size * FVector::RightVector, Start+ 100 * Size * (FVector::RightVector+FVector::UpVector),FColor::Red, true, -1, 0, thickness);
	DrawDebugLine(GetWorld(),Start+ 100 * Size * (FVector::ForwardVector+FVector::RightVector), Start + 100 * Size * (FVector::RightVector+FVector::ForwardVector+FVector::UpVector),FColor::Red, true, -1, 0, thickness);
	DrawDebugLine(GetWorld(),Start, Start + 100 * Size * FVector::UpVector,FColor::Red, true, -1, 0, thickness);
}
#endif

