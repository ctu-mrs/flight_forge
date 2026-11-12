#include "SimServer.h"

#include <array>
#include <exception>

#include "Common/TcpSocketBuilder.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "SocketSubsystem.h"
#include "Sockets.h"

#include "SimServerInternal.h"

/* construction //{ */

FSimServer::FSimServer(const int32 InPort, const FString& InBindAddress) : Port(InPort), BindAddress(InBindAddress)
{
}

FSimServer::~FSimServer()
{
	Stop();
}

int32 FSimServer::AllocateRobotId()
{
	static std::atomic<int32> NextRobotId{0};
	return NextRobotId++;
}

/* //} */

/* Start() / Stop() //{ */

bool FSimServer::Start()
{
	if (bRunning)
	{
		return true;
	}

	FIPv4Address Address;
	if (!FIPv4Address::Parse(BindAddress, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("FlightForge: invalid bind address %s, falling back to 127.0.0.1"), *BindAddress);
		BindAddress = TEXT("127.0.0.1");
		FIPv4Address::Parse(BindAddress, Address);
	}

	ListenSocket = FTcpSocketBuilder(TEXT("FlightForgeServer")).AsReusable().BoundToEndpoint(FIPv4Endpoint(Address, Port)).Listening(64).Build();
	if (!ListenSocket)
	{
		UE_LOG(LogTemp, Error, TEXT("FlightForge: could not listen on %s:%d"), *BindAddress, Port);
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("FlightForge: wire 1.0 server listening on %s:%d"), *BindAddress, Port);

	bRunning     = true;
	AcceptThread = std::thread([this]() { AcceptLoop(); });
	return true;
}

void FSimServer::Stop(TFunction<void()> Pump)
{
	if (!bRunning.exchange(false))
	{
		return;
	}

	if (AcceptThread.joinable())
	{
		AcceptThread.join();
	}

	{
		std::lock_guard<std::mutex> Lock(ConnectionsMutex);
		for (const std::unique_ptr<FConnection>& Connection : Connections)
		{
			std::lock_guard<std::mutex> SocketLock(Connection->SocketMutex);
			if (Connection->Socket)
			{
				Connection->Socket->Shutdown(ESocketShutdownMode::ReadWrite);
			}
		}
	}

	// a connection may be mid-request on an instruction only the caller's thread runs
	for (;;)
	{
		bool bAllDone = true;
		{
			std::lock_guard<std::mutex> Lock(ConnectionsMutex);
			for (const std::unique_ptr<FConnection>& Connection : Connections)
			{
				bAllDone = bAllDone && Connection->bDone;
			}
		}
		if (bAllDone)
		{
			break;
		}
		if (Pump)
		{
			Pump();
		}
		FPlatformProcess::Sleep(0.001f);
	}

	ReapConnections(true);

	if (ListenSocket)
	{
		ListenSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenSocket);
		ListenSocket = nullptr;
	}
}

/* //} */

/* AcceptLoop() //{ */

void FSimServer::AcceptLoop()
{
	while (bRunning)
	{
		bool bPending = false;
		if (!ListenSocket->WaitForPendingConnection(bPending, FTimespan::FromMilliseconds(100)) || !bPending)
		{
			ReapConnections(false);
			continue;
		}

		const TSharedRef<FInternetAddr> Address = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		FSocket* Socket = ListenSocket->Accept(*Address, TEXT("FlightForgeConnection"));
		if (!Socket)
		{
			continue;
		}

		// request/response is what Nagle punishes: the reply's last segment would wait on a delayed ACK
		Socket->SetNoDelay(true);
		Socket->SetNonBlocking(false);
		int32 Actual = 0;
		Socket->SetSendBufferSize(4 << 20, Actual);
		Socket->SetReceiveBufferSize(4 << 20, Actual);

		auto Connection    = std::make_unique<FConnection>();
		Connection->Socket = Socket;
		Connection->Peer   = Address->ToString(true);

		FConnection* Raw = Connection.get();
		{
			std::lock_guard<std::mutex> Lock(ConnectionsMutex);
			Connections.push_back(std::move(Connection));
		}
		Raw->Thread = std::thread([this, Raw]() { ServeConnection(Raw); });
	}
}

