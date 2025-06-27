#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Sensor.generated.h"


//enum SensorType : int;

enum class SensorType : int
{
	CAMERA = 0,
	LIDAR = 1,
	RANGEFINDER = 2,
	MAX_SENSOR_TYPE
};

class ADronePawn;

UCLASS()
class UEDS_API USensor : public USceneComponent
{
	GENERATED_BODY()

public:
	USensor();
	
	UFUNCTION(BlueprintCallable)
	void Initialize(int InSensorID);
	
	int GetSensorID();

	// virtual void GetData();
	// virtual void GetConfig();
	// virtual void SetConfig();
		
protected:
	virtual void BeginPlay() override;

	ADronePawn* Owner;
	
	int SensorID;
	FName Name;
	SensorType SensorType;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
