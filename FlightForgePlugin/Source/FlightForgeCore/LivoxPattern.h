// Livox non-repetitive scan patterns, loaded from packed .ffpat tables.

#pragma once

#include "CoreMinimal.h"

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
// loaded once per process and shared. Sensors are named after their pattern
// file (Content/Lidar/<name>.ffpat), so adding one is a drop-in rather than a
// protocol change. Get() is thread-safe and returns nullptr for an unknown or
// malformed pattern.
class FLivoxPatternLibrary
{
public:
  static const FLivoxPattern* Get(const FString& SensorName);
};

//}
