#pragma once

#include "Modules/ModuleManager.h"
#include "SerializableExtended.h"

class MESSAGESERIALIZATION_API Serialization : public IModuleInterface {
public:
  static void SerializeRequest(Serializable::Common::NetworkRequest& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::Common::Ping::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::Drone::GetLocation::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::Drone::GetCrashState::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::Drone::SetLocation::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::Drone::GetRgbCameraData::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::Drone::GetStereoCameraData::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::Drone::GetRgbSegCameraData::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::Drone::GetRotation::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::Drone::SetRotation::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::Drone::SetLocationAndRotation::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::Drone::SetLocationAndRotationAsync::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::Drone::GetLidarData::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::Drone::GetLidarSegData::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::Drone::GetLidarConfig::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::Drone::SetLidarConfig::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::Drone::GetRgbCameraConfig::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::Drone::SetRgbCameraConfig::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::Drone::GetStereoCameraConfig::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::Drone::SetStereoCameraConfig::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::Drone::GetMoveLineVisible::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::Drone::SetMoveLineVisible::Request& Request, std::stringstream& InputStream);

  static void SerializeRequest(Serializable::GameMode::GetDrones::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::GameMode::SpawnDrone::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::GameMode::RemoveDrone::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::GameMode::GetCameraCaptureMode::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::GameMode::SetCameraCaptureMode::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::GameMode::GetFps::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::GameMode::GetApiVersion::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::GameMode::GetTime::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::GameMode::SetGraphicsSettings::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::GameMode::SwitchWorldLevel::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::GameMode::SetForestDensity::Request& Request, std::stringstream& InputStream);
  static void SerializeRequest(Serializable::GameMode::SetForestHillyLevel::Request& Request, std::stringstream& InputStream);

  static void DeserializeResponse(Serializable::Common::NetworkResponse& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::Common::Ping::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::Drone::GetLocation::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::Drone::GetCrashState::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::Drone::SetLocation::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::Drone::GetRgbCameraData::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::Drone::GetRgbSegCameraData::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::Drone::GetStereoCameraData::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::Drone::GetRotation::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::Drone::SetRotation::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::Drone::SetLocationAndRotation::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::Drone::SetLocationAndRotationAsync::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::Drone::GetLidarData::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::Drone::GetLidarSegData::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::Drone::GetLidarConfig::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::Drone::SetLidarConfig::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::Drone::GetRgbCameraConfig::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::Drone::SetRgbCameraConfig::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::Drone::GetStereoCameraConfig::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::Drone::SetStereoCameraConfig::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::Drone::GetMoveLineVisible::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::Drone::SetMoveLineVisible::Response& Response, std::stringstream& OutputStream);

  static void DeserializeResponse(Serializable::GameMode::GetDrones::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::GameMode::SpawnDrone::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::GameMode::RemoveDrone::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::GameMode::GetCameraCaptureMode::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::GameMode::SetCameraCaptureMode::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::GameMode::GetFps::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::GameMode::GetApiVersion::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::GameMode::GetTime::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::GameMode::SetGraphicsSettings::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::GameMode::SwitchWorldLevel::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::GameMode::SetForestDensity::Response& Response, std::stringstream& OutputStream);
  static void DeserializeResponse(Serializable::GameMode::SetForestHillyLevel::Response& Response, std::stringstream& OutputStream);
};