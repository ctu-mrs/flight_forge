// Copyright [2022] <Jakub Jirkal>
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include "serializable_shared.h"

namespace Serializable::Drone::GetLocation
{
inline std::unique_ptr<Response> FVectorToResponse(FVector& Vector) {
  auto response = std::make_unique<Response>();
  response->x   = Vector.X;
  response->y   = Vector.Y;
  response->z   = Vector.Z;
  return response;
}
}  // namespace Serializable::Drone::GetLocation

namespace Serializable::GameMode::GetWorldOrigin
{
  inline std::unique_ptr<Response> FVectorToResponse(FVector& Vector) {
    auto response = std::make_unique<Response>();
    response->x   = Vector.X;
    response->y   = Vector.Y;
    response->z   = Vector.Z;
    return response;
  }
}  // namespace Serializable::GameMode::GetWorldOrigin

namespace Serializable::Drone::GetRotation
{
inline std::unique_ptr<Response> FRotatorToResponse(FRotator& Rotator) {
  auto response   = std::make_unique<Response>();
  response->pitch = Rotator.Pitch;
  response->yaw   = Rotator.Yaw;
  response->roll  = Rotator.Roll;
  return response;
}
}  // namespace Serializable::Drone::GetRotation
