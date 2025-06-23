#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "Sensor.h"
#include "RangeFinder.generated.h"


#define DEFAULT_RANGEFINDER_BEAM_LENGTH 3000


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

	void GetRangefinderData(double& range);

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
private:
	FRangefinderConfig RangefinderConfig;

#if PLATFORM_WINDOWS
  std::unique_ptr<FWindowsCriticalSection> RangefinderHitsCriticalSection;
#else
  std::unique_ptr<FPThreadsCriticalSection> RangefinderHitsCriticalSection;
#endif
	
	
};
