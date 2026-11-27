#pragma once

#include <memory>
#include <vector>

#include "CoreMinimal.h"
#include "Sensor.h"
#include "FlightForgeCore/Instruction.h"
#include "FlightForgeCore/LivoxPattern.h"
#include "Lidar.generated.h"

class USceneCaptureComponent2D;
class UTextureRenderTarget2D;
struct FLidarGpuRequest;

/* #define DEFAULT_LIDAR_BEAM_HOR 256  // 100 */
/* #define DEFAULT_LIDAR_BEAM_VER 32   // 15 */
#define DEFAULT_LIDAR_BEAM_HOR 2048
#define DEFAULT_LIDAR_BEAM_VER 128

#define DEFAULT_LIDAR_BEAM_LENGTH 1000
#define DEFAULT_LIDAR_SHOW_BEAMS false 

// numbered as the wire's LidarBackend
enum class ELidarBackend : uint8
{
	Auto = 0,
	Cpu  = 1,
	Gpu  = 2,
};

struct FLidarConfig
{
	bool   Enable;
	bool Livox;
	FString LivoxSensor;
	bool   ShowBeams;
	bool   ShowSensorBody = false;
	double BeamLength;

	int BeamHorRays;
	int BeamVertRays;

	double   Frequency;
	FVector  Offset;
	FRotator Orientation;
	/* double   FOVHor; */
	double  FOVHorLeft;
	double  FOVHorRight;
	/* double   FOVVert; */
	double   FOVVertUp;
	double   FOVVertDown;
	double   vertRayDiff;
	double   horRayDif;

	bool SemanticSegmentation;

	ELidarBackend Backend = ELidarBackend::Auto;
};

/* FLivoxCursor //{ */

// Playback state of one Livox stream. The pattern advances with elapsed world
// time rather than per request, so a client polling at 100 Hz and one polling
// at 10 Hz walk the same pattern at the same speed - the 10 Hz client just gets
// ten times as much of it per message. Slices are contiguous and
// non-overlapping, so merging ten 100 Hz messages reproduces one 10 Hz message
// exactly.
struct FLivoxCursor
{
	double     PatternPos = 0.0;   // fractional index, monotonic
	double     LastTime   = -1.0;  // world seconds; <0 until the first slice
	FTransform LastPose;           // lidar pose at LastTime, for intra-slice interpolation
	bool       bPoseValid = false;
};

//}

/* FLivoxSlice //{ */

// one Livox request's rays, planned before any is traced; a ray's pose is FromPose slerped to RefPose by its batch
struct FLivoxSlice
{
	const FLivoxPattern* Pattern = nullptr;

	double     Now          = 0.0;
	int32      NumRays      = 0;
	int32      PatternStart = 0;
	int32      RaysPerBatch = 1;
	int32      NumBatches   = 1;
	double     MinRangeCm   = 0.0;
	double     BeamLength   = 0.0;
	FTransform FromPose;
	FTransform RefPose;

	double BatchAlpha(int32 Batch) const { return NumBatches > 1 ? (Batch + 0.5) / NumBatches : 1.0; }
};

//}

/* FLidarScan //{ */

constexpr uint8 LidarChannelNone      = 0;
constexpr uint8 LidarChannelIntensity = 1;
constexpr uint8 LidarChannelLabels    = 2;

// one scan; the channel vectors are empty unless asked for, otherwise parallel to Points
struct FLidarScan
{
	FTransform SensorPose;
	double     Stamp = 0.0;
	uint32     Rows  = 0;
	uint32     Cols  = 0;

	std::vector<FVector3f> Points;  // m, sensor frame (REP-103), NaN for no return
	std::vector<int32>  Intensity;   // material class per point, -1 without a return
	std::vector<int32>  ClassId;     // segmentation stencil, landscape 1, -1 without a return
	std::vector<uint32> InstanceId;  // the instance camera's id space, 0 background

	bool bGpu = false;
};

//}

UCLASS()
class FLIGHTFORGECORE_API ULidar : public USensor
{
	GENERATED_BODY()

public:
	ULidar();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	FLidarConfig GetLidarConfig();
	bool         SetLidarConfig(const FLidarConfig& Config);
	

