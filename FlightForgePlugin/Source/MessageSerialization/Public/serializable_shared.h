// Copyright [2022] <Jakub Jirkal>
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#include <array>
#include <vector>
#include <string>

#include <cereal/types/array.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

namespace Serializable
{
namespace Common
{
enum MessageType : unsigned short
{
  ping = 0x1
};

/* NetworkRequest //{ */

struct NetworkRequest
{
  NetworkRequest() = default;
  explicit NetworkRequest(unsigned short _type) : type(_type) {
  }

  unsigned short type;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(type);
  }
};

//}

/* NetworkResponse //{ */

struct NetworkResponse
{
  NetworkResponse() = default;
  explicit NetworkResponse(unsigned short _type) : type(_type), status(true) {
  }
  explicit NetworkResponse(bool _status) : status(_status) {
  }
  explicit NetworkResponse(unsigned short _type, bool _status) : type(_type), status(_status) {
  }

  unsigned short type;
  bool           status;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(type, status);
  }
};

//}

/* Ping //{ */

namespace Ping
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::ping)) {
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::ping)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::ping, _status) {
  }
};
}  // namespace Ping

//}

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
  add_sensor                      = 24,
  remove_sensor                   = 25,
  list_sensors                    = 26,
  get_event_camera_data           = 27,
  get_event_camera_config         = 28,
  set_event_camera_config         = 29,
  get_fisheye_camera_data         = 30,
  get_fisheye_camera_config       = 31,
  set_fisheye_camera_config       = 32,
  add_device                      = 33,
  list_devices                    = 34,
};

/* struct LidarConfig //{ */

struct LidarConfig
{
  bool   Enable;
  bool   ShowBeams;
  double BeamLength;

  int BeamHorRays;
  int BeamVertRays;

  double Frequency;

  double OffsetX;
  double OffsetY;
  double OffsetZ;

  double OrientationPitch;
  double OrientationYaw;
  double OrientationRoll;

  double FOVHorLeft;
  double FOVHorRight;
  double FOVVertUp;
  double FOVVertDown;
  bool Livox;

  // Which non-repetitive scan pattern to replay when Livox is set: "avia",
  // "mid360", or empty for none. Matches the Content/Lidar/<name>.ffpat stem,
  // so a new sensor is a new pattern file and needs no protocol change.
  std::string LivoxSensor;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(Enable, ShowBeams, BeamLength, BeamHorRays, BeamVertRays, Frequency, OffsetX, OffsetY, OffsetZ, OrientationPitch, OrientationYaw, OrientationRoll,
            FOVHorLeft, FOVHorRight, FOVVertUp, FOVVertDown, Livox, LivoxSensor);
  }
};

//}

/* struct CameraIntrinsics //{ */

// pinhole intrinsics in pixels, image origin top-left. With use_custom_ unset the
// fov_/resolution path drives the projection and Get returns the values the engine
// derives from them; when set, the projection matrix is built from fx/fy/cx/cy
// directly (off-axis when cx/cy are off-center) and fov_ is ignored.
struct CameraIntrinsics
{
  bool   use_custom_ = false;
  double fx_         = 0.0;
  double fy_         = 0.0;
  double cx_         = 0.0;
  double cy_         = 0.0;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(use_custom_, fx_, fy_, cx_, cy_);
  }
};

//}

/* struct CameraDistortion //{ */

// Brown-Conrady lens distortion, OpenCV convention (radial k1..k3, tangential p1, p2),
// applied in the normalized coordinates described by the intrinsics. The server
// renders an automatically-sized overscan frustum and warps, so segmentation stays
// pixel-exact (nearest-neighbor) and RGB is bilinear.
struct CameraDistortion
{
  bool   enable_ = false;
  double k1_     = 0.0;
  double k2_     = 0.0;
  double k3_     = 0.0;
  double p1_     = 0.0;
  double p2_     = 0.0;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(enable_, k1_, k2_, k3_, p1_, p2_);
  }
};

//}

