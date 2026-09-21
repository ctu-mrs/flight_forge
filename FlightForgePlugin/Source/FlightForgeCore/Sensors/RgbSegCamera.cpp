#include "RgbSegCamera.h"

#include "HAL/IConsoleManager.h"
#include "Materials/Material.h"
#include "Materials/MaterialRenderProxy.h"
#include "TextureResource.h"

#include "Serialize.h"
#include "FlightForgeCore/DronePawn.h"

URgbSegCamera::URgbSegCamera() : UCamera()
{
	SensorType = SensorType::RGB_SEG_CAMERA;
	Name = TEXT("RGB_SEG_CAMERA");

	InstructionQueue = std::make_unique<TQueue<std::shared_ptr<FInstruction<URgbSegCamera>>>>();
}


void URgbSegCamera::BeginPlay()
{
	Super::BeginPlay();
	

	RenderTarget2D = NewObject<UTextureRenderTarget2D>();
	RenderTarget2D->InitCustomFormat(640, 480, PF_B8G8R8A8, false);
	RenderTarget2D->RenderTargetFormat = RTF_RGBA8;
	RenderTarget2D->bGPUSharedFlag     = true;
	
	SceneCaptureComponent2D->CaptureSource = SCS_FinalColorHDR;
	SceneCaptureComponent2D->TextureTarget = RenderTarget2D;
	SceneCaptureComponent2D->ShowFlags.SetTemporalAA(true);
	SceneCaptureComponent2D->bAlwaysPersistRenderingState = true;
	SceneCaptureComponent2D->bCaptureEveryFrame           = false;
	SceneCaptureComponent2D->bCaptureOnMovement           = false;
	SceneCaptureComponent2D->bUseRayTracingIfEnabled      = false;
	// PostProcessMaterial is an EditAnywhere property, which only a Blueprint-placed
	// component would fill in; sensors are built at runtime, so load the plugin's own
	// material here. Without it the capture is an ordinary render, not a mask.
	if (!PostProcessMaterial)
	{
		PostProcessMaterial = LoadObject<UMaterial>(nullptr, TEXT("/FlightForgePlugin/PP_segmentationMat.PP_segmentationMat"));
	}

	if (PostProcessMaterial)
	{
		SceneCaptureComponent2D->AddOrUpdateBlendable(PostProcessMaterial, 1);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("URgbSegCamera: could not load PP_segmentationMat; this camera will return an unlabelled scene render"));
	}

	// Stencil segmentation needs r.CustomDepth=3 ("with stencil"); at 1 the stencil buffer
	// is never written, SetCustomDepthStencilValue does nothing, and this camera quietly
	// returns an ordinary render instead of a mask.
	if (static IConsoleVariable* CustomDepth = IConsoleManager::Get().FindConsoleVariable(TEXT("r.CustomDepth")); CustomDepth && CustomDepth->GetInt() != 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("URgbSegCamera: r.CustomDepth is %d, needs 3 (Custom Depth-Stencil Pass = 'Enabled with Stencil'); "
		                              "segmentation output will be an unlabelled scene render"),
		       CustomDepth->GetInt());
	}

	rgb_camera_config_.ShowCameraComponent    = false;
	rgb_camera_config_.Offset                 = FVector(0, 0, 0);
	rgb_camera_config_.Orientation            = FRotator(0, 0, 0);
	rgb_camera_config_.FOVAngle               = 90;
	rgb_camera_config_.Width                  = 640;
	rgb_camera_config_.Height                 = 480;
	rgb_camera_config_.enable_motion_blur     = true;
	rgb_camera_config_.motion_blur_amount     = 1.0;
	rgb_camera_config_.motion_blur_distortion = 50.0;
	
	SetRgbSegCameraConfig(rgb_camera_config_);
	SetCameraCaptureMode(Owner->GetCameraCaptureMode());
	
}


void URgbSegCamera::DrainInstructions()
{
	std::shared_ptr<FInstruction<URgbSegCamera>> Instruction;
	while (InstructionQueue->Dequeue(Instruction))
	{
		Instruction->Function(*this);
		Instruction->Finished = true;
	}
}

void URgbSegCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	auto start = FPlatformTime::Seconds();

	DrainInstructions();
	
	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES)
	{
		if ((FPlatformTime::Seconds() - camera_last_request_time_) > 1.0) {
		  if (SceneCaptureComponent2D->bCaptureEveryFrame) {
		    SceneCaptureComponent2D->bCaptureEveryFrame = false;
		    CameraBufferCriticalSection->Lock();
		    CameraRendered = false;
		    CameraBufferCriticalSection->Unlock();
		    UE_LOG(LogTemp, Warning, TEXT("Turning off RGB segmented capture"));
		  }
		} else {
		  if (!SceneCaptureComponent2D->bCaptureEveryFrame) {
		    SceneCaptureComponent2D->bCaptureEveryFrame = true;
		    UE_LOG(LogTemp, Warning, TEXT("Turning on RGB segmented capture"));
		  } else {
		    if (ShouldCapture())
		    {
		    	UpdateCamera(true, start);
		    }
		  }
		}
	}

	if (bReadbackDue)
	{
		bReadbackDue = false;
		SetPendingReadback(EnqueueReadback(RenderTarget2D, DeferredStamp));
	}

	PumpReadback();

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool URgbSegCamera::SetRgbSegCameraConfig(const FRgbCameraConfig& Config)
{
	UE_LOG(LogTemp, Log, TEXT("URgbSegCamera::SetRgbSegCameraConfig"));

	if (!ValidDimensions(Config.Width, Config.Height))
	{
		return false;
	}

	rgb_camera_config_ = Config;

	CameraBufferCriticalSection->Lock();

	SceneCaptureMeshHolder->SetVisibility(Config.ShowCameraComponent);
	SceneCaptureMeshHolder->SetRelativeLocation(Config.Offset);
	SceneCaptureMeshHolder->SetRelativeRotation(Config.Orientation);

	SetupCaptureProjection(Config.Intrinsics, Config.Distortion, Config.FOVAngle, Config.Width, Config.Height, {SceneCaptureComponent2D}, {RenderTarget2D});

	SceneCaptureComponent2D->TextureTarget                = RenderTarget2D;
	SceneCaptureComponent2D->bAlwaysPersistRenderingState = true;
	SceneCaptureComponent2D->bCaptureEveryFrame           = false;
	SceneCaptureComponent2D->bCaptureOnMovement           = false;
	SceneCaptureComponent2D->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);

	// Re-assert the segmentation material: anything that assigns PostProcessSettings
	// wholesale (a level post-process baseline, for one) replaces WeightedBlendables and
	// silently turns this back into an ordinary colour camera.
	if (PostProcessMaterial)
	{
		SceneCaptureComponent2D->AddOrUpdateBlendable(PostProcessMaterial, 1);
	}

	UE_LOG(LogTemp, Log, TEXT("URgbSegCamera: %d blendable(s) on the capture"), SceneCaptureComponent2D->PostProcessSettings.WeightedBlendables.Array.Num());

	CameraBufferCriticalSection->Unlock();
	
	return true;
}

// game thread; starts the capture and hands the pixel transfer to the GPU
// readback pipeline instead of stalling on ReadPixels()
void URgbSegCamera::UpdateCamera(bool isExternallyLocked, double stamp)
{
	if (HasPendingReadback()) {
		return;  // previous transfer still in flight
	}

	// While the blendable's shaders are still compiling a capture renders without it,
	// handing the client a photograph labelled as a mask. Only the render thread knows
	// when the compiled shaders are actually in use, so ask it and wait for the answer.
	if (PostProcessMaterial && !BlendableLive->load()) {
		const FMaterialRenderProxy*  Proxy        = PostProcessMaterial->GetRenderProxy();
		const ERHIFeatureLevel::Type FeatureLevel = GetWorld()->GetFeatureLevel();

		ENQUEUE_RENDER_COMMAND(FlightForgeSegBlendableProbe)(
			[Proxy, FeatureLevel, Flag = BlendableLive](FRHICommandListImmediate&)
			{
				if (Proxy && Proxy->GetMaterialNoFallback(FeatureLevel))
				{
					Flag->store(true);
				}
			});

		return;
	}

	SceneCaptureComponent2D->CaptureSource = SCS_FinalColorLDR;
	SceneCaptureComponent2D->TextureTarget = RenderTarget2D;

	// the capture's first renders see an empty custom-stencil input, and the mask material
	// passes scene colour through for unlabelled pixels - photographs; render and discard
	if (WarmupCapturesRemaining > 0)
	{
		--WarmupCapturesRemaining;
		SceneCaptureComponent2D->CaptureScene();
		return;
	}

	if (bReadbackDue)
	{
		return;
	}

	if (!SceneCaptureComponent2D->bCaptureEveryFrame && !bEverRendered)
	{
		SceneCaptureComponent2D->CaptureScene();
	}
	else if (!SceneCaptureComponent2D->bCaptureEveryFrame)
	{
		SceneCaptureComponent2D->CaptureSceneDeferred();
		DeferredStamp = stamp;
		bReadbackDue  = true;
		return;
	}

	SetPendingReadback(EnqueueReadback(RenderTarget2D, stamp));
}

