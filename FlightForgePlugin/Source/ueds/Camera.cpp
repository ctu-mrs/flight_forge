#include "Camera.h"

#include "DronePawn.h"
#include "ImageUtils.h"
#include "Instruction.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"


UCamera::UCamera()
{
	SensorType = SensorType::CAMERA;
	Name = TEXT("Camera");
	
	//maybe change
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup    = TG_PrePhysics;

#if PLATFORM_WINDOWS
	RgbCameraBufferCriticalSection    = std::make_unique<FWindowsCriticalSection>();
	StereoCameraBufferCriticalSection = std::make_unique<FWindowsCriticalSection>();
	RgbSegCameraBufferCriticalSection = std::make_unique<FWindowsCriticalSection>();
#else
	RgbCameraBufferCriticalSection    = std::make_unique<FPThreadsCriticalSection>();
	StereoCameraBufferCriticalSection = std::make_unique<FPThreadsCriticalSection>();
	RgbSegCameraBufferCriticalSection = std::make_unique<FPThreadsCriticalSection>();
#endif

	SceneCaptureMeshHolderRgb = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SceneCaptureMeshHolderRgb"));
	SceneCaptureMeshHolderRgb->SetupAttachment(this);
  
	SceneCaptureMeshHolderStereoLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SceneCaptureMeshHolderStereoLeft"));
	SceneCaptureMeshHolderStereoLeft->SetupAttachment(this);

	SceneCaptureMeshHolderStereoRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SceneCaptureMeshHolderStereoRight"));
	SceneCaptureMeshHolderStereoRight->SetupAttachment(this);

	SceneCaptureComponent2DRgb = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2DRgb"));
	SceneCaptureComponent2DRgb->SetupAttachment(SceneCaptureMeshHolderRgb);

	SceneCaptureComponent2DRgbSeg = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2DRgbSeg"));
	SceneCaptureComponent2DRgbSeg->SetupAttachment(SceneCaptureMeshHolderRgb);

	SceneCaptureComponent2DStereoLeft = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2DStereoLeft"));
	SceneCaptureComponent2DStereoLeft->SetupAttachment(SceneCaptureMeshHolderStereoLeft);

	SceneCaptureComponent2DStereoRight = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2DStereoRight"));
	SceneCaptureComponent2DStereoRight->SetupAttachment(SceneCaptureMeshHolderStereoRight);
}


