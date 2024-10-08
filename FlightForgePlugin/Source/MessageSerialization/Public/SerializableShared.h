// Copyright [2022] <Jakub Jirkal>
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <vector>

namespace Serializable
{
namespace Common
{
enum MessageType : unsigned short
{
  ping = 0x1
};

struct NetworkRequest
{
  NetworkRequest() = default;
  explicit NetworkRequest(unsigned short _type) : type(_type){};

  unsigned short type;
};

struct NetworkResponse
{
  NetworkResponse() = default;
  explicit NetworkResponse(unsigned short _type) : type(_type), status(true){};
  explicit NetworkResponse(bool _status) : status(_status){};
  explicit NetworkResponse(unsigned short _type, bool _status) : type(_type), status(_status){};

  unsigned short type;
  bool           status;
};

namespace Ping
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::ping){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::ping){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::ping, _status){};
};
}  // namespace Ping
}  // namespace Common

namespace Drone
{
enum MessageType : unsigned short
{
  get_location                    = 2,
  set_location                    = 3,
  get_rgb_camera_data             = 4,
  get_stereo_camera_data          = 5,
  get_rotation                    = 6,
  set_rotation                    = 7,
  set_location_and_rotation       = 8,
  get_lidar_data                  = 9,
  get_lidar_config                = 10,
  set_lidar_config                = 11,
  get_rgb_camera_config           = 12,
  set_rgb_camera_config           = 13,
  get_stereo_camera_config        = 14,
  set_stereo_camera_config        = 15,
  get_move_line_visible           = 16,
  set_move_line_visible           = 17,
  get_rgb_seg_camera_data         = 18,
  get_lidar_seg                   = 19,
  set_location_and_rotation_async = 20,
  get_crash_state                 = 21,
  get_lidar_int                   = 22,  
  get_rangefinder_data            = 23,
};

struct RangefinderConfig
{
  double beamLength;
  double offsetX;
  double offsetY;
  double offsetZ;
};

struct LidarConfig
{
  bool   Enable;
  bool   showBeams;
  double beamLength;

  int BeamHorRays;
  int BeamVertRays;

  double Frequency;
  double offsetX;
  double offsetY;
  double offsetZ;

  double orientationPitch;
  double orientationYaw;
  double orientationRoll;

  double FOVHor;
  double FOVVert;
};

struct RgbCameraConfig
{
  bool showDebugCamera;

  double offsetX;
  double offsetY;
  double offsetZ;

  double orientationPitch;
  double orientationYaw;
  double orientationRoll;

  double angleFOV;

  int Width;
  int Height;

  bool enable_temporal_aa;
  bool enable_raytracing;
  bool enable_hdr;
};

struct StereoCameraConfig
{
  bool showDebugCamera;

  double offsetX;
  double offsetY;
  double offsetZ;

  double orientationPitch;
  double orientationYaw;
  double orientationRoll;

  double angleFOV;

  int Width;
  int Height;

  double baseline;

  bool enable_temporal_aa;
  bool enable_raytracing;
  bool enable_hdr;
};

namespace GetLocation
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::get_location){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::get_location){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_location, _status){};

  double x;
  double y;
  double z;
};
}  // namespace GetLocation

namespace GetCrashState
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::get_crash_state){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::get_crash_state){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_crash_state, _status){};

  bool crashed;
};
}  // namespace GetCrashState

namespace SetLocation
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::set_location){};

  double x;
  double y;
  double z;
  bool   checkCollisions;
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::set_location){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_location, _status){};

  double teleportedToX;
  double teleportedToY;
  double teleportedToZ;

  bool   isHit;
  double impactPointX;
  double impactPointY;
  double impactPointZ;
};
}  // namespace SetLocation

namespace GetRgbCameraData
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::get_rgb_camera_data){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::get_rgb_camera_data){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_rgb_camera_data, _status){};

  std::vector<unsigned char> image_data_;
  double                     stamp_;
};
}  // namespace GetRgbCameraData

namespace GetStereoCameraData
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::get_stereo_camera_data){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::get_stereo_camera_data){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_stereo_camera_data, _status){};

  std::vector<unsigned char> image_left_;
  std::vector<unsigned char> image_right_;
  double                     stamp_;
};
}  // namespace GetStereoCameraData

