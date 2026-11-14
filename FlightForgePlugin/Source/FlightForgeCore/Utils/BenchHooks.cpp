#include "BenchHooks.h"

#include "Containers/Ticker.h"
#include "DynamicRHI.h"
#include "Engine/Engine.h"
#include "HAL/IConsoleManager.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/CommandLine.h"
#include "Misc/DateTime.h"
#include "Misc/FileHelper.h"
#include "RenderTimer.h"

namespace
{

TAutoConsoleVariable<float> CVarStatsInterval(TEXT("FlightForge.Bench.StatsInterval"), 0.0f,
                                              TEXT("seconds between FlightForgeStats log lines (mean game, render, RHI and GPU ms per frame); 0: off"));

FTSTicker::FDelegateHandle BenchTickerHandle;

/* frame stats //{ */

struct FBenchSums
{
	double Start  = 0.0;
	int32  Frames = 0;
	double Game = 0.0, Render = 0.0, Rhi = 0.0, Gpu = 0.0;
};

FBenchSums BenchSums;

void TickStats()
{
	const float  Interval = CVarStatsInterval.GetValueOnGameThread();
	const double Now      = FPlatformTime::Seconds();
	if (Interval <= 0.0f)
	{
		BenchSums = FBenchSums();
		return;
	}
	if (BenchSums.Start <= 0.0)
	{
		BenchSums.Start = Now;
		return;
	}

	BenchSums.Frames++;
	BenchSums.Game += FPlatformTime::ToMilliseconds(GGameThreadTime);
	BenchSums.Render += FPlatformTime::ToMilliseconds(GRenderThreadTime);
	BenchSums.Rhi += FPlatformTime::ToMilliseconds(GRHIThreadTime);
	BenchSums.Gpu += FPlatformTime::ToMilliseconds(RHIGetGPUFrameCycles(0));

	const double Span = Now - BenchSums.Start;
	if (Span >= Interval)
	{
		const double N = FMath::Max(BenchSums.Frames, 1);
		UE_LOG(LogTemp, Log, TEXT("FlightForgeStats: t=%.3f frames=%d fps=%.2f game=%.3f render=%.3f rhi=%.3f gpu=%.3f"), FDateTime::UtcNow().ToUnixTimestampDecimal(),
		       BenchSums.Frames, BenchSums.Frames / Span, BenchSums.Game / N, BenchSums.Render / N, BenchSums.Rhi / N, BenchSums.Gpu / N);
		BenchSums       = FBenchSums();
		BenchSums.Start = Now;
	}
}

//}

/* command file //{ */

FString BenchCommandFile;
int64   BenchCommandOffset = 0;
double  BenchNextPoll      = 0.0;

void TickCommands()
{
#if !UE_BUILD_SHIPPING
	if (BenchCommandFile.IsEmpty() || !GEngine || FPlatformTime::Seconds() < BenchNextPoll)
	{
		return;
	}
	BenchNextPoll = FPlatformTime::Seconds() + 0.2;

	FString Text;
	if (!FFileHelper::LoadFileToString(Text, *BenchCommandFile) || Text.Len() <= BenchCommandOffset)
	{
		return;
	}

	const int32 End = Text.Find(TEXT("\n"), ESearchCase::CaseSensitive, ESearchDir::FromEnd);
	if (End < BenchCommandOffset)
	{
		return;  // a line still being written
	}

	TArray<FString> Lines;
	Text.Mid(BenchCommandOffset, End - BenchCommandOffset).ParseIntoArrayLines(Lines);
	BenchCommandOffset = End + 1;

	for (const FString& Line : Lines)
	{
		UE_LOG(LogTemp, Log, TEXT("FlightForgeBench: exec %s"), *Line);
		GEngine->Exec(GEngine->GetCurrentPlayWorld(), *Line);
	}
#endif
}

//}

bool TickBench(float)
{
	TickStats();
	TickCommands();
	return true;
}

}  // namespace

/* FBenchHooks //{ */

void FBenchHooks::Start()
{
	FParse::Value(FCommandLine::Get(), TEXT("-FlightForgeBenchCommands="), BenchCommandFile);
	BenchTickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateStatic(&TickBench));
}

void FBenchHooks::Stop()
{
	FTSTicker::GetCoreTicker().RemoveTicker(BenchTickerHandle);
}

/* //} */
