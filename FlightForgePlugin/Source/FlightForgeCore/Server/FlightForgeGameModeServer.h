#pragma once

#include <mutex>

#include "MessageSerialization/Public/SerializableExtended.h"
#include "TcpServer.h"

#define API_VERSION_MAJOR Serializable::api_version_major
#define API_VERSION_MINOR Serializable::api_version_minor

class AFlightForgeGameModeBase;

class FlightForgeGameModeServer : public TcpServer
{
public:
	FlightForgeGameModeServer(const int Port, AFlightForgeGameModeBase* InitialGameMode = nullptr): TcpServer(Port), GameMode(InitialGameMode) {};

	void SetCurrentGameMode(AFlightForgeGameModeBase* NewGameMode);
protected:
	virtual bool Route(const FTCPClient& Client, std::shared_ptr<std::stringstream> InputStream) override;

private:
	AFlightForgeGameModeBase* GameMode;
	std::mutex GameModeMutex;

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

	bool SetMutualDroneVisibility(const FTCPClient& Client, Serializable::GameMode::SetMutualVisibility::Request& Request);

	bool SpawnObjectsFromYaml(const FTCPClient& Client, Serializable::GameMode::SpawnObjectsFromYaml::Request& Request);

	bool RemoveSpawnedObject(const FTCPClient& Client, Serializable::GameMode::RemoveSpawnedObject::Request& Request);

	bool MoveSpawnedObject(const FTCPClient& Client, Serializable::GameMode::MoveSpawnedObject::Request& Request);

	bool ListSpawnedObjects(const FTCPClient& Client, Serializable::GameMode::ListSpawnedObjects::Request& Request);

	bool ExportScene(const FTCPClient& Client, Serializable::GameMode::ExportScene::Request& Request);

	bool ListAssets(const FTCPClient& Client, Serializable::GameMode::ListAssets::Request& Request);
	bool GetWorldMesh(const FTCPClient& Client, Serializable::GameMode::GetWorldMesh::Request& Request);
	bool GetDistanceBatch(const FTCPClient& Client, Serializable::GameMode::GetDistanceBatch::Request& Request);
	bool SetTimeDilation(const FTCPClient& Client, Serializable::GameMode::SetTimeDilation::Request& Request);
	bool SetFixedTimestep(const FTCPClient& Client, Serializable::GameMode::SetFixedTimestep::Request& Request);
	bool SetLockstep(const FTCPClient& Client, Serializable::GameMode::SetLockstep::Request& Request);
	bool Step(const FTCPClient& Client, Serializable::GameMode::Step::Request& Request);
};
