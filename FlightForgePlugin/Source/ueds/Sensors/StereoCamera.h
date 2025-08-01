// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera.h"
#include "StereoCamera.generated.h"


struct FStereoCameraConfig
{
	bool ShowCameraComponent;

	FVector  Offset;
	FRotator Orientation;
	double   FOVAngle;
	int      Width;
	int      Height;
	double   baseline;
	bool     enable_temporal_aa;
	bool     enable_hdr;
	bool     enable_raytracing;
};


UCLASS()
class UEDS_API UStereoCamera : public UCamera
{
	GENERATED_BODY()

public:
	UStereoCamera();

	//base camera properties are used as stereo left
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTextureRenderTarget2D* RenderTarget2DStereoRight;
	
	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
	UStaticMeshComponent* SceneCaptureMeshHolderStereoRight;

	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
	USceneCaptureComponent2D* SceneCaptureComponent2DStereoRight;

	TArray<FColor> StereoRightCameraBuffer;
	
	//FStereoCameraConfig GetStereoCameraConfig();

	std::unique_ptr<TQueue<std::shared_ptr<FInstruction<UStereoCamera>>>> InstructionQueue;
	
	virtual void BeginDestroy() override;
	
protected:
	virtual void BeginPlay() override;

	FStereoCameraConfig stereo_camera_config_;

	bool GetStereoCameraDataFromServerThread(TArray<uint8>& image_left, TArray<uint8>& image_right, double &stamp);

	std::unique_ptr<TArray<uint8>> CompressedStereoRightCameraData = std::make_unique<TArray<uint8>>();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	bool SetStereoCameraConfig(const FStereoCameraConfig& Config);

	virtual void UpdateCamera(bool isExternallyLocked, double stamp) override;
	
	virtual void GetConfig(std::stringstream& OutputStream) override;
	virtual void SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream) override;
	virtual void GetData(std::stringstream& OutputStream) override;
};
