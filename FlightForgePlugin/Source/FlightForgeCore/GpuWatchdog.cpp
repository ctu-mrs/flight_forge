#include "GpuWatchdog.h"

#include <atomic>
#include <cstdio>

#include "Containers/Ticker.h"
#include "HAL/IConsoleManager.h"
#include "HAL/PlatformMisc.h"
#include "HAL/PlatformProcess.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "Misc/CommandLine.h"
#include "Misc/CoreDelegates.h"
#include "Misc/Parse.h"
#include "RHICommandList.h"
#include "RenderingThread.h"

static TAutoConsoleVariable<float> CVarGpuWatchdogTimeout(
    TEXT("FlightForge.GpuWatchdog.Timeout"), 120.0f,
    TEXT("Seconds the render thread may leave a queued command unrun before the simulator logs an error and exits with code 3; 0 disables. ")
        TEXT("-FlightForgeGpuWatchdog=<s> sets it. Until the first frame has rendered the limit is five times longer."),
    ECVF_Default);

/* state //{ */

namespace
{

constexpr uint8  StallExitCode    = 3;
constexpr double ColdStartFactor  = 5.0;
constexpr double MaxReportSeconds = 30.0;
constexpr float  PollSeconds      = 0.5f;
constexpr double MaxStepSeconds   = 1.0;
constexpr double ResumeLogSeconds = 5.0;

std::atomic<bool>   GProbeQueued{false};
std::atomic<uint64> GProbesRun{0};
std::atomic<double> GStallSeconds{0.0};

double Limit()
{
	const double Timeout = CVarGpuWatchdogTimeout.GetValueOnAnyThread();
	return GProbesRun.load() > 0 ? Timeout : Timeout * ColdStartFactor;
}

double ReportAfter()
{
	return FMath::Min(MaxReportSeconds, Limit() / 2.0);
}

void ExitOnStall(const double Stalled) //{
{
	const FString Message = FString::Printf(TEXT("FlightForge: GPU watchdog: the render thread has left a queued command unrun for %.0f s (limit %.0f s, FlightForge.GpuWatchdog.Timeout); ")
	                                            TEXT("the GPU is lost or hung, exiting with code %d"),
	                                        Stalled, Limit(), StallExitCode);
	UE_LOG(LogTemp, Error, TEXT("%s"), *Message);
	fprintf(stderr, "%s\n", TCHAR_TO_UTF8(*Message));
	fflush(stderr);

	// a stuck game thread never reaches a clean exit
	FPlatformProcess::SleepNoStats(1.0f);
	FPlatformMisc::RequestExitWithStatus(true, StallExitCode, TEXT("FlightForge GPU watchdog"));
} //}

class FWatchdogThread : public FRunnable
{
public:
	std::atomic<bool> bStop{false};

	virtual uint32 Run() override //{
	{
		double Last     = FPlatformTime::Seconds();
		uint64 SeenRuns = 0;
		double Stalled  = 0.0;
		bool   bWarned  = false;

		while (!bStop.load())
		{
			FPlatformProcess::SleepNoStats(PollSeconds);

			// a suspended process or a debugger stop is not a stall
			const double Now  = FPlatformTime::Seconds();
			const double Step = FMath::Min(Now - Last, MaxStepSeconds);
			Last              = Now;

			const uint64 Runs = GProbesRun.load();
			if (!GProbeQueued.load() || Runs != SeenRuns || CVarGpuWatchdogTimeout.GetValueOnAnyThread() <= 0.0f)
			{
				if (Stalled >= ResumeLogSeconds)
				{
					UE_LOG(LogTemp, Log, TEXT("FlightForge: GPU watchdog: the render thread resumed after %.1f s"), Stalled);
				}
				SeenRuns = Runs;
				Stalled  = 0.0;
				bWarned  = false;
				GStallSeconds.store(0.0);
				continue;
			}

			Stalled += Step;
			GStallSeconds.store(Stalled);

			if (!bWarned && Stalled >= ReportAfter())
			{
				UE_LOG(LogTemp, Error, TEXT("FlightForge: GPU watchdog: %s"), *FGpuWatchdog::StallMessage());
				bWarned = true;
			}

			if (Stalled >= Limit())
			{
				ExitOnStall(Stalled);
			}
		}
		return 0;
	} //}
};

FWatchdogThread  GWatchdog;
FRunnableThread* GThread = nullptr;
FDelegateHandle  GEndFrameHandle;

}  // namespace

