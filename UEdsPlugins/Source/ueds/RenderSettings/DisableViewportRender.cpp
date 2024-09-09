// Fill out your copyright notice in the Description page of Project Settings.


#include "DisableViewportRender.h"

#include "Kismet/GameplayStatics.h"

void UDisableViewportRender::EnableViewPortRender(const bool bEnabled)
{
	if(UGameViewportClient* VClient = GEngine->GameViewport)
	{
		VClient->bDisableWorldRendering = !bEnabled;
	}
}
