#include "Sensor.h"
#include "DronePawn.h"

USensor::USensor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USensor::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ADronePawn>(GetOwner());

	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("USensor::BeginPlay - Owner is not a valid ADronePawn"));
	}
}


void USensor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