/* struct CameraNoise //{ */

// Sensor noise applied in the linearized domain after the geometric warps:
// heteroscedastic Gaussian approximating shot + read noise
// (sigma = sqrt(shot_scale * signal + read_sigma^2), signal in [0,1]),
// plus optional per-row offsets (CMOS banding). Deterministic per frame stamp.
struct CameraNoise
{
  bool   enable_     = false;
  double shot_scale_ = 2.0e-4;
  double read_sigma_ = 3.0e-3;
  double row_sigma_  = 0.0;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(enable_, shot_scale_, read_sigma_, row_sigma_);
  }
};

//}

/* struct CameraRollingShutter //{ */

// First-order rolling shutter: each row is shifted by the rotational image flow
// accumulated over its readout delay (top row -readout/2, bottom +readout/2),
// using the camera's angular velocity measured between captures. Translation
// (parallax) is ignored - the standard gyro-only approximation.
struct CameraRollingShutter
{
  bool   enable_       = false;
  double readout_time_ = 0.03;  // seconds, top-to-bottom

  template <class Archive>
  void serialize(Archive& archive) {
    archive(enable_, readout_time_);
  }
};

//}

/* struct CameraExposure //{ */

// physically-based exposure; when manual_ is set the abstract auto-exposure is
// replaced by the EV100 model driven by shutter time, ISO and f-stop
struct CameraExposure
{
  bool   manual_;
  double shutter_time_;     // seconds
  double iso_;
  double ev_compensation_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(manual_, shutter_time_, iso_, ev_compensation_);
  }
};

//}

/* struct CameraLensEffects //{ */

// artistic-model lens effects (UE's vignette/CA are not calibrated physics -
// suited to domain randomization, not to matching a specific lens)
struct CameraLensEffects
{
  double fstop_;
  double focal_distance_;   // meters; <= 0 disables depth of field
  double sensor_width_mm_;
  double vignette_intensity_;
  double chromatic_aberration_;
  double bloom_intensity_;
  double lens_flare_intensity_;
  double white_temp_;       // Kelvin; <= 0 keeps engine default
  double white_tint_;

  // derive motion blur length from shutter_time_/frame time instead of the
  // free-standing motion_blur_amount_
  bool motion_blur_from_shutter_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(fstop_, focal_distance_, sensor_width_mm_, vignette_intensity_, chromatic_aberration_, bloom_intensity_, lens_flare_intensity_, white_temp_,
            white_tint_, motion_blur_from_shutter_);
  }
};

//}

/* struct RgbCameraConfig //{ */

struct RgbCameraConfig
{
  bool show_debug_camera_;

  double offset_x_;
  double offset_y_;
  double offset_z_;

  double rotation_pitch_;
  double rotation_yaw_;
  double rotation_roll_;

  double fov_;

  int width_;
  int height_;

  bool enable_temporal_aa_;
  bool enable_raytracing_;
  bool enable_hdr_;

  bool     enable_motion_blur_;
  double   motion_blur_amount_;
  double   motion_blur_distortion_;

  CameraExposure      exposure_;
  CameraLensEffects   lens_;
  CameraIntrinsics    intrinsics_;
  CameraDistortion    distortion_;
  CameraNoise         noise_;
  CameraRollingShutter rolling_shutter_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(show_debug_camera_, offset_x_, offset_y_, offset_z_, rotation_pitch_, rotation_yaw_, rotation_roll_, fov_, width_, height_, enable_temporal_aa_,
            enable_raytracing_, enable_hdr_, enable_motion_blur_, motion_blur_amount_, motion_blur_distortion_, exposure_, lens_, intrinsics_, distortion_,
            noise_, rolling_shutter_);
  }
};

//}

/* struct StereoCameraConfig //{ */

struct StereoCameraConfig
{
  bool show_debug_camera_;

  double offset_x_left_;
  double offset_y_left_;
  double offset_z_left_;

  double offset_x_right_;
  double offset_y_right_;
  double offset_z_right_;

