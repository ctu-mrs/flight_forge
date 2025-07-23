#pragma once

#include "CoreMinimal.h"
#include "ueds/Instruction.h"
#include "ImageUtils.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Sensor.h"

#include "Camera.generated.h"


using Serializable::GameMode::CameraCaptureModeEnum;

UCLASS()
class UEDS_API UCamera : public USensor
{
	GENERATED_BODY()

public:
	UCamera();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UTextureRenderTarget2D* RenderTarget2D;

	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
	UStaticMeshComponent* SceneCaptureMeshHolder;

	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite)
	USceneCaptureComponent2D* SceneCaptureComponent2D;
	
	UPROPERTY(EditAnywhere, Category = "Segmentation PostProcess Setup")
	UMaterial* PostProcessMaterial = nullptr; //is never set in code


#if PLATFORM_WINDOWS
	std::unique_ptr<FWindowsCriticalSection> CameraBufferCriticalSection;
#else
	std::unique_ptr<FPThreadsCriticalSection> CameraBufferCriticalSection;
#endif
	
	TArray<FColor> CameraBuffer;

	double camera_last_request_time_ = 0;

	double stamp_ = 0;
	
	virtual void BeginDestroy() override;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void SetCameraCaptureMode(CameraCaptureModeEnum CaptureMode);
	
	virtual void UpdateCamera(bool isExternallyLocked, double stamp);
	
	virtual void GetConfig(std::stringstream& OutputStream) override;
	virtual void SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream) override;
	virtual void GetData(std::stringstream& OutputStream) override;

	
protected:
	CameraCaptureModeEnum CameraCaptureMode = CameraCaptureModeEnum::CAPTURE_ALL_FRAMES;
	bool CameraNeedsRefresh = false;

	std::unique_ptr<TArray<uint8>> CompressedCameraData = std::make_unique<TArray<uint8>>();

	bool CameraDataNeedsCompress         = false;

	bool CameraRendered                  = false;

	void TransformImageArray(int32 ImageWidth, int32 ImageHeight, const TArray<FColor> &SrcData, TArray<uint8> &DstData);
};
