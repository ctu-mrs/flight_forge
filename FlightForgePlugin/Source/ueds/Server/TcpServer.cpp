#include "TcpServer.h"

#include "Common/TcpSocketBuilder.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Sockets.h"
#include "Networking.h"

#include "MessageSerialization/Public/SerializableExtended.h"
#include "MessageSerialization/Public/Serialize.h"

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

void TcpServer::Stop()
{
	if(ListenSocket != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Stopping server"));
		
		// stop server thread
		ServerIsRunning = false;
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

	if(Request.type == Serializable::Common::MessageType::ping)
	{
		std::stringstream OutputStream;
		Serializable::Common::Ping::Response Response(true);
		Serialization::DeserializeResponse<Serializable::Common::Ping::Response>(Response, OutputStream);
		return Respond(Client, OutputStream);
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
	auto OutBytes = new uint8[Client.SendBufferSize + 3];
	for(int i = 0; i < Message.length(); i++)
		OutBytes[i] = Message[i];

	OutBytes[Message.length()] = END_OF_MESSAGE; // mark end of message
	OutBytes[Message.length()+1] = END_OF_MESSAGE; // mark end of message
	OutBytes[Message.length()+2] = END_OF_MESSAGE; // mark end of message

	/* UE_LOG(LogTemp, Warning, TEXT("Message.length: %llu"), Message.length()+3); */
	
	int32 BytesSent;
	const bool Sent = Client.Socket->Send(OutBytes, Message.length() + 3, BytesSent);
	
	/* UE_LOG(LogTemp, Warning, TEXT("SendResult: %d"), Sent); */
	/* UE_LOG(LogTemp, Warning, TEXT("BytesSent: %d"), BytesSent); */
	delete[] OutBytes;
	return Sent;
}

bool TcpServer::Respond(const FTCPClient& Client, const std::stringstream& Message) const
{
	return Respond(Client, Message.str());
}

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

	FIPv4Address IPAddress;
	FIPv4Address::Parse(FString("127.0.0.1"), IPAddress);
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

				auto Client = std::make_unique<FTCPClient>();
				Client->Socket = std::unique_ptr<FSocket>(ClientSocket);
				Client->StrAddr = StrAddr;
				Client->InBuffer = std::make_unique<TArray<uint8>>();
				Client->InBuffer->SetNumUninitialized(1024);
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
							//UE_LOG(LogTemp, Warning, TEXT("Client %s has pending data of size %d"), *Client->StrAddr, PendingDataSize);

							int32 ReadBytes;
							Client->Socket->Recv(Client->InBuffer->GetData(), Client->InBuffer->Num(), ReadBytes);
							//UE_LOG(LogTemp, Warning, TEXT("Client %s read data of size %d"), *Client->StrAddr, ReadBytes);

							try
							{
								Route(*Client, BytesToStream(Client->InBuffer->GetData(), ReadBytes));
							}
							catch (...)
							{
								// TODO
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