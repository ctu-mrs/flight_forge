#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LidarComponent.generated.h"

UCLASS()
class UEDS_API ULidarComponent : public USceneComponent
{
    GENERATED_BODY()

public:
ULidarComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
