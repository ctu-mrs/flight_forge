#include "Sensor.h"
#include "DronePawn.h"

USensor::USensor()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USensor::Initialize(int InSensorID)
{
	SensorID = InSensorID;
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

int USensor::GetSensorID()
{
	return SensorID;
}

