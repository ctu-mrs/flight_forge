#include "Serialize.h"
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/memory.hpp>

void Serialization::SerializeRequest(Serializable::Common::NetworkRequest& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}

void Serialization::SerializeRequest(Serializable::Common::Ping::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}

void Serialization::SerializeRequest(Serializable::Drone::GetLocation::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}

void Serialization::SerializeRequest(Serializable::Drone::GetCrashState::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}

void Serialization::SerializeRequest(Serializable::Drone::GetRotation::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}

void Serialization::SerializeRequest(Serializable::Drone::SetLocation::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type, Request.x, Request.y, Request.z, Request.checkCollisions);
}

void Serialization::SerializeRequest(Serializable::Drone::SetRotation::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type, Request.pitch, Request.yaw, Request.roll);
}

void Serialization::SerializeRequest(Serializable::Drone::GetRgbCameraData::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}

void Serialization::SerializeRequest(Serializable::Drone::GetStereoCameraData::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}

void Serialization::SerializeRequest(Serializable::Drone::GetRgbSegCameraData::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}

void Serialization::SerializeRequest(Serializable::Drone::SetLocationAndRotation::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type, Request.x, Request.y, Request.z, Request.pitch, Request.yaw, Request.roll, Request.should_collide);
}

void Serialization::SerializeRequest(Serializable::Drone::SetLocationAndRotationAsync::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type, Request.x, Request.y, Request.z, Request.pitch, Request.yaw, Request.roll, Request.should_collide);
}

void Serialization::SerializeRequest(Serializable::Drone::GetRangefinderData::Request& Request,
  std::stringstream& InputStream)
{
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}

void Serialization::SerializeRequest(Serializable::Drone::GetLidarData::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}

void Serialization::SerializeRequest(Serializable::Drone::GetLidarSegData::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}

void Serialization::SerializeRequest(Serializable::Drone::GetLidarIntData::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}
void Serialization::SerializeRequest(Serializable::Drone::GetLidarConfig::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}

struct LidarConfigSerializable : public Serializable::Drone::LidarConfig
{
  LidarConfigSerializable() = default;

  void setData(Serializable::Drone::LidarConfig& _LidarData) {

    Enable = _LidarData.Enable;

    showBeams    = _LidarData.showBeams;
    beamLength   = _LidarData.beamLength;
    BeamHorRays  = _LidarData.BeamHorRays;
    BeamVertRays = _LidarData.BeamVertRays;
    Frequency    = _LidarData.Frequency;
    offsetX      = _LidarData.offsetX;
    offsetY      = _LidarData.offsetY;
    offsetZ      = _LidarData.offsetZ;

    orientationPitch = _LidarData.orientationPitch;
    orientationYaw   = _LidarData.orientationYaw;
    orientationRoll  = _LidarData.orientationRoll;

    FOVHor  = _LidarData.FOVHor;
    FOVVert = _LidarData.FOVVert;
  }

  void outputData(Serializable::Drone::LidarConfig& _LidarData) {
    _LidarData.Enable           = Enable;
    _LidarData.showBeams        = showBeams;
    _LidarData.beamLength       = beamLength;
    _LidarData.BeamHorRays      = BeamHorRays;
    _LidarData.BeamVertRays     = BeamVertRays;
    _LidarData.Frequency        = Frequency;
    _LidarData.offsetX          = offsetX;
    _LidarData.offsetY          = offsetY;
    _LidarData.offsetZ          = offsetZ;
    _LidarData.orientationPitch = orientationPitch;
    _LidarData.orientationYaw   = orientationYaw;
    _LidarData.orientationRoll  = orientationRoll;
    _LidarData.FOVHor           = FOVHor;
    _LidarData.FOVVert          = FOVVert;
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(Enable, showBeams, beamLength, BeamHorRays, BeamVertRays, Frequency, offsetX, offsetY, offsetZ, orientationPitch, orientationYaw, orientationRoll,
            FOVHor, FOVVert);
  }
};

void Serialization::SerializeRequest(Serializable::Drone::SetLidarConfig::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  LidarConfigSerializable    ConfigSerializable{};
  InputArchive(Request.type, ConfigSerializable);
  ConfigSerializable.outputData(Request.config);
}

void Serialization::SerializeRequest(Serializable::Drone::GetRgbCameraConfig::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}

void Serialization::SerializeRequest(Serializable::Drone::GetStereoCameraConfig::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}

struct RgbCameraConfigSerializable : public Serializable::Drone::RgbCameraConfig
{
  RgbCameraConfigSerializable() = default;

