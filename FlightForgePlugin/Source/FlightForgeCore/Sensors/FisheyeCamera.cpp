#include "FisheyeCamera.h"

#include "Async/ParallelFor.h"
#include "TextureResource.h"

#include "Serialize.h"
#include "FlightForgeCore/DronePawn.h"
#include "FlightForgeCore/SimClock.h"
#include "RenderingThread.h"

/* face basis //{ */

namespace
{
struct FFaceBasis
{
	FVector  Forward;
	FVector  Right;
	FVector  Up;
	FRotator Rotation;
};

// forward, right, left, up, down - camera-local
const FFaceBasis FaceBases[UFisheyeCamera::NumFaces] = {
    {FVector(1, 0, 0), FVector(0, 1, 0), FVector(0, 0, 1), FRotator(0, 0, 0)},
    {FVector(0, 1, 0), FVector(-1, 0, 0), FVector(0, 0, 1), FRotator(0, 90, 0)},
    {FVector(0, -1, 0), FVector(1, 0, 0), FVector(0, 0, 1), FRotator(0, -90, 0)},
    {FVector(0, 0, 1), FVector(0, 1, 0), FVector(-1, 0, 0), FRotator(90, 0, 0)},
    {FVector(0, 0, -1), FVector(0, 1, 0), FVector(1, 0, 0), FRotator(-90, 0, 0)},
};

// faces render slightly wider than 90 degrees so bilinear taps at face borders
// stay inside valid pixels instead of seaming
constexpr float FaceFovDeg = 95.0f;
}  // namespace

//}

UFisheyeCamera::UFisheyeCamera() : UCamera()
{
	SensorType = SensorType::FISHEYE_CAMERA;
	Name = TEXT("FISHEYE_CAMERA");

	InstructionQueue = std::make_unique<TQueue<std::shared_ptr<FInstruction<UFisheyeCamera>>>>();

	FaceCaptures.SetNum(NumFaces);
	FaceCaptures[0] = SceneCaptureComponent2D;

	for (int32 Face = 1; Face < NumFaces; ++Face)
	{
		FaceCaptures[Face] = CreateDefaultSubobject<USceneCaptureComponent2D>(*FString::Printf(TEXT("FisheyeFaceCapture%d"), Face));
		FaceCaptures[Face]->SetupAttachment(SceneCaptureMeshHolder);
	}

	FaceBuffers.SetNum(NumFaces);
}

int32 UFisheyeCamera::FaceSize() const
{
	const int32 Longest = FMath::Max(fisheye_camera_config_.Width, fisheye_camera_config_.Height);
	return FMath::Clamp((int32)FMath::RoundUpToPowerOfTwo(FMath::Max(Longest / 2, 1)), 256, 1024);
}

/* BeginPlay() / Initialize() //{ */

