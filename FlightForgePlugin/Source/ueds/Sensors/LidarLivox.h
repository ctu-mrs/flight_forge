#pragma once


#include "CoreMinimal.h"
#include "Lidar.h"
#include "LidarLivox.generated.h"


struct FLivoxDataPoint {
	double Time;
	double Azimuth; // in radians
	double Zenith;  // in radians
};

UCLASS()
class UEDS_API ULidarLivox : public ULidar
{
	GENERATED_BODY()

public:
	ULidarLivox();

	TArray<FLivoxDataPoint> LivoxData;
	int StartIndex;
	
protected:
	virtual void BeginPlay() override;

	virtual void UpdateLidar(bool isExternallyLocked) override;
	virtual void UpdateIntLidar(bool isExternallyLocked) override;
	virtual void UpdateSegLidar(bool isExternallyLocked) override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	bool LoadCSVData(const FString& FilePath);
	

private:
	FString CSVFilePath;
};
