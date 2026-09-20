#include "TcpServer.h"

#include "Common/TcpSocketBuilder.h"
#include "HAL/PlatformProcess.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Sockets.h"
#include "Networking.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"

#include <exception>

#include "MessageSerialization/Public/SerializableExtended.h"
#include "MessageSerialization/Public/Serialize.h"
#include "MessageSerialization/Public/serializable_schema.h"

// https://github.com/getnamo/TCP-Unreal/blob/master/Source/TCPWrapper/Private/TCPServerComponent.cpp

TFuture<void> TcpServer::RunLambda(TFunction<void()> Function, bool UseGameThread)
{
	return UseGameThread ? Async(EAsyncExecution::TaskGraphMainThread, Function) : Async(EAsyncExecution::Thread, Function);
}

TcpServer::TcpServer(const int32 Port)
{
	this->Port = Port;

	MaxBufferSize = 2097152; // 2mb
	ReceiveBufferSize = 0;
	SendBufferSize = 0;
	MaxBacklog = 8;

	ListenSocket = nullptr;
	ServerIsRunning = false;
}

int TcpServer::GetPort()
{
	return this->Port;
}

bool TcpServer::SetPort(int _Port)
{
	if(ServerIsRunning)
	{
		return false;
	}

	this->Port = _Port;
	return true;
}

void TcpServer::Stop(TFunction<void()> PumpWhileStopping)
{
	if(ListenSocket != nullptr)
	{
		ServerIsRunning = false;
		if (PumpWhileStopping)
		{
			while (!ServerFuture.IsReady())
			{
				PumpWhileStopping();
				FPlatformProcess::Sleep(0.001f);
			}
		}
		ServerFuture.Get();
		
		ListenSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenSocket);
		ListenSocket = nullptr;
	}
}

TcpServer::~TcpServer()
{
	this->Stop();
}

int32 TcpServer::StB(const FString& String, uint8* OutBytes, int32 MaxBufferSize)
{
	int32 NumBytes = 0;
	const TCHAR* CharPos = *String;

	while( *CharPos && NumBytes < MaxBufferSize)
	{
		OutBytes[ NumBytes ] = (int8)(*CharPos);
		CharPos++;
		++NumBytes;
	}
	return NumBytes;
}

std::shared_ptr<std::stringstream> TcpServer::BytesToStream(const uint8* In, int32 Count)
{
	auto Stream = std::make_shared<std::stringstream>();

	while (Count)
	{
		*Stream << *In;

		++In;
		Count--;
	}

	return Stream;
}

bool TcpServer::Route(const FTCPClient& Client, std::shared_ptr<std::stringstream> InputStream)
{
	Serializable::Common::NetworkRequest Request;
	Serialization::SerializeRequest<Serializable::Common::NetworkRequest>(Request, *InputStream);
	RequestSeq = Request.seq;

	if(Request.type == Serializable::Common::MessageType::ping)
	{
		std::stringstream OutputStream;
		Serializable::Common::Ping::Response Response(true);
		Serialization::DeserializeResponse<Serializable::Common::Ping::Response>(Response, OutputStream);
		return Respond(Client, OutputStream);
	}

	if(Request.type == Serializable::Common::MessageType::get_meta)
	{
		static const std::string Catalogue = [] {
			std::stringstream Stream;
			auto Response = Serializable::Schema::BuildMetaResponse();
			Serialization::DeserializeResponse(Response, Stream);
			return Stream.str();
		}();
		return Respond(Client, Catalogue);
	}

	return false;
}

bool TcpServer::Respond(const FTCPClient& Client, const FString& Message) const
{
	//UE_LOG(LogTemp, Warning, TEXT("OutString: %s"), *Message);

	auto OutBytes = new uint8[Client.SendBufferSize];
	const auto OutBytesSize = StB(Message, OutBytes, SendBufferSize);
	
	int32 BytesSent;
	const bool Sent = Client.Socket->Send(OutBytes, OutBytesSize, BytesSent);
	delete[] OutBytes;
	//UE_LOG(LogTemp, Warning, TEXT("SendResult: %d"), Sent);
	//UE_LOG(LogTemp, Warning, TEXT("BytesSent: %d"), BytesSent);

	return Sent;
}