  void setData(Serializable::Drone::RgbCameraConfig& _CameraData) {

    showDebugCamera = _CameraData.showDebugCamera;

    offsetX = _CameraData.offsetX;
    offsetY = _CameraData.offsetY;
    offsetZ = _CameraData.offsetZ;

    orientationPitch = _CameraData.orientationPitch;
    orientationYaw   = _CameraData.orientationYaw;
    orientationRoll  = _CameraData.orientationRoll;

    angleFOV = _CameraData.angleFOV;

    Width  = _CameraData.Width;
    Height = _CameraData.Height;

    enable_temporal_aa = _CameraData.enable_temporal_aa;
    enable_raytracing = _CameraData.enable_raytracing;
    enable_hdr = _CameraData.enable_hdr;
  }

  void outputData(Serializable::Drone::RgbCameraConfig& _CameraData) {

    _CameraData.showDebugCamera = showDebugCamera;

    _CameraData.offsetX = offsetX;
    _CameraData.offsetY = offsetY;
    _CameraData.offsetZ = offsetZ;

    _CameraData.orientationPitch = orientationPitch;
    _CameraData.orientationYaw   = orientationYaw;
    _CameraData.orientationRoll  = orientationRoll;

    _CameraData.angleFOV = angleFOV;

    _CameraData.Width  = Width;
    _CameraData.Height = Height;

    _CameraData.enable_temporal_aa = enable_temporal_aa;
    _CameraData.enable_raytracing = enable_raytracing;
    _CameraData.enable_hdr = enable_hdr;
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(showDebugCamera, offsetX, offsetY, offsetZ, orientationPitch, orientationYaw, orientationRoll, angleFOV, Width, Height, enable_temporal_aa, enable_raytracing, enable_hdr);
  }
};

void Serialization::SerializeRequest(Serializable::Drone::SetRgbCameraConfig::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  RgbCameraConfigSerializable  ConfigSerializable{};
  InputArchive(Request.type, ConfigSerializable);
  ConfigSerializable.outputData(Request.config);
}

struct StereoCameraConfigSerializable : public Serializable::Drone::StereoCameraConfig
{
  StereoCameraConfigSerializable() = default;

  void setData(Serializable::Drone::StereoCameraConfig& _CameraData) {

    showDebugCamera = _CameraData.showDebugCamera;

    offsetX = _CameraData.offsetX;
    offsetY = _CameraData.offsetY;
    offsetZ = _CameraData.offsetZ;

    orientationPitch = _CameraData.orientationPitch;
    orientationYaw   = _CameraData.orientationYaw;
    orientationRoll  = _CameraData.orientationRoll;

    angleFOV = _CameraData.angleFOV;

    Width  = _CameraData.Width;
    Height = _CameraData.Height;

    baseline = _CameraData.baseline;

    enable_temporal_aa = _CameraData.enable_temporal_aa;
    enable_raytracing = _CameraData.enable_raytracing;
    enable_hdr = _CameraData.enable_hdr;
  }

  void outputData(Serializable::Drone::StereoCameraConfig& _CameraData) {

    _CameraData.showDebugCamera = showDebugCamera;

    _CameraData.offsetX = offsetX;
    _CameraData.offsetY = offsetY;
    _CameraData.offsetZ = offsetZ;

    _CameraData.orientationPitch = orientationPitch;
    _CameraData.orientationYaw   = orientationYaw;
    _CameraData.orientationRoll  = orientationRoll;

    _CameraData.angleFOV = angleFOV;

    _CameraData.Width  = Width;
    _CameraData.Height = Height;

    _CameraData.baseline = baseline;

    _CameraData.enable_temporal_aa = enable_temporal_aa;
    _CameraData.enable_raytracing = enable_raytracing;
    _CameraData.enable_hdr = enable_hdr;
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(showDebugCamera, offsetX, offsetY, offsetZ, orientationPitch, orientationYaw, orientationRoll, angleFOV, Width, Height, baseline, enable_temporal_aa, enable_raytracing, enable_hdr);
  }
};

void Serialization::SerializeRequest(Serializable::Drone::SetStereoCameraConfig::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  StereoCameraConfigSerializable  ConfigSerializable{};
  InputArchive(Request.type, ConfigSerializable);
  ConfigSerializable.outputData(Request.config);
}

void Serialization::SerializeRequest(Serializable::Drone::GetMoveLineVisible::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}

void Serialization::SerializeRequest(Serializable::Drone::SetMoveLineVisible::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type, Request.visible);
}

void Serialization::SerializeRequest(Serializable::GameMode::GetDrones::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}

void Serialization::SerializeRequest(Serializable::GameMode::SpawnDrone::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}

void Serialization::SerializeRequest(Serializable::GameMode::SpawnDroneAtLocation::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type, Request.x, Request.y, Request.z, Request.idMesh);
}

void Serialization::SerializeRequest(Serializable::GameMode::RemoveDrone::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type, Request.port);
}

void Serialization::SerializeRequest(Serializable::GameMode::SetForestDensity::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type, Request.Density_Level);
}

