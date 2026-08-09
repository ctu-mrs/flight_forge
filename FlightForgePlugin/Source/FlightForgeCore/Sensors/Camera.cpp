#include "Camera.h"

#include "ContentStreaming.h"
#include "Engine/PostProcessVolume.h"
#include "EngineUtils.h"
#include "UnrealEngine.h"
#include "RenderingThread.h"
#include "TextureResource.h"
#include "FlightForgeCore/DronePawn.h"

#include "Serialize.h"

UCamera::UCamera()
{
	PrimaryComponentTick.bCanEverTick = true;
	//PrimaryComponentTick.TickGroup    = TG_PrePhysics;

	CameraBufferCriticalSection = std::make_unique<FCriticalSection>();

	FrameReadyEvent = FPlatformProcess::GetSynchEventFromPool(false);

	SceneCaptureMeshHolder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SceneCaptureMeshHolder"));
	SceneCaptureMeshHolder->SetupAttachment(this);

	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2D"));
	SceneCaptureComponent2D->SetupAttachment(SceneCaptureMeshHolder);
}


void UCamera::BeginPlay()
{
	Super::BeginPlay();

	//Tick should be called after DronePawn empties InstructionQueue
	PrimaryComponentTick.AddPrerequisite(Owner, Owner->PrimaryActorTick);

	// TODO check https://github.com/TimmHess/UnrealImageCapture
}

void UCamera::Initialize(int InSensorID)
{
	Super::Initialize(InSensorID);
	
	
	SceneCaptureMeshHolder->SetMobility(EComponentMobility::Movable);
	if (!SceneCaptureMeshHolder->IsRegistered()) 
		SceneCaptureMeshHolder->RegisterComponent();
	
	
	SceneCaptureComponent2D->SetMobility(EComponentMobility::Movable);
	if (!SceneCaptureComponent2D->IsRegistered())
		SceneCaptureComponent2D->RegisterComponent();
}

void UCamera::BeginDestroy()
{
	if (CameraBufferCriticalSection)
	{
		CameraBufferCriticalSection->Lock();
		CameraRendered = false;
		CameraBuffer.Empty();
		CameraBufferCriticalSection->Unlock();
	}

	// wake any server thread still waiting on a frame; it will see CameraRendered == false
	if (FrameReadyEvent)
	{
		FrameCounter++;
		FrameReadyEvent->Trigger();
		FPlatformProcess::ReturnSynchEventToPool(FrameReadyEvent);
		FrameReadyEvent = nullptr;
	}

	Super::BeginDestroy();
}

/* async readback machinery //{ */

TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> UCamera::EnqueueReadback(UTextureRenderTarget2D* RT, double Stamp)
{
	FTextureRenderTargetResource* Resource = RT ? RT->GameThread_GetRenderTargetResource() : nullptr;

	if (!Resource)
	{
		return nullptr;
	}

	TSharedPtr<FCameraReadback, ESPMode::ThreadSafe> Slot = MakeShared<FCameraReadback, ESPMode::ThreadSafe>();
	Slot->Size     = FIntPoint(RT->SizeX, RT->SizeY);
	Slot->Stamp    = Stamp;
	Slot->Readback = MakeUnique<FRHIGPUTextureReadback>(TEXT("FlightForgeCameraReadback"));

	ENQUEUE_RENDER_COMMAND(FlightForgeEnqueueReadback)(
		[Slot, Resource](FRHICommandListImmediate& RHICmdList)
		{
			if (FRHITexture* Texture = Resource->GetRenderTargetTexture())
			{
				Slot->Readback->EnqueueCopy(RHICmdList, Texture, FResolveRect());
			}
			else
			{
				// nothing to read; complete with empty pixels so the poller does not hang
				Slot->bCopied = true;
			}
		});

	return Slot;
}

