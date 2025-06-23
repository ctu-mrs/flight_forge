#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Sensor.generated.h"

class ADronePawn;

UCLASS()
class UEDS_API USensor : public USceneComponent
{
	GENERATED_BODY()

public:
	USensor();
	
	FName Name;

protected:
	virtual void BeginPlay() override;

	ADronePawn* Owner;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
