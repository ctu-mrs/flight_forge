#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MessageSerialization/Public/serializable_shared.h"
#include <fstream>
#include "Sensor.generated.h"


enum class SensorType : int
{
	RGB_CAMERA = 0,
	LIDAR = 1,
	RANGEFINDER = 2,
	LIDAR_LIVOX = 3,
	RGB_SEG_CAMERA = 4,
	STEREO_CAMERA = 5,
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
	virtual void Initialize(int InSensorID);
	FName Name;
    
    virtual void GetData(std::stringstream& OutputStream) {};
    virtual void GetConfig(std::stringstream& OutputStream) {};
    virtual void SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream) {};
	int GetSensorID();
		
protected:
	virtual void BeginPlay() override;

	ADronePawn* Owner;
	
	int SensorID;
	SensorType SensorType;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
