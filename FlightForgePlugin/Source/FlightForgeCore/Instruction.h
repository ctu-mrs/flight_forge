#pragma once

#include <atomic>
#include <functional>

#include "Containers/Queue.h"
#include "HAL/PlatformProcess.h"
#include "HAL/PlatformTime.h"

// set on a thread whose wait gave up; the server turns it into an error reply
inline thread_local bool GInstructionTimedOut = false;

template <typename T>
struct FInstruction
{
	static constexpr double TimeoutSeconds = 30.0;

	std::atomic<bool> Finished{false};

	std::function<void(T& Obj)> Function;

	// owner's thread
	void Run(T& Obj)
	{
		if (!bClaimed.exchange(true))
		{
			Function(Obj);
		}
		Finished = true;
	}

	// false when the owner never started it in time; it then never runs, so it may capture the waiter's stack
	bool Wait(const double Timeout = TimeoutSeconds)
	{
		const double Deadline = FPlatformTime::Seconds() + Timeout;
		while (!Finished.load())
		{
			if (FPlatformTime::Seconds() > Deadline && !bClaimed.exchange(true))
			{
				GInstructionTimedOut = true;
				return false;
			}
			FPlatformProcess::SleepNoStats(0.0001f);
		}
		return true;
	}

private:
	std::atomic<bool> bClaimed{false};
};

// every server connection's thread enqueues, the owner's game thread drains
template <typename T>
using TInstructionQueue = TQueue<std::shared_ptr<FInstruction<T>>, EQueueMode::Mpsc>;
