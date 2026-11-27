#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DeviceMeshLibrary.generated.h"

class UglTFRuntimeAsset;
class UStaticMesh;
class UStaticMeshComponent;

/* UDeviceMeshLoad //{ */

// one glTF housing on its way in: parsed and built off the game thread, then handed to every component waiting for it
UCLASS()
class UDeviceMeshLoad : public UObject
{
	GENERATED_BODY()

public:
	struct FTarget
	{
		TWeakObjectPtr<UStaticMeshComponent> Component;
		FVector                              Offset;
		FRotator                             Rotation;
	};

	void Begin(const FString& InPath);
	void Add(const FTarget& Target);

private:
	UFUNCTION()
	void OnMesh(UStaticMesh* StaticMesh);

	void Apply(const FTarget& Target) const;

	FString Path;
	double  Start = 0.0;

	UPROPERTY()
	TObjectPtr<UglTFRuntimeAsset> Asset;

	UPROPERTY()
	TObjectPtr<UStaticMesh> Mesh;

	TArray<FTarget> Targets;
};

//}

/* UDeviceMeshLibrary //{ */

// device housings imported once per session and shared by every drone that mounts the device
UCLASS()
class FLIGHTFORGECORE_API UDeviceMeshLibrary : public UObject
{
	GENERATED_BODY()

public:
	void Request(const FString& Path, UStaticMeshComponent* Component, const FVector& Offset, const FRotator& Rotation);

private:
	UPROPERTY()
	TMap<FString, TObjectPtr<UDeviceMeshLoad>> Loads;
};

//}
