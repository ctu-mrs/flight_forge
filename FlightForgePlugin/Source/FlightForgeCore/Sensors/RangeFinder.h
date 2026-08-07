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
class FLIGHTFORGECORE_API URangeFinder : public USensor
{
	GENERATED_BODY()

public:
	URangeFinder();
	
	void GetData(std::stringstream& OutputStream) override;
	void GetConfig(std::stringstream& OutputStream) override;
	void SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream) override;
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
	void GetRangefinderData(double& range);

  std::unique_ptr<FCriticalSection> RangefinderHitsCriticalSection;
	
	
};
