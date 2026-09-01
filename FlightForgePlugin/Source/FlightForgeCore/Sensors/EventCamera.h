#pragma once

#include <atomic>

#include "CoreMinimal.h"
#include "Camera.h"
#include "EventGen.h"
#include "EventCamera.generated.h"


/* FEventCameraConfig //{ */

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

	double ThresholdSigma;
	double RefractoryPeriod;
	double CutoffHz;
	double LeakRateHz;
	double ShotNoiseRateHz;
	double HotPixelFraction;

	double MaxEventRateHz;  // readout ceiling, 0 = unlimited
	int    MaxEventsPerFrame;

	bool   UseGpu;
	uint32 Seed;
};

//}

// event camera on linear scene radiance: each capture runs the pixel model (EventGen.usf or its CPU port)
UCLASS()
class FLIGHTFORGECORE_API UEventCamera : public UCamera
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

	// game thread: blocks until every in-flight frame has been turned into events
	virtual void DrainReadback() override;

	// tick length keeping the largest per-pixel change under one threshold, in [MinDt, CurrentDt]
	double SuggestedTimestep(double CurrentDt, double MinDt) const;

protected:
	virtual void BeginPlay() override;

	FEventCameraConfig event_camera_config_;

	/* capture pipeline //{ */

	static constexpr int32 MaxInflight = 3;

	bool   bStreaming    = false;
	bool   bNeedsSeed    = true;
	uint32 FrameIndex    = 0;
	uint32 LastCount     = 0;
	double LastCaptureStamp = 0.0;
	double TimeEpoch     = 0.0;  // origin of the wrapping microsecond clock

	struct FCpuFrame
	{
		TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Slot;
		double                                           TPrev      = 0.0;
		double                                           TCur       = 0.0;
		bool                                             bSeed      = false;
		uint32                                           FrameIndex = 0;
	};

	TSharedPtr<FEventGpuState, ESPMode::ThreadSafe>          GpuState;
	TArray<TSharedPtr<FEventReadback, ESPMode::ThreadSafe>>  GpuInflight;
	TArray<FCpuFrame>                                        CpuInflight;

	TArray<FVector4f> CpuState;
	TArray<uint32>    CpuLastEvent;
	FIntPoint         CpuStateSize = FIntPoint::ZeroValue;

	FEventGenParams BuildParams(double TPrev, double TCur) const;

	void EnqueueGpuFrame(const FEventGenParams& Params, double TPrev, double TCur);
	void PumpInflight();
	void RunCpuModel(const FCameraReadback& Frame, const FEventGenParams& Params, TArray<uint64>& OutPacked, float& OutMaxLogChange);

	void PublishFrame(TArray<uint64>& Packed, uint32 Count, float MaxLogChange, double TPrev, double TCur);

	//}

	/* output stream //{ */

	struct FStreamEvent
	{
		uint32 XYP;
		double Stamp;
	};

	std::unique_ptr<FCriticalSection> EventsCriticalSection;
	TArray<FStreamEvent>              EventBuffer;
	uint32                            Dropped        = 0;
	double                            LastFrameStamp = 0.0;

	std::atomic<double> LastFrameDt{0.0};
	std::atomic<float>  LastMaxLogChange{0.0f};

	//}
};