void UFisheyeCamera::BeginPlay()
{
	Super::BeginPlay();

	fisheye_camera_config_.ShowCameraComponent = false;
	fisheye_camera_config_.Offset              = DEFAULT_CAMERA_MOUNT;
	fisheye_camera_config_.Orientation         = FRotator(0, 0, 0);
	fisheye_camera_config_.FOVAngle            = 180;
	fisheye_camera_config_.Width               = 640;
	fisheye_camera_config_.Height              = 640;
	fisheye_camera_config_.LensModel           = EFisheyeLensModel::EQUIDISTANT;
	fisheye_camera_config_.FullFrame           = false;

	const int32 Size = FaceSize();

	FaceRenderTargets.SetNum(NumFaces);

	for (int32 Face = 0; Face < NumFaces; ++Face)
	{
		FaceRenderTargets[Face] = NewObject<UTextureRenderTarget2D>();
		FaceRenderTargets[Face]->InitCustomFormat(Size, Size, PF_B8G8R8A8, false);
		FaceRenderTargets[Face]->RenderTargetFormat = RTF_RGBA8;
		FaceRenderTargets[Face]->bGPUSharedFlag     = true;

		USceneCaptureComponent2D* Capture = FaceCaptures[Face];

		// temporal effects are per-face and would seam at the boundaries
		Capture->CaptureSource = SCS_FinalColorLDR;
		Capture->TextureTarget = FaceRenderTargets[Face];
		Capture->FOVAngle      = FaceFovDeg;
		Capture->ShowFlags.SetTemporalAA(false);
		Capture->ShowFlags.SetMotionBlur(false);
		Capture->bAlwaysPersistRenderingState = true;
		Capture->bCaptureEveryFrame           = false;
		Capture->bCaptureOnMovement           = false;
		Capture->SetRelativeRotation(FaceBases[Face].Rotation);
	}

	RenderTarget2D = FaceRenderTargets[0];

	for (int32 Face = 0; Face < NumFaces; ++Face)
	{
		ApplyLevelPostProcessBaseline(FaceCaptures[Face]);

		// Screen-space post effects are computed per face, so anything that varies with
		// what a face happens to see leaves a straight seam along the face boundary and
		// the result reads as the inside of a box. Exposure is the worst of them: a face
		// full of sky adapts differently from one full of ground. Lock them all.
		FPostProcessSettings& Settings = FaceCaptures[Face]->PostProcessSettings;

		Settings.bOverride_AutoExposureMinBrightness = true;
		Settings.bOverride_AutoExposureMaxBrightness = true;
		Settings.AutoExposureMinBrightness           = 1.0f;
		Settings.AutoExposureMaxBrightness           = 1.0f;

		Settings.bOverride_VignetteIntensity = true;
		Settings.VignetteIntensity           = 0.0f;

		Settings.bOverride_BloomIntensity = true;
		Settings.BloomIntensity           = 0.0f;

		Settings.bOverride_SceneFringeIntensity = true;
		Settings.SceneFringeIntensity           = 0.0f;
	}

	SetFisheyeCameraConfig(fisheye_camera_config_);
	SetCameraCaptureMode(Owner->GetCameraCaptureMode());
}

void UFisheyeCamera::Initialize(int InSensorID)
{
	Super::Initialize(InSensorID);

	for (int32 Face = 1; Face < NumFaces; ++Face)
	{
		FaceCaptures[Face]->SetMobility(EComponentMobility::Movable);
		if (!FaceCaptures[Face]->IsRegistered())
			FaceCaptures[Face]->RegisterComponent();
	}
}

//}

void UFisheyeCamera::BeginDestroy()
{
	FaceBuffers.Empty();

	Super::BeginDestroy();
}

/* lens model //{ */

double UFisheyeCamera::FocalPx(const EFisheyeLensModel Model, const double FovDeg, const int32 Width, const int32 Height, const bool FullFrame)
{
	const double HalfFov     = FMath::DegreesToRadians(FMath::Clamp(FovDeg, 10.0, 220.0)) * 0.5;
	const double ImageRadius = FullFrame ? 0.5 * FMath::Sqrt((double)(Width * Width + Height * Height)) : 0.5 * FMath::Min(Width, Height);
	switch (Model)
	{
		case EFisheyeLensModel::EQUISOLID: return ImageRadius / (2.0 * FMath::Sin(HalfFov * 0.5));
		case EFisheyeLensModel::STEREOGRAPHIC: return ImageRadius / (2.0 * FMath::Tan(HalfFov * 0.5));
		default: return ImageRadius / HalfFov;  // KB without custom intrinsics: the polynomial distorts an equidistant fit
	}
}

FCameraIntrinsics UFisheyeCamera::EffectiveFisheyeIntrinsics(const FFisheyeCameraConfig& Config)
{
	if (Config.Intrinsics.UseCustom && Config.Intrinsics.Fx > 0.0 && Config.Intrinsics.Fy > 0.0)
	{
		return Config.Intrinsics;
	}
	FCameraIntrinsics K;
	K.Fx = K.Fy = FocalPx(Config.LensModel, Config.FOVAngle, Config.Width, Config.Height, Config.FullFrame);
	K.Cx        = Config.Width * 0.5;
	K.Cy        = Config.Height * 0.5;
	return K;
}