void Serialization::SerializeRequest(Serializable::GameMode::SetForestHillyLevel::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type, Request.Hilly_Level);
}

void Serialization::SerializeRequest(Serializable::GameMode::GetWorldOrigin::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}

void Serialization::SerializeRequest(Serializable::GameMode::GetCameraCaptureMode::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}

void Serialization::SerializeRequest(Serializable::GameMode::SetCameraCaptureMode::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type, Request.cameraCaptureMode);
}

void Serialization::SerializeRequest(Serializable::GameMode::GetFps::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}

void Serialization::SerializeRequest(Serializable::GameMode::GetApiVersion::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}

void Serialization::SerializeRequest(Serializable::GameMode::GetTime::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type);
}

void Serialization::SerializeRequest(Serializable::GameMode::SetGraphicsSettings::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type, Request.graphicsSettings);
}

void Serialization::SerializeRequest(Serializable::GameMode::SwitchWorldLevel::Request& Request, std::stringstream& InputStream) {
  InputStream.seekg(0);
  cereal::BinaryInputArchive InputArchive(InputStream);
  InputArchive(Request.type, Request.WorldLevelEnum);
}

void Serialization::DeserializeResponse(Serializable::Common::NetworkResponse& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status);
}

void Serialization::DeserializeResponse(Serializable::Common::Ping::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status);
}

void Serialization::DeserializeResponse(Serializable::Drone::GetLocation::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status, Response.x, Response.y, Response.z);
}

void Serialization::DeserializeResponse(Serializable::Drone::GetCrashState::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status, Response.crashed);
}

void Serialization::DeserializeResponse(Serializable::Drone::GetRotation::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status, Response.pitch, Response.yaw, Response.roll);
}

void Serialization::DeserializeResponse(Serializable::Drone::SetLocation::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status, Response.teleportedToX, Response.teleportedToY, Response.teleportedToZ, Response.isHit, Response.impactPointX,
                Response.impactPointY, Response.impactPointZ);
}

void Serialization::DeserializeResponse(Serializable::Drone::SetRotation::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status, Response.rotatedToPitch, Response.rotatedToYaw, Response.rotatedToRoll, Response.isHit, Response.impactPointX,
                Response.impactPointY, Response.impactPointZ);
}

void Serialization::DeserializeResponse(Serializable::Drone::GetRgbCameraData::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status, Response.image_data_, Response.stamp_);
}

void Serialization::DeserializeResponse(Serializable::Drone::GetStereoCameraData::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status, Response.image_left_, Response.image_right_, Response.stamp_);
}

void Serialization::DeserializeResponse(Serializable::Drone::GetRgbSegCameraData::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status, Response.imageData, Response.stamp_);
}

void Serialization::DeserializeResponse(Serializable::Drone::SetLocationAndRotation::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status, Response.teleportedToX, Response.teleportedToY, Response.teleportedToZ, Response.rotatedToPitch,
                Response.rotatedToYaw, Response.rotatedToRoll, Response.isHit, Response.impactPointX, Response.impactPointY, Response.impactPointZ);
}

void Serialization::DeserializeResponse(Serializable::Drone::SetLocationAndRotationAsync::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status);
}

struct LidarDataSerializable : public Serializable::Drone::GetLidarData::LidarData
{
  LidarDataSerializable() = default;

  void setData(Serializable::Drone::GetLidarData::LidarData& _LidarData) {
    distance   = _LidarData.distance;
    directionX = _LidarData.directionX;
    directionY = _LidarData.directionY;
    directionZ = _LidarData.directionZ;
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(distance, directionX, directionY, directionZ);
  }
};

struct LidarSegDataSerializable : public Serializable::Drone::GetLidarSegData::LidarSegData
{
  LidarSegDataSerializable() = default;

  void setData(Serializable::Drone::GetLidarSegData::LidarSegData& _LidarSegData) {
    distance     = _LidarSegData.distance;
    directionX   = _LidarSegData.directionX;
    directionY   = _LidarSegData.directionY;
    directionZ   = _LidarSegData.directionZ;
    segmentation = _LidarSegData.segmentation;
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(distance, directionX, directionY, directionZ, segmentation);
  }
};

struct LidarIntDataSerializable : public Serializable::Drone::GetLidarIntData::LidarIntData
{
  LidarIntDataSerializable() = default;

  void setData(Serializable::Drone::GetLidarIntData::LidarIntData& _LidarIntData) {
    distance     = _LidarIntData.distance;
    directionX   = _LidarIntData.directionX;
    directionY   = _LidarIntData.directionY;
    directionZ   = _LidarIntData.directionZ;
    intensity = _LidarIntData.intensity;
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(distance, directionX, directionY, directionZ, intensity);
  }
};
void Serialization::DeserializeResponse(Serializable::Drone::GetRangefinderData::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status, Response.range);
}