void UCamera::IssueResolve(const TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>& Slot)
{
	ENQUEUE_RENDER_COMMAND(FlightForgeResolveReadback)(
		[Slot](FRHICommandListImmediate& RHICmdList)
		{
			void* Data             = nullptr;
			int32 RowPitchInPixels = 0;
			Slot->Readback->LockTexture(RHICmdList, Data, RowPitchInPixels);

			if (Data)
			{
				const int32 W = Slot->Size.X;
				const int32 H = Slot->Size.Y;
				Slot->Pixels.SetNumUninitialized(W * H);

				const FColor* Src = static_cast<const FColor*>(Data);

				if (RowPitchInPixels == W)
				{
					FMemory::Memcpy(Slot->Pixels.GetData(), Src, (int64)W * H * sizeof(FColor));
				}
				else
				{
					for (int32 Row = 0; Row < H; ++Row)
					{
						FMemory::Memcpy(Slot->Pixels.GetData() + (int64)Row * W, Src + (int64)Row * RowPitchInPixels, W * sizeof(FColor));
					}
				}
			}

			Slot->Readback->Unlock();
			Slot->bCopied = true;
		});
}

bool UCamera::PumpReadback()
{
	if (!PendingReadback)
	{
		return false;
	}

	if (PendingReadback->bCopied)
	{
		CameraBufferCriticalSection->Lock();
		CameraBuffer     = MoveTemp(PendingReadback->Pixels);
		CameraBufferSize = PendingReadback->Size;
		stamp_           = PendingReadback->Stamp;
		CameraRendered   = CameraBuffer.Num() > 0;
		CameraBufferCriticalSection->Unlock();

		PendingReadback.Reset();
		NotifyFrameReady();
		return true;
	}

	if (PendingReadback->Readback && PendingReadback->Readback->IsReady() && !PendingReadback->bResolveIssued.exchange(true))
	{
		IssueResolve(PendingReadback);
	}

	return false;
}

void UCamera::NotifyFrameReady()
{
	FrameCounter++;

	if (FrameReadyEvent)
	{
		FrameReadyEvent->Trigger();
	}
}

bool UCamera::WaitForNewFrame(uint64 BaselineFrame, double TimeoutSeconds)
{
	const double Deadline = FPlatformTime::Seconds() + TimeoutSeconds;

	while (FrameCounter.load() == BaselineFrame)
	{
		const double Remaining = Deadline - FPlatformTime::Seconds();

		if (Remaining <= 0.0 || !FrameReadyEvent)
		{
			return false;
		}

		FrameReadyEvent->Wait(FTimespan::FromSeconds(FMath::Min(Remaining, 0.1)));
	}

	return true;
}

//}

void UCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	PushStreamingViewHint();

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/* streaming + post-process baseline //{ */

void UCamera::PushStreamingViewHint()
{
	if (!SceneCaptureComponent2D || !RenderTarget2D)
	{
		return;
	}

	const bool bActive = SceneCaptureComponent2D->bCaptureEveryFrame || (FPlatformTime::Seconds() - camera_last_request_time_) < 2.0;

	if (!bActive)
	{
		return;
	}

	const float ScreenSize    = (float)RenderTarget2D->SizeX;
	const float HalfFovRad    = FMath::DegreesToRadians(FMath::Clamp(SceneCaptureComponent2D->FOVAngle, 1.0f, 170.0f) * 0.5f);
	const float FOVScreenSize = ScreenSize / FMath::Tan(HalfFovRad);

	IStreamingManager::Get().AddViewInformation(SceneCaptureComponent2D->GetComponentLocation(), ScreenSize, FOVScreenSize);
}

void UCamera::ApplyLevelPostProcessBaseline(USceneCaptureComponent2D* Capture)
{
	UWorld* World = GetWorld();

	if (!Capture || !World)
	{
		return;
	}

	for (TActorIterator<APostProcessVolume> It(World); It; ++It)
	{
		if (It->bEnabled && It->bUnbound)
		{
			Capture->PostProcessSettings    = It->Settings;
			Capture->PostProcessBlendWeight = It->BlendWeight;
			return;
		}
	}
}

//}

/* intrinsics projection //{ */

