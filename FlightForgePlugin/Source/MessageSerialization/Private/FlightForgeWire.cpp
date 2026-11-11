#include "FlightForgeWire.h"

static_assert(sizeof(flightforge::wire::Pose) == 56 && alignof(flightforge::wire::Pose) == 8);
static_assert(sizeof(flightforge::wire::Vec3f) == 12);
static_assert(static_cast<int>(flightforge::wire::SensorType::DEPTH_CAMERA) == 9);