void Serialization::DeserializeResponse(Serializable::Drone::GetLidarData::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);

  const auto                         LidarDataSize = Response.lidarData.size();
  std::vector<LidarDataSerializable> Data(LidarDataSize, LidarDataSerializable{});
  for (int i = 0; i < LidarDataSize; i++) {
    Data[i].setData(Response.lidarData[i]);
  }

  OutputArchive(Response.type, Response.status, Response.startX, Response.startY, Response.startZ, Data);
}

void Serialization::DeserializeResponse(Serializable::Drone::GetLidarSegData::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);

  const auto                            LidarSegDataSize = Response.lidarSegData.size();
  std::vector<LidarSegDataSerializable> Data(LidarSegDataSize, LidarSegDataSerializable{});
  for (int i = 0; i < LidarSegDataSize; i++) {
    Data[i].setData(Response.lidarSegData[i]);
  }

  OutputArchive(Response.type, Response.status, Response.startX, Response.startY, Response.startZ, Data);
}

void Serialization::DeserializeResponse(Serializable::Drone::GetLidarIntData::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);

  const auto                            LidarIntDataSize = Response.lidarIntData.size();
  std::vector<LidarIntDataSerializable> Data(LidarIntDataSize, LidarIntDataSerializable{});
  for (int i = 0; i < LidarIntDataSize; i++) {
    Data[i].setData(Response.lidarIntData[i]);
  }

  OutputArchive(Response.type, Response.status, Response.startX, Response.startY, Response.startZ, Data);
}
void Serialization::DeserializeResponse(Serializable::Drone::GetLidarConfig::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  LidarConfigSerializable     ConfigSerializable{};
  ConfigSerializable.setData(Response.config);
  OutputArchive(Response.type, Response.status, ConfigSerializable);
}

void Serialization::DeserializeResponse(Serializable::Drone::SetLidarConfig::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status);
}

void Serialization::DeserializeResponse(Serializable::Drone::GetRgbCameraConfig::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  RgbCameraConfigSerializable    ConfigSerializable{};
  ConfigSerializable.setData(Response.config);
  OutputArchive(Response.type, Response.status, ConfigSerializable);
}

void Serialization::DeserializeResponse(Serializable::Drone::SetRgbCameraConfig::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status);
}

void Serialization::DeserializeResponse(Serializable::Drone::GetStereoCameraConfig::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  StereoCameraConfigSerializable    ConfigSerializable{};
  ConfigSerializable.setData(Response.config);
  OutputArchive(Response.type, Response.status, ConfigSerializable);
}

void Serialization::DeserializeResponse(Serializable::Drone::SetStereoCameraConfig::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status);
}

void Serialization::DeserializeResponse(Serializable::Drone::GetMoveLineVisible::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status, Response.visible);
}

void Serialization::DeserializeResponse(Serializable::Drone::SetMoveLineVisible::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status);
}

void Serialization::DeserializeResponse(Serializable::GameMode::GetDrones::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status, Response.ports);
}

void Serialization::DeserializeResponse(Serializable::GameMode::SpawnDrone::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status, Response.port);
}

void Serialization::DeserializeResponse(Serializable::GameMode::SpawnDroneAtLocation::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status, Response.port);
}

void Serialization::DeserializeResponse(Serializable::GameMode::RemoveDrone::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status);
}

void Serialization::DeserializeResponse(Serializable::GameMode::SetForestDensity::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status);
}

void Serialization::DeserializeResponse(Serializable::GameMode::SetForestHillyLevel::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status);
}

void Serialization::DeserializeResponse(Serializable::GameMode::GetCameraCaptureMode::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status, Response.cameraCaptureMode);
}

void Serialization::DeserializeResponse(Serializable::GameMode::SetCameraCaptureMode::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status);
}

void Serialization::DeserializeResponse(Serializable::GameMode::GetFps::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status, Response.fps);
}

void Serialization::DeserializeResponse(Serializable::GameMode::GetApiVersion::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status, Response.api_version);
}

void Serialization::DeserializeResponse(Serializable::GameMode::GetTime::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status, Response.time);
}

void Serialization::DeserializeResponse(Serializable::GameMode::SetGraphicsSettings::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status);
}

void Serialization::DeserializeResponse(Serializable::GameMode::SwitchWorldLevel::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status);
}

void Serialization::DeserializeResponse(Serializable::GameMode::GetWorldOrigin::Response& Response, std::stringstream& OutputStream) {
  cereal::BinaryOutputArchive OutputArchive(OutputStream);
  OutputArchive(Response.type, Response.status, Response.x, Response.y, Response.z);
}

IMPLEMENT_MODULE(Serialization, MessageSerialization)
