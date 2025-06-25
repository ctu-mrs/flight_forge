#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Sensor.generated.h"


enum SensorType : int;
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
