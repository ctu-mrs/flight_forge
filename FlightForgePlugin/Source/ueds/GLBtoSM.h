#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GLBtoSM.generated.h"

UCLASS(Blueprintable)
class UEDS_API AGLBtoSM : public AActor {
GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintImplementableEvent)
	UStaticMesh* GLBtoSM(const FString& AbsPathToFile);
};
