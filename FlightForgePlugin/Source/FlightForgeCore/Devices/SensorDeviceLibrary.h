#pragma once

#include "CoreMinimal.h"
#include "FlightForgeCore/Sensors/Sensor.h"

class ADronePawn;
class USensor;

/* FSensorDeviceSpec //{ */

enum class EDeviceMeshShape : uint8
{
	Box,
	Cylinder,
};

// one sensor a device creates: its type, its pose within the device housing,
// and a configurator that applies the device's datasheet numbers to the
// freshly created sensor (absolute pose already composed with the mount)
struct FDeviceSensorEntry
{
	SensorType Type;

	FVector  RelOffset   = FVector::ZeroVector;
	FRotator RelRotation = FRotator::ZeroRotator;

	TFunction<void(USensor*, const FVector& AbsOffset, const FRotator& AbsRotation)> Configure;
};

struct FSensorDeviceSpec
{
	FString Name;
	FString Description;

	EDeviceMeshShape MeshShape = EDeviceMeshShape::Box;
	FVector          MeshDimensionsCm = FVector(5, 5, 5);

	TArray<FDeviceSensorEntry> Sensors;
};

//}

// Preconfigured real-world sensor devices, instantiable by name via the
// add_device message. Datasheet values are approximations of the shipping
// hardware; adding a device is one entry in BuildRegistry().
class FSensorDeviceLibrary
{
public:
	static const TArray<FSensorDeviceSpec>& Get();

	static const FSensorDeviceSpec* Find(const FString& Name);
};