bool URgbSegCamera::GetRgbSegCameraFromServerThread(TArray<uint8>& OutArray, double& stamp) {

	camera_last_request_time_ = FPlatformTime::Seconds();

	CameraBufferCriticalSection->Lock();
	const bool bHaveFrame = CameraRendered;
	CameraBufferCriticalSection->Unlock();

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND && !SceneCaptureComponent2D->bCaptureEveryFrame && WarmupCapturesRemaining <= 0 && CachedTargetResource.load() && bEverRendered) {
		if (!RequestFrameFromServerThread(RenderTarget2D, FPlatformTime::Seconds(), FVector::ZeroVector, false, false, false, 2.0)) {
			UE_LOG(LogTemp, Warning, TEXT("URgbSegCamera: timed out waiting for a frame"));
		}
	} else if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND || !bHaveFrame) {
		// the blendable probe and the warm-up captures each take a request of their own
		bool bGot = false;
		for (int32 Attempt = 0; Attempt < 8 && !bGot; ++Attempt) {
			const uint64 Baseline = FrameCounter.load();

			auto Instruction      = std::make_shared<FInstruction<URgbSegCamera>>();
			Instruction->Function = [](URgbSegCamera& _URgbSegCamera) { _URgbSegCamera.UpdateCamera(true, FPlatformTime::Seconds()); };
			InstructionQueue->Enqueue(Instruction);

			bGot = WaitForRequestedFrame(Instruction->Finished, Baseline, 2.0);
		}
		if (!bGot) {
			UE_LOG(LogTemp, Warning, TEXT("URgbSegCamera: timed out waiting for a frame"));
		}
	}

	TArray<FColor>                                        RawCopy;
	FIntPoint                                             RawSize;
	TSharedPtr<const FDistortionLUT, ESPMode::ThreadSafe> LUT;
	{
		CameraBufferCriticalSection->Lock();

		if (!CameraRendered) {
			CameraBufferCriticalSection->Unlock();
			UE_LOG(LogTemp, Warning, TEXT("URgbSegCamera: camera is not rendered, returning"));
			return false;
		}

		RawCopy = CameraBuffer;
		RawSize = CameraBufferSize;
		stamp   = stamp_;
		LUT     = DistortionLUT;

		CameraBufferCriticalSection->Unlock();
	}

	// nearest-neighbor so class labels never blend across the warp
	if (LUT && RawSize == LUT->SourceSize) {
		TArray<FColor> Warped;
		ResampleImage(*LUT, RawCopy, Warped, true);
		RawCopy = MoveTemp(Warped);
		RawSize = LUT->OutputSize;
	}

	TransformImageArray(RawSize.X, RawSize.Y, RawCopy, OutArray);

	return true;
}

void URgbSegCamera::GetConfig(std::stringstream& OutputStream)
{
	Serializable::Drone::GetRgbCameraConfig::Response ResponseRGB(true);
	ResponseRGB.config = Serializable::Drone::RgbCameraConfig{};

	ResponseRGB.config.show_debug_camera_= rgb_camera_config_.ShowCameraComponent;

	ResponseRGB.config.offset_x_ = rgb_camera_config_.Offset.X;
	ResponseRGB.config.offset_y_ = rgb_camera_config_.Offset.Y;
	ResponseRGB.config.offset_z_ = rgb_camera_config_.Offset.Z;

	ResponseRGB.config.rotation_pitch_ = rgb_camera_config_.Orientation.Pitch;
	ResponseRGB.config.rotation_yaw_ = rgb_camera_config_.Orientation.Yaw;
	ResponseRGB.config.rotation_roll_ = rgb_camera_config_.Orientation.Roll;

	ResponseRGB.config.fov_ = rgb_camera_config_.FOVAngle;

	ResponseRGB.config.width_ = rgb_camera_config_.Width;
	ResponseRGB.config.height_ = rgb_camera_config_.Height;

	ResponseRGB.config.enable_temporal_aa_ = rgb_camera_config_.enable_temporal_aa;
	ResponseRGB.config.enable_hdr_ = rgb_camera_config_.enable_hdr;
	ResponseRGB.config.enable_raytracing_ = rgb_camera_config_.enable_raytracing;

	ResponseRGB.config.enable_motion_blur_ = rgb_camera_config_.enable_motion_blur;
	ResponseRGB.config.motion_blur_distortion_ = rgb_camera_config_.motion_blur_distortion;
	ResponseRGB.config.motion_blur_amount_ = rgb_camera_config_.motion_blur_amount;

	const FCameraIntrinsics Effective =
	    EffectiveIntrinsics(rgb_camera_config_.Intrinsics, rgb_camera_config_.FOVAngle, rgb_camera_config_.Width, rgb_camera_config_.Height);
	ResponseRGB.config.intrinsics_.use_custom_ = rgb_camera_config_.Intrinsics.UseCustom;
	ResponseRGB.config.intrinsics_.fx_         = Effective.Fx;
	ResponseRGB.config.intrinsics_.fy_         = Effective.Fy;
	ResponseRGB.config.intrinsics_.cx_         = Effective.Cx - WirePixelShift;
	ResponseRGB.config.intrinsics_.cy_         = Effective.Cy - WirePixelShift;

	ResponseRGB.config.distortion_.enable_ = rgb_camera_config_.Distortion.Enable;
	ResponseRGB.config.distortion_.k1_     = rgb_camera_config_.Distortion.K1;
	ResponseRGB.config.distortion_.k2_     = rgb_camera_config_.Distortion.K2;
	ResponseRGB.config.distortion_.k3_     = rgb_camera_config_.Distortion.K3;
	ResponseRGB.config.distortion_.p1_     = rgb_camera_config_.Distortion.P1;
	ResponseRGB.config.distortion_.p2_     = rgb_camera_config_.Distortion.P2;

	Serialization::DeserializeResponse(ResponseRGB, OutputStream);
	UE_LOG(LogTemp, Log, TEXT("Got CameraRGBSeg config"));
}