namespace GetRgbSegCameraData
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::get_rgb_seg_camera_data){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::get_rgb_seg_camera_data){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_rgb_seg_camera_data, _status){};

  std::vector<unsigned char> imageData;
  double                     stamp_;
};
}  // namespace GetRgbSegCameraData

namespace GetRotation
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::get_rotation){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::get_rotation){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_rotation, _status){};

  double pitch;
  double yaw;
  double roll;
};
}  // namespace GetRotation

namespace SetRotation
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::set_rotation){};

  double pitch;
  double yaw;
  double roll;
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::set_rotation){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_rotation, _status){};

  double rotatedToPitch;
  double rotatedToYaw;
  double rotatedToRoll;

  bool   isHit;
  double impactPointX;
  double impactPointY;
  double impactPointZ;
};
}  // namespace SetRotation

namespace SetLocationAndRotation
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::set_location_and_rotation){};

  double x;
  double y;
  double z;

  double pitch;
  double yaw;
  double roll;

  bool should_collide;
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::set_location_and_rotation){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_location_and_rotation, _status){};

  double teleportedToX;
  double teleportedToY;
  double teleportedToZ;

  double rotatedToPitch;
  double rotatedToYaw;
  double rotatedToRoll;

  bool   isHit;
  double impactPointX;
  double impactPointY;
  double impactPointZ;
};
}  // namespace SetLocationAndRotation

namespace SetLocationAndRotationAsync
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::set_location_and_rotation_async){};

  double x;
  double y;
  double z;

  double pitch;
  double yaw;
  double roll;

  bool should_collide;
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::set_location_and_rotation_async){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_location_and_rotation_async, _status){};
};
}  // namespace SetLocationAndRotationAsync

namespace GetLidarData
{
struct LidarData
{
  LidarData() = default;

  double distance;
  double directionX;
  double directionY;
  double directionZ;
};

struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::get_lidar_data){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::get_lidar_data){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_lidar_data, _status){};

  double startX;
  double startY;
  double startZ;

  std::vector<LidarData> lidarData;
};
}  // namespace GetLidarData

namespace GetRangefinderData
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(MessageType::get_rangefinder_data){};
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(MessageType::get_rangefinder_data){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_rangefinder_data, _status){};

    double range;
  };
}  // namespace GetRangefinderData

namespace GetLidarSegData
{
struct LidarSegData
{
  LidarSegData() = default;

  double distance;
  double directionX;
  double directionY;
  double directionZ;
  int    segmentation;
};

struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::get_lidar_seg){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::get_lidar_seg){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_lidar_seg, _status){};

  double startX;
  double startY;
  double startZ;

  std::vector<LidarSegData> lidarSegData;
};
}  // namespace GetLidarSegData


namespace GetLidarIntData
{
struct LidarIntData
{
  LidarIntData() = default;

  double distance;
  double directionX;
  double directionY;
  double directionZ;
  int intensity;
};

struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::get_lidar_int){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::get_lidar_int){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_lidar_int, _status){};

  double startX;
  double startY;
  double startZ;

  std::vector<LidarIntData> lidarIntData;
};
}  // namespace GetLidarIntData


namespace GetLidarConfig
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::get_lidar_config){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::get_lidar_config){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_lidar_config, _status){};

  LidarConfig config;
};
}  // namespace GetLidarConfig

namespace SetLidarConfig
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::set_lidar_config){};

  LidarConfig config;
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::set_lidar_config){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_lidar_config, _status){};
};
}  // namespace SetLidarConfig

namespace GetRgbCameraConfig
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::get_rgb_camera_config){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::get_rgb_camera_config){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_rgb_camera_config, _status){};

  RgbCameraConfig config;
};
}  // namespace GetRgbCameraConfig

namespace GetStereoCameraConfig
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::get_stereo_camera_config){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::get_stereo_camera_config){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_stereo_camera_config, _status){};

  StereoCameraConfig config;
};
}  // namespace GetStereoCameraConfig

namespace SetRgbCameraConfig
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::set_rgb_camera_config){};

  RgbCameraConfig config;
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::set_rgb_camera_config){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_rgb_camera_config, _status){};
};
}  // namespace SetRgbCameraConfig

namespace SetStereoCameraConfig
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::set_stereo_camera_config){};

  StereoCameraConfig config;
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::set_stereo_camera_config){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_stereo_camera_config, _status){};
};
}  // namespace SetStereoCameraConfig

namespace GetMoveLineVisible
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::get_move_line_visible){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::get_move_line_visible){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_move_line_visible, _status){};

  bool visible;
};
}  // namespace GetMoveLineVisible