FMatrix UCamera::IntrinsicsProjectionMatrix(const FCameraIntrinsics& Intrinsics, const int32 Width, const int32 Height)
{
	// Reversed-Z with an infinite far plane, matching FReversedZPerspectiveMatrix; in
	// the centered case (cx = W/2, cy = H/2, fx per the FOV) the two are identical.
	// The off-axis terms shift NDC so that pixel (u,v) = (fx·X/Z + cx, fy·(-Y)/Z + cy)
	// with the image origin top-left and view-space Y up.
	return FMatrix(
	    FPlane(2.0 * Intrinsics.Fx / Width, 0.0, 0.0, 0.0),
	    FPlane(0.0, 2.0 * Intrinsics.Fy / Height, 0.0, 0.0),
	    FPlane(2.0 * Intrinsics.Cx / Width - 1.0, 1.0 - 2.0 * Intrinsics.Cy / Height, 0.0, 1.0),
	    FPlane(0.0, 0.0, GNearClippingPlane, 0.0));
}

FCameraIntrinsics UCamera::EffectiveIntrinsics(const FCameraIntrinsics& Intrinsics, const double FovDeg, const int32 Width, const int32 Height)
{
	if (Intrinsics.UseCustom && Intrinsics.Fx > 0.0 && Intrinsics.Fy > 0.0)
	{
		return Intrinsics;
	}

	FCameraIntrinsics Effective;
	Effective.Fx = (Width * 0.5) / FMath::Tan(FMath::DegreesToRadians(FovDeg) * 0.5);
	Effective.Fy = Effective.Fx;
	Effective.Cx = Width * 0.5;
	Effective.Cy = Height * 0.5;

	return Effective;
}

void UCamera::ApplyProjection(USceneCaptureComponent2D* Capture, const FCameraIntrinsics& Intrinsics, const double FovDeg, const int32 Width,
                              const int32 Height)
{
	// FOVAngle stays meaningful either way: it drives culling bounds and is the
	// fallback when the custom matrix is disabled again.
	Capture->FOVAngle = FovDeg;

	const bool bValid = Intrinsics.UseCustom && Intrinsics.Fx > 0.0 && Intrinsics.Fy > 0.0 && Width > 0 && Height > 0;

	Capture->bUseCustomProjectionMatrix = bValid;

	if (bValid)
	{
		Capture->CustomProjectionMatrix = IntrinsicsProjectionMatrix(Intrinsics, Width, Height);
	}
}

//}

/* distortion LUT //{ */