bool TcpServer::Respond(const FTCPClient& Client, const std::string& Message) const
{
	TArray<uint8> OutBytes;
	OutBytes.SetNumUninitialized(Message.length() + 3);
	FMemory::Memcpy(OutBytes.GetData(), Message.data(), Message.length());

	// every response starts with the NetworkResponse header {u16 type, bool status, u32 seq}
	constexpr int32 SeqOffset = 3;
	if (Message.length() >= SeqOffset + sizeof(RequestSeq))
	{
		FMemory::Memcpy(OutBytes.GetData() + SeqOffset, &RequestSeq, sizeof(RequestSeq));
	}

	OutBytes[Message.length()]     = END_OF_MESSAGE;
	OutBytes[Message.length() + 1] = END_OF_MESSAGE;
	OutBytes[Message.length() + 2] = END_OF_MESSAGE;

	// Send takes what fits in the socket buffer and reports success for the prefix, so a
	// reply larger than that arrives truncated unless the rest is pushed after it. Mesh
	// and point-cloud replies are far past that threshold.
	int32 TotalSent = 0;
	while (TotalSent < OutBytes.Num())
	{
		int32      BytesSent = 0;
		const bool bSent     = Client.Socket->Send(OutBytes.GetData() + TotalSent, OutBytes.Num() - TotalSent, BytesSent);

		if (!bSent)
		{
			UE_LOG(LogTemp, Error, TEXT("TcpServer: send failed %d bytes into a %d byte reply"), TotalSent, OutBytes.Num());
			return false;
		}

		if (BytesSent <= 0)
		{
			// the buffer is full and the peer has not drained it yet
			FPlatformProcess::Sleep(0.0002f);
			continue;
		}

		TotalSent += BytesSent;
	}

	return true;
}

bool TcpServer::Respond(const FTCPClient& Client, const std::stringstream& Message) const
{
	return Respond(Client, Message.str());
}

/* ReceiveMessage() //{ */

bool TcpServer::ReceiveMessage(const FTCPClient& Client, TArray<uint8>& OutMessage) const
{
	// Anything smaller than this is taken to be complete as soon as the socket goes idle,
	// which keeps the small commands that dominate the drone servers free of added
	// latency. Only a payload large enough to plausibly span several TCP segments is
	// worth waiting on.
	constexpr int32 SegmentedMessageThreshold = 1024;
	constexpr int32 MaxIdlePolls              = 8;
	constexpr float IdlePollSeconds           = 0.0005f;

	OutMessage.Reset();

	int32 IdlePolls = 0;
	while(true)
	{
		uint32 PendingDataSize = 0;
		if(!Client.Socket->HasPendingData(PendingDataSize) || PendingDataSize == 0)
		{
			if(OutMessage.Num() < SegmentedMessageThreshold || IdlePolls >= MaxIdlePolls)
			{
				break;
			}

			++IdlePolls;
			FPlatformProcess::Sleep(IdlePollSeconds);
			continue;
		}

		if(static_cast<int64>(OutMessage.Num()) + PendingDataSize > MaxBufferSize)
		{
			UE_LOG(LogTemp, Error, TEXT("FlightForge: request from %s exceeds the %d byte limit, dropping"),
				*Client.StrAddr, MaxBufferSize);
			return false;
		}

		const int32 Offset = OutMessage.Num();
		OutMessage.SetNumUninitialized(Offset + static_cast<int32>(PendingDataSize));

		int32 ReadBytes = 0;
		if(!Client.Socket->Recv(OutMessage.GetData() + Offset, PendingDataSize, ReadBytes) || ReadBytes <= 0)
		{
			OutMessage.SetNum(Offset);
			break;
		}

		OutMessage.SetNum(Offset + ReadBytes);
		IdlePolls = 0;
	}

	return OutMessage.Num() > 0;
}

/* //} */

