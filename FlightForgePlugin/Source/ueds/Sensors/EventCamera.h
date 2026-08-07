#pragma once

#include "CoreMinimal.h"
#include "Camera.h"
#include "EventCamera.generated.h"


struct FEventCameraConfig
{
	bool ShowCameraComponent;

	FVector  Offset;
	FRotator Orientation;
	double   FOVAngle;
	int      Width;
	int      Height;

	// log-intensity contrast thresholds; an event fires each time a pixel's log
	// intensity moves by one threshold since its last event
	double ContrastThresholdPos;
	double ContrastThresholdNeg;

	// stamp events with sim time instead of wall clock; with a fixed engine
	// timestep this yields an exact-rate stream regardless of GPU load
	bool UseSimTime;
};

/* FCameraEvent //{ */

struct FCameraEvent
{
	uint16 X;
	uint16 Y;
	int8   Polarity;  // +1 brighter, -1 darker
	double Stamp;
};

//}

// Frame-differencing event camera: renders like an RGB camera through the
// async readback pipeline, then quantizes per-pixel log-intensity change into
// events. Temporal resolution equals the sim frame rate; events within one
// frame share the frame's stamp.
UCLASS()
class UEDS_API UEventCamera : public UCamera
{
	GENERATED_BODY()

public:
	UEventCamera();

	std::unique_ptr<TQueue<std::shared_ptr<FInstruction<UEventCamera>>>> InstructionQueue;

	bool SetEventCameraConfig(const FEventCameraConfig& Config);

	FEventCameraConfig GetEventCameraConfig() const { return event_camera_config_; }

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void UpdateCamera(bool isExternallyLocked, double stamp) override;

	virtual void GetConfig(std::stringstream& OutputStream) override;
	virtual void SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream) override;
	virtual void GetData(std::stringstream& OutputStream) override;

	virtual void BeginDestroy() override;

protected:
	virtual void BeginPlay() override;

	FEventCameraConfig event_camera_config_;

	// per-pixel log intensity at the time of each pixel's last event
	TArray<float> ReferenceLogIntensity;

	// per-pixel log intensity of the previous frame, for crossing-time interpolation
	TArray<float> PrevLogIntensity;

	FIntPoint ReferenceSize = FIntPoint::ZeroValue;

	std::unique_ptr<FCriticalSection> EventsCriticalSection;
	TArray<FCameraEvent> EventBuffer;
	double LastFrameStamp = 0.0;

	// generates events from the frame just published to CameraBuffer
	void ProcessPublishedFrame();
};
