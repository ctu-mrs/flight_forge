#pragma once

#include "MessageSerialization/Public/SerializableExtended.h"
#include "TcpServer.h"

#define API_VERSION 9

class AuedsGameModeBase;

class UedsGameModeServer : public TcpServer
{
public:
	UedsGameModeServer(AuedsGameModeBase& _GameMode, const int Port): TcpServer(Port), GameMode(&_GameMode) {};

protected:
	virtual bool Route(const FTCPClient& Client, std::shared_ptr<std::stringstream> InputStream) override;

private:
	AuedsGameModeBase* GameMode;

	bool GetDrones(const FTCPClient& Client, Serializable::GameMode::GetDrones::Request& Request);

	bool SpawnDrone(const FTCPClient& Client, Serializable::GameMode::SpawnDrone::Request& Request);
	
	bool SpawnDroneAtLocation(const FTCPClient& Client, Serializable::GameMode::SpawnDroneAtLocation::Request& Request);

	bool RemoveDrone(const FTCPClient& Client, Serializable::GameMode::RemoveDrone::Request& Request);

	bool GetCameraCaptureMode(const FTCPClient& Client, Serializable::GameMode::GetCameraCaptureMode::Request& Request);

	bool SetCameraCaptureMode(const FTCPClient& Client, Serializable::GameMode::SetCameraCaptureMode::Request& Request);

	bool GetFPS(const FTCPClient& Client, Serializable::GameMode::GetFps::Request& Request);

	bool GetTime(const FTCPClient& Client, Serializable::GameMode::GetTime::Request& Request);

	bool SetGraphicsSettings(const FTCPClient& Client, Serializable::GameMode::SetGraphicsSettings::Request& Request);

	bool SwitchWorldLevel(const FTCPClient& Client, Serializable::GameMode::SwitchWorldLevel::Request& Request);

	bool GetApiVersion(const FTCPClient& Client, Serializable::GameMode::GetApiVersion::Request& Request);

	bool SetForestDensity(const FTCPClient& Client, Serializable::GameMode::SetForestDensity::Request& Request);

	bool SetForestHillyLevel(const FTCPClient& Client, Serializable::GameMode::SetForestHillyLevel::Request& Request);

	bool GetWorldOrigin(const FTCPClient& Client, Serializable::GameMode::GetWorldOrigin::Request& Request);

	bool SetWeather(const FTCPClient& Client, Serializable::GameMode::SetWeather::Request& Request);

	bool SetDatetime(const FTCPClient& Client, Serializable::GameMode::SetDaytime::Request& Request);
};
