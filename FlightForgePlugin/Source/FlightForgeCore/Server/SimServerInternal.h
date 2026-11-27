#pragma once

#include "CoreMinimal.h"
#include "HAL/PlatformProcess.h"

#include "FlightForgeCore/FlightForgeGameModeBase.h"
#include "FlightForgeCore/RobotPawn.h"
#include "FlightForgeCore/Utils/WireFrame.h"
#include "SimServer.h"
#include "WireReply.h"

namespace ffw = flightforge::wire;

// Owner's EndPlay pumps its queue, so this cannot hang on teardown
template <typename TOwner, typename TFunction>
void RunOnGameThread(TOwner& Owner, TFunction&& Function)
{
	auto Instruction      = std::make_shared<FInstruction<TOwner>>();
	Instruction->Function = Forward<TFunction>(Function);
	Owner.InstructionQueue->Enqueue(Instruction);
	Instruction->Wait();
}

template <typename TOwner, typename TFunction>
void PostToGameThread(TOwner& Owner, TFunction&& Function)
{
	auto Instruction      = std::make_shared<FInstruction<TOwner>>();
	Instruction->Function = Forward<TFunction>(Function);
	Owner.InstructionQueue->Enqueue(Instruction);
}

inline FString WireName(const ffw::RequestBody Body)
{
	return UTF8_TO_TCHAR(ffw::EnumNameRequestBody(Body));
}