// Called when the game starts
void UCamera::BeginPlay()
{
	Super::BeginPlay();

	// TODO check https://github.com/TimmHess/UnrealImageCapture
  RenderTarget2DRgb = NewObject<UTextureRenderTarget2D>();
  RenderTarget2DRgb->InitCustomFormat(640, 480, PF_B8G8R8A8, false);
  RenderTarget2DRgb->RenderTargetFormat = RTF_RGBA8;
  RenderTarget2DRgb->bGPUSharedFlag     = true;

  RenderTarget2DStereoLeft = NewObject<UTextureRenderTarget2D>();
  RenderTarget2DStereoLeft->InitCustomFormat(640, 480, PF_B8G8R8A8, false);
  RenderTarget2DStereoLeft->RenderTargetFormat = RTF_RGBA8;
  RenderTarget2DStereoLeft->bGPUSharedFlag     = true;

  RenderTarget2DStereoRight = NewObject<UTextureRenderTarget2D>();
  RenderTarget2DStereoRight->InitCustomFormat(640, 480, PF_B8G8R8A8, false);
  RenderTarget2DStereoRight->RenderTargetFormat = RTF_RGBA8;
  RenderTarget2DStereoRight->bGPUSharedFlag     = true;

  RenderTarget2DRgbSeg = NewObject<UTextureRenderTarget2D>();
  RenderTarget2DRgbSeg->InitCustomFormat(640, 480, PF_B8G8R8A8, false);
  RenderTarget2DRgbSeg->RenderTargetFormat = RTF_RGBA8;
  RenderTarget2DRgbSeg->bGPUSharedFlag     = true;
  
  SceneCaptureComponent2DRgb->CaptureSource = SCS_FinalColorHDR;
  SceneCaptureComponent2DRgb->TextureTarget = RenderTarget2DRgb;
  SceneCaptureComponent2DRgb->ShowFlags.SetTemporalAA(true);
  SceneCaptureComponent2DRgb->bAlwaysPersistRenderingState = true;
  SceneCaptureComponent2DRgb->bCaptureEveryFrame           = false;
  SceneCaptureComponent2DRgb->bCaptureOnMovement           = false;
  SceneCaptureComponent2DRgb->bUseRayTracingIfEnabled      = true;

  SceneCaptureComponent2DRgbSeg->CaptureSource = SCS_FinalColorHDR;
  SceneCaptureComponent2DRgbSeg->TextureTarget = RenderTarget2DRgbSeg;
  SceneCaptureComponent2DRgbSeg->ShowFlags.SetTemporalAA(true);
  SceneCaptureComponent2DRgbSeg->bAlwaysPersistRenderingState = true;
  SceneCaptureComponent2DRgbSeg->bCaptureEveryFrame           = false;
  SceneCaptureComponent2DRgbSeg->bCaptureOnMovement           = false;
  SceneCaptureComponent2DRgbSeg->bUseRayTracingIfEnabled      = false;
  SceneCaptureComponent2DRgbSeg->AddOrUpdateBlendable(PostProcessMaterial, 1);

  SceneCaptureComponent2DStereoLeft->CaptureSource = SCS_FinalColorHDR;
  SceneCaptureComponent2DStereoLeft->TextureTarget = RenderTarget2DStereoLeft;
  SceneCaptureComponent2DStereoLeft->ShowFlags.SetTemporalAA(true);
  SceneCaptureComponent2DStereoLeft->bAlwaysPersistRenderingState = true;
  SceneCaptureComponent2DStereoLeft->bCaptureEveryFrame           = false;
  SceneCaptureComponent2DStereoLeft->bCaptureOnMovement           = false;
  SceneCaptureComponent2DStereoLeft->bUseRayTracingIfEnabled      = true;

  SceneCaptureComponent2DStereoRight->CaptureSource = SCS_FinalColorHDR;
  SceneCaptureComponent2DStereoRight->TextureTarget = RenderTarget2DStereoRight;
  SceneCaptureComponent2DStereoRight->ShowFlags.SetTemporalAA(true);
  SceneCaptureComponent2DStereoRight->bAlwaysPersistRenderingState = true;
  SceneCaptureComponent2DStereoRight->bCaptureEveryFrame           = false;
  SceneCaptureComponent2DStereoRight->bCaptureOnMovement           = false;
  SceneCaptureComponent2DStereoRight->bUseRayTracingIfEnabled      = true;

  rgb_camera_config_.ShowCameraComponent    = false;
  rgb_camera_config_.Offset                 = FVector(0, 0, 0);
  rgb_camera_config_.Orientation            = FRotator(0, 0, 0);
  rgb_camera_config_.FOVAngle               = 90;
  rgb_camera_config_.Width                  = 640;
  rgb_camera_config_.Height                 = 480;
  rgb_camera_config_.enable_motion_blur     = true;
  rgb_camera_config_.motion_blur_amount     = 1.0;
  rgb_camera_config_.motion_blur_distortion = 50.0;

  stereo_camera_config_.ShowCameraComponent = false;
  stereo_camera_config_.Offset              = FVector(0, 0, 0);
  stereo_camera_config_.Orientation         = FRotator(0, 0, 0);
  stereo_camera_config_.FOVAngle            = 90;
  stereo_camera_config_.Width               = 640;
  stereo_camera_config_.Height              = 480;
  stereo_camera_config_.baseline            = 0.1;

  SetRgbCameraConfig(rgb_camera_config_);
  SetStereoCameraConfig(stereo_camera_config_);
}

void UCamera::BeginDestroy()
{
	// Cleanup buffer when component is destroyed
	RgbCameraBuffer.Empty();
	StereoLeftCameraBuffer.Empty();
	StereoRightCameraBuffer.Empty();
	
	Super::BeginDestroy();
}