TSharedPtr<const FDistortionLUT, ESPMode::ThreadSafe> UCamera::BuildDistortionLUT(const FCameraDistortion& Distortion, const FCameraIntrinsics& EffectiveK,
                                                                                  const int32 Width, const int32 Height)
{
	if (Width <= 0 || Height <= 0 || EffectiveK.Fx <= 0.0 || EffectiveK.Fy <= 0.0)
	{
		return nullptr;
	}

	TSharedRef<FDistortionLUT, ESPMode::ThreadSafe> LUT = MakeShared<FDistortionLUT, ESPMode::ThreadSafe>();
	LUT->OutputSize                                     = FIntPoint(Width, Height);
	LUT->Source.SetNumUninitialized(Width * Height);

	// For each output pixel of the distorted image, invert the Brown-Conrady model
	// (fixed-point iteration, the standard cv::undistortPoints scheme) to find the
	// ideal-pinhole ray, i.e. where that pixel's content lives in the clean render.
	TArray<FVector2d> Pinhole;
	Pinhole.SetNumUninitialized(Width * Height);

	double MinX = TNumericLimits<double>::Max(), MaxX = -TNumericLimits<double>::Max();
	double MinY = TNumericLimits<double>::Max(), MaxY = -TNumericLimits<double>::Max();

	for (int32 V = 0; V < Height; ++V)
	{
		for (int32 U = 0; U < Width; ++U)
		{
			const double Xd = (U + 0.5 - EffectiveK.Cx) / EffectiveK.Fx;
			const double Yd = (V + 0.5 - EffectiveK.Cy) / EffectiveK.Fy;

			double Xu = Xd;
			double Yu = Yd;

			for (int32 Iteration = 0; Iteration < 8; ++Iteration)
			{
				const double R2     = Xu * Xu + Yu * Yu;
				const double Radial = 1.0 + R2 * (Distortion.K1 + R2 * (Distortion.K2 + R2 * Distortion.K3));
				const double Dx     = 2.0 * Distortion.P1 * Xu * Yu + Distortion.P2 * (R2 + 2.0 * Xu * Xu);
				const double Dy     = Distortion.P1 * (R2 + 2.0 * Yu * Yu) + 2.0 * Distortion.P2 * Xu * Yu;

				Xu = (Xd - Dx) / Radial;
				Yu = (Yd - Dy) / Radial;
			}

			const double Sx = Xu * EffectiveK.Fx + EffectiveK.Cx;
			const double Sy = Yu * EffectiveK.Fy + EffectiveK.Cy;

			MinX = FMath::Min(MinX, Sx);
			MaxX = FMath::Max(MaxX, Sx);
			MinY = FMath::Min(MinY, Sy);
			MaxY = FMath::Max(MaxY, Sy);

			Pinhole[V * Width + U] = FVector2d(Sx, Sy);
		}
	}

	// Overscan sized from the actual excursion outside the nominal image, +1px of
	// bilinear margin, capped so pathological coefficients cannot explode the target.
	const int32 PadX = FMath::Clamp(FMath::CeilToInt32(FMath::Max3(0.0, -MinX, MaxX - Width)) + 1, 0, Width / 2);
	const int32 PadY = FMath::Clamp(FMath::CeilToInt32(FMath::Max3(0.0, -MinY, MaxY - Height)) + 1, 0, Height / 2);

	LUT->Pad        = FIntPoint(PadX, PadY);
	LUT->SourceSize = FIntPoint(Width + 2 * PadX, Height + 2 * PadY);

	for (int32 Index = 0; Index < Pinhole.Num(); ++Index)
	{
		// pixel-center coordinates to array-index space: index = center - 0.5
		LUT->Source[Index] = FVector2f(Pinhole[Index].X + PadX - 0.5, Pinhole[Index].Y + PadY - 0.5);
	}

	return LUT;
}

void UCamera::ResampleImage(const FDistortionLUT& LUT, const TArray<FColor>& Source, TArray<FColor>& OutResampled, const bool bNearestNeighbor)
{
	const int32 SrcW = LUT.SourceSize.X;
	const int32 SrcH = LUT.SourceSize.Y;

	OutResampled.SetNumUninitialized(LUT.Source.Num());

	const FColor* Src = Source.GetData();
	FColor*       Dst = OutResampled.GetData();

	for (int32 Index = 0; Index < LUT.Source.Num(); ++Index)
	{
		const float X = LUT.Source[Index].X;
		const float Y = LUT.Source[Index].Y;

		if (bNearestNeighbor)
		{
			const int32 U = FMath::Clamp(FMath::RoundToInt32(X), 0, SrcW - 1);
			const int32 V = FMath::Clamp(FMath::RoundToInt32(Y), 0, SrcH - 1);

			Dst[Index] = Src[V * SrcW + U];
			continue;
		}

		const int32 U0 = FMath::Clamp(FMath::FloorToInt32(X), 0, SrcW - 1);
		const int32 V0 = FMath::Clamp(FMath::FloorToInt32(Y), 0, SrcH - 1);
		const int32 U1 = FMath::Min(U0 + 1, SrcW - 1);
		const int32 V1 = FMath::Min(V0 + 1, SrcH - 1);

		const float FracX = FMath::Clamp(X - U0, 0.0f, 1.0f);
		const float FracY = FMath::Clamp(Y - V0, 0.0f, 1.0f);

		const FColor& C00 = Src[V0 * SrcW + U0];
		const FColor& C10 = Src[V0 * SrcW + U1];
		const FColor& C01 = Src[V1 * SrcW + U0];
		const FColor& C11 = Src[V1 * SrcW + U1];

		const auto Lerp2 = [FracX, FracY](const uint8 A00, const uint8 A10, const uint8 A01, const uint8 A11) {
			const float Top    = A00 + (A10 - A00) * FracX;
			const float Bottom = A01 + (A11 - A01) * FracX;
			return static_cast<uint8>(FMath::RoundToInt32(Top + (Bottom - Top) * FracY));
		};

		Dst[Index] = FColor(Lerp2(C00.R, C10.R, C01.R, C11.R), Lerp2(C00.G, C10.G, C01.G, C11.G), Lerp2(C00.B, C10.B, C01.B, C11.B), 255);
	}
}

