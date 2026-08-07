#include "SensorDeviceLibrary.h"

#include "ueds/Sensors/EventCamera.h"
#include "ueds/Sensors/FisheyeCamera.h"
#include "ueds/Sensors/Lidar.h"
#include "ueds/Sensors/RangeFinder.h"
#include "ueds/Sensors/RgbCamera.h"
#include "ueds/Sensors/StereoCamera.h"

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

void ConfigureGridLidar(USensor* Sensor, const FVector& Offset, const FRotator& Rotation, int HorRays, int VertRays, double FovVertUpDeg, double FovVertDownDeg,
                        double RangeCm, double FrequencyHz)
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
		Config.FOVHorLeft   = 180;
		Config.FOVHorRight  = 180;
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

void ConfigureEvent(USensor* Sensor, const FVector& Offset, const FRotator& Rotation, double FovDeg, int W, int H, double Contrast)
{
	if (UEventCamera* Cam = Cast<UEventCamera>(Sensor))
	{
		FEventCameraConfig Config = Cam->GetEventCameraConfig();
		Config.Offset               = Offset;
		Config.Orientation          = Rotation;
		Config.FOVAngle             = FovDeg;
		Config.Width                = W;
		Config.Height               = H;
		Config.ContrastThresholdPos = Contrast;
		Config.ContrastThresholdNeg = Contrast;
		Cam->SetEventCameraConfig(Config);
	}
}

void ConfigureFisheye(USensor* Sensor, const FVector& Offset, const FRotator& Rotation, double FovDeg, int W, int H, EFisheyeLensModel Model)
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
		Rgb.RelOffset = FVector(0, -4.0, 0);  // RGB module outboard of the left imager
		Rgb.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureRgb(S, O, R, 69, 1280, 720); };
		D.Sensors.Add(Rgb);

		Devices.Add(D);
	}

	{
		FSensorDeviceSpec D;
		D.Name             = TEXT("ouster_os0_32");
		D.Description      = TEXT("Ouster OS0-32: 32 ch, 1024 cols, 90deg vertical FOV, 50 m");
		D.MeshShape        = EDeviceMeshShape::Cylinder;
		D.MeshDimensionsCm = FVector(8.5, 8.5, 5.8);

		FDeviceSensorEntry Lidar;
		Lidar.Type      = SensorType::LIDAR;
		Lidar.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureGridLidar(S, O, R, 1024, 32, 45, 45, 5000, 10); };
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
		Lidar.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureGridLidar(S, O, R, 1024, 64, 22.5, 22.5, 12000, 10); };
		D.Sensors.Add(Lidar);

		Devices.Add(D);
	}

	{
		FSensorDeviceSpec D;
		D.Name             = TEXT("livox_mid360");
		D.Description      = TEXT("Livox Mid-360: non-repetitive 360x59deg pattern, 40 m");
		D.MeshShape        = EDeviceMeshShape::Cylinder;
		D.MeshDimensionsCm = FVector(6.5, 6.5, 6.0);

		FDeviceSensorEntry Lidar;
		Lidar.Type      = SensorType::LIDAR;
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
		D.Name             = TEXT("garmin_lidar_lite_v3");
		D.Description      = TEXT("Garmin LIDAR-Lite v3 rangefinder, 40 m, downward");
		D.MeshShape        = EDeviceMeshShape::Box;
		D.MeshDimensionsCm = FVector(2.0, 4.8, 4.0);

		FDeviceSensorEntry Finder;
		Finder.Type      = SensorType::RANGEFINDER;
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
		Event.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureEvent(S, O, R, 70, 346, 260, 0.25); };
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
		Event.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureEvent(S, O, R, 70, 1280, 720, 0.25); };
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
		Fisheye.Configure = [](USensor* S, const FVector& O, const FRotator& R) { ConfigureFisheye(S, O, R, 220, 1024, 1024, EFisheyeLensModel::EQUIDISTANT); };
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