	virtual void GetConfig(FWireReply& Reply) override;
	virtual void SetConfig(const flightforge::wire::Request& Request, FWireReply& Reply) override;
	virtual void GetData(const flightforge::wire::Request& Request, FWireReply& Reply) override;
	virtual void SetMount(const FVector& OffsetCm, const FRotator& Rotation) override;
	virtual void DrainInstructions() override;

	// the backend a scan would use now
	bool UsesGpu() const;

protected:
	virtual void BeginPlay() override;

	// one trace with the channels asked for; Cursor is the Livox pattern position to advance
	void Scan(FLivoxCursor& Cursor, uint8 Channels, FLidarScan& Out);

	// false when the GPU gave nothing back, and the caller traces on the CPU
	bool ScanGpu(FLivoxCursor& Cursor, uint8 Channels, FLidarScan& Out);

	// game thread: plans the rays, positions the capture and renders it with the trace job attached
	void SubmitGpuScan(FLivoxCursor& Cursor, FLidarGpuRequest& Request);

	void EnsureGpuCapture();

	TSharedPtr<const TArray<FVector4f>, ESPMode::ThreadSafe> RasterRays();

	void Label(FLidarScan& Out, const std::vector<const AActor*>& HitActors, const std::vector<const UPrimitiveComponent*>& HitComponents) const;

	// game thread
	void RebuildComponentIndex();

	std::unique_ptr<TInstructionQueue<ULidar>> InstructionQueue;

	UPROPERTY()
	USceneCaptureComponent2D* GpuCapture = nullptr;

	UPROPERTY()
	UTextureRenderTarget2D* GpuTarget = nullptr;

	TSharedPtr<const TArray<FVector4f>, ESPMode::ThreadSafe> RasterRayCache;
	FIntVector4                                              RasterRayKey = FIntVector4(0);
	FVector4                                                 RasterFovKey = FVector4::Zero();

	FCriticalSection                                         ComponentIndexLock;
	TMap<uint32, TWeakObjectPtr<const UPrimitiveComponent>> ComponentIndex;

	bool bWarnedGpuFallback = false;

	// the last raster scan, served to reads that come sooner than its frequency allows
	bool ReuseLastScan(uint8 Channels, FLidarScan& Out) const;

	FLidarScan LastScan;
	uint8      LastScanChannels = LidarChannelNone;
	bool       bHaveLastScan    = false;

	// Traces one elapsed-time slice of the active Livox pattern. Prepare(count) is
	// called once the slice size is known so the caller can size its buffer, then
	// Store(index, distance, direction, hit, bHasReturn) reports each ray. Distance
	// and direction are expressed in the lidar frame at the end of the slice, so a
	// moving drone yields the same intra-scan distortion a real sensor produces.
	// Returns the number of rays traced, or 0 if no pattern is available.
	// bNeedPhysicalMaterial resolves a material per hit; only seg and intensity read it.
	int32 CastLivoxRays(FLivoxCursor& Cursor, TFunctionRef<void(int32)> Prepare,
	                    TFunctionRef<void(int32, double, const FVector&, const FHitResult&, bool)> Store, bool bNeedPhysicalMaterial = false);

	// false without a pattern or a world; CommitLivoxSlice moves the cursor past the slice
	bool PlanLivoxSlice(FLivoxCursor& Cursor, FLivoxSlice& Out);
	static void CommitLivoxSlice(FLivoxCursor& Cursor, const FLivoxSlice& Slice);

	const FLivoxPattern* GetLivoxPattern() const;

	void ResetLivoxCursorsIfChanged(bool LivoxWas, const FString& LivoxSensorWas);

	// Skips this sensor's own housing so the beams still see the airframe around it.
	void IgnoreOwnBody(FCollisionQueryParams& Params) const;

	// Intensity doubles as a material class id here, not a radiometric return.
	static int MaterialIntensity(const FString& PhysMatName);

	FLivoxCursor LivoxCursor;

	std::unique_ptr<FCriticalSection> LidarHitsCriticalSection;

	FLidarConfig LidarConfig;
};