bool UFisheyeCamera::PixelToAngles(const FFisheyeCameraConfig& Config, const FCameraIntrinsics& K, const double px, const double py, double& Theta, double& Phi)
{
	const double HalfFov = FMath::DegreesToRadians(FMath::Clamp(Config.FOVAngle, 10.0, 220.0)) * 0.5;

	// radial distance in focal units: r = theta for an ideal equidistant lens
	const double U = (px - K.Cx) / K.Fx;
	const double V = (py - K.Cy) / K.Fy;
	const double R = FMath::Sqrt(U * U + V * V);

	switch (Config.LensModel)
	{
		case EFisheyeLensModel::EQUISOLID:
			if (R > 2.0) return false;
			Theta = 2.0 * FMath::Asin(R * 0.5);
			break;
		case EFisheyeLensModel::STEREOGRAPHIC:
			Theta = 2.0 * FMath::Atan(R * 0.5);
			break;
		case EFisheyeLensModel::KANNALA_BRANDT:
		{
			// invert r = theta (1 + k1 theta^2 + ...) by Newton, as OpenCV's fisheye undistort does
			Theta = R;
			for (int32 i = 0; i < 10; ++i)
			{
				const double T2 = Theta * Theta, T4 = T2 * T2, T6 = T4 * T2, T8 = T4 * T4;
				const double F  = Theta * (1.0 + Config.K1 * T2 + Config.K2 * T4 + Config.K3 * T6 + Config.K4 * T8) - R;
				const double dF = 1.0 + 3.0 * Config.K1 * T2 + 5.0 * Config.K2 * T4 + 7.0 * Config.K3 * T6 + 9.0 * Config.K4 * T8;
				if (FMath::Abs(dF) < 1.0e-12) return false;
				Theta -= F / dF;
				if (!FMath::IsFinite(Theta) || Theta < 0.0) return false;
			}
			break;
		}
		default:
			Theta = R;
			break;
	}

	if (!(Theta <= HalfFov))
	{
		return false;
	}
	Phi = FMath::Atan2(V, U);
	return true;
}

void UFisheyeCamera::KannalaBrandtCoefficients(const FFisheyeCameraConfig& Config, double& K1, double& K2, double& K3, double& K4)
{
	switch (Config.LensModel)
	{
		case EFisheyeLensModel::EQUISOLID:  // 2 sin(t/2) = t - t^3/24 + t^5/1920 - t^7/322560 + t^9/92897280
			K1 = -1.0 / 24.0; K2 = 1.0 / 1920.0; K3 = -1.0 / 322560.0; K4 = 1.0 / 92897280.0;
			break;
		case EFisheyeLensModel::STEREOGRAPHIC:  // 2 tan(t/2) = t + t^3/12 + t^5/120 + 17 t^7/20160 + 31 t^9/362880
			K1 = 1.0 / 12.0; K2 = 1.0 / 120.0; K3 = 17.0 / 20160.0; K4 = 31.0 / 362880.0;
			break;
		case EFisheyeLensModel::KANNALA_BRANDT:
			K1 = Config.K1; K2 = Config.K2; K3 = Config.K3; K4 = Config.K4;
			break;
		default:
			K1 = K2 = K3 = K4 = 0.0;
			break;
	}
}

