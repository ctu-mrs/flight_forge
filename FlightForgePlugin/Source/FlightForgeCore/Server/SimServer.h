#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "CoreMinimal.h"

class AFlightForgeGameModeBase;
class ARobotPawn;
class FSocket;
struct FWireReply;

namespace flightforge::wire
{
struct Request;
enum class RequestBody : uint8_t;
}

// a robot's share of the server; bClosing is guarded by the server's robot registry
struct FRobotSync
{
	std::mutex         StateMutex;
	std::atomic<int32> InFlight{0};
	bool               bClosing = false;
};

// frames are {u32 length, u8 flags, u8[3] reserved} little-endian, then one FlatBuffer
class FLIGHTFORGECORE_API FSimServer
{
public:
	static constexpr int32  FrameHeaderBytes = 8;
	static constexpr uint32 MaxRequestBytes  = 256u << 20;

	FSimServer(int32 InPort, const FString& InBindAddress);
	~FSimServer();

	bool Start();

	// joins every connection thread, running Pump meanwhile
	void Stop(TFunction<void()> Pump = nullptr);

	int32 GetPort() const { return Port; }

	/* game thread //{ */

	void AttachGameMode(AFlightForgeGameModeBase* InGameMode);
	void DetachGameMode(AFlightForgeGameModeBase* InGameMode, TFunctionRef<void()> Pump);

	void RegisterRobot(ARobotPawn* Pawn);
	void ReleaseRobot(ARobotPawn* Pawn, TFunctionRef<void()> Pump);

	/* //} */

	static int32 AllocateRobotId();

	static bool HasHandler(flightforge::wire::RequestBody Body);

	// verifies and routes one frame body, leaving a finished Response in Reply
	void Handle(const uint8* Data, size_t Size, FWireReply& Reply);

private:
	/* leases //{ */

	struct FGameModeLease
	{
		AFlightForgeGameModeBase* GameMode = nullptr;
		FSimServer*               Server   = nullptr;

		FGameModeLease() = default;
		FGameModeLease(AFlightForgeGameModeBase* InGameMode, FSimServer* InServer) : GameMode(InGameMode), Server(InServer) {}
		FGameModeLease(const FGameModeLease&) = delete;
		FGameModeLease& operator=(const FGameModeLease&) = delete;
		FGameModeLease(FGameModeLease&& Other) noexcept : GameMode(Other.GameMode), Server(Other.Server) { Other.Server = nullptr; }
		~FGameModeLease();

		explicit operator bool() const { return GameMode != nullptr; }
		AFlightForgeGameModeBase* operator->() const { return GameMode; }
	};

	struct FRobotLease
	{
		ARobotPawn*                 Pawn = nullptr;
		int32                       Id   = -1;
		std::shared_ptr<FRobotSync> Sync;

		FRobotLease() = default;
		FRobotLease(const FRobotLease&) = delete;
		FRobotLease& operator=(const FRobotLease&) = delete;
		FRobotLease(FRobotLease&& Other) noexcept : Pawn(Other.Pawn), Id(Other.Id), Sync(MoveTemp(Other.Sync)) {}
		~FRobotLease();

		explicit operator bool() const { return Pawn != nullptr; }
		ARobotPawn* operator->() const { return Pawn; }
	};

	FGameModeLease LeaseGameMode(FWireReply& Reply);
	FRobotLease    LeaseRobot(int32 RobotId, FWireReply& Reply);
	FRobotLease    LeaseRobot(const flightforge::wire::Request& Request, FWireReply& Reply);

	// marks the robot closing, so it takes no new requests, and waits out the ones in flight
	std::shared_ptr<FRobotSync> CloseRobot(int32 RobotId);

	/* //} */

	/* transport //{ */

	struct FConnection
	{
		std::mutex        SocketMutex;
		FSocket*          Socket = nullptr;
		std::thread       Thread;
		std::atomic<bool> bDone{false};
		FString           Peer;
	};

	void AcceptLoop();
	void ServeConnection(FConnection* Connection);
	void ReapConnections(bool bAll);

	static void FinishReply(FWireReply& Reply, uint32 Seq);
	static bool RecvAll(FSocket* Socket, uint8* Data, int32 Count);
	static bool SendAll(FSocket* Socket, const uint8* Data, int32 Count);
	static bool SendFrame(FSocket* Socket, const FWireReply& Reply);

	int32   Port;
	FString BindAddress;

	FSocket*          ListenSocket = nullptr;
	std::thread       AcceptThread;
	std::atomic<bool> bRunning{false};

	std::mutex                                ConnectionsMutex;
	std::vector<std::unique_ptr<FConnection>> Connections;

	/* //} */

	/* shared state //{ */

	std::mutex                GameModeMutex;
	AFlightForgeGameModeBase* AttachedGameMode = nullptr;
	std::atomic<int32>        GameModeInFlight{0};