  double rotation_pitch_left_;
  double rotation_yaw_left_;
  double rotation_roll_left_;

  double rotation_pitch_right_;
  double rotation_yaw_right_;
  double rotation_roll_right_;

  double fov_;

  int width_;
  int height_;

  bool enable_temporal_aa_;
  bool enable_raytracing_;
  bool enable_hdr_;

  // shared by both eyes
  CameraIntrinsics intrinsics_;
  CameraDistortion distortion_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(show_debug_camera_,
            offset_x_left_, offset_y_left_, offset_z_left_,
            offset_x_right_, offset_y_right_, offset_z_right_,
            rotation_pitch_left_, rotation_yaw_left_, rotation_roll_left_,
            rotation_pitch_right_, rotation_yaw_right_, rotation_roll_right_,
            fov_, width_, height_,
            enable_temporal_aa_, enable_raytracing_, enable_hdr_, intrinsics_, distortion_);
  }
};

//}

/* GetLocation //{ */

namespace GetLocation
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_location)) {
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_location)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_location, _status) {
  }

  double x;
  double y;
  double z;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), x, y, z);
  }
};
}  // namespace GetLocation

//}

/* SetLocation //{ */

namespace SetLocation
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::set_location)) {
  }

  double x;
  double y;
  double z;
  bool   checkCollisions;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), x, y, z, checkCollisions);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::set_location)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_location, _status) {
  }

  double teleportedToX;
  double teleportedToY;
  double teleportedToZ;

  bool   isHit;
  double impactPointX;
  double impactPointY;
  double impactPointZ;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), teleportedToX, teleportedToY, teleportedToZ, isHit, impactPointX, impactPointY, impactPointZ);
  }
};
}  // namespace SetLocation

//}

/* GetRgbCameraData //{ */

namespace GetRgbCameraData
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_rgb_camera_data)) {
  }

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), sensor_id_);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_rgb_camera_data)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_rgb_camera_data, _status) {
  }

  std::vector<unsigned char> image_;
  double                     stamp_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), image_, stamp_);
  }
};
}  // namespace GetRgbCameraData

//}

/* GetRgbSegCameraData //{ */

namespace GetRgbSegCameraData
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_rgb_seg_camera_data)) {
  }

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), sensor_id_);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_rgb_seg_camera_data)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_rgb_seg_camera_data, _status) {
  }

  std::vector<unsigned char> image_;
  double                     stamp_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), image_, stamp_);
  }
};
}  // namespace GetRgbSegCameraData

//}

/* GetStereoCameraData //{ */

namespace GetStereoCameraData
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_stereo_camera_data)) {
  }

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), sensor_id_);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_stereo_camera_data)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_stereo_camera_data, _status) {
  }

  std::vector<unsigned char> image_left_;
  std::vector<unsigned char> image_right_;
  double                     stamp_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), image_left_, image_right_, stamp_);
  }
};
}  // namespace GetStereoCameraData

//}

/* GetRotation //{ */

namespace GetRotation
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_rotation)) {
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_rotation)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_rotation, _status) {
  }

  double pitch;
  double yaw;
  double roll;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), pitch, yaw, roll);
  }
};
}  // namespace GetRotation

//}

/* SetRotation //{ */

namespace SetRotation
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::set_rotation)) {
  }

  double pitch;
  double yaw;
  double roll;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), pitch, yaw, roll);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::set_rotation)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_rotation, _status) {
  }

  double rotatedToPitch;
  double rotatedToYaw;
  double rotatedToRoll;

  bool   isHit;
  double impactPointX;
  double impactPointY;
  double impactPointZ;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), rotatedToPitch, rotatedToYaw, rotatedToRoll, isHit, impactPointX, impactPointY, impactPointZ);
  }
};
}  // namespace SetRotation

//}

/* SetLocationAndRotation //{ */

