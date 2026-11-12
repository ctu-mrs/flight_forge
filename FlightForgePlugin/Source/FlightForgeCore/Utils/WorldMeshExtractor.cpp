#include "WorldMeshExtractor.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Chaos/Convex.h"
#include "Chaos/HeightField.h"
#include "Chaos/TriangleMeshImplicitObject.h"
#include "FlightForgeCore/RobotPawn.h"
#include "HAL/IConsoleManager.h"
#include "LandscapeHeightfieldCollisionComponent.h"
#include "PhysicsEngine/BodySetup.h"
#include "StaticMeshResources.h"
#include "WireFrame.h"

static TAutoConsoleVariable<int32> CVarWorldMeshFromCollision(TEXT("ff.WorldMesh.FromCollision"), 0,
                                                              TEXT("1 exports collision geometry even where the render mesh is CPU-readable, as a packaged build must"));

/* AppendLandscape() //{ */

static void AppendLandscape(const ULandscapeHeightfieldCollisionComponent* Heightfield, const FVector& GroundOrigin, const FBox& SceneBounds,
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

		Scene[Index] = FVector3f(FWireFrame::ToGround(W, GroundOrigin));
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

/* local geometry //{ */

namespace
{

struct FLocalMesh
{
	TArray<FVector3f> Vertices;
	TArray<uint32>    Indices;

	// UE's own winding: the outward normal is (C - A) x (B - A)
	void AddTriangle(const FVector& A, const FVector& B, const FVector& C, const FVector& Outward)
	{
		const uint32 Base     = Vertices.Num();
		const bool   bFlipped = FVector::DotProduct(FVector::CrossProduct(C - A, B - A), Outward) < 0.0;
		Vertices.Add(FVector3f(A));
		Vertices.Add(FVector3f(bFlipped ? C : B));
		Vertices.Add(FVector3f(bFlipped ? B : C));
		Indices.Append({Base, Base + 1, Base + 2});
	}
};

// cooked meshes drop their CPU copy after upload unless they allow CPU access
bool FromRenderData(const UStaticMesh& Mesh, FLocalMesh& Out)
{
	const FStaticMeshRenderData* RenderData = Mesh.GetRenderData();
	if (!RenderData || RenderData->LODResources.Num() == 0)
	{
		return false;
	}

	const FStaticMeshLODResources& LOD       = RenderData->LODResources[0];
	const FPositionVertexBuffer&   Positions = LOD.VertexBuffers.PositionVertexBuffer;
	if (!Positions.GetVertexData() || Positions.GetNumVertices() == 0 || LOD.IndexBuffer.GetNumIndices() < 3)
	{
		return false;
	}

	LOD.IndexBuffer.GetCopy(Out.Indices);
	if (Out.Indices.Num() < 3)
	{
		return false;
	}

	Out.Vertices.SetNumUninitialized(Positions.GetNumVertices());
	for (uint32 i = 0; i < Positions.GetNumVertices(); ++i)
	{
		Out.Vertices[i] = Positions.VertexPosition(i);
	}
	return true;
}

bool FromCollision(const UStaticMesh& Mesh, FLocalMesh& Out)
{
	const UBodySetup* Setup = Mesh.GetBodySetup();
	if (!Setup)
	{
		return false;
	}

	for (const Chaos::FTriangleMeshImplicitObjectPtr& TriMesh : Setup->TriMeshGeometries)
	{
		if (!TriMesh)
		{
			continue;
		}

		const uint32 Base      = Out.Vertices.Num();
		const auto&  Particles = TriMesh->Particles();
		for (int32 i = 0; i < static_cast<int32>(Particles.Size()); ++i)
		{
			Out.Vertices.Add(FVector3f(Particles.GetX(i)));
		}

		// Chaos cooks its triangles in the opposite order to the render mesh's
		const auto Append = [&Out, Base](const auto& Triangles) {
			for (const auto& Triangle : Triangles)
			{
				Out.Indices.Append({Base + Triangle[0], Base + Triangle[2], Base + Triangle[1]});
			}
		};
		const Chaos::FTrimeshIndexBuffer& Elements = TriMesh->Elements();
		if (Elements.RequiresLargeIndices())
		{
			Append(Elements.GetLargeIndexBuffer());
		}
		else
		{
			Append(Elements.GetSmallIndexBuffer());
		}
	}

	if (Out.Indices.Num() > 0)
	{
		return true;
	}

	for (const FKBoxElem& Box : Setup->AggGeom.BoxElems)
	{
		const FTransform Pose   = Box.GetTransform();
		const FVector    Half   = FVector(Box.X, Box.Y, Box.Z) * 0.5;
		const FVector    Center = Pose.GetLocation();
		const auto       Corner = [&Pose, &Half](const double X, const double Y, const double Z) { return Pose.TransformPosition(FVector(X, Y, Z) * Half); };

		for (int32 Axis = 0; Axis < 3; ++Axis)
		{
			for (const double Side : {-1.0, 1.0})
			{
				FVector Q[4];
				for (int32 k = 0; k < 4; ++k)
				{
					FVector Local;
					Local[Axis]           = Side;
					Local[(Axis + 1) % 3] = (k == 1 || k == 2) ? 1.0 : -1.0;
					Local[(Axis + 2) % 3] = k >= 2 ? 1.0 : -1.0;
					Q[k]                  = Corner(Local.X, Local.Y, Local.Z);
				}
				const FVector Outward = (Q[0] + Q[2]) * 0.5 - Center;
				Out.AddTriangle(Q[0], Q[1], Q[2], Outward);
				Out.AddTriangle(Q[0], Q[2], Q[3], Outward);
			}
		}
	}

	for (const FKConvexElem& Element : Setup->AggGeom.ConvexElems)
	{
		const auto& Convex = Element.GetChaosConvexMesh();
		if (!Convex)
		{
			continue;
		}

		const FTransform Pose = Element.GetTransform();
		FVector          Center(0.0);
		for (int32 v = 0; v < Convex->NumVertices(); ++v)
		{
			Center += Pose.TransformPosition(FVector(Convex->GetVertex(v)));
		}
		Center /= FMath::Max(1, Convex->NumVertices());

		for (int32 Plane = 0; Plane < Convex->NumPlanes(); ++Plane)
		{
			const int32 Count = Convex->NumPlaneVertices(Plane);
			const auto  At    = [&](const int32 k) { return Pose.TransformPosition(FVector(Convex->GetVertex(Convex->GetPlaneVertex(Plane, k)))); };
			for (int32 k = 1; k + 1 < Count; ++k)
			{
				const FVector A = At(0);
				Out.AddTriangle(A, At(k), At(k + 1), A - Center);
			}
		}
	}

	return Out.Indices.Num() > 0;
}

}  // namespace

/* //} */

/* Extract() //{ */

bool FWorldMeshExtractor::Extract(UWorld* World, const FVector& GroundOrigin, const FBox& SceneBounds, const int32 MaxTriangles, FWorldMeshResult& Out)
{
	Out = FWorldMeshResult();

	if (!World)
	{
		return false;
	}

	const bool bFilter = SceneBounds.IsValid != 0;

	FBox WorldBounds(EForceInit::ForceInit);
	if (bFilter)
	{
		WorldBounds += FWireFrame::FromGround(SceneBounds.Min, GroundOrigin);
		WorldBounds += FWireFrame::FromGround(SceneBounds.Max, GroundOrigin);
	}

	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor* Actor = *It;

		// robots are observers of the world, not part of its ground truth
		if (Actor->IsA(ARobotPawn::StaticClass()) || (Actor->GetParentActor() && Actor->GetParentActor()->IsA(ARobotPawn::StaticClass())))
		{
			continue;
		}

		TInlineComponentArray<ULandscapeHeightfieldCollisionComponent*> Heightfields(Actor);
		for (const ULandscapeHeightfieldCollisionComponent* Heightfield : Heightfields)
		{
			if (Heightfield && (!bFilter || WorldBounds.Intersect(Heightfield->Bounds.GetBox())))
			{
				AppendLandscape(Heightfield, GroundOrigin, SceneBounds, MaxTriangles, Out);
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

			FLocalMesh   Local;
			const UStaticMesh& Mesh = *Component->GetStaticMesh();
			if (!(CVarWorldMeshFromCollision.GetValueOnAnyThread() == 0 && FromRenderData(Mesh, Local)) && !FromCollision(Mesh, Local))
			{
				continue;
			}
			const int32 NumVertices = Local.Vertices.Num();

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

			const TArray<uint32>& MeshIndices = Local.Indices;

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
					const FVector W = ToWorld.TransformPosition(FVector(Local.Vertices[i]));
					Scene[i]        = FVector3f(FWireFrame::ToGround(W, GroundOrigin));
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
