#include "DistanceFieldQuery.h"
#include "WireFrame.h"

#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "FlightForgeCore/RobotPawn.h"
#include "Landscape.h"

namespace
{

/* terrain + overlap helpers //{ */

bool IsTerrain(const AActor* Actor)
{
	return Actor && (Actor->IsA(ALandscape::StaticClass()) || Actor->ActorHasTag(FName(TEXT("terrain"))));
}

bool SphereBlocked(UWorld* World, const FVector& Center, const float RadiusCm, const bool bTerrainOnly, const FCollisionQueryParams& Params)
{
	if (!bTerrainOnly)
	{
		return World->OverlapBlockingTestByChannel(Center, FQuat::Identity, ECC_WorldStatic, FCollisionShape::MakeSphere(RadiusCm), Params);
	}

	TArray<FOverlapResult> Overlaps;
	World->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity, ECC_WorldStatic, FCollisionShape::MakeSphere(RadiusCm), Params);

	for (const FOverlapResult& Overlap : Overlaps)
	{
		if (IsTerrain(Overlap.GetActor()))
		{
			return true;
		}
	}

	return false;
}

/* //} */

}  // namespace

/* Batch() //{ */

bool FDistanceFieldQuery::Batch(UWorld* World, const FVector& GroundOrigin, const TArray<float>& QueryPoints, const double MaxRadiusM, double ToleranceM,
                                const bool bTerrainOnly, TArray<float>& OutDistances)
{
	constexpr int64 MaxQueries = 2000000;

	OutDistances.Reset();

	if (!World || QueryPoints.Num() == 0 || QueryPoints.Num() % 3 != 0 || QueryPoints.Num() / 3 > MaxQueries || MaxRadiusM <= 0.0)
	{
		return false;
	}

	ToleranceM = FMath::Clamp(ToleranceM, 0.0001, MaxRadiusM);

	// complex collision is the surface the lidar hits, but it is hollow - containment
	// only registers against simple collision, so the inside test uses its own params
	FCollisionQueryParams Params(FName(TEXT("FlightForgeDistanceQuery")), /*bTraceComplex=*/true);
	FCollisionQueryParams InsideParams(FName(TEXT("FlightForgeInsideQuery")), /*bTraceComplex=*/false);
	for (TActorIterator<ARobotPawn> It(World); It; ++It)
	{
		Params.AddIgnoredActor(*It);
		InsideParams.AddIgnoredActor(*It);
	}

	const float  MaxRadiusCm = static_cast<float>(MaxRadiusM * 100.0);
	const double ToleranceCm = ToleranceM * 100.0;
	const int32  NumPoints   = QueryPoints.Num() / 3;

	OutDistances.SetNumUninitialized(NumPoints);

	const double StartSeconds = FPlatformTime::Seconds();

	for (int32 i = 0; i < NumPoints; ++i)
	{
		const FVector P = FWireFrame::FromGround(FVector(QueryPoints[i * 3], QueryPoints[i * 3 + 1], QueryPoints[i * 3 + 2]), GroundOrigin);

		if (SphereBlocked(World, P, 0.1f, bTerrainOnly, InsideParams))
		{
			OutDistances[i] = 0.0f;
			continue;
		}

		if (!SphereBlocked(World, P, MaxRadiusCm, bTerrainOnly, Params))
		{
			OutDistances[i] = static_cast<float>(MaxRadiusM);
			continue;
		}

		double Lo = 0.0, Hi = MaxRadiusCm;
		while (Hi - Lo > ToleranceCm)
		{
			const double Mid = 0.5 * (Lo + Hi);
			if (SphereBlocked(World, P, static_cast<float>(Mid), bTerrainOnly, Params))
			{
				Hi = Mid;
			}
			else
			{
				Lo = Mid;
			}
		}

		OutDistances[i] = static_cast<float>(0.5 * (Lo + Hi) / 100.0);

		if ((i + 1) % 100000 == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("DistanceFieldQuery: %d/%d points, %.1f s elapsed - the game thread is intentionally blocked"), i + 1, NumPoints,
			       FPlatformTime::Seconds() - StartSeconds);
		}
	}

	return true;
}

/* //} */
