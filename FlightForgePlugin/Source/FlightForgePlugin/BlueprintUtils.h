// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintUtils.generated.h"

/**
 * 
 */
USTRUCT()
struct FGateData
{
	GENERATED_BODY()
	
	FString GateName;
	FVector Vector;
	float Heading;
};


UCLASS()
class FLIGHTFORGEPLUGIN_API UBlueprintUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category="Csv")
	static void WriteGateTransformToCsv(const FString& Filename, const TArray<FString>& GateNames, const TArray<FVector>& Vectors, const TArray<float>& Heading, bool& bOutResult, FString& OutString); 
};
