#include "Sensor.h"

#include "Async/TaskGraphInterfaces.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "FlightForgeCore/RobotPawn.h"
#include "FlightForgeCore/Server/WireReply.h"
#include "UObject/ConstructorHelpers.h"

USensor::USensor()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USensor::Initialize(int InSensorID)
{
	SensorID = InSensorID;
}


void USensor::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ARobotPawn>(GetOwner());

	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("USensor::BeginPlay - Owner is not a valid ARobotPawn"));
	}
}


void USensor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USensor::GetData(const flightforge::wire::Request& Request, FWireReply& Reply)
{
	Reply.Fail(FString::Printf(TEXT("sensor %d does not serve this request"), SensorID));
}

void USensor::GetConfig(FWireReply& Reply)
{
	Reply.Fail(FString::Printf(TEXT("sensor %d has no config of this kind"), SensorID));
}

void USensor::SetConfig(const flightforge::wire::Request& Request, FWireReply& Reply)
{
	Reply.Fail(FString::Printf(TEXT("sensor %d has no config of this kind"), SensorID));
}

int USensor::GetSensorID()
{
	return SensorID;
}

/* UpdateSensorBody() //{ */

void USensor::UpdateSensorBody(const bool bVisible, const FVector& MountOffset, const FRotator& MountRotation, const FVector& BodySize)
{
	if (!bVisible && !SensorBody)
	{
		return;
	}

	// sensor configuration arrives on the server thread, and creating or registering a
	// component anywhere but the game thread tears down the object system
	if (!IsInGameThread())
	{
		TWeakObjectPtr<USensor> WeakThis(this);
		AsyncTask(ENamedThreads::GameThread, [WeakThis, bVisible, MountOffset, MountRotation, BodySize] {
			if (USensor* Sensor = WeakThis.Get())
			{
				Sensor->UpdateSensorBody(bVisible, MountOffset, MountRotation, BodySize);
			}
		});
		return;
	}

	if (!SensorBody)
	{
		SensorBody = NewObject<UStaticMeshComponent>(this, *FString::Printf(TEXT("SensorBody_%d"), SensorID));
		SensorBody->SetupAttachment(this);
		SensorBody->SetMobility(EComponentMobility::Movable);
		SensorBody->RegisterComponent();

		if (UStaticMesh* Cube = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube")))
		{
			SensorBody->SetStaticMesh(Cube);
		}

		// visible to sensors and to the eye, but never to physics: a housing that pushed
		// the airframe around would change the flight dynamics it is only meant to document
		SensorBody->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SensorBody->SetCollisionObjectType(ECC_WorldStatic);
		SensorBody->SetCollisionResponseToAllChannels(ECR_Block);
		SensorBody->SetGenerateOverlapEvents(false);
		SensorBody->SetCastShadow(false);
	}

	SensorBody->SetRelativeLocation(MountOffset);
	SensorBody->SetRelativeRotation(MountRotation);

	// the engine cube is 100 cm on a side, so scale is the size in metres
	SensorBody->SetRelativeScale3D(BodySize / 100.0);
	SensorBody->SetVisibility(bVisible);
	SensorBody->SetHiddenInGame(!bVisible);
}

/* //} */

/* GetSensorBody() //{ */

UPrimitiveComponent* USensor::GetSensorBody() const
{
	return SensorBody;
}

/* //} */