void UCamera::SetupCaptureProjection(const FCameraIntrinsics& Intrinsics, const FCameraDistortion& Distortion, const double FovDeg, const int32 Width,
                                     const int32 Height, std::initializer_list<USceneCaptureComponent2D*> Captures,
                                     std::initializer_list<UTextureRenderTarget2D*> Targets)
{
	if (Width <= 0 || Height <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid dimensions for RenderTarget2D. Width and Height should be greater than 0."));
		return;
	}

	if (!Distortion.Enable)
	{
		DistortionLUT.Reset();

		for (USceneCaptureComponent2D* Capture : Captures)
		{
			ApplyProjection(Capture, Intrinsics, FovDeg, Width, Height);
		}
		for (UTextureRenderTarget2D* Target : Targets)
		{
			Target->ResizeTarget(Width, Height);
		}

		return;
	}

	const FCameraIntrinsics EffectiveK = EffectiveIntrinsics(Intrinsics, FovDeg, Width, Height);

	TSharedPtr<const FDistortionLUT, ESPMode::ThreadSafe> LUT = BuildDistortionLUT(Distortion, EffectiveK, Width, Height);
	if (!LUT)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCamera: could not build the distortion LUT, rendering undistorted"));
		DistortionLUT.Reset();

		for (USceneCaptureComponent2D* Capture : Captures)
		{
			ApplyProjection(Capture, Intrinsics, FovDeg, Width, Height);
		}
		for (UTextureRenderTarget2D* Target : Targets)
		{
			Target->ResizeTarget(Width, Height);
		}

		return;
	}

	// The clean source render: same focal lengths, principal point shifted by the
	// padding, rendered at the padded size — the wider frustum the warp samples from.
	FCameraIntrinsics PaddedK = EffectiveK;
	PaddedK.UseCustom         = true;
	PaddedK.Cx += LUT->Pad.X;
	PaddedK.Cy += LUT->Pad.Y;

	for (USceneCaptureComponent2D* Capture : Captures)
	{
		ApplyProjection(Capture, PaddedK, FovDeg, LUT->SourceSize.X, LUT->SourceSize.Y);
	}
	for (UTextureRenderTarget2D* Target : Targets)
	{
		Target->ResizeTarget(LUT->SourceSize.X, LUT->SourceSize.Y);
	}

	DistortionLUT = LUT;
}

//}


void UCamera::SetCameraCaptureMode(CameraCaptureModeEnum CaptureMode) {

	CameraCaptureMode = CaptureMode;

	// if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES || CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_MOVEMENT) {
	// 	//UE_LOG(LogTemp, Log, TEXT("Camera updating after capture mode change"));
	//     UpdateCamera(false, 0.0); //used to update only in rgb mode
	// }
}

// packs BGRA pixels into the 3-byte-per-pixel wire layout
void UCamera::TransformImageArray(int32 ImageWidth, int32 ImageHeight, const TArray<FColor>& SrcData, TArray<uint8>& DstData) {
	const int32 NumPixels = ImageWidth * ImageHeight;

	DstData.SetNumUninitialized(NumPixels * 3);

	const FColor* SrcDataPtr = SrcData.GetData();
	uint8*        DstDataPtr = DstData.GetData();

	for (int32 Index = 0; Index < NumPixels; Index++) {
		DstDataPtr[Index * 3 + 0] = SrcDataPtr[Index].B;
		DstDataPtr[Index * 3 + 1] = SrcDataPtr[Index].G;
		DstDataPtr[Index * 3 + 2] = SrcDataPtr[Index].R;
	}
}

void UCamera::UpdateCamera(bool isExternallyLocked, double stamp = 0.0) {}

void UCamera::GetConfig(std::stringstream& OutputStream) {}

void UCamera::SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream) {}

void UCamera::GetData(std::stringstream& OutputStream) {}

