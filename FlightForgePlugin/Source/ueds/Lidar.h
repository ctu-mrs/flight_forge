#pragma once

#include <memory>
#include <vector>

#include "CoreMinimal.h"
#include "Sensor.h"
#include "MessageSerialization/Public/serializable_shared.h"
#include "Lidar.generated.h"

#define DEFAULT_LIDAR_BEAM_HOR 256  // 100
#define DEFAULT_LIDAR_BEAM_VER 32   // 15

#define DEFAULT_LIDAR_BEAM_LENGTH 1000
/* #define DEFAULT_LIDAR_BEAM_HOR 2048 */
/* #define DEFAULT_LIDAR_BEAM_VER 128 */
#define DEFAULT_LIDAR_SHOW_BEAMS false 

struct FLidarConfig
{
	bool   Enable;
	bool Livox; 
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

struct FLivoxDataPoint {
	double Time;
	double Azimuth; // in radians
	double Zenith;  // in radians
};

UCLASS()
class UEDS_API ULidar : public USensor
{
	GENERATED_BODY()

public:
	ULidar();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void GetLidarHits(std::vector<Serializable::Drone::GetLidarData::LidarData>& OutLidarData, FVector& OutStart);

	void GetSegLidarHits(std::vector<Serializable::Drone::GetLidarSegData::LidarSegData>& OutLidarSegData, FVector& OutStart);

	void GetIntLidarHits(std::vector<Serializable::Drone::GetLidarIntData::LidarIntData>& OutLidarIntData, FVector& OutStart);

	FLidarConfig GetLidarConfig();
	bool         SetLidarConfig(const FLidarConfig& Config);
	
	bool LoadCSVData(const FString& FilePath);
	
	TArray<FLivoxDataPoint> LivoxData;
	//bool bLivox;
	int StartIndex;
	
	FString CSVFilePath;

protected:
	virtual void BeginPlay() override;
	
private:
	void UpdateLidar(bool isExternallyLocked);

	void UpdateSegLidar(bool isExternallyLocked);

	void UpdateIntLidar(bool isExternallyLocked);

	#if PLATFORM_WINDOWS
		std::unique_ptr<FWindowsCriticalSection> LidarHitsCriticalSection;
		std::unique_ptr<FWindowsCriticalSection> LidarSegHitsCriticalSection;
		std::unique_ptr<FWindowsCriticalSection> LidarIntHitsCriticalSection;
	#else
		std::unique_ptr<FPThreadsCriticalSection> LidarHitsCriticalSection;
		std::unique_ptr<FPThreadsCriticalSection> LidarSegHitsCriticalSection;
		std::unique_ptr<FPThreadsCriticalSection> LidarIntHitsCriticalSection;
	#endif
	std::unique_ptr<std::vector<std::tuple<double, double, double, double>>>      LidarHits;
	std::unique_ptr<std::vector<std::tuple<double, double, double, double, int>>> LidarSegHits;
	std::unique_ptr<std::vector<std::tuple<double, double, double, double, int>>> LidarIntHits;
	std::unique_ptr<FVector>                                                      LidarHitStart;

	FLidarConfig LidarConfig;
};
