#include "SimClock.h"

std::atomic<bool>   FFlightForgeSimClock::bLockstep{false};
std::atomic<double> FFlightForgeSimClock::FixedDt{0.0};
std::atomic<double> FFlightForgeSimClock::AdaptiveMinDt{0.0};