// Called every frame
void UCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	auto start = FPlatformTime::Seconds();
	
	{
    RgbCameraBufferCriticalSection->Lock();

    if ((FPlatformTime::Seconds() - rgb_camera_last_request_time_) > 1.0) {
      if (SceneCaptureComponent2DRgb->bCaptureEveryFrame) {
        SceneCaptureComponent2DRgb->bCaptureEveryFrame = false;
        RgbCameraRendered                              = false;
        UE_LOG(LogTemp, Warning, TEXT("Turning off RGB capture"));
      }
    } else {
      if (!SceneCaptureComponent2DRgb->bCaptureEveryFrame) {
        SceneCaptureComponent2DRgb->bCaptureEveryFrame = true;
        UE_LOG(LogTemp, Warning, TEXT("Turning on RGB capture"));
      } else {
        UpdateCamera(true, CAMERA_MODE_RGB, start);
      }
    }

    RgbCameraBufferCriticalSection->Unlock();
  }

  {
    RgbSegCameraBufferCriticalSection->Lock();

    if ((FPlatformTime::Seconds() - rgb_seg_camera_last_request_time_) > 1.0) {
      if (SceneCaptureComponent2DRgbSeg->bCaptureEveryFrame) {
        SceneCaptureComponent2DRgbSeg->bCaptureEveryFrame = false;
        RgbSegCameraRendered                              = false;
        UE_LOG(LogTemp, Warning, TEXT("Turning off RGB segmented capture"));
      }
    } else {
      if (!SceneCaptureComponent2DRgbSeg->bCaptureEveryFrame) {
        SceneCaptureComponent2DRgbSeg->bCaptureEveryFrame = true;
        UE_LOG(LogTemp, Warning, TEXT("Turning on RGB segmented capture"));
      } else {
        UpdateCamera(true, CAMERA_MODE_RGB_SEG, start);
      }
    }

    RgbSegCameraBufferCriticalSection->Unlock();
  }

  {
    StereoCameraBufferCriticalSection->Lock();

    if ((FPlatformTime::Seconds() - stereo_camera_last_request_time_) > 1.0) {
      if (SceneCaptureComponent2DStereoLeft->bCaptureEveryFrame) {
        SceneCaptureComponent2DStereoLeft->bCaptureEveryFrame  = false;
        SceneCaptureComponent2DStereoRight->bCaptureEveryFrame = false;
        StereoCameraRendered                                   = false;
        UE_LOG(LogTemp, Warning, TEXT("Turning off Stereo capture"));
      }
    } else {
      if (!SceneCaptureComponent2DStereoLeft->bCaptureEveryFrame) {
        UE_LOG(LogTemp, Warning, TEXT("Turning on Stereo capture"));
        SceneCaptureComponent2DStereoLeft->bCaptureEveryFrame  = true;
        SceneCaptureComponent2DStereoRight->bCaptureEveryFrame = true;
      } else {
        UpdateCamera(true, CAMERA_MODE_STEREO, start);
      }
    }

    StereoCameraBufferCriticalSection->Unlock();
  }
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FRgbCameraConfig UCamera::GetRgbCameraConfig() {
	return rgb_camera_config_;
}

FStereoCameraConfig UCamera::GetStereoCameraConfig() {
	return stereo_camera_config_;
}

void UCamera::UpdateCamera(bool isExternallyLocked, int type = 1, double stamp = 0.0) {

  switch (type) {

    case CAMERA_MODE_NONE: {
      break;
    }

    case CAMERA_MODE_RGB: {

      if (!isExternallyLocked) {
        RgbCameraBufferCriticalSection->Lock();
      }
      const auto Resource = RenderTarget2DRgb->GameThread_GetRenderTargetResource();
      UE_LOG(LogTemp, Warning, TEXT("resource"));


      Resource->ReadPixels(RgbCameraBuffer); //this probably crashes
      UE_LOG(LogTemp, Warning, TEXT("read pixels"));
    
        
      RgbCameraDataNeedsCompress = true;

      rgb_stamp_ = stamp;

      RgbCameraRendered = true;

      if (!isExternallyLocked) {
        RgbCameraBufferCriticalSection->Unlock();
      }
      UE_LOG(LogTemp, Warning, TEXT("break"));
      break;
    }

    case CAMERA_MODE_STEREO: {

      if (!isExternallyLocked) {
        StereoCameraBufferCriticalSection->Lock();
      }

      const auto ResourceLeft = RenderTarget2DStereoLeft->GameThread_GetRenderTargetResource();

      const auto ResourceRight = RenderTarget2DStereoRight->GameThread_GetRenderTargetResource();

      ResourceLeft->ReadPixels(StereoLeftCameraBuffer);
      ResourceRight->ReadPixels(StereoRightCameraBuffer);

      StereoCameraDataNeedsCompress = true;

      StereoCameraRendered = true;

      stereo_stamp_ = stamp;

      if (!isExternallyLocked) {
        StereoCameraBufferCriticalSection->Unlock();
      }

      break;
    }

    case CAMERA_MODE_RGB_SEG: {

      SceneCaptureComponent2DRgbSeg->CaptureSource = SCS_FinalColorLDR;
      SceneCaptureComponent2DRgbSeg->TextureTarget = RenderTarget2DRgbSeg;
      // SceneCaptureComponent2DRgb->CaptureScene();

      if (!isExternallyLocked) {
        RgbSegCameraBufferCriticalSection->Lock();
      }

      const auto ResourceSeg = RenderTarget2DRgbSeg->GameThread_GetRenderTargetResource();

      ResourceSeg->ReadPixels(RgbSegCameraBuffer);

      RgbSegCameraDataNeedsCompress = false;

      RgbSegCameraRendered = true;

      rgb_seg_stamp_ = stamp;

      if (!isExternallyLocked) {
        RgbSegCameraBufferCriticalSection->Unlock();
      }

      break;
    }

    default: {
      SceneCaptureComponent2DRgb->CaptureSource = SCS_FinalColorLDR;
      SceneCaptureComponent2DRgb->TextureTarget = RenderTarget2DRgb;
      break;
    }
  }
}

