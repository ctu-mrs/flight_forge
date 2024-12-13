// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CsvPrinter.generated.h"

/**
 * 
 */
UCLASS()
class FLIGHTFORGEPLUGIN_API UCsvPrinter : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category="Csv")
	static void WriteGateTransformToCsv(const FString& Filename, const TArray<FString>& GateNames, const TArray<FVector>& Vectors, const TArray<float>& Heading, bool& bOutResult, FString& OutString); 
};