namespace SetMoveLineVisible
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::set_move_line_visible){};

  bool visible;
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::set_move_line_visible){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_move_line_visible, _status){};
};
}  // namespace SetMoveLineVisible

}  // namespace Drone

namespace GameMode
{
enum MessageType : unsigned short
{
  get_drones              = 2,
  spawn_drone             = 3,
  remove_drone            = 4,
  get_camera_capture_mode = 5,
  set_camera_capture_mode = 6,
  get_fps                 = 7,
  get_time                = 8,
  get_api_version         = 9,
  set_graphics_settings   = 10,
  switch_world_level      = 11,
  set_forest_density      = 12,
  set_forest_hilly_level  = 13,
  get_world_origin        = 14,
  spawn_drone_at_location = 15
};

namespace SetForestDensity
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::set_forest_density){};

  int Density_Level;
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::set_forest_density){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_forest_density, _status){};
};
};  // namespace SetForestDensity


namespace SetForestHillyLevel
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::set_forest_hilly_level){}
  int Hilly_Level;
};
struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::set_forest_hilly_level){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_forest_hilly_level, _status){};
};
};  // namespace SetForestHilly

namespace GetDrones
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::get_drones){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::get_drones){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_drones, _status){};

  std::vector<int> ports;
};
};  // namespace GetDrones

namespace SpawnDrone
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::spawn_drone){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::spawn_drone){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::spawn_drone, _status){};

  int port;
};
};  // namespace SpawnDrone

namespace SpawnDroneAtLocation
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(MessageType::spawn_drone_at_location){};

    double x;
    double y;
    double z;
    int idMesh;
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(MessageType::spawn_drone_at_location){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::spawn_drone, _status){};

    int port;
  };
};  // namespace SpawnDrone

namespace RemoveDrone
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::remove_drone){};

  int port;
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::remove_drone){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::remove_drone, _status){};
};
};  // namespace RemoveDrone

enum CameraCaptureModeEnum : unsigned short
{
  CAPTURE_ALL_FRAMES  = 0x0,
  CAPTURE_ON_MOVEMENT = 0x1,
  CAPTURE_ON_DEMAND   = 0x2,
};

namespace GetCameraCaptureMode
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::get_camera_capture_mode){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::get_camera_capture_mode){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_camera_capture_mode, _status){};

  CameraCaptureModeEnum cameraCaptureMode;
};
}  // namespace GetCameraCaptureMode

namespace SetCameraCaptureMode
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::set_camera_capture_mode){};

  CameraCaptureModeEnum cameraCaptureMode;
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::set_camera_capture_mode){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_camera_capture_mode, _status){};
};
}  // namespace SetCameraCaptureMode

namespace GetFps
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::get_fps){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::get_fps){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_fps, _status){};

  float fps;
};
}  // namespace GetFps

namespace GetApiVersion
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::get_api_version){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::get_api_version){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_api_version, _status){};

  int api_version;
};
}  // namespace GetApiVersion

namespace GetTime
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::get_time){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(MessageType::get_time){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_time, _status){};

  double time;
};
}  // namespace GetTime

enum GraphicsSettingsEnum : unsigned short
{
  LOW = 0,
  MEDIUM = 1,
  HIGH = 2,
  EPIC = 3,
  CINEMATIC = 4
};

namespace SetGraphicsSettings
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(MessageType::set_graphics_settings){};

    GraphicsSettingsEnum  graphicsSettings;
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(MessageType::set_graphics_settings){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_graphics_settings, _status){};
  };

}// namespace SetGraphicsSettings

enum WorldLevelEnum : unsigned short
{
  VALLEY,
  FOREST,
  INFINITE_FOREST,
  WAREHOUSE,
  CAVE,
  ERDING_AIRBASE,
  TEMESVAR
};

namespace SwitchWorldLevel
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(MessageType::switch_world_level){};

    WorldLevelEnum WorldLevelEnum;
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(MessageType::switch_world_level){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::switch_world_level, _status){};
  };
}// namespace SwitchWorldLevel

namespace GetWorldOrigin
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(MessageType::get_world_origin){};
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(MessageType::get_world_origin){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_world_origin, _status){};

    double x;
    double y;
    double z;
  };
}  // namespace GetWorldOrigin

};  // namespace GameMode

}  // namespace Serializable
