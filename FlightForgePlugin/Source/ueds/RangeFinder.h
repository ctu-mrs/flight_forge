#pragma once

#include "CoreMinimal.h"
#include "Sensor.h"
#include "RangeFinder.generated.h"

struct FRangefinderConfig
{
	double BeamLength;
	FVector Offset;
};

UCLASS()
class UEDS_API URangeFinder : public USensor
{
	GENERATED_BODY()

public:
	URangeFinder();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
