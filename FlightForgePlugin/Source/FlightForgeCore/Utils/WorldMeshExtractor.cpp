#include "WorldMeshExtractor.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Chaos/HeightField.h"
#include "FlightForgeCore/DronePawn.h"
#include "LandscapeHeightfieldCollisionComponent.h"
#include "StaticMeshResources.h"

/* AppendLandscape() //{ */

static void AppendLandscape(const ULandscapeHeightfieldCollisionComponent* Heightfield, const FVector& WorldOrigin, const FBox& SceneBounds,
                            const int32 MaxTriangles, FWorldMeshResult& Out)
{
	// the cooked physics heightfield is what a packaged build has; the editor's raw height data is editor-only
	const Chaos::FHeightField* Field = Heightfield->HeightfieldRef.IsValid() ? Heightfield->HeightfieldRef->HeightfieldGeometry.GetReference() : nullptr;
	if (!Field || Field->GetNumCols() <= 1 || Field->GetNumRows() != Field->GetNumCols())
	{
		return;
	}

	const int32 Size    = Field->GetNumCols();
	const bool  bFilter = SceneBounds.IsValid != 0;

	// the physics body carries the component's rotation and translation; the scale is baked into the field
	FTransform ToWorld = Heightfield->GetComponentTransform();
	ToWorld.SetScale3D(FVector::OneVector);

	TArray<FVector3f> Scene;
	Scene.SetNumUninitialized(Size * Size);
	for (int32 Index = 0; Index < Size * Size; ++Index)
	{
		const FVector W = ToWorld.TransformPosition(FVector(Field->GetPointScaled(Index)));

		Scene[Index] = FVector3f(static_cast<float>((W.X - WorldOrigin.X) / 100.0), static_cast<float>(-(W.Y - WorldOrigin.Y) / 100.0),
		                         static_cast<float>((W.Z - WorldOrigin.Z) / 100.0));
	}

	// a landscape component covers hundreds of metres, so without per-quad clipping a
	// small query still drags the whole tile over the wire
	TArray<int32> Remap;
	Remap.Init(-1, Size * Size);

	const auto Emit = [&Remap, &Scene, &Out](const int32 Corner) {
		if (Remap[Corner] < 0)
		{
			Remap[Corner] = Out.Vertices.Num() / 3;
			Out.Vertices.Add(Scene[Corner].X);
			Out.Vertices.Add(Scene[Corner].Y);
			Out.Vertices.Add(Scene[Corner].Z);
		}
		Out.Indices.Add(static_cast<uint32>(Remap[Corner]));
	};

	bool bEmitted = false;

	for (int32 y = 0; y < Size - 1; ++y)
	{
		for (int32 x = 0; x < Size - 1; ++x)
		{
			if (MaxTriangles > 0 && Out.Indices.Num() / 3 + 2 > MaxTriangles)
			{
				Out.bTruncated = true;
				Out.SourceCount += bEmitted ? 1 : 0;
				return;
			}

			const int32 I00 = y * Size + x;
			const int32 I10 = I00 + 1;
			const int32 I01 = I00 + Size;
			const int32 I11 = I01 + 1;

			if (bFilter)
			{
				FBox Quad(EForceInit::ForceInit);
				for (const int32 Corner : {I00, I10, I01, I11})
				{
					Quad += FVector(Scene[Corner]);
				}
				if (!SceneBounds.Intersect(Quad))
				{
					continue;
				}
			}

			Emit(I00); Emit(I11); Emit(I10);
			Emit(I00); Emit(I01); Emit(I11);
			bEmitted = true;
		}
	}

	if (bEmitted)
	{
		Out.SourceCount++;
	}
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
				AppendLandscape(Heightfield, WorldOrigin, SceneBounds, MaxTriangles, Out);
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

			TArray<FVector3f> Scene;
			TArray<int32>     Remap;

			for (const FTransform& ToWorld : Transforms)
			{
				if (MaxTriangles > 0 && Out.Indices.Num() / 3 >= MaxTriangles)
				{
					Out.bTruncated = true;
					return true;
				}

				Scene.SetNumUninitialized(NumVertices);
				for (int32 i = 0; i < NumVertices; ++i)
				{
					const FVector W = ToWorld.TransformPosition(FVector(LOD.VertexBuffers.PositionVertexBuffer.VertexPosition(i)));
					Scene[i]        = FVector3f(static_cast<float>((W.X - WorldOrigin.X) / 100.0), static_cast<float>(-(W.Y - WorldOrigin.Y) / 100.0),
					                            static_cast<float>((W.Z - WorldOrigin.Z) / 100.0));
				}

				// vertices are emitted only once a triangle keeps them, so a clipped export
				// carries no orphans; -1 means this instance has not emitted that vertex yet
				Remap.Init(-1, NumVertices);

				// UE winds clockwise in its left-handed frame; the y flip mirrors that into
				// counter-clockwise outward in the right-handed one, so the order stands
				for (int32 i = 0; i + 2 < MeshIndices.Num(); i += 3)
				{
					if (MaxTriangles > 0 && Out.Indices.Num() / 3 >= MaxTriangles)
					{
						Out.bTruncated = true;
						return true;
					}

					const uint32 Corners[3] = {MeshIndices[i], MeshIndices[i + 1], MeshIndices[i + 2]};

					// a triangle straddling the edge is kept whole: clipping it would leave
					// a hole exactly where a query box meets the geometry it asked about
					if (bFilter)
					{
						FBox Tri(EForceInit::ForceInit);
						for (const uint32 Corner : Corners)
						{
							Tri += FVector(Scene[Corner]);
						}
						if (!SceneBounds.Intersect(Tri))
						{
							continue;
						}
					}

					for (const uint32 Corner : Corners)
					{
						if (Remap[Corner] < 0)
						{
							Remap[Corner] = Out.Vertices.Num() / 3;
							Out.Vertices.Add(Scene[Corner].X);
							Out.Vertices.Add(Scene[Corner].Y);
							Out.Vertices.Add(Scene[Corner].Z);
						}
						Out.Indices.Add(static_cast<uint32>(Remap[Corner]));
					}
				}
			}

			Out.SourceCount++;
		}
	}

	return true;
}

/* //} */