namespace SetLocationAndRotation
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::set_location_and_rotation)) {
  }

  double x;
  double y;
  double z;

  double pitch;
  double yaw;
  double roll;

  bool should_collide;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), x, y, z, pitch, yaw, roll, should_collide);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::set_location_and_rotation)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_location_and_rotation, _status) {
  }

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

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), teleportedToX, teleportedToY, teleportedToZ, rotatedToPitch, rotatedToYaw, rotatedToRoll, isHit,
            impactPointX, impactPointY, impactPointZ);
  }
};
}  // namespace SetLocationAndRotation

//}

/* SetLocationAndRotationAsync //{ */

namespace SetLocationAndRotationAsync
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::set_location_and_rotation_async)) {
  }

  double x;
  double y;
  double z;

  double pitch;
  double yaw;
  double roll;

  bool should_collide;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), x, y, z, pitch, yaw, roll, should_collide);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::set_location_and_rotation_async)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_location_and_rotation_async, _status) {
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this));
  }
};
}  // namespace SetLocationAndRotationAsync

//}

namespace GetRangefinderData
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_rangefinder_data)){};
  
  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), sensor_id_);
  }
};

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_rangefinder_data)){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_rangefinder_data, _status){};

    double range;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkResponse>(this), range);
    }
  };
}  // namespace GetRangefinderData

/* GetLidarData //{ */

namespace GetLidarData
{
struct LidarData
{
  LidarData() = default;

  double distance;
  double directionX;
  double directionY;
  double directionZ;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(distance, directionX, directionY, directionZ);
  }
};

struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_lidar_data)) {
  }

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), sensor_id_);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_lidar_data)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_lidar_data, _status) {
  }

  double startX;
  double startY;
  double startZ;

  std::vector<LidarData> lidarData;
  
  double                     stamp_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), startX, startY, startZ, lidarData, stamp_);
  }
};
}  // namespace GetLidarData

//}

/* GetLidarSegData //{ */

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
  template <class Archive>
  void serialize(Archive& archive) {
    archive(distance, directionX, directionY, directionZ, segmentation);
  }
};

struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_lidar_seg)) {
  }

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), sensor_id_);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_lidar_seg)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_lidar_seg, _status) {
  }

  double startX;
  double startY;
  double startZ;

  std::vector<LidarSegData> lidarSegData;

  double                     stamp_;
  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), startX, startY, startZ, lidarSegData, stamp_);
  }
};
}  // namespace GetLidarSegData

//}

/* GetLidarIntData //{ */

namespace GetLidarIntData
{
struct LidarIntData
{
  LidarIntData() = default;

  double distance;
  double directionX;
  double directionY;
  double directionZ;
  int    intensity;
  template <class Archive>
  void serialize(Archive& archive) {
    archive(distance, directionX, directionY, directionZ, intensity);
  }
};

struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_lidar_int)) {
  }

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), sensor_id_);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_lidar_int)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_lidar_int, _status) {
  }

  double startX;
  double startY;
  double startZ;

  std::vector<LidarIntData> lidarIntData;
  
  double                     stamp_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), startX, startY, startZ, lidarIntData, stamp_);
  }
};
}  // namespace GetLidarIntData

//}

/* GetLidarConfig //{ */

namespace GetLidarConfig
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_lidar_config)){};

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), sensor_id_);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_lidar_config)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_lidar_config, _status){};

  LidarConfig config;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), config);
  }
};
}  // namespace GetLidarConfig

//}

/* SetLidarConfig //{ */

namespace SetLidarConfig
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::set_lidar_config)){};

  int sensor_id_ = -1;

  LidarConfig config;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), sensor_id_, config);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::set_lidar_config)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_lidar_config, _status){};
};
}  // namespace SetLidarConfig

//}

/* GetRgbCameraConfig //{ */

namespace GetRgbCameraConfig
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_rgb_camera_config)){};

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), sensor_id_);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_rgb_camera_config)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_rgb_camera_config, _status){};

  RgbCameraConfig config;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), config);
  }
};
}  // namespace GetRgbCameraConfig

//}

/* SetRgbCameraConfig //{ */

