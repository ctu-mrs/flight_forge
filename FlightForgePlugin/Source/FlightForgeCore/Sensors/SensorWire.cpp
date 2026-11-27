#include "SensorWire.h"

namespace ffw = flightforge::wire;

/* camera blocks //{ */

std::unique_ptr<ffw::Pose> FSensorWire::Mount(const FVector& OffsetCm, const FRotator& Rotation)
{
	return std::make_unique<ffw::Pose>(FWireFrame::ToMountPose(OffsetCm, Rotation));
}

void FSensorWire::Mount(const std::unique_ptr<ffw::Pose>& In, FVector& OutOffsetCm, FRotator& OutRotation)
{
	FWireFrame::FromMountPose(In.get(), OutOffsetCm, OutRotation);
}

std::unique_ptr<ffw::CameraIntrinsicsT> FSensorWire::Intrinsics(const bool bUseCustom, const FCameraIntrinsics& Effective)
{
	auto Out        = std::make_unique<ffw::CameraIntrinsicsT>();
	Out->use_custom = bUseCustom;
	Out->fx         = Effective.Fx;
	Out->fy         = Effective.Fy;
	Out->cx         = Effective.Cx - WirePixelShift;
	Out->cy         = Effective.Cy - WirePixelShift;
	return Out;
}

FCameraIntrinsics FSensorWire::Intrinsics(const ffw::CameraIntrinsicsT* In)
{
	FCameraIntrinsics Out;
	if (In)
	{
		Out.UseCustom = In->use_custom;
		Out.Fx        = In->fx;
		Out.Fy        = In->fy;
		Out.Cx        = In->cx + WirePixelShift;
		Out.Cy        = In->cy + WirePixelShift;
	}
	return Out;
}

std::unique_ptr<ffw::CameraDistortionT> FSensorWire::Distortion(const FCameraDistortion& In)
{
	auto Out    = std::make_unique<ffw::CameraDistortionT>();
	Out->enable = In.Enable;
	Out->k1     = In.K1;
	Out->k2     = In.K2;
	Out->k3     = In.K3;
	Out->p1     = In.P1;
	Out->p2     = In.P2;
	return Out;
}

FCameraDistortion FSensorWire::Distortion(const ffw::CameraDistortionT* In)
{
	FCameraDistortion Out;
	if (In)
	{
		Out.Enable = In->enable;
		Out.K1     = In->k1;
		Out.K2     = In->k2;
		Out.K3     = In->k3;
		Out.P1     = In->p1;
		Out.P2     = In->p2;
	}
	return Out;
}

std::unique_ptr<ffw::CameraNoiseT> FSensorWire::Noise(const FCameraNoise& In)
{
	auto Out        = std::make_unique<ffw::CameraNoiseT>();
	Out->enable     = In.Enable;
	Out->shot_scale = In.ShotScale;
	Out->read_sigma = In.ReadSigma;
	Out->row_sigma  = In.RowSigma;
	return Out;
}

FCameraNoise FSensorWire::Noise(const ffw::CameraNoiseT* In)
{
	FCameraNoise Out;
	if (In)
	{
		Out.Enable    = In->enable;
		Out.ShotScale = In->shot_scale;
		Out.ReadSigma = In->read_sigma;
		Out.RowSigma  = In->row_sigma;
	}
	return Out;
}

std::unique_ptr<ffw::CameraRollingShutterT> FSensorWire::RollingShutter(const FCameraRollingShutter& In)
{
	auto Out          = std::make_unique<ffw::CameraRollingShutterT>();
	Out->enable       = In.Enable;
	Out->readout_time = In.ReadoutTime;
	return Out;
}

FCameraRollingShutter FSensorWire::RollingShutter(const ffw::CameraRollingShutterT* In)
{
	FCameraRollingShutter Out;
	if (In)
	{
		Out.Enable      = In->enable;
		Out.ReadoutTime = In->readout_time;
	}
	return Out;
}

std::unique_ptr<ffw::CameraExposureT> FSensorWire::Exposure(const FCameraExposure& In)
{
	auto Out             = std::make_unique<ffw::CameraExposureT>();
	Out->manual          = In.Manual;
	Out->shutter_time    = In.ShutterTime;
	Out->iso             = In.ISO;
	Out->ev_compensation = In.EvCompensation;
	return Out;
}

