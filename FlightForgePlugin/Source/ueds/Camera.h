#pragma once

#include "CoreMinimal.h"
#include "Sensor.h"
#include "Camera.generated.h"

enum CameraMode
{
	CAMERA_MODE_NONE         = 0,
	CAMERA_MODE_RGB          = 1,
	CAMERA_MODE_STEREO       = 2,
	CAMERA_MODE_RGB_SEG      = 3,
};

struct FRgbCameraConfig
{
	bool ShowCameraComponent;

	FVector  Offset;
	FRotator Orientation;
	double   FOVAngle;
	int      Width;
	int      Height;
	bool     enable_temporal_aa;
	bool     enable_hdr;
	bool     enable_raytracing;
	bool     enable_motion_blur;
	double   motion_blur_amount;
	double   motion_blur_distortion;
};

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
class UEDS_API UCamera : public USensor
{
	GENERATED_BODY()

public:
	UCamera();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