namespace SetRgbCameraConfig
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::set_rgb_camera_config)){};

  int sensor_id_ = -1;

  RgbCameraConfig config;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), sensor_id_, config);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::set_rgb_camera_config)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_rgb_camera_config, _status){};
};
}  // namespace SetRgbCameraConfig

//}

/* GetStereoCameraConfig //{ */

namespace GetStereoCameraConfig
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_stereo_camera_config)){};

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), sensor_id_);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_stereo_camera_config)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_stereo_camera_config, _status){};

  StereoCameraConfig config;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), config);
  }
};
}  // namespace GetStereoCameraConfig

//}

/* SetStereoCameraConfig //{ */

namespace SetStereoCameraConfig
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::set_stereo_camera_config)){};

  int sensor_id_ = -1;

  StereoCameraConfig config;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), sensor_id_, config);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::set_stereo_camera_config)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_stereo_camera_config, _status){};
};
}  // namespace SetStereoCameraConfig

//}

/* AddSensor //{ */

// sensor_type_ matches the plugin's SensorType enum: 0 rgb camera, 1 lidar,
// 2 rangefinder, 3 livox lidar, 4 rgb segmentation camera, 5 stereo camera
namespace AddSensor
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::add_sensor)){};

  int sensor_type_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), sensor_type_);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::add_sensor)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::add_sensor, _status){};

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), sensor_id_);
  }
};
}  // namespace AddSensor

//}

/* RemoveSensor //{ */

namespace RemoveSensor
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::remove_sensor)){};

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), sensor_id_);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::remove_sensor)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::remove_sensor, _status){};
};
}  // namespace RemoveSensor

//}

/* ListSensors //{ */

namespace ListSensors
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::list_sensors)){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::list_sensors)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::list_sensors, _status){};

  std::vector<int> sensor_ids_;
  std::vector<int> sensor_types_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), sensor_ids_, sensor_types_);
  }
};
}  // namespace ListSensors

//}

/* EventCameraConfig //{ */

struct EventCameraConfig
{
  bool show_debug_camera_;

  double offset_x_;
  double offset_y_;
  double offset_z_;

  double rotation_pitch_;
  double rotation_yaw_;
  double rotation_roll_;

  double fov_;

  int width_;
  int height_;

  // log-intensity contrast thresholds; an event fires each time the pixel's
  // log intensity moves by one threshold since its last event
  double contrast_threshold_pos_;
  double contrast_threshold_neg_;

  // stamp events with simulation time instead of wall clock; pair with a fixed
  // engine timestep to get an exact-rate event stream regardless of GPU load
  bool use_sim_time_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(show_debug_camera_, offset_x_, offset_y_, offset_z_, rotation_pitch_, rotation_yaw_, rotation_roll_, fov_, width_, height_,
            contrast_threshold_pos_, contrast_threshold_neg_, use_sim_time_);
  }
};

//}

/* GetEventCameraData //{ */

namespace GetEventCameraData
{
struct Event
{
  Event() = default;

  unsigned short x;
  unsigned short y;
  signed char    polarity;  // +1 brighter, -1 darker
  double         stamp;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(x, y, polarity, stamp);
  }
};

struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_event_camera_data)){};

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), sensor_id_);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_event_camera_data)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_event_camera_data, _status){};

  std::vector<Event> events_;
  double             stamp_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), events_, stamp_);
  }
};
}  // namespace GetEventCameraData

//}

/* GetEventCameraConfig //{ */

namespace GetEventCameraConfig
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_event_camera_config)){};

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), sensor_id_);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_event_camera_config)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_event_camera_config, _status){};

  EventCameraConfig config;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), config);
  }
};
}  // namespace GetEventCameraConfig

//}

/* SetEventCameraConfig //{ */

namespace SetEventCameraConfig
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::set_event_camera_config)){};

  int sensor_id_ = -1;

  EventCameraConfig config;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), sensor_id_, config);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::set_event_camera_config)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_event_camera_config, _status){};
};
}  // namespace SetEventCameraConfig

