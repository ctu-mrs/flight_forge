#include "LidarComponent.h"

ULidarComponent::ULidarComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void ULidarComponent::BeginPlay()
{
    Super::BeginPlay();
}

void ULidarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    UE_LOG(LogTemp, Log, TEXT("a"));
}