#include "WorldMeshExtractor.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "FlightForgeCore/DronePawn.h"
#include "LandscapeDataAccess.h"
#include "LandscapeHeightfieldCollisionComponent.h"
#include "StaticMeshResources.h"

/* AppendLandscape() //{ */

static void AppendLandscape(const ULandscapeHeightfieldCollisionComponent* Heightfield, const FVector& WorldOrigin, const int32 MaxTriangles,
                            FWorldMeshResult& Out)
{
	const int32 Size = Heightfield->CollisionSizeQuads + 1;
	if (Size <= 1)
	{
		return;
	}

	if (MaxTriangles > 0 && Out.Indices.Num() / 3 + (Size - 1) * (Size - 1) * 2 > MaxTriangles)
	{
		Out.bTruncated = true;
		return;
	}

	const uint16* Heights = static_cast<const uint16*>(const_cast<ULandscapeHeightfieldCollisionComponent*>(Heightfield)->CollisionHeightData.LockReadOnly());
	if (!Heights)
	{
		return;
	}

	const FTransform ToWorld = Heightfield->GetComponentTransform();
	const uint32     Base    = Out.Vertices.Num() / 3;

	Out.Vertices.Reserve(Out.Vertices.Num() + Size * Size * 3);
	for (int32 y = 0; y < Size; ++y)
	{
		for (int32 x = 0; x < Size; ++x)
		{
			const float   Z = LandscapeDataAccess::GetLocalHeight(Heights[y * Size + x]);
			const FVector W = ToWorld.TransformPosition(FVector(x * Heightfield->CollisionScale, y * Heightfield->CollisionScale, Z));

			Out.Vertices.Add(static_cast<float>((W.X - WorldOrigin.X) / 100.0));
			Out.Vertices.Add(static_cast<float>(-(W.Y - WorldOrigin.Y) / 100.0));
			Out.Vertices.Add(static_cast<float>((W.Z - WorldOrigin.Z) / 100.0));
		}
	}

	const_cast<ULandscapeHeightfieldCollisionComponent*>(Heightfield)->CollisionHeightData.Unlock();

	Out.Indices.Reserve(Out.Indices.Num() + (Size - 1) * (Size - 1) * 6);
	for (int32 y = 0; y < Size - 1; ++y)
	{
		for (int32 x = 0; x < Size - 1; ++x)
		{
			const uint32 I00 = Base + y * Size + x;
			const uint32 I10 = I00 + 1;
			const uint32 I01 = I00 + Size;
			const uint32 I11 = I01 + 1;

			Out.Indices.Add(I00); Out.Indices.Add(I11); Out.Indices.Add(I10);
			Out.Indices.Add(I00); Out.Indices.Add(I01); Out.Indices.Add(I11);
		}
	}

	Out.SourceCount++;
}

/* //} */

/* Extract() //{ */