Serializable::Drone::CameraInfo UFisheyeCamera::FisheyeInfo(const FFisheyeCameraConfig& C)
{
	const FCameraIntrinsics K = EffectiveFisheyeIntrinsics(C);

	Serializable::Drone::CameraInfo Info;
	Info.width_       = C.Width;
	Info.height_      = C.Height;
	switch (C.LensModel)
	{
		case EFisheyeLensModel::EQUISOLID: Info.projection_ = "fisheye_equisolid"; break;
		case EFisheyeLensModel::STEREOGRAPHIC: Info.projection_ = "fisheye_stereographic"; break;
		case EFisheyeLensModel::KANNALA_BRANDT: Info.projection_ = "fisheye_kannala_brandt"; break;
		default: Info.projection_ = "fisheye_equidistant"; break;
	}
	Info.distortion_model_ = "equidistant";
	Info.fx_               = K.Fx;
	Info.fy_               = K.Fy;
	Info.cx_               = K.Cx - WirePixelShift;
	Info.cy_               = K.Cy - WirePixelShift;
	Info.d_.resize(4);
	KannalaBrandtCoefficients(C, Info.d_[0], Info.d_[1], Info.d_[2], Info.d_[3]);
	Info.fov_deg_        = C.FOVAngle;
	Info.offset_x_       = C.Offset.X;
	Info.offset_y_       = C.Offset.Y;
	Info.offset_z_       = C.Offset.Z;
	Info.rotation_pitch_ = C.Orientation.Pitch;
	Info.rotation_yaw_   = C.Orientation.Yaw;
	Info.rotation_roll_  = C.Orientation.Roll;
	return Info;
}

void UFisheyeCamera::DescribeCameras(std::vector<Serializable::Drone::CameraInfo>& Out)
{
	auto Info         = FisheyeInfo(fisheye_camera_config_);
	Info.sensor_id_   = GetSensorID();
	Info.sensor_type_ = static_cast<int>(GetSensorType());
	Out.push_back(Info);
}

//}

/* RebuildLUT() //{ */

void UFisheyeCamera::RebuildLUT()
{
	const int32 W = fisheye_camera_config_.Width;
	const int32 H = fisheye_camera_config_.Height;

	if (W <= 0 || H <= 0)
	{
		return;
	}

	const float TanHalfFace = FMath::Tan(FMath::DegreesToRadians(FaceFovDeg) * 0.5f);
	const int32 Size        = FaceSize();

	const FFisheyeCameraConfig Config = fisheye_camera_config_;
	const FCameraIntrinsics    K      = EffectiveFisheyeIntrinsics(Config);

	TSharedPtr<TArray<FFisheyeLUTEntry>, ESPMode::ThreadSafe> NewLUT = MakeShared<TArray<FFisheyeLUTEntry>, ESPMode::ThreadSafe>();
	NewLUT->SetNumUninitialized(W * H);

	ParallelFor(H, [&](int32 py) {
		for (int32 px = 0; px < W; ++px)
		{
			FFisheyeLUTEntry& Entry = (*NewLUT)[py * W + px];
			Entry.Face = -1;

			double Theta = 0.0, Phi = 0.0;
			if (!PixelToAngles(Config, K, px + 0.5, py + 0.5, Theta, Phi))
			{
				continue;
			}

			// image +u is camera right (+Y), image +v is down (-Z)
			const FVector Dir(FMath::Cos(Theta), FMath::Sin(Theta) * FMath::Cos(Phi), -FMath::Sin(Theta) * FMath::Sin(Phi));

			for (int32 Face = 0; Face < NumFaces; ++Face)
			{
				const FFaceBasis& B    = FaceBases[Face];
				const float       FDot = (float)FVector::DotProduct(Dir, B.Forward);

				if (FDot < 0.05f)
				{
					continue;
				}

				const float UF = (float)FVector::DotProduct(Dir, B.Right) / FDot / TanHalfFace;
				const float VF = -(float)FVector::DotProduct(Dir, B.Up) / FDot / TanHalfFace;

				if (FMath::Abs(UF) <= 1.0f && FMath::Abs(VF) <= 1.0f)
				{
					Entry.Face = (int8)Face;
					Entry.FX   = (UF * 0.5f + 0.5f) * (Size - 1);
					Entry.FY   = (VF * 0.5f + 0.5f) * (Size - 1);
					break;
				}
			}
		}
	});

	CameraBufferCriticalSection->Lock();
	LUT     = NewLUT;
	LUTSize = FIntPoint(W, H);
	CameraBufferCriticalSection->Unlock();
}

//}

/* capture pipeline //{ */

void UFisheyeCamera::SetCaptureEveryFrame(bool bEnable)
{
	for (int32 Face = 0; Face < NumFaces; ++Face)
	{
		FaceCaptures[Face]->bCaptureEveryFrame = bEnable;
	}
}