bool UCamera::GetRgbCameraDataFromServerThread(TArray<uint8>& OutArray, double& stamp) {

	RgbCameraBufferCriticalSection->Lock();

	rgb_camera_last_request_time_ = FPlatformTime::Seconds();

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND) {
		auto Instruction      = std::make_shared<FInstruction<ADronePawn>>();
		Instruction->Function = [](ADronePawn& _DronePawn) { _DronePawn.UpdateCamera(true, CAMERA_MODE_RGB, 0.0); };
		Owner->InstructionQueue->Enqueue(Instruction);

		FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });
	}

	if (!RgbCameraRendered) {
		UE_LOG(LogTemp, Warning, TEXT("DronePawn(): rgb camera is not rendered, returning"));
		RgbCameraBufferCriticalSection->Unlock();
		return false;
	}

	if (RgbCameraDataNeedsCompress) {
		FImageUtils::ThumbnailCompressImageArray(RenderTarget2DRgb->SizeX, RenderTarget2DRgb->SizeY, RgbCameraBuffer, *CompressedRgbCameraData);
		RgbCameraDataNeedsCompress = false;
	}

	const auto Size = (*CompressedRgbCameraData).Num();
	OutArray.SetNumUninitialized(Size);
	FMemory::Memcpy(OutArray.GetData(), (*CompressedRgbCameraData).GetData(), Size * sizeof(uint8));

	stamp = rgb_stamp_;

	RgbCameraBufferCriticalSection->Unlock();

	return true;
}

bool UCamera::GetStereoCameraDataFromServerThread(TArray<uint8>& image_left, TArray<uint8>& image_right, double& stamp) {

	{
		StereoCameraBufferCriticalSection->Lock();

		stereo_camera_last_request_time_ = FPlatformTime::Seconds();

		if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND) {
			auto Instruction      = std::make_shared<FInstruction<ADronePawn>>();
			Instruction->Function = [](ADronePawn& _DronePawn) { _DronePawn.UpdateCamera(true, CAMERA_MODE_STEREO, 0.0); };
			Owner->InstructionQueue->Enqueue(Instruction);
			FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });
		}

		if (!StereoCameraRendered) {
			UE_LOG(LogTemp, Warning, TEXT("DronePawn(): stereo camera is not rendered, returning"));
			StereoCameraBufferCriticalSection->Unlock();
			return false;
		}

		if (StereoCameraDataNeedsCompress) {

			FImageUtils::ThumbnailCompressImageArray(RenderTarget2DStereoLeft->SizeX, RenderTarget2DStereoLeft->SizeY, StereoLeftCameraBuffer,
													 *CompressedStereoLeftCameraData);
			FImageUtils::ThumbnailCompressImageArray(RenderTarget2DStereoRight->SizeX, RenderTarget2DStereoRight->SizeY, StereoRightCameraBuffer,
													 *CompressedStereoRightCameraData);
			StereoCameraDataNeedsCompress = false;
		}

		{
			const auto Size = (*CompressedStereoLeftCameraData).Num();
			image_left.SetNumUninitialized(Size);
			FMemory::Memcpy(image_left.GetData(), (*CompressedStereoLeftCameraData).GetData(), Size * sizeof(uint8));
		}

		{
			const auto Size = (*CompressedStereoRightCameraData).Num();
			image_right.SetNumUninitialized(Size);
			FMemory::Memcpy(image_right.GetData(), (*CompressedStereoRightCameraData).GetData(), Size * sizeof(uint8));
		}

		stamp = stereo_stamp_;

		StereoCameraBufferCriticalSection->Unlock();

		return true;
	}
}