bool FWorldMeshExtractor::Extract(UWorld* World, const FVector& WorldOrigin, const FBox& SceneBounds, const int32 MaxTriangles, FWorldMeshResult& Out)
{
	Out = FWorldMeshResult();

	if (!World)
	{
		return false;
	}

	const bool bFilter = SceneBounds.IsValid != 0;

	// scene metres -> world centimetres, with the scene frame's y negation
	FBox WorldBounds(EForceInit::ForceInit);
	if (bFilter)
	{
		const FVector A = WorldOrigin + FVector(SceneBounds.Min.X, -SceneBounds.Max.Y, SceneBounds.Min.Z) * 100.0;
		const FVector B = WorldOrigin + FVector(SceneBounds.Max.X, -SceneBounds.Min.Y, SceneBounds.Max.Z) * 100.0;
		WorldBounds += A;
		WorldBounds += B;
	}

	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor* Actor = *It;

		// robots are observers of the world, not part of its ground truth
		if (Actor->IsA(ADronePawn::StaticClass()) || (Actor->GetParentActor() && Actor->GetParentActor()->IsA(ADronePawn::StaticClass())))
		{
			continue;
		}

		TInlineComponentArray<ULandscapeHeightfieldCollisionComponent*> Heightfields(Actor);
		for (const ULandscapeHeightfieldCollisionComponent* Heightfield : Heightfields)
		{
			if (Heightfield && (!bFilter || WorldBounds.Intersect(Heightfield->Bounds.GetBox())))
			{
				AppendLandscape(Heightfield, WorldOrigin, MaxTriangles, Out);
			}
		}

		TInlineComponentArray<UStaticMeshComponent*> Components(Actor);

		for (const UStaticMeshComponent* Component : Components)
		{
			if (!Component || !Component->IsVisible() || !Component->GetStaticMesh())
			{
				continue;
			}

			// ground truth is what sensors can hit; skyspheres and other cosmetic meshes
			// have collision off and would otherwise wrap the whole export in a shell
			if (Component->GetCollisionEnabled() == ECollisionEnabled::NoCollision)
			{
				continue;
			}

			if (bFilter && !WorldBounds.Intersect(Component->Bounds.GetBox()))
			{
				continue;
			}

			const FStaticMeshRenderData* RenderData = Component->GetStaticMesh()->GetRenderData();
			if (!RenderData || RenderData->LODResources.Num() == 0)
			{
				continue;
			}

			const FStaticMeshLODResources& LOD = RenderData->LODResources[0];
			const int32 NumVertices  = LOD.VertexBuffers.PositionVertexBuffer.GetNumVertices();
			const int32 NumTriangles = LOD.IndexBuffer.GetNumIndices() / 3;

			if (NumVertices == 0 || NumTriangles == 0)
			{
				continue;
			}

			// an instanced component carries one mesh and many transforms; the floor of a
			// modular level is hundreds of instances, and reading only the component
			// transform silently drops all but a phantom copy at its origin
			TArray<FTransform> Transforms;
			if (const UInstancedStaticMeshComponent* Instanced = Cast<UInstancedStaticMeshComponent>(Component))
			{
				const int32 InstanceCount = Instanced->GetInstanceCount();
				Transforms.Reserve(InstanceCount);
				for (int32 i = 0; i < InstanceCount; ++i)
				{
					FTransform InstanceTransform;
					if (Instanced->GetInstanceTransform(i, InstanceTransform, true))
					{
						Transforms.Add(InstanceTransform);
					}
				}
			}
			else
			{
				Transforms.Add(Component->GetComponentTransform());
			}

			TArray<uint32> MeshIndices;
			LOD.IndexBuffer.GetCopy(MeshIndices);

			for (const FTransform& ToWorld : Transforms)
			{
				if (MaxTriangles > 0 && Out.Indices.Num() / 3 + NumTriangles > MaxTriangles)
				{
					Out.bTruncated = true;
					return true;
				}

				const uint32 Base = Out.Vertices.Num() / 3;

				Out.Vertices.Reserve(Out.Vertices.Num() + NumVertices * 3);
				for (int32 i = 0; i < NumVertices; ++i)
				{
					const FVector Local(LOD.VertexBuffers.PositionVertexBuffer.VertexPosition(i));
					const FVector W = ToWorld.TransformPosition(Local);

					Out.Vertices.Add(static_cast<float>((W.X - WorldOrigin.X) / 100.0));
					Out.Vertices.Add(static_cast<float>(-(W.Y - WorldOrigin.Y) / 100.0));
					Out.Vertices.Add(static_cast<float>((W.Z - WorldOrigin.Z) / 100.0));
				}

				// UE winds clockwise in its left-handed frame; the y flip mirrors that into
				// counter-clockwise outward in the right-handed one, so the order stands
				Out.Indices.Reserve(Out.Indices.Num() + MeshIndices.Num());
				for (int32 i = 0; i + 2 < MeshIndices.Num(); i += 3)
				{
					Out.Indices.Add(Base + MeshIndices[i]);
					Out.Indices.Add(Base + MeshIndices[i + 1]);
					Out.Indices.Add(Base + MeshIndices[i + 2]);
				}
			}

			Out.SourceCount++;
		}
	}

	return true;
}

/* //} */
