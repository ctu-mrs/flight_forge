#include "RangeFinder.h"
#include "DronePawn.h"


URangeFinder::URangeFinder()
{
	SensorType = SensorType::RANGEFINDER;
	Name = TEXT("RangeFinder");
	
	PrimaryComponentTick.bCanEverTick = false;
	
	RangefinderConfig.BeamLength = DEFAULT_RANGEFINDER_BEAM_LENGTH;
	RangefinderConfig.Offset     = FVector(0, 0, -10);

#if PLATFORM_WINDOWS
	RangefinderHitsCriticalSection = std::make_unique<FWindowsCriticalSection>();
#else
	RangefinderHitsCriticalSection    = std::make_unique<FPThreadsCriticalSection>();
#endif
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
	// FVector Start                = GetActorLocation() + GetActorRotation().RotateVector(RangefinderConfig.Offset);
	// FVector RangefinderDirection = -GetActorUpVector();

	FVector Start                = Owner->GetActorLocation() + Owner->GetActorRotation().RotateVector(RangefinderConfig.Offset);
	FVector RangefinderDirection = Owner->GetActorUpVector() * -1;
	
	if (UWorld* World = GetWorld()) {
		FHitResult HitResult;
		if (World->LineTraceSingleByChannel(HitResult, Start, Start + RangefinderConfig.BeamLength * RangefinderDirection, ECollisionChannel::ECC_Visibility)) {
			if (HitResult.bBlockingHit) {
				range = HitResult.Distance;
			} else {
				range = -1;
			}
			// UE_LOG(LogTemp, Warning, TEXT("Rangefinder range = %lf"), range);
		}
	}

	RangefinderHitsCriticalSection->Unlock();
}