bool UCamera::GetRgbSegCameraFromServerThread(TArray<uint8>& OutArray, double& stamp) {

	RgbSegCameraBufferCriticalSection->Lock();

	rgb_seg_camera_last_request_time_ = FPlatformTime::Seconds();

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_DEMAND) {
		auto Instruction      = std::make_shared<FInstruction<ADronePawn>>();
		Instruction->Function = [](ADronePawn& _DronePawn) { _DronePawn.UpdateCamera(true, CAMERA_MODE_RGB_SEG, 0.0); };
		Owner->InstructionQueue->Enqueue(Instruction);
		FGenericPlatformProcess::ConditionalSleep([Instruction]() { return Instruction->Finished; });
	}

	if (!RgbSegCameraRendered) {
		UE_LOG(LogTemp, Warning, TEXT("DronePawn(): rgb seg camera is not rendered, returning"));
		RgbSegCameraBufferCriticalSection->Unlock();
		return false;
	}

	/* if (RgbSegCameraDataNeedsCompress) { */
	/*   FImageUtils::ThumbnailCompressImageArray(RenderTarget2DRgbSeg->SizeX, RenderTarget2DRgbSeg->SizeY, RgbSegCameraBuffer, *CompressedRgbSegCameraData); */
	/*   RgbSegCameraDataNeedsCompress = false; */
	/* } */

	auto byteArray = MakeUnique<TArray<uint8>>();
	TransformImageArray(RenderTarget2DRgbSeg->SizeX, RenderTarget2DRgbSeg->SizeY, RgbSegCameraBuffer, *byteArray);

	const auto Size = (*byteArray).Num();
	OutArray.SetNumUninitialized(Size);
	FMemory::Memcpy(OutArray.GetData(), (*byteArray).GetData(), Size * sizeof(uint8));

	RgbSegCameraBufferCriticalSection->Unlock();

	stamp = rgb_seg_stamp_;

	return true;
}

bool UCamera::SetRgbCameraConfig(const FRgbCameraConfig& Config) {

  UE_LOG(LogTemp, Warning, TEXT("ADronePawn::SetRgbCameraConfig"));

  rgb_camera_config_ = Config;

  RgbCameraBufferCriticalSection->Lock();
  RgbSegCameraBufferCriticalSection->Lock();

  SceneCaptureMeshHolderRgb->SetVisibility(Config.ShowCameraComponent);
  SceneCaptureMeshHolderRgb->SetRelativeLocation(Config.Offset);
  SceneCaptureMeshHolderRgb->SetRelativeRotation(FRotator(0, 0, 0));
  SceneCaptureMeshHolderRgb->SetRelativeRotation(Config.Orientation);

  SceneCaptureComponent2DRgb->FOVAngle = Config.FOVAngle;

  SceneCaptureComponent2DRgbSeg->FOVAngle = Config.FOVAngle;

  if (Config.enable_hdr) {
    SceneCaptureComponent2DRgb->CaptureSource = SCS_FinalColorHDR;
  } else {
    SceneCaptureComponent2DRgb->CaptureSource = SCS_FinalColorLDR;
  }

  SceneCaptureComponent2DRgb->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);

  SceneCaptureComponent2DRgb->bUseRayTracingIfEnabled = Config.enable_raytracing;

  SceneCaptureComponent2DRgb->TextureTarget                        = RenderTarget2DRgb;
  SceneCaptureComponent2DRgb->bAlwaysPersistRenderingState         = true;
  SceneCaptureComponent2DRgb->bCaptureEveryFrame                   = false;
  SceneCaptureComponent2DRgb->bCaptureOnMovement                   = false;
  SceneCaptureComponent2DRgb->PostProcessSettings.MotionBlurAmount = Config.motion_blur_amount;
  SceneCaptureComponent2DRgb->PostProcessSettings.MotionBlurMax    = Config.motion_blur_distortion;
  SceneCaptureComponent2DRgb->ShowFlags.SetMotionBlur(Config.enable_motion_blur);

  SceneCaptureComponent2DRgbSeg->TextureTarget                = RenderTarget2DRgbSeg;
  SceneCaptureComponent2DRgbSeg->bAlwaysPersistRenderingState = true;
  SceneCaptureComponent2DRgbSeg->bCaptureEveryFrame           = false;
  SceneCaptureComponent2DRgbSeg->bCaptureOnMovement           = false;
  SceneCaptureComponent2DRgbSeg->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);


  if (Config.Width > 0 && Config.Height > 0) {
    RenderTarget2DRgb->ResizeTarget(Config.Width, Config.Height);
    RenderTarget2DRgbSeg->ResizeTarget(Config.Width, Config.Height);
  } else {
    UE_LOG(LogTemp, Warning, TEXT("Invalid dimensions for RenderTarget2D. Width and Height should be greater than 0."));
  }

  RgbCameraBufferCriticalSection->Unlock();
  RgbSegCameraBufferCriticalSection->Unlock();

  return true;
}

