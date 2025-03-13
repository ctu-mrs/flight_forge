// Copyright (C) 2023 Gonçalo Marques - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "GenericPlatform/ICursor.h"
#include "UINavGameViewportClient.generated.h"

class FViewport;
struct FWorldContext;
class UGameInstance;

/**
 * 
 */
UCLASS()
class UINAVIGATION_API UUINavGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	
public:
	virtual void Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice = true) override;

	virtual EMouseCursor::Type GetCursor(FViewport* InViewport, int32 X, int32 Y) override;
};
