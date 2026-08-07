// Livox non-repetitive scan patterns, loaded from packed .ffpat tables.

#pragma once

#include "CoreMinimal.h"

/* ELivoxSensor //{ */

// Wire values; keep stable, clients depend on the integers.
enum class ELivoxSensor : int32
{
  None   = 0,
  Mid360 = 1,
  Avia   = 2,
};

//}

/* FLivoxPattern //{ */

// One sensor's full scan pattern. Directions are unit vectors in the lidar's
// local UE frame, in emission order; index i is emitted at i/PointRate seconds
// into the pattern, which is what makes a per-point timestamp derivable on the
// client without putting one on the wire.
struct FLivoxPattern
{
  TArray<FVector3f> Directions;

  double PointRate  = 0.0;  // points per second
  double Period     = 0.0;  // seconds for one full pattern loop
  double MinRangeCm = 0.0;
  double MaxRangeCm = 0.0;

  int32 Num() const {
    return Directions.Num();
  }
};

//}

/* FLivoxPatternLibrary //{ */

// Patterns are large (10-12 MB) and identical for every drone, so they are
// loaded once per process and shared. Get() is thread-safe and returns nullptr
// if the table is missing or malformed.
class FLivoxPatternLibrary
{
public:
  static const FLivoxPattern* Get(ELivoxSensor Sensor);

  static const TCHAR* Name(ELivoxSensor Sensor);
};

//}
