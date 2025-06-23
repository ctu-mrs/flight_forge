#include "Sensor.h"


// Sets default values for this component's properties
USensor::USensor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void USensor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USensor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

