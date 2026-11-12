#pragma once

#include "CoreMinimal.h"
#include "Camera.h"
#include "RgbCamera.h"
#include "FlightForgeCore/Server/WireReply.h"
#include "FlightForgeCore/Utils/WireFrame.h"

// sensor configs and frames <-> wire tables; intrinsics cross with WirePixelShift
struct FSensorWire
{
	/* camera blocks //{ */

	static std::unique_ptr<flightforge::wire::Pose> Mount(const FVector& OffsetCm, const FRotator& Rotation);
	static void Mount(const std::unique_ptr<flightforge::wire::Pose>& In, FVector& OutOffsetCm, FRotator& OutRotation);

	static std::unique_ptr<flightforge::wire::CameraIntrinsicsT> Intrinsics(bool bUseCustom, const FCameraIntrinsics& Effective);
	static FCameraIntrinsics Intrinsics(const flightforge::wire::CameraIntrinsicsT* In);

	static std::unique_ptr<flightforge::wire::CameraDistortionT> Distortion(const FCameraDistortion& In);
	static FCameraDistortion Distortion(const flightforge::wire::CameraDistortionT* In);

	static std::unique_ptr<flightforge::wire::CameraNoiseT> Noise(const FCameraNoise& In);
	static FCameraNoise Noise(const flightforge::wire::CameraNoiseT* In);

	static std::unique_ptr<flightforge::wire::CameraRollingShutterT> RollingShutter(const FCameraRollingShutter& In);
	static FCameraRollingShutter RollingShutter(const flightforge::wire::CameraRollingShutterT* In);

	static std::unique_ptr<flightforge::wire::CameraExposureT> Exposure(const FCameraExposure& In);
	static FCameraExposure Exposure(const flightforge::wire::CameraExposureT* In);

	static std::unique_ptr<flightforge::wire::CameraLensEffectsT> Lens(const FCameraLensEffects& In);
	static FCameraLensEffects Lens(const flightforge::wire::CameraLensEffectsT* In);

	static std::unique_ptr<flightforge::wire::ImageEncodingT> Encoding(const FCameraCompression& In);
	static FCameraCompression Encoding(const flightforge::wire::ImageEncodingT* In);

	/* //} */

	/* configs //{ */

	// the rgb, depth and both segmentation cameras share this table
	static void ToWire(const FRgbCameraConfig& In, flightforge::wire::RgbCameraConfigT& Out);
	static bool FromWire(const flightforge::wire::Request& Request, FRgbCameraConfig& Out, FWireReply& Reply);

	static void ReplyConfig(const FRgbCameraConfig& Config, FWireReply& Reply);

	static bool ValidPinhole(double FovDeg, int64 Width, int64 Height, FWireReply& Reply);

	/* //} */

	/* frames //{ */

	// the layout a decoder produces from a colour frame
	static flightforge::wire::PixelFormat ColourFormat(ECameraCodec Codec);
	static flightforge::wire::Compression ToCompression(ECameraCodec Codec);

	static flatbuffers::Offset<flightforge::wire::Image> Image(FWireReply& Reply, const FCameraFrame& Frame, flightforge::wire::PixelFormat Format);

	/* //} */
};

template <typename TSensor, typename TFunction>
void RunOnSensorQueue(TSensor& Sensor, TFunction&& Function)
{
	auto Instruction      = std::make_shared<FInstruction<TSensor>>();
	Instruction->Function = Forward<TFunction>(Function);
	Sensor.InstructionQueue->Enqueue(Instruction);
	Instruction->Wait();
}
