// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera.h"
#include "RgbCamera.h"
#include "RgbSegCamera.generated.h"


UCLASS()
class UEDS_API URgbSegCamera : public UCamera
{
	GENERATED_BODY()

public:
	URgbSegCamera();

	std::unique_ptr<TQueue<std::shared_ptr<FInstruction<URgbSegCamera>>>> InstructionQueue;

protected:
	virtual void BeginPlay() override;

	bool GetRgbSegCameraFromServerThread(TArray<uint8>& OutArray, double &stamp);

	FRgbCameraConfig rgb_camera_config_;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	bool SetRgbSegCameraConfig(const FRgbCameraConfig& Config);

	virtual void UpdateCamera(bool isExternallyLocked, double stamp) override;
	
	virtual void GetConfig(std::stringstream& OutputStream) override;
	virtual void SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream) override;
	
	virtual void GetData(std::stringstream& OutputStream) override;
};