void FSimServer::ReapConnections(const bool bAll)
{
	std::vector<std::unique_ptr<FConnection>> Finished;
	{
		std::lock_guard<std::mutex> Lock(ConnectionsMutex);
		for (auto It = Connections.begin(); It != Connections.end();)
		{
			if (bAll || (*It)->bDone)
			{
				Finished.push_back(std::move(*It));
				It = Connections.erase(It);
			}
			else
			{
				++It;
			}
		}
	}

	for (const std::unique_ptr<FConnection>& Connection : Finished)
	{
		if (Connection->Thread.joinable())
		{
			Connection->Thread.join();
		}
	}
}

/* //} */

/* ServeConnection() //{ */

void FSimServer::ServeConnection(FConnection* Connection)
{
	FSocket*      Socket = Connection->Socket;
	TArray<uint8> Body;

	while (bRunning)
	{
		uint8 Header[FrameHeaderBytes];
		if (!RecvAll(Socket, Header, FrameHeaderBytes))
		{
			break;
		}

		const uint32 Length   = Header[0] | (Header[1] << 8) | (Header[2] << 16) | (static_cast<uint32>(Header[3]) << 24);
		const bool   bPlainly = Header[4] == 0 && Header[5] == 0 && Header[6] == 0 && Header[7] == 0;

		// a length this far out means the stream is not ours, and skipping it could block for gigabytes
		if (Length > MaxRequestBytes)
		{
			UE_LOG(LogTemp, Warning, TEXT("FlightForge: %s sent a %u byte frame, closing the connection"), *Connection->Peer, Length);
			break;
		}

		Body.SetNumUninitialized(Length, EAllowShrinking::No);
		if (Length > 0 && !RecvAll(Socket, Body.GetData(), Length))
		{
			break;
		}

		FWireReply Reply;
		if (!bPlainly)
		{
			Reply.Fail(TEXT("frame flags and reserved bytes must be zero"));
			FinishReply(Reply, 0);
		}
		else
		{
			Handle(Body.GetData(), Length, Reply);
		}

		if (!SendFrame(Socket, Reply))
		{
			break;
		}
	}

	{
		std::lock_guard<std::mutex> Lock(Connection->SocketMutex);
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
		Connection->Socket = nullptr;
	}
	Connection->bDone = true;
}

/* //} */

/* Handle() //{ */

void FSimServer::Handle(const uint8* Data, const size_t Size, FWireReply& Reply)
{
	flatbuffers::Verifier::Options Options;
	Options.max_depth  = 64;
	Options.max_tables = 10000000;

	flatbuffers::Verifier Verifier(Data, Size, Options);
	if (!Data || Size == 0 || !ffw::VerifyRequestBuffer(Verifier))
	{
		Reply.Fail(TEXT("the frame does not hold a valid flightforge.wire.Request"));
		FinishReply(Reply, 0);
		return;
	}

	const ffw::Request* Request = ffw::GetRequest(Data);
	const FHandler      Handler = HasHandler(Request->body_type()) ? Handlers()[static_cast<size_t>(Request->body_type())] : nullptr;

	if (!Handler)
	{
		Reply.Fail(FString::Printf(TEXT("no handler for request body %d"), static_cast<int32>(Request->body_type())));
		FinishReply(Reply, Request->seq());
		return;
	}

	GInstructionTimedOut = false;
	try
	{
		(this->*Handler)(*Request, Reply);
	}
	catch (const std::exception& Exception)
	{
		Reply.Builder.Clear();
		Reply.Fail(FString::Printf(TEXT("%s failed: %hs"), *WireName(Request->body_type()), Exception.what()));
	}
	catch (...)
	{
		Reply.Builder.Clear();
		Reply.Fail(FString::Printf(TEXT("%s failed"), *WireName(Request->body_type())));
	}

	if (GInstructionTimedOut)
	{
		Reply.Builder.Clear();
		Reply.Fail(FString::Printf(TEXT("%s: the game thread did not take it within %.0f s"), *WireName(Request->body_type()), FInstruction<int>::TimeoutSeconds));
	}

	FinishReply(Reply, Request->seq());
}

