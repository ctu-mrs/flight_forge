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
	EVENT_CAMERA = 6,
	FISHEYE_CAMERA = 7,
	INSTANCE_SEG_CAMERA = 8,
	DEPTH_CAMERA = 9,
	MAX_SENSOR_TYPE
};

class ADronePawn;

UCLASS()
class FLIGHTFORGECORE_API USensor : public USceneComponent
{
	GENERATED_BODY()

public:
	USensor();
	
	UFUNCTION(BlueprintCallable, Category = "FlightForge|Sensors")
	virtual void Initialize(int InSensorID);
	FName Name;
    
    virtual void GetData(std::stringstream& OutputStream) {};
    virtual void GetConfig(std::stringstream& OutputStream) {};
    virtual void SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream) {};

	// runs queued server-thread instructions on the game thread; the tick does this, and so does the pawn while its server stops
	virtual void DrainInstructions() {};
	int GetSensorID();

	::SensorType GetSensorType() const { return SensorType; }

	/**
	 * A visible body at the sensor's mount point, so a rig can be checked by eye and so
	 * the sensor occludes the way the real hardware does. Traces from the sensor that
	 * owns it skip it - a lidar cannot see its own housing - but every other sensor can,
	 * as can this one's view of the airframe it is bolted to.
	 */
	void UpdateSensorBody(bool bVisible, const FVector& MountOffset, const FRotator& MountRotation, const FVector& BodySize);

	UPrimitiveComponent* GetSensorBody() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	UStaticMeshComponent* SensorBody = nullptr;

	ADronePawn* Owner;
	
	int SensorID;
	SensorType SensorType;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