//}

/* FisheyeCameraConfig //{ */

struct FisheyeCameraConfig
{
  bool show_debug_camera_;

  double offset_x_;
  double offset_y_;
  double offset_z_;

  double rotation_pitch_;
  double rotation_yaw_;
  double rotation_roll_;

  // full diagonal field of view in degrees, up to 220
  double fov_;

  int width_;
  int height_;

  // 0 equidistant (r = f*theta), 1 equisolid (r = 2f*sin(theta/2)),
  // 2 stereographic (r = 2f*tan(theta/2))
  int lens_model_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(show_debug_camera_, offset_x_, offset_y_, offset_z_, rotation_pitch_, rotation_yaw_, rotation_roll_, fov_, width_, height_, lens_model_);
  }
};

//}

/* GetFisheyeCameraData //{ */

namespace GetFisheyeCameraData
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_fisheye_camera_data)){};

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), sensor_id_);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_fisheye_camera_data)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_fisheye_camera_data, _status){};

  std::vector<unsigned char> image_;
  double                     stamp_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), image_, stamp_);
  }
};
}  // namespace GetFisheyeCameraData

//}

/* GetFisheyeCameraConfig //{ */

namespace GetFisheyeCameraConfig
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_fisheye_camera_config)){};

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), sensor_id_);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_fisheye_camera_config)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_fisheye_camera_config, _status){};

  FisheyeCameraConfig config;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), config);
  }
};
}  // namespace GetFisheyeCameraConfig

//}

/* SetFisheyeCameraConfig //{ */

namespace SetFisheyeCameraConfig
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::set_fisheye_camera_config)){};

  int sensor_id_ = -1;

  FisheyeCameraConfig config;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), sensor_id_, config);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::set_fisheye_camera_config)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_fisheye_camera_config, _status){};
};
}  // namespace SetFisheyeCameraConfig

//}

/* AddDevice //{ */

// Instantiates a preconfigured real-world sensor device (see list_devices) at
// a mount pose on the drone. A device may create several sensors (e.g. a
// RealSense D435i is a stereo pair plus an RGB camera); their ids come back in
// creation order. show_mesh_ attaches a roughly device-sized placeholder mesh
// at the mount pose for visual debugging.
namespace AddDevice
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::add_device)){};

  std::string device_name_;

  double offset_x_;
  double offset_y_;
  double offset_z_;

  double rotation_pitch_;
  double rotation_yaw_;
  double rotation_roll_;

  bool show_mesh_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), device_name_, offset_x_, offset_y_, offset_z_, rotation_pitch_, rotation_yaw_, rotation_roll_,
            show_mesh_);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::add_device)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::add_device, _status){};

  std::vector<int> sensor_ids_;
  std::vector<int> sensor_types_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), sensor_ids_, sensor_types_);
  }
};
}  // namespace AddDevice

//}

/* ListDevices //{ */

namespace ListDevices
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::list_devices)){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::list_devices)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::list_devices, _status){};

  std::vector<std::string> names_;
  std::vector<std::string> descriptions_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), names_, descriptions_);
  }
};
}  // namespace ListDevices

//}

/* GetMoveLineVisible //{ */

namespace GetMoveLineVisible
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_move_line_visible)){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_move_line_visible)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_move_line_visible, _status){};

  bool visible;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), visible);
  }
};
}  // namespace GetMoveLineVisible

//}

/* SetMoveLineVisible //{ */

namespace SetMoveLineVisible
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::set_move_line_visible)){};

  bool visible;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), visible);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::set_move_line_visible)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_move_line_visible, _status){};
};
}  // namespace SetMoveLineVisible

//}

/* GetCrashState //{ */

namespace GetCrashState
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_crash_state)) {
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_crash_state)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_crash_state, _status) {
  }

  bool crashed;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), crashed);
  }
};
}  // namespace GetCrashState