void URgbSegCamera::SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream)
{
	Serializable::Drone::SetRgbCameraConfig::Request Request;
	Serialization::SerializeRequest(Request, *InputStream);
	
	FRgbCameraConfig Config;
	Config.ShowCameraComponent = Request.config.show_debug_camera_;

	Config.Offset = FVector(Request.config.offset_x_, Request.config.offset_y_, Request.config.offset_z_);
	Config.Orientation = FRotator(Request.config.rotation_pitch_, Request.config.rotation_yaw_, Request.config.rotation_roll_);

	Config.FOVAngle = Request.config.fov_;

	Config.Width = Request.config.width_;
	Config.Height = Request.config.height_;

	Config.enable_hdr = Request.config.enable_hdr_;
	Config.enable_temporal_aa = Request.config.enable_temporal_aa_;
	Config.enable_raytracing = Request.config.enable_raytracing_;

	Config.enable_motion_blur = Request.config.enable_motion_blur_;
	Config.motion_blur_amount = Request.config.motion_blur_amount_;
	Config.motion_blur_distortion = Request.config.motion_blur_distortion_;

	Config.Intrinsics.UseCustom = Request.config.intrinsics_.use_custom_;
	Config.Intrinsics.Fx        = Request.config.intrinsics_.fx_;
	Config.Intrinsics.Fy        = Request.config.intrinsics_.fy_;
	Config.Intrinsics.Cx        = Request.config.intrinsics_.cx_ + WirePixelShift;
	Config.Intrinsics.Cy        = Request.config.intrinsics_.cy_ + WirePixelShift;

	Config.Distortion.Enable = Request.config.distortion_.enable_;
	Config.Distortion.K1     = Request.config.distortion_.k1_;
	Config.Distortion.K2     = Request.config.distortion_.k2_;
	Config.Distortion.K3     = Request.config.distortion_.k3_;
	Config.Distortion.P1     = Request.config.distortion_.p1_;
	Config.Distortion.P2     = Request.config.distortion_.p2_;

	bool Status = false;

	// auto Instruction = std::make_shared<FInstruction<ADronePawn>>();
	//
	// Instruction->Function = [ID = this->SensorID, &Config, &Status](ADronePawn& _ADronePawn) {
	// 	Status = _ADronePawn.SetRgbCameraConfig(Config, ID); //wrong
	// };
	//
	// Owner->InstructionQueue->Enqueue(Instruction);

	auto Instruction      = std::make_shared<FInstruction<URgbSegCamera>>();
	Instruction->Function = [&Config, &Status](URgbSegCamera& _URgbSegCamera)
	{
		Status = _URgbSegCamera.SetRgbSegCameraConfig(Config);
	};
	InstructionQueue->Enqueue(Instruction);
		
	FGenericPlatformProcess::ConditionalSleep([Instruction](){return Instruction->Finished;});

	Serializable::Drone::SetRgbCameraConfig::Response Response(Status);
	Serialization::DeserializeResponse(Response, OutputStream);
	UE_LOG(LogTemp, Log, TEXT("Set RGBSegCamera config"));
}

void URgbSegCamera::GetData(std::stringstream& OutputStream)
{
	TArray<uint8> CompressedBitmap;
	double stamp;
	bool res = GetRgbSegCameraFromServerThread(CompressedBitmap, stamp);

	if (!res) {
		Serializable::Drone::GetRgbSegCameraData::Response Response(false);

		Serialization::DeserializeResponse(Response, OutputStream);
		return;
	}

	Serializable::Drone::GetRgbSegCameraData::Response Response(true);
	Response.image_ = std::vector<unsigned char>(CompressedBitmap.Num());

	for(int i = 0; i < CompressedBitmap.Num(); i++) {
		Response.image_[i] = CompressedBitmap[i];
	}

	Response.stamp_ = stamp;

	Serialization::DeserializeResponse(Response, OutputStream);
}
