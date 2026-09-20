#pragma once

#include "CoreMinimal.h"
#include "Async/Future.h"

#include <memory>
#include <sstream>

class FSocket;

#define END_OF_MESSAGE '$'

struct FTCPClient
{
	std::unique_ptr<FSocket> Socket;
	FString StrAddr;
	uint32_t ClientId;
	std::unique_ptr<TArray<uint8>> InBuffer;
	int32 SendBufferSize;
};

class TcpServer
{
public:
	TcpServer(int Port);
	virtual ~TcpServer();

	static int32 StB(const FString& String, uint8* OutBytes, int32 MaxBufferSize);
	static std::shared_ptr<std::stringstream> BytesToStream(const uint8* In, int32 Count);

	bool SetPort(int _Port);
	int GetPort();

	void Run();
	void Stop();
protected:
	int32 MaxBufferSize;
	int32 ReceiveBufferSize;
	int32 SendBufferSize;
	int32 MaxBacklog;
	
	virtual bool Route(const FTCPClient &Client, std::shared_ptr<std::stringstream> InputStream);

	// Reads one whole request. Requests carry no length prefix or terminator, so a
	// message is every byte arriving before the client goes idle; this is unambiguous
	// because a client sends one request and then blocks on the response.
	bool ReceiveMessage(const FTCPClient& Client, TArray<uint8>& OutMessage) const;

	static TFuture<void> RunLambda(TFunction<void()> Function, bool UseGameThread);

	bool Respond(const FTCPClient& Client, const FString& Message) const;
	bool Respond(const FTCPClient& Client, const std::string& Message) const;
	bool Respond(const FTCPClient& Client, const std::stringstream& Message) const;

	// the seq of the request being routed; Respond echoes it into the response header
	uint32_t RequestSeq = 0;
private:
	int32 Port;
	FSocket* ListenSocket;
	TFuture<void> ServerFuture;
	bool ServerIsRunning;
};