void UFisheyeCamera::DrainInstructions()
{
	std::shared_ptr<FInstruction<UFisheyeCamera>> Instruction;
	while (InstructionQueue->Dequeue(Instruction))
	{
		Instruction->Function(*this);
		Instruction->Finished = true;
	}
}

void UFisheyeCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	auto start = FPlatformTime::Seconds();

	DrainInstructions();

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES)
	{
		const bool bWanted = (FPlatformTime::Seconds() - camera_last_request_time_) < 1.0;

		if (bWanted && !bStreamOn)
		{
			bStreamOn = true;
			SetCaptureEveryFrame(CaptureRate <= 0.0);
			UE_LOG(LogTemp, Warning, TEXT("Turning on fisheye capture"));
		}
		else if (!bWanted && bStreamOn)
		{
			bStreamOn = false;
			SetCaptureEveryFrame(false);
			CameraBufferCriticalSection->Lock();
			CameraRendered = false;
			CameraBufferCriticalSection->Unlock();
			UE_LOG(LogTemp, Warning, TEXT("Turning off fisheye capture, %u frames folded"), FramesFolded);
		}
		else if (bWanted && ShouldCapture() && CaptureDue())
		{
			UpdateCamera(true, start);
		}
	}

	PumpFisheyeReadback();

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// game thread; captures all five faces and hands the transfers to the GPU
// readback pipeline
void UFisheyeCamera::UpdateCamera(bool isExternallyLocked, double stamp)
{
	if (InflightFaceSets.Num() >= MaxInflight)
	{
		if (!FFlightForgeSimClock::bLockstep.load())
		{
			FramesFolded++;
			return;
		}

		const double Deadline = FPlatformTime::Seconds() + 5.0;
		while (InflightFaceSets.Num() >= MaxInflight && FPlatformTime::Seconds() < Deadline)
		{
			FlushRenderingCommands();
			if (!PumpFisheyeReadback() && InflightFaceSets.Num() >= MaxInflight)
			{
				FPlatformProcess::SleepNoStats(0.0002f);
			}
		}
		if (InflightFaceSets.Num() >= MaxInflight)
		{
			return;
		}
	}

	if (!SceneCaptureComponent2D->bCaptureEveryFrame)
	{
		for (int32 Face = 0; Face < NumFaces; ++Face)
		{
			FaceCaptures[Face]->CaptureScene();
		}
	}

	TArray<TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>> Set;
	for (int32 Face = 0; Face < NumFaces; ++Face)
	{
		Set.Add(EnqueueReadback(FaceRenderTargets[Face], stamp));
	}
	InflightFaceSets.Add(MoveTemp(Set));
}

// publishes only when all five faces have landed, so a frame is seam-consistent
bool UFisheyeCamera::PumpFisheyeReadback()
{
	bool bPublished = false;

	while (InflightFaceSets.Num() > 0)
	{
		TArray<TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>>& Faces = InflightFaceSets[0];

		bool bAllCopied = Faces.Num() == NumFaces;

		for (const auto& Face : Faces)
		{
			if (!Face)
			{
				bAllCopied = false;
				continue;
			}
			if (Face->bCopied)
			{
				continue;
			}
			bAllCopied = false;
			if (Face->Readback && Face->Readback->IsReady() && !Face->bResolveIssued.exchange(true))
			{
				IssueResolve(Face);
			}
		}

		if (!bAllCopied)
		{
			// a set with a missing face can never complete
			if (Faces.Num() != NumFaces || Faces.Contains(nullptr))
			{
				InflightFaceSets.RemoveAt(0);
				continue;
			}
			break;
		}

		CameraBufferCriticalSection->Lock();

		bool bComplete = true;
		for (int32 Face = 0; Face < NumFaces; ++Face)
		{
			FaceBuffers[Face] = MoveTemp(Faces[Face]->Pixels);
			bComplete &= FaceBuffers[Face].Num() > 0;
		}
		PublishedFaceSize = Faces[0]->Size.X;
		stamp_            = Faces[0]->Stamp;
		CameraRendered    = bComplete;

		CameraBufferCriticalSection->Unlock();

		InflightFaceSets.RemoveAt(0);

		NotifyFrameReady();
		bPublished = true;
	}

	return bPublished;
}

