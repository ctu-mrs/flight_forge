#include "SensorDeviceLibrary.h"

#include "FlightForgeCore/Sensors/DepthCamera.h"
#include "FlightForgeCore/Sensors/EventCamera.h"
#include "FlightForgeCore/Sensors/FisheyeCamera.h"
#include "FlightForgeCore/Sensors/Lidar.h"
#include "FlightForgeCore/Sensors/RangeFinder.h"
#include "FlightForgeCore/Sensors/RgbCamera.h"
#include "FlightForgeCore/Sensors/StereoCamera.h"

/* configurators //{ */

namespace
{
// UE units are centimeters throughout

void ConfigureRgb(USensor* Sensor, const FVector& Offset, const FRotator& Rotation, double FovDeg, int W, int H)
{
	if (URgbCamera* Cam = Cast<URgbCamera>(Sensor))
	{
		FRgbCameraConfig Config = Cam->GetRgbCameraConfig();
		Config.Offset      = Offset;
		Config.Orientation = Rotation;
		Config.FOVAngle    = FovDeg;
		Config.Width       = W;
		Config.Height      = H;
		Cam->SetRgbCameraConfig(Config);
	}
}

void ConfigureStereo(USensor* Sensor, const FVector& Offset, const FRotator& Rotation, double BaselineCm, double FovDeg, int W, int H)
{
	if (UStereoCamera* Cam = Cast<UStereoCamera>(Sensor))
	{
		FStereoCameraConfig Config = Cam->GetStereoCameraConfig();
		Config.Offset_left       = Offset + Rotation.RotateVector(FVector(0, -BaselineCm * 0.5, 0));
		Config.Offset_right      = Offset + Rotation.RotateVector(FVector(0, BaselineCm * 0.5, 0));
		Config.Orientation_left  = Rotation;
		Config.Orientation_right = Rotation;
		Config.FOVAngle          = FovDeg;
		Config.Width             = W;
		Config.Height            = H;
		Cam->SetStereoCameraConfig(Config);
	}
}

void ConfigureGridLidar(USensor* Sensor, const FVector& Offset, const FRotator& Rotation, int HorRays, int VertRays, double FovHorDeg, double FovVertUpDeg,
                        double FovVertDownDeg, double RangeCm, double FrequencyHz)
{
	if (ULidar* Lidar = Cast<ULidar>(Sensor))
	{
		FLidarConfig Config = Lidar->GetLidarConfig();
		Config.Enable       = true;
		Config.Livox        = false;
		Config.Offset       = Offset;
		Config.Orientation  = Rotation;
		Config.BeamHorRays  = HorRays;
		Config.BeamVertRays = VertRays;
		Config.FOVHorLeft   = FovHorDeg * 0.5;
		Config.FOVHorRight  = FovHorDeg * 0.5;
		Config.FOVVertUp    = FovVertUpDeg;
		Config.FOVVertDown  = FovVertDownDeg;
		Config.BeamLength   = RangeCm;
		Config.Frequency    = FrequencyHz;
		Lidar->SetLidarConfig(Config);
	}
}

void ConfigureLivox(USensor* Sensor, const FVector& Offset, const FRotator& Rotation, const FString& Pattern, double RangeCm, double FrequencyHz)
{
	if (ULidar* Lidar = Cast<ULidar>(Sensor))
	{
		FLidarConfig Config = Lidar->GetLidarConfig();
		Config.Enable      = true;
		Config.Livox       = true;
		Config.LivoxSensor = Pattern;
		Config.Offset      = Offset;
		Config.Orientation = Rotation;
		Config.BeamLength  = RangeCm;
		Config.Frequency   = FrequencyHz;
		Lidar->SetLidarConfig(Config);
	}
}

void ConfigureRangefinder(USensor* Sensor, const FVector& Offset, double RangeCm)
{
	if (URangeFinder* Finder = Cast<URangeFinder>(Sensor))
	{
		FRangefinderConfig Config = Finder->GetRangefinderConfig();
		Config.Offset     = Offset;
		Config.BeamLength = RangeCm;
		Finder->SetRangefinderConfig(Config);
	}
}

struct FEventPixelSpec
{
	double ContrastPos;
	double ContrastNeg;
	double Sigma;
	double RefractoryS;
	double CutoffHz;
	double MaxEventRateHz;
};

void ConfigureEvent(USensor* Sensor, const FVector& Offset, const FRotator& Rotation, double FovDeg, int W, int H, const FEventPixelSpec& Pixel)
{
	if (UEventCamera* Cam = Cast<UEventCamera>(Sensor))
	{
		FEventCameraConfig Config = Cam->GetEventCameraConfig();
		Config.Offset               = Offset;
		Config.Orientation          = Rotation;
		Config.FOVAngle             = FovDeg;
		Config.Width                = W;
		Config.Height               = H;
		Config.ContrastThresholdPos = Pixel.ContrastPos;
		Config.ContrastThresholdNeg = Pixel.ContrastNeg;
		Config.ThresholdSigma       = Pixel.Sigma;
		Config.RefractoryPeriod     = Pixel.RefractoryS;
		Config.CutoffHz             = Pixel.CutoffHz;
		Config.MaxEventRateHz       = Pixel.MaxEventRateHz;
		Cam->SetEventCameraConfig(Config);
	}
}

void ConfigureFisheye(USensor* Sensor, const FVector& Offset, const FRotator& Rotation, double FovDeg, int W, int H, EFisheyeLensModel Model, bool bFullFrame)
{
	if (UFisheyeCamera* Cam = Cast<UFisheyeCamera>(Sensor))
	{
		FFisheyeCameraConfig Config = Cam->GetFisheyeCameraConfig();
		Config.Offset      = Offset;
		Config.Orientation = Rotation;
		Config.FOVAngle    = FovDeg;
		Config.Width       = W;
		Config.Height      = H;
		Config.LensModel   = Model;
		Config.FullFrame   = bFullFrame;
		Cam->SetFisheyeCameraConfig(Config);
	}
}
}  // namespace

