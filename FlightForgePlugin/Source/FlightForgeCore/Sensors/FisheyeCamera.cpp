#include "FisheyeCamera.h"

#include "Async/ParallelFor.h"
#include "TextureResource.h"

#include "Serialize.h"
#include "FlightForgeCore/DronePawn.h"

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

	// base SceneCaptureComponent2D is face 0 (forward)
	FaceCaptures.SetNum(NumFaces);
	FaceCaptures[0] = SceneCaptureComponent2D;

	for (int32 Face = 1; Face < NumFaces; ++Face)
	{
		FaceCaptures[Face] = CreateDefaultSubobject<USceneCaptureComponent2D>(*FString::Printf(TEXT("FisheyeFaceCapture%d"), Face));
		FaceCaptures[Face]->SetupAttachment(SceneCaptureMeshHolder);
	}

	FaceBuffers.SetNum(NumFaces);
	PendingFaces.SetNum(NumFaces);
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
	fisheye_camera_config_.Offset              = FVector(0, 0, 0);
	fisheye_camera_config_.Orientation         = FRotator(0, 0, 0);
	fisheye_camera_config_.FOVAngle            = 180;
	fisheye_camera_config_.Width               = 640;
	fisheye_camera_config_.Height              = 640;
	fisheye_camera_config_.LensModel           = EFisheyeLensModel::EQUIDISTANT;

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

/* RebuildLUT() //{ */

void UFisheyeCamera::RebuildLUT()
{
	const int32 W = fisheye_camera_config_.Width;
	const int32 H = fisheye_camera_config_.Height;

	if (W <= 0 || H <= 0)
	{
		return;
	}

	const float HalfFov     = FMath::DegreesToRadians((float)FMath::Clamp(fisheye_camera_config_.FOVAngle, 10.0, 220.0)) * 0.5f;
	const float TanHalfFace = FMath::Tan(FMath::DegreesToRadians(FaceFovDeg) * 0.5f);
	const float ImageRadius = 0.5f * FMath::Min(W, H);
	const int32 Size        = FaceSize();

	const EFisheyeLensModel Model = fisheye_camera_config_.LensModel;

	TSharedPtr<TArray<FFisheyeLUTEntry>, ESPMode::ThreadSafe> NewLUT = MakeShared<TArray<FFisheyeLUTEntry>, ESPMode::ThreadSafe>();
	NewLUT->SetNumUninitialized(W * H);

	ParallelFor(H, [&](int32 py) {
		for (int32 px = 0; px < W; ++px)
		{
			FFisheyeLUTEntry& Entry = (*NewLUT)[py * W + px];
			Entry.Face = -1;

			const float U = (px + 0.5f - 0.5f * W) / ImageRadius;
			const float V = (py + 0.5f - 0.5f * H) / ImageRadius;
			const float R = FMath::Sqrt(U * U + V * V);

			if (R > 1.0f)
			{
				continue;
			}

			float Theta = 0.0f;
			switch (Model)
			{
				case EFisheyeLensModel::EQUISOLID:
					Theta = 2.0f * FMath::Asin(FMath::Clamp(R * FMath::Sin(HalfFov * 0.5f), -1.0f, 1.0f));
					break;
				case EFisheyeLensModel::STEREOGRAPHIC:
					Theta = 2.0f * FMath::Atan(R * FMath::Tan(HalfFov * 0.5f));
					break;
				case EFisheyeLensModel::EQUIDISTANT:
				default:
					Theta = R * HalfFov;
					break;
			}

			const float Phi = FMath::Atan2(V, U);

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

void UFisheyeCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	auto start = FPlatformTime::Seconds();

	std::shared_ptr<FInstruction<UFisheyeCamera>> Instruction;
	while (InstructionQueue->Dequeue(Instruction)) {
		Instruction->Function(*this);
		Instruction->Finished = true;
	}

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES)
	{
		if ((FPlatformTime::Seconds() - camera_last_request_time_) > 1.0) {
			if (SceneCaptureComponent2D->bCaptureEveryFrame) {
				SetCaptureEveryFrame(false);
				CameraBufferCriticalSection->Lock();
				CameraRendered = false;
				CameraBufferCriticalSection->Unlock();
				UE_LOG(LogTemp, Warning, TEXT("Turning off fisheye capture"));
			}
		} else {
			if (!SceneCaptureComponent2D->bCaptureEveryFrame) {
				SetCaptureEveryFrame(true);
				UE_LOG(LogTemp, Warning, TEXT("Turning on fisheye capture"));
			} else {
				UpdateCamera(true, start);
			}
		}
	}

	PumpFisheyeReadback();

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// game thread; captures all five faces and hands the transfers to the GPU
// readback pipeline
void UFisheyeCamera::UpdateCamera(bool isExternallyLocked, double stamp)
{
	for (int32 Face = 0; Face < NumFaces; ++Face)
	{
		if (PendingFaces[Face]) {
			return;  // previous transfer still in flight
		}
	}

	if (!SceneCaptureComponent2D->bCaptureEveryFrame)
	{
		for (int32 Face = 0; Face < NumFaces; ++Face)
		{
			FaceCaptures[Face]->CaptureScene();
		}
	}

	for (int32 Face = 0; Face < NumFaces; ++Face)
	{
		PendingFaces[Face] = EnqueueReadback(FaceRenderTargets[Face], stamp);
	}
}

// publishes only when all five faces have landed, so a frame is seam-consistent
bool UFisheyeCamera::PumpFisheyeReadback()
{
	bool bAnyPending = false;
	bool bAllCopied  = true;

	for (int32 Face = 0; Face < NumFaces; ++Face)
	{
		if (!PendingFaces[Face])
		{
			bAllCopied = false;
			continue;
		}

		bAnyPending = true;

		if (PendingFaces[Face]->bCopied)
		{
			continue;
		}

		bAllCopied = false;

		if (PendingFaces[Face]->Readback && PendingFaces[Face]->Readback->IsReady() && !PendingFaces[Face]->bResolveIssued.exchange(true))
		{
			IssueResolve(PendingFaces[Face]);
		}
	}

	if (!bAnyPending || !bAllCopied)
	{
		return false;
	}

	CameraBufferCriticalSection->Lock();

	bool bComplete = true;
	for (int32 Face = 0; Face < NumFaces; ++Face)
	{
		FaceBuffers[Face] = MoveTemp(PendingFaces[Face]->Pixels);
		bComplete &= FaceBuffers[Face].Num() > 0;
	}
	PublishedFaceSize = PendingFaces[0]->Size.X;
	stamp_            = PendingFaces[0]->Stamp;
	CameraRendered    = bComplete;

	CameraBufferCriticalSection->Unlock();

	for (int32 Face = 0; Face < NumFaces; ++Face)
	{
		PendingFaces[Face].Reset();
	}

	NotifyFrameReady();
	return true;
}

//}

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

	FImageUtils::ThumbnailCompressImageArray(OutSize.X, OutSize.Y, OutImage, OutArray);

	return true;
}

//}

/* SetFisheyeCameraConfig() //{ */

bool UFisheyeCamera::SetFisheyeCameraConfig(const FFisheyeCameraConfig& Config)
{
	fisheye_camera_config_ = Config;

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
	Response.config.height_     = fisheye_camera_config_.Height;
	Response.config.lens_model_ = static_cast<int>(fisheye_camera_config_.LensModel);

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
	Config.Height    = Request.config.height_;
	Config.LensModel = static_cast<EFisheyeLensModel>(FMath::Clamp(Request.config.lens_model_, 0, 2));

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