/* //} */

/* FGpuWatchdog //{ */

void FGpuWatchdog::Start()
{
	if (GEndFrameHandle.IsValid() || GIsEditor || IsRunningCommandlet())
	{
		return;
	}

	float Timeout = CVarGpuWatchdogTimeout.GetValueOnGameThread();
	if (FParse::Value(FCommandLine::Get(), TEXT("FlightForgeGpuWatchdog="), Timeout))
	{
		CVarGpuWatchdogTimeout->Set(Timeout, ECVF_SetByCommandline);
	}

	GEndFrameHandle = FCoreDelegates::OnEndFrame.AddStatic(&FGpuWatchdog::Probe);
	GThread         = FRunnableThread::Create(&GWatchdog, TEXT("FlightForgeGpuWatchdog"), 0, TPri_AboveNormal);
	UE_LOG(LogTemp, Log, TEXT("FlightForge: GPU watchdog exits after %.0f s without render thread progress (FlightForge.GpuWatchdog.Timeout)"), Timeout);
}

void FGpuWatchdog::Stop()
{
	FCoreDelegates::OnEndFrame.Remove(GEndFrameHandle);
	GEndFrameHandle.Reset();
	if (GThread)
	{
		GWatchdog.bStop = true;
		GThread->WaitForCompletion();
		delete GThread;
		GThread = nullptr;
	}
}

void FGpuWatchdog::Probe()
{
	if (GProbeQueued.exchange(true))
	{
		return;
	}
	ENQUEUE_RENDER_COMMAND(FlightForgeWatchdogProbe)([](FRHICommandListImmediate&) {
		GProbesRun++;
		GProbeQueued = false;
	});
}

void FGpuWatchdog::FlushRenderingCommands()
{
	Probe();
	::FlushRenderingCommands();
}

bool FGpuWatchdog::IsStalled()
{
	return CVarGpuWatchdogTimeout.GetValueOnAnyThread() > 0.0f && GStallSeconds.load() >= ReportAfter();
}

FString FGpuWatchdog::StallMessage()
{
	return FString::Printf(TEXT("the render thread has made no progress for %.0f s (GPU lost or hung?); the simulator exits at %.0f s"), GStallSeconds.load(), Limit());
}

/* //} */

/* FlightForge.GpuWatchdog.Stall //{ */

#if !UE_BUILD_SHIPPING

namespace
{

void Stall(const TArray<FString>& Args)
{
	bool   bRhi    = false;
	double Seconds = 1.0e9;
	float  After   = 0.0f;
	for (const FString& Arg : Args)
	{
		if (Arg == TEXT("rhi"))
		{
			bRhi = true;
		}
		else if (!FParse::Value(*Arg, TEXT("after="), After) && Arg.IsNumeric())
		{
			Seconds = FCString::Atod(*Arg);
		}
	}

	const auto Block = [Seconds]() {
		const double End = FPlatformTime::Seconds() + Seconds;
		while (FPlatformTime::Seconds() < End)
		{
			FPlatformProcess::SleepNoStats(0.1f);
		}
	};

	FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([bRhi, Seconds, Block](float) {
		UE_LOG(LogTemp, Warning, TEXT("FlightForge: stalling the %s thread for %.0f s on request"), bRhi ? TEXT("RHI") : TEXT("render"), Seconds);
		ENQUEUE_RENDER_COMMAND(FlightForgeDebugStall)([bRhi, Block](FRHICommandListImmediate& RHICmdList) {
			if (!bRhi)
			{
				Block();
				return;
			}
			RHICmdList.EnqueueLambda([Block](FRHICommandListImmediate&) { Block(); });
			RHICmdList.ImmediateFlush(EImmediateFlushType::DispatchToRHIThread);
		});
		return false;
	}), After);
}

FAutoConsoleCommand GStallCommand(TEXT("FlightForge.GpuWatchdog.Stall"),
                                  TEXT("Debug: block the render thread, or the RHI thread with `rhi`, for <seconds> (default forever), optionally `after=<seconds>`."),
                                  FConsoleCommandWithArgsDelegate::CreateStatic(&Stall));

}  // namespace

#endif

/* //} */