//}

void UFisheyeCamera::DrainReadback()
{
	const double Deadline = FPlatformTime::Seconds() + 5.0;

	while (InflightFaceSets.Num() > 0 && FPlatformTime::Seconds() < Deadline)
	{
		FlushRenderingCommands();
		if (!PumpFisheyeReadback() && InflightFaceSets.Num() > 0)
		{
			FPlatformProcess::SleepNoStats(0.0002f);
		}
	}
}

/* GetFisheyeCameraDataFromServerThread() //{ */

bool UFisheyeCamera::GetFisheyeCameraDataFromServerThread(TArray<uint8>& OutArray, double& stamp)
{
	camera_last_request_time_ = FPlatformTime::Seconds();

	CameraBufferCriticalSection->Lock();
	const bool bHaveFrame = CameraRendered;
	CameraBufferCriticalSection->Unlock();

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND || (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES && !bHaveFrame)) {
		const uint64 Baseline = FrameCounter.load();

		auto Instruction      = std::make_shared<FInstruction<UFisheyeCamera>>();
		Instruction->Function = [](UFisheyeCamera& _UFisheyeCamera) { _UFisheyeCamera.UpdateCamera(true, FPlatformTime::Seconds()); };
		InstructionQueue->Enqueue(Instruction);

		if (!WaitForNewFrame(Baseline, 2.0)) {
			UE_LOG(LogTemp, Warning, TEXT("UFisheyeCamera: timed out waiting for a frame"));
		}
	}

	TArray<TArray<FColor>> Faces;
	int32                  Size;
	TSharedPtr<const TArray<FFisheyeLUTEntry>, ESPMode::ThreadSafe> LocalLUT;
	FIntPoint              OutSize;
	{
		CameraBufferCriticalSection->Lock();

		if (!CameraRendered || !LUT.IsValid()) {
			CameraBufferCriticalSection->Unlock();
			UE_LOG(LogTemp, Warning, TEXT("UFisheyeCamera: camera is not rendered, returning"));
			return false;
		}

		Faces    = FaceBuffers;
		Size     = PublishedFaceSize;
		LocalLUT = LUT;
		OutSize  = LUTSize;
		stamp    = stamp_;

		CameraBufferCriticalSection->Unlock();
	}

	if (OutSize.X <= 0 || OutSize.Y <= 0 || LocalLUT->Num() != OutSize.X * OutSize.Y)
	{
		return false;
	}

	// bilinear LUT resample from the five faces, outside every lock
	TArray<FColor> OutImage;
	OutImage.SetNumUninitialized(OutSize.X * OutSize.Y);

	ParallelFor(OutSize.Y, [&](int32 py) {
		for (int32 px = 0; px < OutSize.X; ++px)
		{
			const int32             Index = py * OutSize.X + px;
			const FFisheyeLUTEntry& Entry = (*LocalLUT)[Index];

			if (Entry.Face < 0 || Faces[Entry.Face].Num() != Size * Size)
			{
				OutImage[Index] = FColor::Black;
				continue;
			}

			const TArray<FColor>& FaceData = Faces[Entry.Face];

			const int32 X0 = FMath::Clamp((int32)Entry.FX, 0, Size - 1);
			const int32 Y0 = FMath::Clamp((int32)Entry.FY, 0, Size - 1);
			const int32 X1 = FMath::Min(X0 + 1, Size - 1);
			const int32 Y1 = FMath::Min(Y0 + 1, Size - 1);

			const float FracX = FMath::Clamp(Entry.FX - X0, 0.0f, 1.0f);
			const float FracY = FMath::Clamp(Entry.FY - Y0, 0.0f, 1.0f);

			const FColor& C00 = FaceData[Y0 * Size + X0];
			const FColor& C10 = FaceData[Y0 * Size + X1];
			const FColor& C01 = FaceData[Y1 * Size + X0];
			const FColor& C11 = FaceData[Y1 * Size + X1];

			auto Lerp2 = [&](uint8 A, uint8 B, uint8 C, uint8 D) -> uint8 {
				const float Top = A + (B - A) * FracX;
				const float Bot = C + (D - C) * FracX;
				return (uint8)FMath::RoundToInt(Top + (Bot - Top) * FracY);
			};

			OutImage[Index] = FColor(Lerp2(C00.R, C10.R, C01.R, C11.R), Lerp2(C00.G, C10.G, C01.G, C11.G), Lerp2(C00.B, C10.B, C01.B, C11.B), 255);
		}
	});

	CompressFrame(OutImage, OutSize, fisheye_camera_config_.Compression, false, OutArray);

	return true;
}