//}

/* BuildRegistry() //{ */

static TArray<FSensorDeviceSpec> BuildRegistry()
{
	TArray<FSensorDeviceSpec> Devices;

	{
		FSensorDeviceSpec D;
		D.Name             = TEXT("realsense_d435i");
		D.Description      = TEXT("Intel RealSense D435i: 5 cm stereo pair 848x480 87deg + RGB 1280x720 69deg");
		D.MeshShape        = EDeviceMeshShape::Box;
		D.MeshDimensionsCm = FVector(2.5, 9.0, 2.5);

		FDeviceSensorEntry Stereo;
		Stereo.Type      = SensorType::STEREO_CAMERA;
		Stereo.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureStereo(S, O, R, 5.0, 87, 848, 480); };
		D.Sensors.Add(Stereo);

		FDeviceSensorEntry Rgb;
		Rgb.Type      = SensorType::RGB_CAMERA;
		Rgb.RelOffset = FVector(0, -4.0, 0);
		Rgb.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureRgb(S, O, R, 69, 1280, 720); };
		D.Sensors.Add(Rgb);
		Devices.Add(D);
	}

	{
		FSensorDeviceSpec D;
		D.Name             = TEXT("azure_kinect_dk");
		D.Description      = TEXT("Azure Kinect DK: ToF depth NFOV 640x576 75deg + RGB 1280x720 90deg (depth currently ideal - no ToF noise model yet)");
		D.MeshShape        = EDeviceMeshShape::Box;
		D.MeshDimensionsCm = FVector(10.3, 12.6, 3.9);

		FDeviceSensorEntry Depth;
		Depth.Type      = SensorType::DEPTH_CAMERA;
		Depth.Configure = [](USensor* S, const FVector& O, const FRotator& R)
		{
			if (UDepthCamera* Camera = Cast<UDepthCamera>(S))
			{
				FRgbCameraConfig Config = Camera->GetDepthCameraConfig();
				Config.Offset           = O;
				Config.Orientation      = R;
				Config.FOVAngle         = 75;
				Config.Width            = 640;
				Config.Height           = 576;
				Camera->SetDepthCameraConfig(Config);
			}
		};
		D.Sensors.Add(Depth);

		FDeviceSensorEntry Rgb;
		Rgb.Type      = SensorType::RGB_CAMERA;
		Rgb.RelOffset = FVector(0, -3.2, 0);  
		Rgb.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureRgb(S, O, R, 90, 1280, 720); };
		D.Sensors.Add(Rgb);

		Devices.Add(D);
	}

	{
		FSensorDeviceSpec D;
		D.Name             = TEXT("ouster_os0_32");
		D.Description      = TEXT("Ouster OS0-32: 32 ch, 1024 cols, 90deg vertical FOV, 50 m");
		D.MeshShape        = EDeviceMeshShape::Cylinder;
		D.MeshDimensionsCm = FVector(8.5, 8.5, 5.8);
		D.MeshFile         = TEXT("OUSTER_Rev7_OS0");

		FDeviceSensorEntry Lidar;
		Lidar.Type      = SensorType::LIDAR;
		Lidar.RelOffset = FVector(0, 0, 3.82);  // beam origin above the base, Rev7 drawing
		Lidar.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureGridLidar(S, O, R, 1024, 32, 360, 45, 45, 5000, 10); };
		D.Sensors.Add(Lidar);

		Devices.Add(D);
	}

	{
		FSensorDeviceSpec D;
		D.Name             = TEXT("ouster_os1_64");
		D.Description      = TEXT("Ouster OS1-64: 64 ch, 1024 cols, 45deg vertical FOV, 120 m");
		D.MeshShape        = EDeviceMeshShape::Cylinder;
		D.MeshDimensionsCm = FVector(8.5, 8.5, 7.4);

		FDeviceSensorEntry Lidar;
		Lidar.Type      = SensorType::LIDAR;
		Lidar.RelOffset = FVector(0, 0, 3.82);  // Rev7 lidar_to_sensor z, ouster-sdk metadata
		Lidar.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureGridLidar(S, O, R, 1024, 64, 360, 22.5, 22.5, 12000, 10); };
		D.Sensors.Add(Lidar);

		Devices.Add(D);
	}

	{
		FSensorDeviceSpec D;
		D.Name             = TEXT("ouster_os0_128");
		D.Description      = TEXT("Ouster OS0-128 Rev7: 128 ch, 1024 cols, 90.8deg vertical FOV, 75 m");
		D.MeshShape        = EDeviceMeshShape::Cylinder;
		D.MeshDimensionsCm = FVector(8.7, 8.7, 5.835);
		D.MeshFile         = TEXT("OUSTER_Rev7_OS0");

		FDeviceSensorEntry Lidar;
		Lidar.Type      = SensorType::LIDAR;
		Lidar.RelOffset = FVector(0, 0, 3.82);
		Lidar.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureGridLidar(S, O, R, 1024, 128, 360, 45.4, 45.4, 7500, 10); };
		D.Sensors.Add(Lidar);

		Devices.Add(D);
	}

	{
		FSensorDeviceSpec D;
		D.Name             = TEXT("ouster_os1_128");
		D.Description      = TEXT("Ouster OS1-128 Rev7: 128 ch, 1024 cols, 42.4deg vertical FOV, 170 m");
		D.MeshShape        = EDeviceMeshShape::Cylinder;
		D.MeshDimensionsCm = FVector(8.7, 8.7, 5.835);

		FDeviceSensorEntry Lidar;
		Lidar.Type      = SensorType::LIDAR;
		Lidar.RelOffset = FVector(0, 0, 3.82);
		Lidar.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureGridLidar(S, O, R, 1024, 128, 360, 21.2, 21.2, 17000, 10); };
		D.Sensors.Add(Lidar);

		Devices.Add(D);
	}

	{
		FSensorDeviceSpec D;
		D.Name             = TEXT("ouster_os2_128");
		D.Description      = TEXT("Ouster OS2-128 Rev7: 128 ch, 1024 cols, 22.5deg vertical FOV, 350 m");
		D.MeshShape        = EDeviceMeshShape::Cylinder;
		D.MeshDimensionsCm = FVector(11.96, 11.96, 9.89);

		FDeviceSensorEntry Lidar;
		Lidar.Type      = SensorType::LIDAR;
		Lidar.RelOffset = FVector(0, 0, 7.83);  // Rev6 ouster-sdk metadata, Rev7 value unpublished
		Lidar.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureGridLidar(S, O, R, 1024, 128, 360, 11.25, 11.25, 35000, 10); };
		D.Sensors.Add(Lidar);

		Devices.Add(D);
	}

	{
		FSensorDeviceSpec D;
		D.Name             = TEXT("livox_mid360");
		D.Description      = TEXT("Livox Mid-360: non-repetitive 360x59deg pattern, 40 m");
		D.MeshShape        = EDeviceMeshShape::Cylinder;
		D.MeshDimensionsCm = FVector(6.5, 6.5, 6.0);
		D.MeshFile         = TEXT("Mid360");
		D.MeshRotation     = FRotator(-90, 90, 0);  // CAD has the base along +X and the dome along -X
		D.MeshOffsetCm     = FVector(0, 0, 3.42);

		FDeviceSensorEntry Lidar;
		Lidar.Type      = SensorType::LIDAR;
		Lidar.RelOffset = FVector(0, 0, 4.7);  // point-cloud origin O above the bottom surface, user manual drawing
		Lidar.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureLivox(S, O, R, TEXT("mid360"), 4000, 10); };
		D.Sensors.Add(Lidar);

		Devices.Add(D);
	}

	{
		FSensorDeviceSpec D;
		D.Name             = TEXT("livox_avia");
		D.Description      = TEXT("Livox Avia: non-repetitive 70x77deg pattern, 190 m");
		D.MeshShape        = EDeviceMeshShape::Box;
		D.MeshDimensionsCm = FVector(6.5, 9.1, 6.1);

		FDeviceSensorEntry Lidar;
		Lidar.Type      = SensorType::LIDAR;
		Lidar.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureLivox(S, O, R, TEXT("avia"), 19000, 10); };
		D.Sensors.Add(Lidar);

		Devices.Add(D);
	}

	{
		FSensorDeviceSpec D;
		D.Name             = TEXT("odin1");
		D.Description      = TEXT("Manifold Odin1: 120x90deg solid-state lidar 240x180 @10 Hz, 70 m + 173deg-diagonal global-shutter RGB 1600x1296");
		D.MeshShape        = EDeviceMeshShape::Box;
		D.MeshDimensionsCm = FVector(10.0, 6.2, 4.3);
		D.MeshFile         = TEXT("Odin1");
		D.MeshRotation     = FRotator(0, 180, 0);  // CAD has the optics along -X and the mounting plate along +X
		D.MeshOffsetCm     = FVector(0, 0, 3.1);

		// both look out of the front face; the camera sits left of the lidar window
		FDeviceSensorEntry Lidar;
		Lidar.Type      = SensorType::LIDAR;
		Lidar.RelOffset = FVector(2.0, 1.3, 3.1);
		Lidar.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureGridLidar(S, O, R, 240, 180, 120, 45, 45, 7000, 10); };
		D.Sensors.Add(Lidar);

		// 129 x 104 x 173 deg on a full frame is an equidistant lens whose image circle reaches the corners
		FDeviceSensorEntry Rgb;
		Rgb.Type      = SensorType::FISHEYE_CAMERA;
		Rgb.RelOffset = FVector(2.0, -3.4, 3.1);
		Rgb.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureFisheye(S, O, R, 173, 1600, 1296, EFisheyeLensModel::EQUIDISTANT, true); };
		D.Sensors.Add(Rgb);

		Devices.Add(D);
	}

	{
		FSensorDeviceSpec D;
		D.Name             = TEXT("garmin_lidar_lite_v3");
		D.Description      = TEXT("Garmin LIDAR-Lite v3 rangefinder, 40 m, downward");
		D.MeshShape        = EDeviceMeshShape::Box;
		D.MeshDimensionsCm = FVector(2.0, 4.8, 4.0);
		D.MeshFile         = TEXT("Garmin");
		D.MeshRotation     = FRotator(0, 0, 180);  // CAD lenses point up, the rangefinder looks down

		FDeviceSensorEntry Finder;
		Finder.Type      = SensorType::RANGEFINDER;
		Finder.RelOffset = FVector(0, 0, -3.9);  // lens face of the housing hanging under the mount
		Finder.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureRangefinder(S, O, 4000); };
		D.Sensors.Add(Finder);

		Devices.Add(D);
	}

	{
		FSensorDeviceSpec D;
		D.Name             = TEXT("davis346");
		D.Description      = TEXT("iniVation DAVIS346 event camera: 346x260, ~70deg with kit lens");
		D.MeshShape        = EDeviceMeshShape::Box;
		D.MeshDimensionsCm = FVector(4.0, 4.0, 5.5);

		FDeviceSensorEntry Event;
		Event.Type      = SensorType::EVENT_CAMERA;
		// docs.inivation.com davis346: 12 MEPS arbitrated readout, 14.3 % ON / 22.5 % OFF median contrast
		Event.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureEvent(S, O, R, 70, 346, 260, {0.143, 0.225, 0.04, 1e-4, 200.0, 12e6}); };
		D.Sensors.Add(Event);

		Devices.Add(D);
	}

	{
		FSensorDeviceSpec D;
		D.Name             = TEXT("prophesee_evk4");
		D.Description      = TEXT("Prophesee EVK4 HD event camera: 1280x720, ~70deg with kit lens");
		D.MeshShape        = EDeviceMeshShape::Box;
		D.MeshDimensionsCm = FVector(3.0, 3.0, 3.6);

		FDeviceSensorEntry Event;
		Event.Type      = SensorType::EVENT_CAMERA;
		// Sony IMX636: 1.06 Geps readout, 25 % nominal contrast; past its rate the ERC drops events spatially and temporally
		Event.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureEvent(S, O, R, 70, 1280, 720, {0.25, 0.25, 0.03, 2e-5, 1000.0, 1.06e9}); };
		D.Sensors.Add(Event);

		Devices.Add(D);
	}

	{
		FSensorDeviceSpec D;
		D.Name             = TEXT("prophesee_genx320");
		D.Description      = TEXT("Prophesee GenX320 event sensor kit: 320x320, ~70deg lens");
		D.MeshShape        = EDeviceMeshShape::Box;
		D.MeshDimensionsCm = FVector(3.0, 3.0, 3.6);  // kit housing unpublished, EVK4's

		FDeviceSensorEntry Event;
		Event.Type      = SensorType::EVENT_CAMERA;
		// 25 % nominal contrast; readout limit unpublished, so no ceiling, the rest are the generic defaults
		Event.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureEvent(S, O, R, 70, 320, 320, {0.25, 0.25, 0.03, 1e-4, 300.0, 0.0}); };
		D.Sensors.Add(Event);

		Devices.Add(D);
	}

	{
		FSensorDeviceSpec D;
		D.Name             = TEXT("inivation_dvxplorer");
		D.Description      = TEXT("iniVation DVXplorer event camera: 640x480, ~70deg with kit lens");
		D.MeshShape        = EDeviceMeshShape::Box;
		D.MeshDimensionsCm = FVector(4.0, 4.0, 5.5);  // same 40x60x25 mm case as the DAVIS346

		FDeviceSensorEntry Event;
		Event.Type      = SensorType::EVENT_CAMERA;
		// 165 MEPS readout scanned at 5 kHz, so one event per pixel per 200 us; 13 % median, 27.5 % at 99.9 % of pixels
		Event.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureEvent(S, O, R, 70, 640, 480, {0.13, 0.13, 0.047, 2e-4, 300.0, 165e6}); };
		D.Sensors.Add(Event);

		Devices.Add(D);
	}

	{
		FSensorDeviceSpec D;
		D.Name             = TEXT("fisheye_220");
		D.Description      = TEXT("Generic 220deg equidistant fisheye, 1024x1024");
		D.MeshShape        = EDeviceMeshShape::Cylinder;
		D.MeshDimensionsCm = FVector(3.0, 3.0, 3.0);

		FDeviceSensorEntry Fisheye;
		Fisheye.Type      = SensorType::FISHEYE_CAMERA;
		Fisheye.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureFisheye(S, O, R, 220, 1024, 1024, EFisheyeLensModel::EQUIDISTANT, false); };
		D.Sensors.Add(Fisheye);

		Devices.Add(D);
	}

	return Devices;
}

//}

const TArray<FSensorDeviceSpec>& FSensorDeviceLibrary::Get()
{
	static const TArray<FSensorDeviceSpec> Registry = BuildRegistry();
	return Registry;
}

const FSensorDeviceSpec* FSensorDeviceLibrary::Find(const FString& Name)
{
	for (const FSensorDeviceSpec& Device : Get())
	{
		if (Device.Name.Equals(Name, ESearchCase::IgnoreCase))
		{
			return &Device;
		}
	}

	return nullptr;
}
