#include "Camera.h"

#include "ueds/DronePawn.h"

#include "Serialize.h"

UCamera::UCamera()
{
	PrimaryComponentTick.bCanEverTick = true;
	//PrimaryComponentTick.TickGroup    = TG_PrePhysics;

#if PLATFORM_WINDOWS
	CameraBufferCriticalSection = std::make_unique<FWindowsCriticalSection>();
#else
	CameraBufferCriticalSection = std::make_unique<FPThreadsCriticalSection>();
#endif

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

void UCamera::BeginDestroy()
{
	// Cleanup buffer when component is destroyed
	CameraBuffer.Empty();
	
	Super::BeginDestroy();
}

void UCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UCamera::SetCameraCaptureMode(CameraCaptureModeEnum CaptureMode) {

	CameraCaptureMode = CaptureMode;

	// if (CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ALL_FRAMES || CameraCaptureMode == CameraCaptureModeEnum::CAPTURE_ON_MOVEMENT) {
	// 	//UE_LOG(LogTemp, Log, TEXT("Camera updating after capture mode change"));
	//     UpdateCamera(false, 0.0); //used to update only in rgb mode
	// }
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

void UCamera::UpdateCamera(bool isExternallyLocked, double stamp = 0.0) {}

void UCamera::GetConfig(std::stringstream& OutputStream) {}

void UCamera::SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream) {}

void UCamera::GetData(std::stringstream& OutputStream) {}