FCameraExposure FSensorWire::Exposure(const ffw::CameraExposureT* In)
{
	FCameraExposure Out;
	if (In)
	{
		Out.Manual         = In->manual;
		Out.ShutterTime    = In->shutter_time;
		Out.ISO            = In->iso;
		Out.EvCompensation = In->ev_compensation;
	}
	return Out;
}

std::unique_ptr<ffw::CameraLensEffectsT> FSensorWire::Lens(const FCameraLensEffects& In)
{
	auto Out                      = std::make_unique<ffw::CameraLensEffectsT>();
	Out->fstop                    = In.Fstop;
	Out->focal_distance           = In.FocalDistance;
	Out->sensor_width_mm          = In.SensorWidthMm;
	Out->vignette_intensity       = In.VignetteIntensity;
	Out->chromatic_aberration     = In.ChromaticAberration;
	Out->bloom_intensity          = In.BloomIntensity;
	Out->lens_flare_intensity     = In.LensFlareIntensity;
	Out->white_temp               = In.WhiteTemp;
	Out->white_tint               = In.WhiteTint;
	Out->motion_blur_from_shutter = In.MotionBlurFromShutter;
	return Out;
}

FCameraLensEffects FSensorWire::Lens(const ffw::CameraLensEffectsT* In)
{
	FCameraLensEffects Out;
	if (In)
	{
		Out.Fstop                 = In->fstop;
		Out.FocalDistance         = In->focal_distance;
		Out.SensorWidthMm         = In->sensor_width_mm;
		Out.VignetteIntensity     = In->vignette_intensity;
		Out.ChromaticAberration   = In->chromatic_aberration;
		Out.BloomIntensity        = In->bloom_intensity;
		Out.LensFlareIntensity    = In->lens_flare_intensity;
		Out.WhiteTemp             = In->white_temp;
		Out.WhiteTint             = In->white_tint;
		Out.MotionBlurFromShutter = In->motion_blur_from_shutter;
	}
	return Out;
}

std::unique_ptr<ffw::ImageEncodingT> FSensorWire::Encoding(const FCameraCompression& In)
{
	auto Out         = std::make_unique<ffw::ImageEncodingT>();
	Out->compression = ToCompression(In.Codec);
	Out->quality     = static_cast<uint8_t>(FMath::Clamp(In.Quality, 1, 100));
	return Out;
}

FCameraCompression FSensorWire::Encoding(const ffw::ImageEncodingT* In)
{
	FCameraCompression Out;
	if (In)
	{
		switch (In->compression)
		{
			case ffw::Compression::NONE: Out.Codec = ECameraCodec::Raw; break;
			case ffw::Compression::PNG: Out.Codec = ECameraCodec::Png; break;
			default: Out.Codec = ECameraCodec::Jpeg; break;
		}
		Out.Quality = FMath::Clamp<int32>(In->quality, 1, 100);
	}
	return Out;
}

/* //} */

/* configs //{ */

void FSensorWire::ToWire(const FRgbCameraConfig& In, ffw::RgbCameraConfigT& Out)
{
	Out.show_debug_camera      = In.ShowCameraComponent;
	Out.mount                  = Mount(In.Offset, In.Orientation);
	Out.fov                    = In.FOVAngle;
	Out.width                  = In.Width;
	Out.height                 = In.Height;
	Out.enable_temporal_aa     = In.enable_temporal_aa;
	Out.enable_raytracing      = In.enable_raytracing;
	Out.enable_hdr             = In.enable_hdr;
	Out.enable_motion_blur     = In.enable_motion_blur;
	Out.motion_blur_amount     = In.motion_blur_amount;
	Out.motion_blur_distortion = In.motion_blur_distortion;
	Out.exposure               = Exposure(In.Exposure);
	Out.lens                   = Lens(In.Lens);
	Out.intrinsics = Intrinsics(In.Intrinsics.UseCustom, UCamera::EffectiveIntrinsics(In.Intrinsics, In.FOVAngle, In.Width, In.Height));
	Out.distortion = Distortion(In.Distortion);
	Out.noise      = Noise(In.Noise);
	Out.rolling_shutter = RollingShutter(In.RollingShutter);
	Out.encoding        = Encoding(In.Compression);
	Out.capture_rate    = In.CaptureRate;
}

