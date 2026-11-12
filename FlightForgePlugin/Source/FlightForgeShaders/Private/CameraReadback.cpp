#include "CameraReadback.h"

#include "Misc/CoreDelegates.h"
#include "RenderingThread.h"

/* FCameraReadback //{ */

FCameraReadback::FCameraReadback()
{
	Copied = FPlatformProcess::GetSynchEventFromPool(true);
}

FCameraReadback::~FCameraReadback()
{
	if (Copied)
	{
		FPlatformProcess::ReturnSynchEventToPool(Copied);
	}
}

/* //} */

/* ResolveReadback_RenderThread() //{ */

void ResolveReadback_RenderThread(FCameraReadback& Slot)
{
	int32 RowPitchInPixels = 0;
	void* Data             = Slot.Readback->Lock(RowPitchInPixels);

	if (Data)
	{
		const int32 W = Slot.Size.X;
		const int32 H = Slot.Size.Y;

		if (Slot.bLuma)
		{
			Slot.LumaPixels.SetNumUninitialized(W * H);

			for (int32 Row = 0; Row < H; ++Row)
			{
				float* DstRow = Slot.LumaPixels.GetData() + (int64)Row * W;

				if (Slot.bLumaFloat32)
				{
					const FLinearColor* SrcRow = static_cast<const FLinearColor*>(Data) + (int64)Row * RowPitchInPixels;
					for (int32 Col = 0; Col < W; ++Col)
					{
						DstRow[Col] = 0.2126f * SrcRow[Col].R + 0.7152f * SrcRow[Col].G + 0.0722f * SrcRow[Col].B;
					}
				}
				else
				{
					const FFloat16Color* SrcRow = static_cast<const FFloat16Color*>(Data) + (int64)Row * RowPitchInPixels;
					for (int32 Col = 0; Col < W; ++Col)
					{
						DstRow[Col] = 0.2126f * SrcRow[Col].R.GetFloat() + 0.7152f * SrcRow[Col].G.GetFloat() + 0.0722f * SrcRow[Col].B.GetFloat();
					}
				}
			}
		}
		else if (Slot.bHalfRgba || Slot.bFloatRgba)
		{
			Slot.Pixels.SetNumUninitialized(W * H);

			const auto Encode = [](const float Linear)
			{
				const float Clamped = FMath::Clamp(Linear, 0.0f, 1.0f);
				const float Srgb    = Clamped <= 0.0031308f ? Clamped * 12.92f : 1.055f * FMath::Pow(Clamped, 1.0f / 2.4f) - 0.055f;
				return static_cast<uint8>(FMath::RoundToInt(Srgb * 255.0f));
			};

			// every half-float value encoded once: a pow per channel per pixel was the resolve's whole cost
			static const TArray<uint8> HalfLut = [&Encode]
			{
				TArray<uint8> Table;
				Table.SetNumUninitialized(65536);
				for (int32 i = 0; i < 65536; ++i)
				{
					FFloat16 Half;
					Half.Encoded = static_cast<uint16>(i);
					Table[i]     = Encode(Half.GetFloat());
				}
				return Table;
			}();

			for (int32 Row = 0; Row < H; ++Row)
			{
				FColor* DstRow = Slot.Pixels.GetData() + (int64)Row * W;

				if (Slot.bFloatRgba)
				{
					const FLinearColor* SrcRow = static_cast<const FLinearColor*>(Data) + (int64)Row * RowPitchInPixels;

					for (int32 Col = 0; Col < W; ++Col)
					{
						DstRow[Col] = FColor(Encode(SrcRow[Col].R), Encode(SrcRow[Col].G), Encode(SrcRow[Col].B), 255);
					}
				}
				else
				{
					const FFloat16Color* SrcRow = static_cast<const FFloat16Color*>(Data) + (int64)Row * RowPitchInPixels;

					for (int32 Col = 0; Col < W; ++Col)
					{
						DstRow[Col] = FColor(HalfLut[SrcRow[Col].R.Encoded], HalfLut[SrcRow[Col].G.Encoded], HalfLut[SrcRow[Col].B.Encoded], 255);
					}
				}
			}
		}
		else if (Slot.bFloat16)
		{
			Slot.Float16Pixels.SetNumUninitialized(W * H);

			const FFloat16* Src = static_cast<const FFloat16*>(Data);

			if (RowPitchInPixels == W)
			{
				FMemory::Memcpy(Slot.Float16Pixels.GetData(), Src, (int64)W * H * sizeof(FFloat16));
			}
			else
			{
				for (int32 Row = 0; Row < H; ++Row)
				{
					FMemory::Memcpy(Slot.Float16Pixels.GetData() + (int64)Row * W, Src + (int64)Row * RowPitchInPixels, W * sizeof(FFloat16));
				}
			}
		}
		else
		{
			Slot.Pixels.SetNumUninitialized(W * H);

			const FColor* Src = static_cast<const FColor*>(Data);

			if (RowPitchInPixels == W)
			{
				FMemory::Memcpy(Slot.Pixels.GetData(), Src, (int64)W * H * sizeof(FColor));
			}
			else
			{
				for (int32 Row = 0; Row < H; ++Row)
				{
					FMemory::Memcpy(Slot.Pixels.GetData() + (int64)Row * W, Src + (int64)Row * RowPitchInPixels, W * sizeof(FColor));
				}
			}

			if (Slot.bEncodeSrgb)
			{
				static const auto Lut = []
				{
					TStaticArray<uint8, 256> Table;
					for (int32 i = 0; i < 256; ++i)
					{
						const float Linear = i / 255.0f;
						const float Srgb   = Linear <= 0.0031308f ? Linear * 12.92f : 1.055f * FMath::Pow(Linear, 1.0f / 2.4f) - 0.055f;
						Table[i]           = static_cast<uint8>(FMath::RoundToInt(Srgb * 255.0f));
					}
					return Table;
				}();

				for (FColor& Pixel : Slot.Pixels)
				{
					Pixel.R = Lut[Pixel.R];
					Pixel.G = Lut[Pixel.G];
					Pixel.B = Lut[Pixel.B];
				}
			}
		}
	}

	Slot.Readback->Unlock();
	Slot.bCopied = true;
	Slot.Copied->Trigger();
}