//}

/* SetFisheyeCameraConfig() //{ */

bool UFisheyeCamera::SetFisheyeCameraConfig(const FFisheyeCameraConfig& Config)
{
	if (!ValidDimensions(Config.Width, Config.Height))
	{
		return false;
	}

	fisheye_camera_config_ = Config;
	CaptureRate    = Config.CaptureRate;
	NextCaptureDue = -1.0;
	if (bStreamOn) {
		SetCaptureEveryFrame(CaptureRate <= 0.0);
	}

	CameraBufferCriticalSection->Lock();

	SceneCaptureMeshHolder->SetVisibility(Config.ShowCameraComponent);
	SceneCaptureMeshHolder->SetRelativeLocation(Config.Offset);
	SceneCaptureMeshHolder->SetRelativeRotation(FRotator(0, 0, 0));
	SceneCaptureMeshHolder->SetRelativeRotation(Config.Orientation);

	CameraRendered = false;

	CameraBufferCriticalSection->Unlock();

	const int32 Size = FaceSize();

	for (int32 Face = 0; Face < NumFaces; ++Face)
	{
		if (FaceRenderTargets.IsValidIndex(Face) && FaceRenderTargets[Face])
		{
			FaceRenderTargets[Face]->ResizeTarget(Size, Size);
		}
	}

	RebuildLUT();

	return true;
}

//}

/* GetConfig() / SetConfig() //{ */

void UFisheyeCamera::GetConfig(std::stringstream& OutputStream)
{
	Serializable::Drone::GetFisheyeCameraConfig::Response Response(true);
	Response.config = Serializable::Drone::FisheyeCameraConfig{};

	Response.config.show_debug_camera_ = fisheye_camera_config_.ShowCameraComponent;

	Response.config.offset_x_ = fisheye_camera_config_.Offset.X;
	Response.config.offset_y_ = fisheye_camera_config_.Offset.Y;
	Response.config.offset_z_ = fisheye_camera_config_.Offset.Z;

	Response.config.rotation_pitch_ = fisheye_camera_config_.Orientation.Pitch;
	Response.config.rotation_yaw_   = fisheye_camera_config_.Orientation.Yaw;
	Response.config.rotation_roll_  = fisheye_camera_config_.Orientation.Roll;

	Response.config.fov_        = fisheye_camera_config_.FOVAngle;
	Response.config.width_      = fisheye_camera_config_.Width;
	Response.config.capture_rate_ = fisheye_camera_config_.CaptureRate;
	Response.config.height_     = fisheye_camera_config_.Height;
	Response.config.lens_model_ = static_cast<int>(fisheye_camera_config_.LensModel);
	Response.config.full_frame_ = fisheye_camera_config_.FullFrame;

	const FCameraIntrinsics Effective          = EffectiveFisheyeIntrinsics(fisheye_camera_config_);
	Response.config.intrinsics_.use_custom_ = fisheye_camera_config_.Intrinsics.UseCustom;
	Response.config.intrinsics_.fx_         = Effective.Fx;
	Response.config.intrinsics_.fy_         = Effective.Fy;
	Response.config.intrinsics_.cx_         = Effective.Cx - WirePixelShift;
	Response.config.intrinsics_.cy_         = Effective.Cy - WirePixelShift;
	Response.config.kb_k1_                  = fisheye_camera_config_.K1;
	Response.config.kb_k2_                  = fisheye_camera_config_.K2;
	Response.config.kb_k3_                  = fisheye_camera_config_.K3;
	Response.config.kb_k4_                  = fisheye_camera_config_.K4;

	Response.config.compression_.format_ =
	    fisheye_camera_config_.Compression.bJpeg ? Serializable::Drone::CameraCompression::JPEG : Serializable::Drone::CameraCompression::PNG;
	Response.config.compression_.quality_ = fisheye_camera_config_.Compression.Quality;

	Serialization::DeserializeResponse(Response, OutputStream);
}

