#pragma once

#include <memory>
#include <vector>

#include "CoreMinimal.h"
#include "Sensor.h"
#include "ueds/LivoxPattern.h"
//#include "MessageSerialization/Public/serializable_shared.h"
#include "Lidar.generated.h"

#define DEFAULT_LIDAR_BEAM_HOR 256  // 100
#define DEFAULT_LIDAR_BEAM_VER 32   // 15

#define DEFAULT_LIDAR_BEAM_LENGTH 1000
/* #define DEFAULT_LIDAR_BEAM_HOR 2048 */
/* #define DEFAULT_LIDAR_BEAM_VER 128 */
#define DEFAULT_LIDAR_SHOW_BEAMS false 

enum class FLidarMode : int
{
	DEFAULT = 0,
	SEG = 1,
	INTE = 2, //INT is not allowed
	MAX_MODE_TYPE
};

struct FLidarConfig
{
	bool   Enable;
	bool Livox;
	// Which non-repetitive scan pattern to replay when Livox is set: matches the
	// Content/Lidar/<name>.ffpat stem ("avia", "mid360"), so a new sensor is a
	// new pattern file and needs no protocol change.
	FString LivoxSensor;
	bool   ShowBeams;
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

UCLASS()
class UEDS_API ULidar : public USensor
{
	GENERATED_BODY()

public:
	ULidar();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	FLidarConfig GetLidarConfig();
	bool         SetLidarConfig(const FLidarConfig& Config);
	

	virtual void GetConfig(std::stringstream& OutputStream) override;
	virtual void SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream) override;
	virtual void GetData(std::stringstream& OutputStream) override;

	FLidarMode LidarMode = FLidarMode::DEFAULT;
	
	void SetLidarModeWithInt(int type); 
	
protected:
	virtual void BeginPlay() override;

	void GetLidarHits(std::vector<Serializable::Drone::GetLidarData::LidarData>& OutLidarData, FVector& OutStart, double& OutStamp);

	void GetSegLidarHits(std::vector<Serializable::Drone::GetLidarSegData::LidarSegData>& OutLidarSegData, FVector& OutStart, double& OutStamp);

	void GetIntLidarHits(std::vector<Serializable::Drone::GetLidarIntData::LidarIntData>& OutLidarIntData, FVector& OutStart, double& OutStamp);
	
	virtual void UpdateLidar(bool isExternallyLocked);

	virtual void UpdateSegLidar(bool isExternallyLocked);

	virtual void UpdateIntLidar(bool isExternallyLocked);

	// Traces one elapsed-time slice of the active Livox pattern. Prepare(count) is
	// called once the slice size is known so the caller can size its buffer, then
	// Store(index, distance, direction, hit, bHasReturn) reports each ray. Distance
	// and direction are expressed in the lidar frame at the end of the slice, so a
	// moving drone yields the same intra-scan distortion a real sensor produces.
	// Returns the number of rays traced, or 0 if no pattern is available.
	int32 CastLivoxRays(FLivoxCursor& Cursor, TFunctionRef<void(int32)> Prepare,
	                    TFunctionRef<void(int32, double, const FVector&, const FHitResult&, bool)> Store);

	const FLivoxPattern* GetLivoxPattern() const;

	void ResetLivoxCursorsIfChanged(bool LivoxWas, const FString& LivoxSensorWas);

	// Intensity doubles as a material class id here, not a radiometric return.
	static int MaterialIntensity(const FString& PhysMatName);

	FLivoxCursor LivoxCursor;
	FLivoxCursor LivoxSegCursor;
	FLivoxCursor LivoxIntCursor;

	std::unique_ptr<FCriticalSection> LidarHitsCriticalSection;
	std::unique_ptr<FCriticalSection> LidarSegHitsCriticalSection;
	std::unique_ptr<FCriticalSection> LidarIntHitsCriticalSection;
	std::unique_ptr<std::vector<std::tuple<double, double, double, double>>>      LidarHits;
	std::unique_ptr<std::vector<std::tuple<double, double, double, double, int>>> LidarSegHits;
	std::unique_ptr<std::vector<std::tuple<double, double, double, double, int>>> LidarIntHits;
	std::unique_ptr<FVector>                                                      LidarHitStart;

	FLidarConfig LidarConfig;
};
