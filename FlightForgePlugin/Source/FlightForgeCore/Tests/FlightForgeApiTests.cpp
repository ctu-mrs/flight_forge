// The server's dispatch, without a world.
//   UnrealEditor-Cmd <project> -ExecCmds="Automation RunTests FlightForge.Api" -unattended -nopause

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "FlightForgeCore/Instruction.h"
#include "FlightForgeCore/Server/SimServer.h"
#include "FlightForgeCore/Server/WireReply.h"

namespace ffw = flightforge::wire;

static const ffw::Response* Answer(FSimServer& Server, const uint8* Data, const size_t Size, FWireReply& Reply)
{
	Server.Handle(Data, Size, Reply);
	return flatbuffers::GetRoot<ffw::Response>(Reply.Builder.GetBufferPointer());
}

/* handlers //{ */

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFlightForgeApiHandlersTest, "FlightForge.Api.Handlers",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FFlightForgeApiHandlersTest::RunTest(const FString& Parameters)
{
	for (const ffw::RequestBody Body : ffw::EnumValuesRequestBody())
	{
		if (Body != ffw::RequestBody::NONE)
		{
			TestTrue(*FString::Printf(TEXT("%hs has a handler"), ffw::EnumNameRequestBody(Body)), FSimServer::HasHandler(Body));
		}
	}
	TestFalse(TEXT("NONE has no handler"), FSimServer::HasHandler(ffw::RequestBody::NONE));

	return true;
}

/* //} */

/* verification //{ */

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFlightForgeApiVerifyTest, "FlightForge.Api.Verify",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FFlightForgeApiVerifyTest::RunTest(const FString& Parameters)
{
	FSimServer Server(0, TEXT("127.0.0.1"));

	{
		flatbuffers::FlatBufferBuilder Builder;
		Builder.Finish(ffw::CreateRequest(Builder, 42, flatbuffers::nullopt, flatbuffers::nullopt, ffw::RequestBody::PingRequest,
		                                  ffw::CreatePingRequest(Builder).Union()));

		FWireReply           Reply;
		const ffw::Response* Response = Answer(Server, Builder.GetBufferPointer(), Builder.GetSize(), Reply);
		TestTrue(TEXT("ping answers ok"), Response->ok());
		TestEqual(TEXT("ping echoes seq"), Response->seq(), 42u);
		TestTrue(TEXT("ping answers NONE"), Response->body_type() == ffw::ResponseBody::NONE);
	}

	{
		TArray<uint8> Garbage;
		for (int32 i = 0; i < 64; ++i)
		{
			Garbage.Add(static_cast<uint8>(i * 37 + 11));
		}

		FWireReply           Reply;
		const ffw::Response* Response = Answer(Server, Garbage.GetData(), Garbage.Num(), Reply);
		TestFalse(TEXT("garbage is refused"), Response->ok());
		TestTrue(TEXT("with an error"), Response->error() && Response->error()->size() > 0);
	}

	{
		flatbuffers::FlatBufferBuilder Builder;
		Builder.Finish(ffw::CreateRequest(Builder, 7, flatbuffers::nullopt, flatbuffers::nullopt, ffw::RequestBody::GetTimeRequest,
		                                  ffw::CreateGetTimeRequest(Builder).Union()));

		FWireReply           Reply;
		const ffw::Response* Response = Answer(Server, Builder.GetBufferPointer(), Builder.GetSize() / 2, Reply);
		TestFalse(TEXT("a truncated request is refused"), Response->ok());
	}

	{
		flatbuffers::FlatBufferBuilder Builder;
		Builder.Finish(ffw::CreateRequest(Builder, 9, 3, flatbuffers::nullopt, ffw::RequestBody::GetStateRequest, ffw::CreateGetStateRequest(Builder).Union()));

		FWireReply           Reply;
		const ffw::Response* Response = Answer(Server, Builder.GetBufferPointer(), Builder.GetSize(), Reply);
		TestFalse(TEXT("an unknown robot is refused"), Response->ok());
		TestEqual(TEXT("and the reply still carries the seq"), Response->seq(), 9u);
	}

	return true;
}

/* //} */

/* instructions //{ */

// a wait that gives up must leave nothing behind that later runs against the waiter's dead stack
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFlightForgeApiInstructionTimeoutTest, "FlightForge.Api.InstructionTimeout",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FFlightForgeApiInstructionTimeoutTest::RunTest(const FString& Parameters)
{
	int32 Owner = 0;
	int32 Runs  = 0;

	FInstruction<int32> Abandoned;
	Abandoned.Function   = [&Runs](int32&) { ++Runs; };
	GInstructionTimedOut = false;
	TestFalse(TEXT("an instruction nobody runs times out"), Abandoned.Wait(0.05));
	TestTrue(TEXT("and marks the thread"), GInstructionTimedOut);
	Abandoned.Run(Owner);
	TestEqual(TEXT("and the owner skips it afterwards"), Runs, 0);
	TestTrue(TEXT("while still marking it finished"), Abandoned.Finished.load());

	FInstruction<int32> Taken;
	Taken.Function       = [&Runs](int32&) { ++Runs; };
	GInstructionTimedOut = false;
	Taken.Run(Owner);
	TestTrue(TEXT("an instruction already run is waited out"), Taken.Wait(0.05));
	TestEqual(TEXT("once"), Runs, 1);
	TestFalse(TEXT("without a timeout"), GInstructionTimedOut);

	return true;
}

/* //} */

#endif
