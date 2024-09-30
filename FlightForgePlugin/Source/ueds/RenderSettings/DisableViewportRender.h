// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DisableViewportRender.generated.h"

/**
 * 
 */
UCLASS()
class UEDS_API UDisableViewportRender : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Enable ViewPort Render", Keywords = "viewport rendering enable disable"), Category = "Viewport")
	static void EnableViewPortRender(const bool bEnabled);
};