/* //} */

/* WatchReadback_RenderThread() //{ */

namespace
{
FCriticalSection                                          GWatchedLock;
TArray<TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>> GWatched;
FDelegateHandle                                          GWatchHandle;

void PollWatched_RenderThread()
{
	TArray<TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>> Ready;
	{
		FScopeLock Lock(&GWatchedLock);
		for (int32 i = GWatched.Num() - 1; i >= 0; --i)
		{
			if (GWatched[i]->Readback->IsReady())
			{
				Ready.Add(GWatched[i]);
				GWatched.RemoveAtSwap(i);
			}
		}
	}
	for (const auto& Slot : Ready)
	{
		if (!Slot->bResolveIssued.exchange(true))
		{
			ResolveReadback_RenderThread(*Slot);
		}
	}
}
}  // namespace

void WatchReadback_RenderThread(const TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>& Slot)
{
	FScopeLock Lock(&GWatchedLock);
	if (!GWatchHandle.IsValid())
	{
		GWatchHandle = FCoreDelegates::OnBeginFrameRT.AddStatic(&PollWatched_RenderThread);
	}
	GWatched.Add(Slot);
}

/* //} */

/* RequestResolve() //{ */

void RequestResolve(const TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>& Slot)
{
	if (!Slot || Slot->bCopied || Slot->bResolveIssued || Slot->bResolveQueued.exchange(true))
	{
		return;
	}

	ENQUEUE_RENDER_COMMAND(FlightForgeRequestResolve)([Slot](FRHICommandListImmediate&) {
		Slot->bResolveQueued = false;
		if (Slot->Readback && Slot->Readback->IsReady() && !Slot->bResolveIssued.exchange(true))
		{
			ResolveReadback_RenderThread(*Slot);
		}
	});
}

/* //} */