void FSimServer::FinishReply(FWireReply& Reply, const uint32 Seq)
{
	const auto Error = Reply.Error.empty() ? flatbuffers::Offset<flatbuffers::String>() : Reply.Builder.CreateString(Reply.Error);
	Reply.Builder.Finish(ffw::CreateResponse(Reply.Builder, Seq, Reply.bOk, Error, Reply.Type, Reply.Body));
}

/* //} */

/* socket I/O //{ */

bool FSimServer::RecvAll(FSocket* Socket, uint8* Data, const int32 Count)
{
	int32 Received = 0;
	while (Received < Count)
	{
		int32 Read = 0;
		if (!Socket->Recv(Data + Received, Count - Received, Read))
		{
			return false;
		}
		if (Read == 0)
		{
			// only a socket with a timeout gets here; wait instead of spinning
			if (!Socket->Wait(ESocketWaitConditions::WaitForRead, FTimespan::FromSeconds(1.0)) &&
			    Socket->GetConnectionState() != ESocketConnectionState::SCS_Connected)
			{
				return false;
			}
			continue;
		}
		Received += Read;
	}
	return true;
}

bool FSimServer::SendAll(FSocket* Socket, const uint8* Data, const int32 Count)
{
	int32 Sent = 0;
	while (Sent < Count)
	{
		int32 Written = 0;
		if (!Socket->Send(Data + Sent, Count - Sent, Written))
		{
			return false;
		}
		if (Written <= 0)
		{
			if (!Socket->Wait(ESocketWaitConditions::WaitForWrite, FTimespan::FromSeconds(1.0)) &&
			    Socket->GetConnectionState() != ESocketConnectionState::SCS_Connected)
			{
				return false;
			}
			continue;
		}
		Sent += Written;
	}
	return true;
}

bool FSimServer::SendFrame(FSocket* Socket, const FWireReply& Reply)
{
	const uint32 Size = Reply.Builder.GetSize();
	const uint8  Header[FrameHeaderBytes] = {static_cast<uint8>(Size), static_cast<uint8>(Size >> 8), static_cast<uint8>(Size >> 16), static_cast<uint8>(Size >> 24), 0, 0, 0, 0};

	// small replies go out as one segment
	constexpr uint32 Coalesce = 64 * 1024;
	if (Size <= Coalesce)
	{
		uint8 Frame[FrameHeaderBytes + Coalesce];
		FMemory::Memcpy(Frame, Header, FrameHeaderBytes);
		FMemory::Memcpy(Frame + FrameHeaderBytes, Reply.Builder.GetBufferPointer(), Size);
		return SendAll(Socket, Frame, FrameHeaderBytes + Size);
	}

	return SendAll(Socket, Header, FrameHeaderBytes) && SendAll(Socket, Reply.Builder.GetBufferPointer(), static_cast<int32>(Size));
}

/* //} */

/* game mode //{ */

void FSimServer::AttachGameMode(AFlightForgeGameModeBase* InGameMode)
{
	std::lock_guard<std::mutex> Lock(GameModeMutex);
	AttachedGameMode = InGameMode;
}

void FSimServer::DetachGameMode(AFlightForgeGameModeBase* InGameMode, TFunctionRef<void()> Pump)
{
	{
		std::lock_guard<std::mutex> Lock(GameModeMutex);
		if (AttachedGameMode != InGameMode)
		{
			return;
		}
		AttachedGameMode = nullptr;
	}

	while (GameModeInFlight > 0)
	{
		Pump();
		FPlatformProcess::Sleep(0.001f);
	}
}

FSimServer::FGameModeLease FSimServer::LeaseGameMode(FWireReply& Reply)
{
	std::lock_guard<std::mutex> Lock(GameModeMutex);
	if (!AttachedGameMode)
	{
		Reply.Fail(TEXT("no world is loaded"));
		return FGameModeLease();
	}

	++GameModeInFlight;
	return FGameModeLease(AttachedGameMode, this);
}

FSimServer::FGameModeLease::~FGameModeLease()
{
	if (Server && GameMode)
	{
		--Server->GameModeInFlight;
	}
}

/* //} */

/* robots //{ */