//}

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
  spawn_drone_at_location = 15,
  set_weather             = 16,
  set_daytime             = 17,
  set_mutual_visibility   = 18,
  spawn_objects_from_yaml = 19,
  remove_spawned_object   = 20,
  move_spawned_object     = 21,
  list_spawned_objects    = 22,
  export_scene            = 23,
  list_assets             = 24
};

// Scene-editor poses travel in the same frame as the spawn YAML itself: a right-handed
// (ROS) frame relative to the world origin, metres and degrees. Orientation is roll,
// pitch, yaw.
struct SpawnedObjectInfo
{
  int         id;
  std::string asset;
  int         stencil;

  std::array<double, 3> position;
  std::array<double, 3> orientation;
  std::array<double, 3> scale;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(id, asset, stencil, position, orientation, scale);
  }
};

namespace GetDrones
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_drones)) {
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_drones)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_drones, _status) {
  }

  std::vector<int> ports;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), ports);
  }
};
}  // namespace GetDrones

namespace SetForestDensity
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::set_forest_density){};

  int Density_Level;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), Density_Level);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::set_forest_density)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_forest_density, _status){};
};
};  // namespace SetForestDensity

namespace SetForestHillyLevel
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::set_forest_hilly_level){}
  int Hilly_Level;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), Hilly_Level);
  }
};
struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::set_forest_hilly_level)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_forest_hilly_level, _status){};
};
};  // namespace SetForestHilly


namespace SpawnDrone
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::spawn_drone)) {
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::spawn_drone)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::spawn_drone, _status) {
  }

  int port;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), port);
  }
};
}  // namespace SpawnDrone

namespace SpawnDroneAtLocation
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(MessageType::spawn_drone_at_location){};

    double x;
    double y;
    double z;
    std::string MeshName;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkRequest>(this), x, y, z, MeshName);
    }
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::spawn_drone_at_location)){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::spawn_drone, _status){};

    int port;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkResponse>(this), port);
    }
  };
};  // namespace SpawnDrone

namespace RemoveDrone
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::remove_drone) {
  }

  int port;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), port);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::remove_drone)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::remove_drone, _status) {
  }
};
}  // namespace RemoveDrone


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
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_camera_capture_mode)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_camera_capture_mode, _status){};

  CameraCaptureModeEnum cameraCaptureMode;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), cameraCaptureMode);
  }
};
}  // namespace GetCameraCaptureMode

namespace SetCameraCaptureMode
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::set_camera_capture_mode){};

  CameraCaptureModeEnum cameraCaptureMode;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), cameraCaptureMode);
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::set_camera_capture_mode)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_camera_capture_mode, _status){};
};
}  // namespace SetCameraCaptureMode

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

    int graphicsSettings;

    template <class Archive>
    void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), graphicsSettings);
    }
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::set_graphics_settings)){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_graphics_settings, _status){};
  };

}// namespace SetGraphicsSettings

enum WorldLevelEnum : short
{
  VALLEY,
  FOREST,
  INFINITE_FOREST,
  WAREHOUSE,
  CAVE,
  ERDING_AIRBASE,
  TEMESVAR,
  EletricTowers,
  Race_1,
  Race_2
};
  
namespace SetWeather
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(MessageType::set_weather){};

    int weather_id;

    template <class Archive>
    void serialize(Archive& archive)
    {
      archive(cereal::base_class<Common::NetworkRequest>(this), weather_id);
    }
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::set_weather)){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_weather, _status){};
  };
}

  namespace SetDaytime
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(MessageType::set_daytime){};

    int hour;
    int minute;
    
    template <class Archive>
    void serialize(Archive& archive)
    {
      archive(cereal::base_class<Common::NetworkRequest>(this), hour, minute);
    }
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::set_daytime)){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_daytime, _status){};
  };
}  
  
namespace SwitchWorldLevel
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(MessageType::switch_world_level){};

    std::string worldLevelName;

    template <class Archive>
    void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), worldLevelName);
    }
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::switch_world_level)){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::switch_world_level, _status){};
  };
}// namespace SwitchWorldLevel