bool FSensorWire::FromWire(const ffw::Request& Request, FRgbCameraConfig& Out, FWireReply& Reply)
{
	const ffw::SetRgbCameraConfigRequest* Set = Request.body_as_SetRgbCameraConfigRequest();
	if (!Set || !Set->config())
	{
		Reply.Fail(TEXT("set_rgb_camera_config carries no config"));
		return false;
	}

	const std::unique_ptr<ffw::RgbCameraConfigT> In(Set->config()->UnPack());
	if (!ValidPinhole(In->fov, In->width, In->height, Reply))
	{
		return false;
	}

	Out.ShowCameraComponent = In->show_debug_camera;
	Mount(In->mount, Out.Offset, Out.Orientation);
	Out.FOVAngle               = In->fov;
	Out.Width                  = In->width;
	Out.Height                 = In->height;
	Out.enable_temporal_aa     = In->enable_temporal_aa;
	Out.enable_raytracing      = In->enable_raytracing;
	Out.enable_hdr             = In->enable_hdr;
	Out.enable_motion_blur     = In->enable_motion_blur;
	Out.motion_blur_amount     = In->motion_blur_amount;
	Out.motion_blur_distortion = In->motion_blur_distortion;
	Out.Exposure               = Exposure(In->exposure.get());
	Out.Lens                   = Lens(In->lens.get());
	Out.Intrinsics             = Intrinsics(In->intrinsics.get());
	Out.Distortion             = Distortion(In->distortion.get());
	Out.Noise                  = Noise(In->noise.get());
	Out.RollingShutter         = RollingShutter(In->rolling_shutter.get());
	Out.Compression            = Encoding(In->encoding.get());
	Out.CaptureRate            = In->capture_rate;
	return true;
}

void FSensorWire::ReplyConfig(const FRgbCameraConfig& Config, FWireReply& Reply)
{
	ffw::RgbCameraConfigT Table;
	ToWire(Config, Table);
	const auto Offset = ffw::CreateRgbCameraConfig(Reply.Builder, &Table);
	Reply.Set(ffw::CreateGetRgbCameraConfigResponse(Reply.Builder, Offset));
}

bool FSensorWire::ValidPinhole(const double FovDeg, const int64 Width, const int64 Height, FWireReply& Reply)
{
	if (!(FovDeg > 0.0 && FovDeg < 180.0))
	{
		Reply.Fail(FString::Printf(TEXT("fov %g deg is outside (0, 180)"), FovDeg));
		return false;
	}
	if (!UCamera::ValidDimensions(static_cast<int32>(FMath::Min<int64>(Width, MAX_int32)), static_cast<int32>(FMath::Min<int64>(Height, MAX_int32))))
	{
		Reply.Fail(FString::Printf(TEXT("%lld x %lld px is not a valid image size"), Width, Height));
		return false;
	}
	return true;
}

/* //} */

/* frames //{ */

ffw::PixelFormat FSensorWire::ColourFormat(const ECameraCodec Codec)
{
	switch (Codec)
	{
		case ECameraCodec::Raw: return ffw::PixelFormat::BGRA8;
		case ECameraCodec::Png: return ffw::PixelFormat::RGBA8;
		default: return ffw::PixelFormat::RGB8;
	}
}

ffw::Compression FSensorWire::ToCompression(const ECameraCodec Codec)
{
	switch (Codec)
	{
		case ECameraCodec::Raw: return ffw::Compression::NONE;
		case ECameraCodec::Png: return ffw::Compression::PNG;
		default: return ffw::Compression::JPEG;
	}
}

flatbuffers::Offset<ffw::Image> FSensorWire::Image(FWireReply& Reply, const FCameraFrame& Frame, const ffw::PixelFormat Format)
{
	const auto Data = Reply.CopyVector(Frame.Bytes.GetData(), Frame.Bytes.Num());
	return ffw::CreateImage(Reply.Builder, Frame.Size.X, Frame.Size.Y, Format, ToCompression(Frame.Codec), Data);
}

/* //} */
