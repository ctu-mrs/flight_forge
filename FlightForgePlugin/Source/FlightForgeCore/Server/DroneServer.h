#pragma once

#include "MessageSerialization/Public/SerializableExtended.h"
#include "TcpServer.h"

class ARobotPawn;

class DroneServer : public TcpServer {
public:
  DroneServer(ARobotPawn& _RobotPawn) : TcpServer(1235), RobotPawn(&_RobotPawn){};
  DroneServer(ARobotPawn& _RobotPawn, const int Port) : TcpServer(Port), RobotPawn(&_RobotPawn){};
  ~DroneServer() = default;

protected:
  virtual bool Route(const FTCPClient& Client, std::shared_ptr<std::stringstream> InputStream) override;

private:
  ARobotPawn* RobotPawn;

  bool uav_crashed_ = false;

  bool GetLocation(const FTCPClient& Client, Serializable::Drone::GetLocation::Request& Request);

  bool GetCrashState(const FTCPClient& Client, Serializable::Drone::GetCrashState::Request& Request);

  bool SetLocation(const FTCPClient& Client, Serializable::Drone::SetLocation::Request& Request);
  
  bool GetRotation(const FTCPClient& Client, Serializable::Drone::GetRotation::Request& Request);

  bool SetRotation(const FTCPClient& Client, Serializable::Drone::SetRotation::Request& Request);

  bool SetLocationAndRotation(const FTCPClient& Client, Serializable::Drone::SetLocationAndRotation::Request& Request);

  bool SetLocationAndRotationAsync(const FTCPClient& Client, Serializable::Drone::SetLocationAndRotationAsync::Request& Request);

  bool SetPoseTrajectory(const FTCPClient& Client, Serializable::Drone::SetPoseTrajectory::Request& Request);
  bool SetPropellerSpeeds(const FTCPClient& Client, Serializable::Drone::SetPropellerSpeeds::Request& Request);
  bool GetCameraInfo(const FTCPClient& Client, Serializable::Drone::GetCameraInfo::Request& Request);

  bool GetMoveLineVisible(const FTCPClient& Client, Serializable::Drone::GetMoveLineVisible::Request& Request);

  bool SetMoveLineVisible(const FTCPClient& Client, Serializable::Drone::SetMoveLineVisible::Request& Request);

  bool GetSensorConfig(const FTCPClient& Client, int SensorID);
  bool SetSensorConfig(const FTCPClient& Client, std::shared_ptr<std::stringstream> InputStream, int SensorID);
  bool GetSensorData(const FTCPClient& Client, int SensorID);

  bool AddSensor(const FTCPClient& Client, Serializable::Drone::AddSensor::Request& Request);
  bool RemoveSensor(const FTCPClient& Client, Serializable::Drone::RemoveSensor::Request& Request);
  bool ListSensors(const FTCPClient& Client);

  bool AddDevice(const FTCPClient& Client, Serializable::Drone::AddDevice::Request& Request);
  bool ListDevices(const FTCPClient& Client);
};