namespace GetFps
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::get_fps){};
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_fps)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_fps, _status){};

  float fps;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), fps);
  }
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
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_api_version)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_api_version, _status){};

  int api_version_major;
  int api_version_minor;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), api_version_major, api_version_minor);
  }
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
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_time)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_time, _status){};

  double time;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), time);
  }
};
}  // namespace GetTime

namespace GetWorldOrigin
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(MessageType::get_world_origin){};
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_world_origin)){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_world_origin, _status){};

    double x;
    double y;
    double z;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkResponse>(this), x, y, z);
    }
  };
}  // namespace GetWorldOrigin

namespace SetMutualVisibility
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(MessageType::set_mutual_visibility){};

    bool mutual_visibiliti_enabled;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkRequest>(this), mutual_visibiliti_enabled);
    }
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::set_mutual_visibility)){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_mutual_visibility, _status){};
  };
}

/*SpawnObjectsFromYaml//{*/
namespace SpawnObjectsFromYaml
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(MessageType::spawn_objects_from_yaml){};

    // A whole spawn configuration document; see the spawn tool for the schema.
    std::string yaml;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkRequest>(this), yaml);
    }
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::spawn_objects_from_yaml)){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::spawn_objects_from_yaml, _status){};

    // Handles of the objects that were placed, for later removal.
    std::vector<int> object_ids;

    // Empty when status is true.
    std::string error;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkResponse>(this), object_ids, error);
    }
  };
}
/*//}*/

/*RemoveSpawnedObject//{*/
namespace RemoveSpawnedObject
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(MessageType::remove_spawned_object){};

    // A negative id removes every object placed by the spawn tool.
    int object_id;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkRequest>(this), object_id);
    }
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::remove_spawned_object)){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::remove_spawned_object, _status){};

    int removed_count;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkResponse>(this), removed_count);
    }
  };
}
/*//}*/

/*MoveSpawnedObject//{*/
namespace MoveSpawnedObject
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(MessageType::move_spawned_object){};

    int object_id;

    // ROS frame relative to the world origin; metres, degrees (roll, pitch, yaw).
    std::array<double, 3> position;
    std::array<double, 3> orientation;
    std::array<double, 3> scale = {1.0, 1.0, 1.0};

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkRequest>(this), object_id, position, orientation, scale);
    }
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::move_spawned_object)){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::move_spawned_object, _status){};

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkResponse>(this));
    }
  };
}
/*//}*/

/*ListSpawnedObjects//{*/
namespace ListSpawnedObjects
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(MessageType::list_spawned_objects){};
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::list_spawned_objects)){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::list_spawned_objects, _status){};

    // Current poses, so moves made since spawning are reflected.
    std::vector<SpawnedObjectInfo> objects;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkResponse>(this), objects);
    }
  };
}
/*//}*/

/*ExportScene//{*/
namespace ExportScene
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(MessageType::export_scene){};
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::export_scene)){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::export_scene, _status){};

    // A spawn configuration document reproducing the currently spawned objects,
    // including stencils, so feeding it back to spawn_objects_from_yaml recreates
    // the scene exactly.
    std::string yaml;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkResponse>(this), yaml);
    }
  };
}
/*//}*/

/*ListAssets//{*/
namespace ListAssets
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(MessageType::list_assets){};

    // Package path prefix to search under; empty means /Game.
    std::string path_prefix;

    // Case-insensitive substring filter on the asset name; empty matches everything.
    std::string name_filter;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkRequest>(this), path_prefix, name_filter);
    }
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::list_assets)){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::list_assets, _status){};

    // Spawnable names: cooked object paths plus bare names of on-disk glTF models,
    // all accepted verbatim by the spawn tool's 'asset' field. Capped server-side;
    // 'truncated' says the cap was hit.
    std::vector<std::string> assets;
    bool                     truncated = false;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkResponse>(this), assets, truncated);
    }
  };
}
/*//}*/

}  // namespace GameMode

}  // namespace Serializable