	struct FRobotEntry
	{
		ARobotPawn*                 Pawn = nullptr;
		std::shared_ptr<FRobotSync> Sync;
	};

	std::mutex              RobotsMutex;
	TMap<int32, FRobotEntry> Robots;

	// spawn, remove, world switch, scene edits and the clock take turns
	std::mutex WorldMutex;

	/* //} */

	/* handlers //{ */

	using FHandler = void (FSimServer::*)(const flightforge::wire::Request&, FWireReply&);

	static const FHandler* Handlers();

	void Ping(const flightforge::wire::Request& Request, FWireReply& Reply);
	void GetInfo(const flightforge::wire::Request& Request, FWireReply& Reply);
	void GetTime(const flightforge::wire::Request& Request, FWireReply& Reply);
	void GetFps(const flightforge::wire::Request& Request, FWireReply& Reply);
	void SetTimeDilation(const flightforge::wire::Request& Request, FWireReply& Reply);
	void SetFixedTimestep(const flightforge::wire::Request& Request, FWireReply& Reply);
	void SetLockstep(const flightforge::wire::Request& Request, FWireReply& Reply);
	void Step(const flightforge::wire::Request& Request, FWireReply& Reply);
	void SwitchWorld(const flightforge::wire::Request& Request, FWireReply& Reply);
	void SetWeather(const flightforge::wire::Request& Request, FWireReply& Reply);
	void SetDaytime(const flightforge::wire::Request& Request, FWireReply& Reply);
	void SetGraphicsSettings(const flightforge::wire::Request& Request, FWireReply& Reply);
	void SetForestDensity(const flightforge::wire::Request& Request, FWireReply& Reply);
	void SetForestHillyLevel(const flightforge::wire::Request& Request, FWireReply& Reply);
	void SetMutualVisibility(const flightforge::wire::Request& Request, FWireReply& Reply);
	void GetCaptureMode(const flightforge::wire::Request& Request, FWireReply& Reply);
	void SetCaptureMode(const flightforge::wire::Request& Request, FWireReply& Reply);
	void GetWorldOrigin(const flightforge::wire::Request& Request, FWireReply& Reply);
	void GetClassMap(const flightforge::wire::Request& Request, FWireReply& Reply);

	void SpawnObjectsFromYaml(const flightforge::wire::Request& Request, FWireReply& Reply);
	void RemoveSpawnedObject(const flightforge::wire::Request& Request, FWireReply& Reply);
	void MoveSpawnedObject(const flightforge::wire::Request& Request, FWireReply& Reply);
	void ListSpawnedObjects(const flightforge::wire::Request& Request, FWireReply& Reply);
	void ExportScene(const flightforge::wire::Request& Request, FWireReply& Reply);
	void ListAssets(const flightforge::wire::Request& Request, FWireReply& Reply);
	void LoadLibrary(const flightforge::wire::Request& Request, FWireReply& Reply);
	void ListLibrary(const flightforge::wire::Request& Request, FWireReply& Reply);
	void GetWorldMesh(const flightforge::wire::Request& Request, FWireReply& Reply);
	void GetDistanceBatch(const flightforge::wire::Request& Request, FWireReply& Reply);

	void SpawnRobot(const flightforge::wire::Request& Request, FWireReply& Reply);
	void ListRobots(const flightforge::wire::Request& Request, FWireReply& Reply);
	void RemoveRobot(const flightforge::wire::Request& Request, FWireReply& Reply);
	void GetDescription(const flightforge::wire::Request& Request, FWireReply& Reply);
	void GetState(const flightforge::wire::Request& Request, FWireReply& Reply);
	void SetState(const flightforge::wire::Request& Request, FWireReply& Reply);
	void SetStates(const flightforge::wire::Request& Request, FWireReply& Reply);
	void SetTrajectory(const flightforge::wire::Request& Request, FWireReply& Reply);
	void GetCollision(const flightforge::wire::Request& Request, FWireReply& Reply);
	void GetMoveLineVisible(const flightforge::wire::Request& Request, FWireReply& Reply);
	void SetMoveLineVisible(const flightforge::wire::Request& Request, FWireReply& Reply);

	void AddSensor(const flightforge::wire::Request& Request, FWireReply& Reply);
	void RemoveSensor(const flightforge::wire::Request& Request, FWireReply& Reply);
	void ListSensors(const flightforge::wire::Request& Request, FWireReply& Reply);
	void AddDevice(const flightforge::wire::Request& Request, FWireReply& Reply);
	void ListDevices(const flightforge::wire::Request& Request, FWireReply& Reply);
	void GetCameraInfo(const flightforge::wire::Request& Request, FWireReply& Reply);
	void SensorOp(const flightforge::wire::Request& Request, FWireReply& Reply);

	/* //} */
};