void FSimServer::RegisterRobot(ARobotPawn* Pawn)
{
	std::lock_guard<std::mutex> Lock(RobotsMutex);
	Robots.Add(Pawn->RobotId, FRobotEntry{Pawn, std::make_shared<FRobotSync>()});
}

void FSimServer::ReleaseRobot(ARobotPawn* Pawn, TFunctionRef<void()> Pump)
{
	std::shared_ptr<FRobotSync> Sync;
	{
		std::lock_guard<std::mutex> Lock(RobotsMutex);
		const FRobotEntry* Entry = Robots.Find(Pawn->RobotId);
		if (Entry && Entry->Pawn == Pawn)
		{
			Sync           = Entry->Sync;
			Sync->bClosing = true;
			Robots.Remove(Pawn->RobotId);
		}
	}

	while (Sync && Sync->InFlight > 0)
	{
		Pump();
		FPlatformProcess::Sleep(0.001f);
	}
}

std::shared_ptr<FRobotSync> FSimServer::CloseRobot(const int32 RobotId)
{
	std::shared_ptr<FRobotSync> Sync;
	{
		std::lock_guard<std::mutex> Lock(RobotsMutex);
		if (const FRobotEntry* Entry = Robots.Find(RobotId))
		{
			Sync           = Entry->Sync;
			Sync->bClosing = true;
			Robots.Remove(RobotId);
		}
	}

	// the requests in flight need the game thread, which this connection thread is not
	while (Sync && Sync->InFlight > 0)
	{
		FPlatformProcess::Sleep(0.001f);
	}
	return Sync;
}

FSimServer::FRobotLease FSimServer::LeaseRobot(const int32 RobotId, FWireReply& Reply)
{
	FRobotLease Lease;

	std::lock_guard<std::mutex> Lock(RobotsMutex);
	const FRobotEntry* Entry = Robots.Find(RobotId);
	if (!Entry || Entry->Sync->bClosing)
	{
		Reply.Fail(FString::Printf(TEXT("no robot %d"), RobotId));
		return Lease;
	}

	++Entry->Sync->InFlight;
	Lease.Pawn = Entry->Pawn;
	Lease.Id   = RobotId;
	Lease.Sync = Entry->Sync;
	return Lease;
}

FSimServer::FRobotLease FSimServer::LeaseRobot(const ffw::Request& Request, FWireReply& Reply)
{
	if (!Request.robot_id().has_value())
	{
		Reply.Fail(FString::Printf(TEXT("%s needs a robot_id"), *WireName(Request.body_type())));
		return FRobotLease();
	}
	return LeaseRobot(*Request.robot_id(), Reply);
}

FSimServer::FRobotLease::~FRobotLease()
{
	if (Sync && Pawn)
	{
		--Sync->InFlight;
	}
}

/* //} */

/* Handlers() //{ */

bool FSimServer::HasHandler(const ffw::RequestBody Body)
{
	const size_t Index = static_cast<size_t>(Body);
	return Index > 0 && Index <= static_cast<size_t>(ffw::RequestBody::MAX) && Handlers()[Index] != nullptr;
}

