#pragma once

#include "CoreMinimal.h"
#include "Camera.h"
#include "FisheyeCamera.generated.h"


enum class EFisheyeLensModel : int
{
	EQUIDISTANT   = 0,  // r = f * theta
	EQUISOLID     = 1,  // r = 2f * sin(theta/2)
	STEREOGRAPHIC = 2,  // r = 2f * tan(theta/2)
};

struct FFisheyeCameraConfig
{
	bool ShowCameraComponent;

	FVector  Offset;
	FRotator Orientation;

	// full field of view in degrees, up to 220
	double FOVAngle;

	int Width;
	int Height;

	EFisheyeLensModel LensModel;
};

/* FFisheyeLUTEntry //{ */

// one output pixel's source: a face and bilinear coordinates within it;
// face < 0 marks pixels outside the image circle
struct FFisheyeLUTEntry
{
	int8  Face;
	float FX;
	float FY;
};

//}

// Wide-FOV camera assembled from five 90-degree captures (forward, right,
// left, up, down) resampled through a precomputed lens-projection LUT. A
// planar capture distorts unusably past ~120 degrees; five faces cover up to
// 220 degrees with uniform angular resolution.
UCLASS()
class UEDS_API UFisheyeCamera : public UCamera
{
	GENERATED_BODY()

public:
	UFisheyeCamera();

	static constexpr int32 NumFaces = 5;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TArray<UTextureRenderTarget2D*> FaceRenderTargets;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TArray<USceneCaptureComponent2D*> FaceCaptures;

	std::unique_ptr<TQueue<std::shared_ptr<FInstruction<UFisheyeCamera>>>> InstructionQueue;

	bool SetFisheyeCameraConfig(const FFisheyeCameraConfig& Config);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void UpdateCamera(bool isExternallyLocked, double stamp) override;

	virtual void GetConfig(std::stringstream& OutputStream) override;
	virtual void SetConfig(std::stringstream& OutputStream, std::shared_ptr<std::stringstream> InputStream) override;
	virtual void GetData(std::stringstream& OutputStream) override;

	virtual void BeginDestroy() override;

	bool GetFisheyeCameraDataFromServerThread(TArray<uint8>& OutArray, double& stamp);

protected:
	virtual void BeginPlay() override;
	virtual void Initialize(int InSensorID) override;

	FFisheyeCameraConfig fisheye_camera_config_;

	// published face pixels; guarded by CameraBufferCriticalSection
	TArray<TArray<FColor>> FaceBuffers;
	int32 PublishedFaceSize = 0;

	// swapped atomically under the buffer lock; server threads resample from
	// their own reference outside the lock
	TSharedPtr<const TArray<FFisheyeLUTEntry>, ESPMode::ThreadSafe> LUT;
	FIntPoint LUTSize = FIntPoint::ZeroValue;

	TArray<TSharedPtr<FCameraReadback, ESPMode::ThreadSafe>> PendingFaces;

	int32 FaceSize() const;

	void RebuildLUT();

	// game thread: publishes when all five faces have landed; true when published
	bool PumpFisheyeReadback();

	void SetCaptureEveryFrame(bool bEnable);
};
