#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "Sensor.h"
#include "RangeFinder.generated.h"


#define DEFAULT_RANGEFINDER_BEAM_LENGTH 3000


struct FRangefinderConfig
{
	double   BeamLength;
	FVector  Offset;
	FRotator Rotation = FRotator::ZeroRotator;  // the beam points along the mount's -Z
};


UCLASS()
class FLIGHTFORGECORE_API URangeFinder : public USensor
{
	GENERATED_BODY()

public:
	URangeFinder();
	
	virtual void GetData(const flightforge::wire::Request& Request, FWireReply& Reply) override;
	virtual void SetMount(const FVector& OffsetCm, const FRotator& Rotation) override;
protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	FRangefinderConfig GetRangefinderConfig() const { return RangefinderConfig; }

	void SetRangefinderConfig(const FRangefinderConfig& Config)
	{
		RangefinderHitsCriticalSection->Lock();
		RangefinderConfig = Config;
		RangefinderHitsCriticalSection->Unlock();
	}

private:
	FRangefinderConfig RangefinderConfig;
	// cm, negative without a return
	void GetRangefinderData(double& range);

  std::unique_ptr<FCriticalSection> RangefinderHitsCriticalSection;
	
	
};