bool UCamera::SetStereoCameraConfig(const FStereoCameraConfig& Config) {

  UE_LOG(LogTemp, Warning, TEXT("ADronePawn::SetStereoCameraConfig"));

  stereo_camera_config_ = Config;

  StereoCameraBufferCriticalSection->Lock();

  SceneCaptureMeshHolderStereoLeft->SetVisibility(Config.ShowCameraComponent);
  SceneCaptureMeshHolderStereoLeft->SetRelativeLocation(Config.Offset);
  SceneCaptureMeshHolderStereoLeft->SetRelativeRotation(FRotator(0, 0, 0));
  SceneCaptureMeshHolderStereoLeft->SetRelativeRotation(Config.Orientation);

  SceneCaptureMeshHolderStereoRight->SetVisibility(Config.ShowCameraComponent);
  SceneCaptureMeshHolderStereoRight->SetRelativeLocation(Config.Offset + FVector(0.0, 100.0 * Config.baseline, 0.0));
  SceneCaptureMeshHolderStereoRight->SetRelativeRotation(FRotator(0, 0, 0));
  SceneCaptureMeshHolderStereoRight->SetRelativeRotation(Config.Orientation);

  SceneCaptureComponent2DStereoLeft->FOVAngle  = Config.FOVAngle;
  SceneCaptureComponent2DStereoRight->FOVAngle = Config.FOVAngle;

  if (Config.enable_hdr) {
    SceneCaptureComponent2DStereoLeft->CaptureSource  = SCS_FinalColorHDR;
    SceneCaptureComponent2DStereoRight->CaptureSource = SCS_FinalColorHDR;
  } else {
    SceneCaptureComponent2DStereoLeft->CaptureSource  = SCS_FinalColorLDR;
    SceneCaptureComponent2DStereoRight->CaptureSource = SCS_FinalColorLDR;
  }

  SceneCaptureComponent2DStereoLeft->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);
  SceneCaptureComponent2DStereoRight->ShowFlags.SetTemporalAA(Config.enable_temporal_aa);

  SceneCaptureComponent2DStereoLeft->bUseRayTracingIfEnabled  = Config.enable_raytracing;
  SceneCaptureComponent2DStereoRight->bUseRayTracingIfEnabled = Config.enable_raytracing;

  if (Config.Width > 0 && Config.Height > 0) {
    RenderTarget2DStereoLeft->ResizeTarget(Config.Width, Config.Height);
    RenderTarget2DStereoRight->ResizeTarget(Config.Width, Config.Height);
  } else {
    UE_LOG(LogTemp, Warning, TEXT("Invalid dimensions for RenderTarget2D. Width and Height should be greater than 0."));
  }

  StereoCameraBufferCriticalSection->Unlock();

  return true;
}

void UCamera::SetCameraCaptureMode(CameraCaptureModeEnum CaptureMode) {

	CameraCaptureMode = CaptureMode;

	if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES || CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_MOVEMENT) {
	  UpdateCamera(true, CAMERA_MODE_RGB);
	}
}

void UCamera::TransformImageArray(int32 ImageWidth, int32 ImageHeight, const TArray<FColor>& SrcData, TArray<uint8>& DstData) {
	// Calculate the memory size required for the image data
	int32 MemorySize = ImageWidth * ImageHeight * 4;  // 4 bytes per pixel (RGBA)

	// Resize the destination array to hold the transformed data
	DstData.SetNumUninitialized(MemorySize);

	// Pointer to the source data (BGRA format)
	const FColor* SrcDataPtr = SrcData.GetData();

	// Pointer to the destination data (RGBA format)
	uint8* DstDataPtr = DstData.GetData();

	// Iterate over each pixel and transform BGRA to RGBA
	for (int32 Index = 0; Index < ImageWidth * ImageHeight; Index++) {
		// Copy and swap channels
		DstDataPtr[Index * 3 + 0] = SrcDataPtr[Index].B;  // Red channel
		DstDataPtr[Index * 3 + 1] = SrcDataPtr[Index].G;  // Green channel
		DstDataPtr[Index * 3 + 2] = SrcDataPtr[Index].R;  // Blue channel
	}
}