void UFisheyeCamera::SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream)
{
	Serializable::Drone::SetFisheyeCameraConfig::Request Request;
	Serialization::SerializeRequest(Request, *InputStream);

	FFisheyeCameraConfig Config;
	Config.ShowCameraComponent = Request.config.show_debug_camera_;

	Config.Offset      = FVector(Request.config.offset_x_, Request.config.offset_y_, Request.config.offset_z_);
	Config.Orientation = FRotator(Request.config.rotation_pitch_, Request.config.rotation_yaw_, Request.config.rotation_roll_);

	Config.FOVAngle  = FMath::Clamp(Request.config.fov_, 10.0, 220.0);
	Config.Width     = Request.config.width_;
	Config.CaptureRate = Request.config.capture_rate_;
	Config.Height    = Request.config.height_;
	Config.LensModel = static_cast<EFisheyeLensModel>(FMath::Clamp(Request.config.lens_model_, 0, 3));
	Config.FullFrame = Request.config.full_frame_;

	Config.Intrinsics.UseCustom = Request.config.intrinsics_.use_custom_;
	Config.Intrinsics.Fx        = Request.config.intrinsics_.fx_;
	Config.Intrinsics.Fy        = Request.config.intrinsics_.fy_;
	Config.Intrinsics.Cx        = Request.config.intrinsics_.cx_ + WirePixelShift;
	Config.Intrinsics.Cy        = Request.config.intrinsics_.cy_ + WirePixelShift;
	Config.K1                   = Request.config.kb_k1_;
	Config.K2                   = Request.config.kb_k2_;
	Config.K3                   = Request.config.kb_k3_;
	Config.K4                   = Request.config.kb_k4_;

	Config.Compression.bJpeg   = Request.config.compression_.format_ != Serializable::Drone::CameraCompression::PNG;
	Config.Compression.Quality = Request.config.compression_.quality_;

	bool Status = false;

	auto Instruction      = std::make_shared<FInstruction<UFisheyeCamera>>();
	Instruction->Function = [&Config, &Status](UFisheyeCamera& _UFisheyeCamera)
	{
		Status = _UFisheyeCamera.SetFisheyeCameraConfig(Config);
	};
	InstructionQueue->Enqueue(Instruction);

	FGenericPlatformProcess::ConditionalSleep([Instruction](){return Instruction->Finished;});

	Serializable::Drone::SetFisheyeCameraConfig::Response Response(Status);
	Serialization::DeserializeResponse(Response, OutputStream);
}

//}

/* GetData() //{ */

void UFisheyeCamera::GetData(std::stringstream& OutputStream)
{
	TArray<uint8> CompressedBitmap;
	double stamp;
	bool res = GetFisheyeCameraDataFromServerThread(CompressedBitmap, stamp);

	if (!res) {
		Serializable::Drone::GetFisheyeCameraData::Response Response(false);
		Serialization::DeserializeResponse(Response, OutputStream);
		return;
	}

	Serializable::Drone::GetFisheyeCameraData::Response Response(true);

	Response.image_.assign(CompressedBitmap.GetData(), CompressedBitmap.GetData() + CompressedBitmap.Num());
	Response.stamp_ = stamp;

	Serialization::DeserializeResponse(Response, OutputStream);
}

//}