void TcpServer::Run()
{
	if(ServerIsRunning)
	{
		return;
	}
	
	// Create socket
	ListenSocket = FTcpSocketBuilder(TEXT("DroneSocketPort")) // TODO unify name
		.AsReusable()
		.Build();													

	ListenSocket->SetReceiveBufferSize(MaxBufferSize, ReceiveBufferSize);
	ListenSocket->SetSendBufferSize(MaxBufferSize, SendBufferSize);

	// Bind address is configurable via the "-FlightForgeBindAddress=<ip>" launch
	// argument (e.g. 0.0.0.0 to listen on all interfaces). Defaults to localhost.
	FString BindAddressStr = TEXT("127.0.0.1");
	FParse::Value(FCommandLine::Get(), TEXT("FlightForgeBindAddress="), BindAddressStr);

	FIPv4Address IPAddress;
	if(!FIPv4Address::Parse(BindAddressStr, IPAddress))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("FlightForge: invalid -FlightForgeBindAddress=%s, falling back to 127.0.0.1"),
			*BindAddressStr);
		BindAddressStr = TEXT("127.0.0.1");
		FIPv4Address::Parse(BindAddressStr, IPAddress);
	}
	UE_LOG(LogTemp, Warning, TEXT("FlightForge: TCP server binding to %s:%d"), *BindAddressStr, Port);

	const auto InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(IPAddress.Value);
	InternetAddr->SetPort(Port);

	if(!ListenSocket->Bind(*InternetAddr))
	{
		//UE_LOG(LogTemp, Error, TEXT("Error at Bind"));
	}
	if(!ListenSocket->Listen(MaxBacklog))
	{
		//UE_LOG(LogTemp, Error, TEXT("Error at Listen"));
	}
	//UE_LOG(LogTemp, Warning, TEXT("Server is listening on port %d"), Port);

	// Run server
	ServerFuture = RunLambda([&]()
	{
		ServerIsRunning = true;
		//UE_LOG(LogTemp, Warning, TEXT("Server is running"
		//						""));
		//UE_LOG(LogTemp, Warning, TEXT("ReceiveBufferSize %d"), ReceiveBufferSize);
		//UE_LOG(LogTemp, Warning, TEXT("SendBufferSize %d"), SendBufferSize);
		
		TMap<uint32_t, std::unique_ptr<FTCPClient>> Clients;
		uint32_t ClientIdCounter = 0;
		TQueue<uint32_t> ClientsToDisconnect;

		while(ServerIsRunning)
		{
			bool HasPendingConnection;
			ListenSocket->HasPendingConnection(HasPendingConnection);
			if(HasPendingConnection)
			{
				auto Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
				auto ClientSocket = ListenSocket->Accept(*Addr, TEXT("tcp-client")); // TODO unique name
				const auto StrAddr = Addr->ToString(true);

				// Request/response with the client blocked is what Nagle punishes: the
				// trailing segment waits on a delayed ACK. Nothing here coalesces usefully.
				if (ClientSocket && !ClientSocket->SetNoDelay(true))
				{
					UE_LOG(LogTemp, Warning, TEXT("TcpServer: could not disable Nagle on the client socket; large responses will stall"));
				}

				auto Client = std::make_unique<FTCPClient>();
				Client->Socket = std::unique_ptr<FSocket>(ClientSocket);
				Client->StrAddr = StrAddr;
				Client->InBuffer = std::make_unique<TArray<uint8>>();
				Client->SendBufferSize = SendBufferSize;
				uint32_t ClientId = ClientIdCounter++;
				Client->ClientId = ClientId;
				
				Clients.Add(ClientId, std::move(Client));
				
				//UE_LOG(LogTemp, Warning, TEXT("Got connection from %s with id: %u"), *StrAddr, ClientId);
			}
			
			// TODO Run foreach in threads
			for(const auto &[ClientId ,Client] : Clients)
			{
				ESocketConnectionState ConnectionState = ESocketConnectionState::SCS_NotConnected;
					if (Client->Socket != nullptr) {
						ConnectionState = Client->Socket->GetConnectionState();
					}
					if (ConnectionState != ESocketConnectionState::SCS_Connected)
					{
						ClientsToDisconnect.Enqueue(ClientId);
					}
					else
					{
						uint32 PendingDataSize;
						if(Client->Socket->HasPendingData(PendingDataSize))
						{
							TArray<uint8>& Message = *Client->InBuffer;
							if(ReceiveMessage(*Client, Message))
							{
								try
								{
									Route(*Client, BytesToStream(Message.GetData(), Message.Num()));
								}
								catch (const std::exception& Exception)
								{
									UE_LOG(LogTemp, Error, TEXT("FlightForge: failed to route a %d byte request from %s: %hs"),
										Message.Num(), *Client->StrAddr, Exception.what());
								}
								catch (...)
								{
									UE_LOG(LogTemp, Error, TEXT("FlightForge: failed to route a %d byte request from %s"),
										Message.Num(), *Client->StrAddr);
								}
							}
						}
					}
			}

			uint32_t ToDisconnect;
			while(ClientsToDisconnect.Dequeue(ToDisconnect))
			{
				//UE_LOG(LogTemp, Warning, TEXT("Client of id %u disconnected, removing..."), ToDisconnect);
				Clients.Remove(ToDisconnect);
			}

			FGenericPlatformProcess::ConditionalSleep([this, &Clients]()
			{
				bool HasPendingConnection = false;
				ListenSocket->HasPendingConnection(HasPendingConnection);

				bool ClientHasPendingData = false;
				for(const auto &[ClientId ,Client] : Clients)
				{
					uint32 DataSize;
					if(Client->Socket->HasPendingData(DataSize))
					{
						ClientHasPendingData = true;
						break;
					}
				}
				
				return !ServerIsRunning || HasPendingConnection || ClientHasPendingData;
			});
		}

		// Cleanup
		for(const auto &[ClientId ,Client] : Clients)
		{
			Client->Socket->Close();
		}
		Clients.Empty();
	}, false);
}