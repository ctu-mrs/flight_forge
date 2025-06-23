#pragma once

#include "CoreMinimal.h"
#include "Sensor.h"
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

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
