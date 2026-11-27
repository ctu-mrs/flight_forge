#include "RangeFinder.h"

#include "Engine/HitResult.h"
#include "Engine/World.h"

#include "FlightForgeCore/RobotPawn.h"
#include "FlightForgeCore/Server/WireReply.h"

#include <limits>

URangeFinder::URangeFinder()
{
	SensorType = SensorType::RANGEFINDER;
	Name = TEXT("RangeFinder");
	
	PrimaryComponentTick.bCanEverTick = false;
	
	RangefinderConfig.BeamLength = DEFAULT_RANGEFINDER_BEAM_LENGTH;
	RangefinderConfig.Offset     = FVector(0, 0, -10);

	RangefinderHitsCriticalSection = std::make_unique<FCriticalSection>();
}


void URangeFinder::BeginPlay()
{
	Super::BeginPlay();
}

void URangeFinder::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URangeFinder::GetRangefinderData(double& range) {
	RangefinderHitsCriticalSection->Lock();

	const FQuat   Mount                = Owner->GetActorQuat() * RangefinderConfig.Rotation.Quaternion();
	const FVector Start                = Owner->GetActorLocation() + Owner->GetActorRotation().RotateVector(RangefinderConfig.Offset);
	const FVector RangefinderDirection = Mount.GetUpVector() * -1;
	
	range = -1;

	if (UWorld* World = GetWorld()) {
		FCollisionQueryParams Params(SCENE_QUERY_STAT(FlightForgeRangefinder), true);
		Params.AddIgnoredActor(Owner);

		FHitResult HitResult;
		if (World->LineTraceSingleByChannel(HitResult, Start, Start + RangefinderConfig.BeamLength * RangefinderDirection, ECollisionChannel::ECC_Visibility, Params)
		    && HitResult.bBlockingHit) {
			range = HitResult.Distance;
		}
	}

	RangefinderHitsCriticalSection->Unlock();
}

/* wire //{ */

void URangeFinder::GetData(const flightforge::wire::Request& Request, FWireReply& Reply)
{
	double RangeCm = -1.0;
	GetRangefinderData(RangeCm);

	const UWorld* World = GetWorld();
	const double  Range = RangeCm >= 0.0 ? RangeCm / 100.0 : std::numeric_limits<double>::quiet_NaN();
	Reply.Set(flightforge::wire::CreateRangefinderDataResponse(Reply.Builder, World ? World->GetTimeSeconds() : 0.0, Range));
}

void URangeFinder::SetMount(const FVector& OffsetCm, const FRotator& Rotation)
{
	FRangefinderConfig Config = GetRangefinderConfig();
	Config.Offset             = OffsetCm;
	Config.Rotation           = Rotation;
	SetRangefinderConfig(Config);
}

/* //} */
