#pragma once

#include "MessageSerialization/Public/SerializableExtended.h"
#include "TcpServer.h"

class ADronePawn;

class DroneServer : public TcpServer {
public:
  DroneServer(ADronePawn& _DronePawn) : TcpServer(1235), DronePawn(&_DronePawn){};
  DroneServer(ADronePawn& _DronePawn, const int Port) : TcpServer(Port), DronePawn(&_DronePawn){};
  ~DroneServer() = default;

protected:
  virtual bool Route(const FTCPClient& Client, std::shared_ptr<std::stringstream> InputStream) override;

private:
  ADronePawn* DronePawn;

  bool uav_crashed_ = false;

  bool GetLocation(const FTCPClient& Client, Serializable::Drone::GetLocation::Request& Request);

  bool GetCrashState(const FTCPClient& Client, Serializable::Drone::GetCrashState::Request& Request);

  bool SetLocation(const FTCPClient& Client, Serializable::Drone::SetLocation::Request& Request);

  bool GetRgbCameraData(const FTCPClient& Client, Serializable::Drone::GetRgbCameraData::Request& Request);

  bool GetStereoCameraData(const FTCPClient& Client, Serializable::Drone::GetStereoCameraData::Request& Request);
  
  bool GetRgbSegCameraData(const FTCPClient& Client, Serializable::Drone::GetRgbSegCameraData::Request& Request);

  bool GetDepthCameraData(const FTCPClient& Client, Serializable::Drone::GetDepthCameraData::Request& Request);
  
  bool GetRotation(const FTCPClient& Client, Serializable::Drone::GetRotation::Request& Request);

  bool SetRotation(const FTCPClient& Client, Serializable::Drone::SetRotation::Request& Request);

  bool SetLocationAndRotation(const FTCPClient& Client, Serializable::Drone::SetLocationAndRotation::Request& Request);

  bool SetLocationAndRotationAsync(const FTCPClient& Client, Serializable::Drone::SetLocationAndRotationAsync::Request& Request);

  bool GetRangeFinderData(const FTCPClient& Client, Serializable::Drone::GetRangefinderData::Request& Request);

  bool GetLidarData(const FTCPClient& Client, Serializable::Drone::GetLidarData::Request& Request);
  
  bool GetLidarSegData(const FTCPClient& Client, Serializable::Drone::GetLidarSegData::Request& Request);
  
  bool GetLidarIntData(const FTCPClient& Client, Serializable::Drone::GetLidarIntData::Request& Request);

  bool GetLidarConfig(const FTCPClient& Client, Serializable::Drone::GetLidarConfig::Request& Request);

  bool SetLidarConfig(const FTCPClient& Client, Serializable::Drone::SetLidarConfig::Request& Request);

  bool GetRgbCameraConfig(const FTCPClient& Client, Serializable::Drone::GetRgbCameraConfig::Request& Request);

  bool SetRgbCameraConfig(const FTCPClient& Client, Serializable::Drone::SetRgbCameraConfig::Request& Request);

  bool GetStereoCameraConfig(const FTCPClient& Client, Serializable::Drone::GetStereoCameraConfig::Request& Request);

  bool SetStereoCameraConfig(const FTCPClient& Client, Serializable::Drone::SetStereoCameraConfig::Request& Request);

  bool GetMoveLineVisible(const FTCPClient& Client, Serializable::Drone::GetMoveLineVisible::Request& Request);

  bool SetMoveLineVisible(const FTCPClient& Client, Serializable::Drone::SetMoveLineVisible::Request& Request);
};