const FSimServer::FHandler* FSimServer::Handlers()
{
	using B = ffw::RequestBody;

	static const auto Table = [] {
		std::array<FHandler, static_cast<size_t>(B::MAX) + 1> T{};
		const auto Add = [&T](const B Body, const FHandler Handler) { T[static_cast<size_t>(Body)] = Handler; };

		Add(B::PingRequest, &FSimServer::Ping);
		Add(B::GetInfoRequest, &FSimServer::GetInfo);

		Add(B::GetTimeRequest, &FSimServer::GetTime);
		Add(B::GetFpsRequest, &FSimServer::GetFps);
		Add(B::SetTimeDilationRequest, &FSimServer::SetTimeDilation);
		Add(B::SetFixedTimestepRequest, &FSimServer::SetFixedTimestep);
		Add(B::SetLockstepRequest, &FSimServer::SetLockstep);
		Add(B::StepRequest, &FSimServer::Step);

		Add(B::SwitchWorldRequest, &FSimServer::SwitchWorld);
		Add(B::SetWeatherRequest, &FSimServer::SetWeather);
		Add(B::SetDaytimeRequest, &FSimServer::SetDaytime);
		Add(B::SetGraphicsSettingsRequest, &FSimServer::SetGraphicsSettings);
		Add(B::SetForestDensityRequest, &FSimServer::SetForestDensity);
		Add(B::SetForestHillyLevelRequest, &FSimServer::SetForestHillyLevel);
		Add(B::SetMutualVisibilityRequest, &FSimServer::SetMutualVisibility);
		Add(B::GetCaptureModeRequest, &FSimServer::GetCaptureMode);
		Add(B::SetCaptureModeRequest, &FSimServer::SetCaptureMode);
		Add(B::GetWorldOriginRequest, &FSimServer::GetWorldOrigin);
		Add(B::GetClassMapRequest, &FSimServer::GetClassMap);

		Add(B::SpawnObjectsFromYamlRequest, &FSimServer::SpawnObjectsFromYaml);
		Add(B::RemoveSpawnedObjectRequest, &FSimServer::RemoveSpawnedObject);
		Add(B::MoveSpawnedObjectRequest, &FSimServer::MoveSpawnedObject);
		Add(B::ListSpawnedObjectsRequest, &FSimServer::ListSpawnedObjects);
		Add(B::ExportSceneRequest, &FSimServer::ExportScene);
		Add(B::ListAssetsRequest, &FSimServer::ListAssets);
		Add(B::LoadLibraryRequest, &FSimServer::LoadLibrary);
		Add(B::ListLibraryRequest, &FSimServer::ListLibrary);
		Add(B::GetWorldMeshRequest, &FSimServer::GetWorldMesh);
		Add(B::GetDistanceBatchRequest, &FSimServer::GetDistanceBatch);
		Add(B::GetSurfaceHeightRequest, &FSimServer::GetSurfaceHeight);

		Add(B::SpawnRobotRequest, &FSimServer::SpawnRobot);
		Add(B::ListRobotsRequest, &FSimServer::ListRobots);
		Add(B::RemoveRobotRequest, &FSimServer::RemoveRobot);
		Add(B::GetDescriptionRequest, &FSimServer::GetDescription);
		Add(B::GetStateRequest, &FSimServer::GetState);
		Add(B::SetStateRequest, &FSimServer::SetState);
		Add(B::SetStatesRequest, &FSimServer::SetStates);
		Add(B::SetTrajectoryRequest, &FSimServer::SetTrajectory);
		Add(B::GetCollisionRequest, &FSimServer::GetCollision);
		Add(B::GetMoveLineVisibleRequest, &FSimServer::GetMoveLineVisible);
		Add(B::SetMoveLineVisibleRequest, &FSimServer::SetMoveLineVisible);

		Add(B::AddSensorRequest, &FSimServer::AddSensor);
		Add(B::RemoveSensorRequest, &FSimServer::RemoveSensor);
		Add(B::ListSensorsRequest, &FSimServer::ListSensors);
		Add(B::AddDeviceRequest, &FSimServer::AddDevice);
		Add(B::ListDevicesRequest, &FSimServer::ListDevices);
		Add(B::GetCameraInfoRequest, &FSimServer::GetCameraInfo);

		for (const B Body : {B::GetRgbCameraConfigRequest, B::SetRgbCameraConfigRequest, B::GetStereoCameraConfigRequest, B::SetStereoCameraConfigRequest,
		                     B::GetFisheyeCameraConfigRequest, B::SetFisheyeCameraConfigRequest, B::GetEventCameraConfigRequest,
		                     B::SetEventCameraConfigRequest, B::GetLidarConfigRequest, B::SetLidarConfigRequest, B::GetRgbCameraDataRequest,
		                     B::GetStereoCameraDataRequest, B::GetFisheyeCameraDataRequest, B::GetDepthCameraDataRequest, B::GetRgbSegCameraDataRequest,
		                     B::GetInstanceSegDataRequest, B::GetInstanceSegMapRequest, B::GetEventCameraDataRequest, B::GetLidarScanRequest,
		                     B::GetRangefinderDataRequest})
		{
			Add(Body, &FSimServer::SensorOp);
		}

		return T;
	}();

	return Table.data();
}

/* //} */
