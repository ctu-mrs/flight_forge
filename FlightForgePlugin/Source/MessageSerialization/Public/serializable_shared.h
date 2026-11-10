#pragma once

#include <array>
#include <cstdint>
#include <vector>
#include <string>

#include <cereal/types/array.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

namespace Serializable
{
// bump minor for any wire change; get_meta carries per-op schema hashes so clients can also check op by op
constexpr int api_version_major = 0;
constexpr int api_version_minor = 25;

namespace Common
{
// ids 0x80..0xFF are reserved for Common so both servers can answer them
enum MessageType : unsigned short
{
  ping     = 0x1,
  get_meta = 0xFF
};

/* NetworkRequest //{ */

// seq is set by the client and echoed by the server, so a reply that arrives after its
// request timed out is recognised and dropped instead of being read as the next reply
struct NetworkRequest
{
  NetworkRequest() = default;
  explicit NetworkRequest(unsigned short _type) : type(_type) {
  }

  unsigned short type;
  uint32_t       seq = 0;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(CEREAL_NVP(type), CEREAL_NVP(seq));
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
  uint32_t       seq = 0;  // the server patches it in at byte offset 3, so it must stay the third field

  template <class Archive>
  void serialize(Archive& archive) {
    archive(CEREAL_NVP(type), CEREAL_NVP(status), CEREAL_NVP(seq));
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

/* GetMeta //{ */

// one op of the catalogue; hash_ covers the request and response field layout (see serializable_schema.h)
struct OpMeta
{
  unsigned char  scope_ = 0;  // Registry::Scope
  unsigned short id_    = 0;
  std::string    name_;
  std::string    request_;   // canonical signature, e.g. {u16 type,i32 sensor_id_}
  std::string    response_;
  uint64_t       hash_ = 0;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(CEREAL_NVP(scope_), CEREAL_NVP(id_), CEREAL_NVP(name_), CEREAL_NVP(request_), CEREAL_NVP(response_), CEREAL_NVP(hash_));
  }
};

namespace GetMeta
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_meta)) {
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_meta)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_meta, _status) {
  }

  int                 api_version_major_ = 0;
  int                 api_version_minor_ = 0;
  uint64_t            catalogue_hash_    = 0;  // over every op hash in table order
  std::vector<OpMeta> ops_;
  std::string         catalogue_json_;  // the same catalogue with nested field trees, for clients without the C++ header

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(api_version_major_), CEREAL_NVP(api_version_minor_),
            CEREAL_NVP(catalogue_hash_), CEREAL_NVP(ops_), CEREAL_NVP(catalogue_json_));
  }
};
}  // namespace GetMeta

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
  get_instance_seg_data           = 35,
  get_instance_seg_map            = 36,
  get_depth_camera_data           = 37,
  set_pose_trajectory             = 38,
  set_propeller_speeds            = 39,
  get_camera_info                 = 40,
  get_lidar_scan                  = 41,
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

  // Draws the sensor housing at its mount point and lets the beams see the airframe
  // they are bolted to, the way the real hardware occludes itself. Off by default
  // because a sensor left at the frame's origin sits inside the body and would find
  // nothing but itself.
  bool ShowSensorBody = false;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(CEREAL_NVP(Enable), CEREAL_NVP(ShowBeams), CEREAL_NVP(BeamLength), CEREAL_NVP(BeamHorRays), CEREAL_NVP(BeamVertRays),
            CEREAL_NVP(Frequency), CEREAL_NVP(OffsetX), CEREAL_NVP(OffsetY), CEREAL_NVP(OffsetZ), CEREAL_NVP(OrientationPitch),
            CEREAL_NVP(OrientationYaw), CEREAL_NVP(OrientationRoll), CEREAL_NVP(FOVHorLeft), CEREAL_NVP(FOVHorRight), CEREAL_NVP(FOVVertUp),
            CEREAL_NVP(FOVVertDown), CEREAL_NVP(Livox), CEREAL_NVP(LivoxSensor), CEREAL_NVP(ShowSensorBody));
  }
};

//}

/* struct CameraIntrinsics //{ */

// pinhole intrinsics in pixels, image origin top-left, OpenCV convention (a pixel's centre
// is at its integer index, so a centred camera has cx = W/2 - 0.5). With use_custom_ unset
// the fov_/resolution path drives the projection and Get returns the values the engine
// derives from them; when set, the projection matrix is built from fx/fy/cx/cy directly
// (off-axis when cx/cy are off-center) and fov_ is ignored.
struct CameraIntrinsics
{
  bool   use_custom_ = false;
  double fx_         = 0.0;
  double fy_         = 0.0;
  double cx_         = 0.0;
  double cy_         = 0.0;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(CEREAL_NVP(use_custom_), CEREAL_NVP(fx_), CEREAL_NVP(fy_), CEREAL_NVP(cx_), CEREAL_NVP(cy_));
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
    archive(CEREAL_NVP(enable_), CEREAL_NVP(k1_), CEREAL_NVP(k2_), CEREAL_NVP(k3_), CEREAL_NVP(p1_), CEREAL_NVP(p2_));
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
    archive(CEREAL_NVP(enable_), CEREAL_NVP(shot_scale_), CEREAL_NVP(read_sigma_), CEREAL_NVP(row_sigma_));
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
    archive(CEREAL_NVP(enable_), CEREAL_NVP(readout_time_));
  }
};

//}

/* struct CameraCompression //{ */

// Wire format for the served frame. JPEG is the default for imagery: PNG costs tens of
// milliseconds per frame and several times the bytes, for detail no vision pipeline sees.
// The label cameras (segmentation, instance segmentation) ignore this and always send PNG -
// their pixel values ARE the labels, so any lossy codec invents ids that were never rendered.
struct CameraCompression
{
  enum Format : int
  {
    PNG  = 0,
    JPEG = 1,
  };

  int format_  = JPEG;
  int quality_ = 90;  // 1-100, JPEG only

  template <class Archive>
  void serialize(Archive& archive) {
    archive(CEREAL_NVP(format_), CEREAL_NVP(quality_));
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
    archive(CEREAL_NVP(manual_), CEREAL_NVP(shutter_time_), CEREAL_NVP(iso_), CEREAL_NVP(ev_compensation_));
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
    archive(CEREAL_NVP(fstop_), CEREAL_NVP(focal_distance_), CEREAL_NVP(sensor_width_mm_), CEREAL_NVP(vignette_intensity_),
            CEREAL_NVP(chromatic_aberration_), CEREAL_NVP(bloom_intensity_), CEREAL_NVP(lens_flare_intensity_), CEREAL_NVP(white_temp_),
            CEREAL_NVP(white_tint_), CEREAL_NVP(motion_blur_from_shutter_));
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
  CameraCompression   compression_;

  double capture_rate_ = 0.0;  // Hz in simulated time, 0 = every tick

  template <class Archive>
  void serialize(Archive& archive) {
    archive(CEREAL_NVP(show_debug_camera_), CEREAL_NVP(offset_x_), CEREAL_NVP(offset_y_), CEREAL_NVP(offset_z_), CEREAL_NVP(rotation_pitch_),
            CEREAL_NVP(rotation_yaw_), CEREAL_NVP(rotation_roll_), CEREAL_NVP(fov_), CEREAL_NVP(width_), CEREAL_NVP(height_),
            CEREAL_NVP(enable_temporal_aa_), CEREAL_NVP(enable_raytracing_), CEREAL_NVP(enable_hdr_), CEREAL_NVP(enable_motion_blur_),
            CEREAL_NVP(motion_blur_amount_), CEREAL_NVP(motion_blur_distortion_), CEREAL_NVP(exposure_), CEREAL_NVP(lens_), CEREAL_NVP(intrinsics_),
            CEREAL_NVP(distortion_), CEREAL_NVP(noise_), CEREAL_NVP(rolling_shutter_), CEREAL_NVP(compression_), CEREAL_NVP(capture_rate_));
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
  CameraIntrinsics  intrinsics_;
  CameraDistortion  distortion_;
  CameraCompression compression_;

  double capture_rate_ = 0.0;  // Hz in simulated time, 0 = every tick

  template <class Archive>
  void serialize(Archive& archive) {
    archive(CEREAL_NVP(show_debug_camera_), CEREAL_NVP(offset_x_left_), CEREAL_NVP(offset_y_left_), CEREAL_NVP(offset_z_left_),
            CEREAL_NVP(offset_x_right_), CEREAL_NVP(offset_y_right_), CEREAL_NVP(offset_z_right_), CEREAL_NVP(rotation_pitch_left_),
            CEREAL_NVP(rotation_yaw_left_), CEREAL_NVP(rotation_roll_left_), CEREAL_NVP(rotation_pitch_right_), CEREAL_NVP(rotation_yaw_right_),
            CEREAL_NVP(rotation_roll_right_), CEREAL_NVP(fov_), CEREAL_NVP(width_), CEREAL_NVP(height_), CEREAL_NVP(enable_temporal_aa_),
            CEREAL_NVP(enable_raytracing_), CEREAL_NVP(enable_hdr_), CEREAL_NVP(intrinsics_), CEREAL_NVP(distortion_), CEREAL_NVP(compression_), CEREAL_NVP(capture_rate_));
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
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z), CEREAL_NVP(checkCollisions));
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
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(teleportedToX), CEREAL_NVP(teleportedToY), CEREAL_NVP(teleportedToZ),
            CEREAL_NVP(isHit), CEREAL_NVP(impactPointX), CEREAL_NVP(impactPointY), CEREAL_NVP(impactPointZ));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_));
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
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(image_), CEREAL_NVP(stamp_));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_));
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
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(image_), CEREAL_NVP(stamp_));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_));
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
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(image_left_), CEREAL_NVP(image_right_), CEREAL_NVP(stamp_));
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
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(pitch), CEREAL_NVP(yaw), CEREAL_NVP(roll));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(pitch), CEREAL_NVP(yaw), CEREAL_NVP(roll));
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
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(rotatedToPitch), CEREAL_NVP(rotatedToYaw), CEREAL_NVP(rotatedToRoll),
            CEREAL_NVP(isHit), CEREAL_NVP(impactPointX), CEREAL_NVP(impactPointY), CEREAL_NVP(impactPointZ));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z), CEREAL_NVP(pitch), CEREAL_NVP(yaw),
            CEREAL_NVP(roll), CEREAL_NVP(should_collide));
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
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(teleportedToX), CEREAL_NVP(teleportedToY), CEREAL_NVP(teleportedToZ),
            CEREAL_NVP(rotatedToPitch), CEREAL_NVP(rotatedToYaw), CEREAL_NVP(rotatedToRoll), CEREAL_NVP(isHit), CEREAL_NVP(impactPointX),
            CEREAL_NVP(impactPointY), CEREAL_NVP(impactPointZ));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z), CEREAL_NVP(pitch), CEREAL_NVP(yaw),
            CEREAL_NVP(roll), CEREAL_NVP(should_collide));
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

/* SetPoseTrajectory //{ */

// a pose timeline the pawn interpolates along per tick; t is ascending sim seconds, the end poses hold
struct PoseSample
{
  double t;

  double x;
  double y;
  double z;

  double pitch;
  double yaw;
  double roll;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(CEREAL_NVP(t), CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z), CEREAL_NVP(pitch), CEREAL_NVP(yaw), CEREAL_NVP(roll));
  }
};

namespace SetPoseTrajectory
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::set_pose_trajectory)) {
  }

  std::vector<PoseSample> samples;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(samples));
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::set_pose_trajectory)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_pose_trajectory, _status) {
  }
};
}  // namespace SetPoseTrajectory

/*SetPropellerSpeeds//{*/
namespace SetPropellerSpeeds
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::set_propeller_speeds)) {
  }

  // One angular rate per propeller [rad/s], in the frame's own propeller order:
  // rear-left, rear-right, front-left, front-right, then any further propellers an
  // external model brought with it. Sign is the direction of turn, so a quad's
  // counter-rotating pairs differ in sign. Fewer values than the frame has leaves the
  // remainder untouched; an empty list hands the propellers back to their default spin.
  std::vector<double> speeds;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(speeds));
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::set_propeller_speeds)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_propeller_speeds, _status) {
  }

  // How many propellers this frame actually has, so a client can size its next call.
  int32_t propeller_count = 0;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(propeller_count));
  }
};
}  // namespace SetPropellerSpeeds
/*//}*/

//}

namespace GetRangefinderData
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_rangefinder_data)){};
  
  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_));
  }
};

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_rangefinder_data)){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_rangefinder_data, _status){};

    double range;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(range));
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
    archive(CEREAL_NVP(distance), CEREAL_NVP(directionX), CEREAL_NVP(directionY), CEREAL_NVP(directionZ));
  }
};

struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_lidar_data)) {
  }

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_));
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
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(startX), CEREAL_NVP(startY), CEREAL_NVP(startZ), CEREAL_NVP(lidarData),
            CEREAL_NVP(stamp_));
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
    archive(CEREAL_NVP(distance), CEREAL_NVP(directionX), CEREAL_NVP(directionY), CEREAL_NVP(directionZ), CEREAL_NVP(segmentation));
  }
};

struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_lidar_seg)) {
  }

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_));
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
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(startX), CEREAL_NVP(startY), CEREAL_NVP(startZ), CEREAL_NVP(lidarSegData),
            CEREAL_NVP(stamp_));
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
    archive(CEREAL_NVP(distance), CEREAL_NVP(directionX), CEREAL_NVP(directionY), CEREAL_NVP(directionZ), CEREAL_NVP(intensity));
  }
};

struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_lidar_int)) {
  }

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_));
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
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(startX), CEREAL_NVP(startY), CEREAL_NVP(startZ), CEREAL_NVP(lidarIntData),
            CEREAL_NVP(stamp_));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_));
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_lidar_config)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_lidar_config, _status){};

  LidarConfig config;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(config));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_), CEREAL_NVP(config));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_));
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_rgb_camera_config)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_rgb_camera_config, _status){};

  RgbCameraConfig config;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(config));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_), CEREAL_NVP(config));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_));
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_stereo_camera_config)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_stereo_camera_config, _status){};

  StereoCameraConfig config;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(config));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_), CEREAL_NVP(config));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_type_));
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::add_sensor)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::add_sensor, _status){};

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(sensor_id_));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_));
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
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(sensor_ids_), CEREAL_NVP(sensor_types_));
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

  double   threshold_sigma_;     // per-pixel threshold mismatch, std in log units
  double   refractory_period_;   // seconds a pixel is blind after its event
  double   cutoff_hz_;           // photoreceptor bandwidth at full-scale intensity
  double   leak_rate_hz_;        // ON events per pixel per second from leakage
  double   shot_noise_rate_hz_;  // noise events per pixel per second at full scale, up to 4x in the dark
  double   hot_pixel_fraction_;  // share of pixels with 100x the noise rate
  double   max_event_rate_hz_;   // readout ceiling, excess dropped; 0 = unlimited
  int      max_events_per_frame_;
  bool     use_gpu_;             // compute shader, else the CPU port of the same model
  unsigned seed_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(CEREAL_NVP(show_debug_camera_), CEREAL_NVP(offset_x_), CEREAL_NVP(offset_y_), CEREAL_NVP(offset_z_), CEREAL_NVP(rotation_pitch_),
            CEREAL_NVP(rotation_yaw_), CEREAL_NVP(rotation_roll_), CEREAL_NVP(fov_), CEREAL_NVP(width_), CEREAL_NVP(height_),
            CEREAL_NVP(contrast_threshold_pos_), CEREAL_NVP(contrast_threshold_neg_), CEREAL_NVP(use_sim_time_), CEREAL_NVP(threshold_sigma_),
            CEREAL_NVP(refractory_period_), CEREAL_NVP(cutoff_hz_), CEREAL_NVP(leak_rate_hz_), CEREAL_NVP(shot_noise_rate_hz_),
            CEREAL_NVP(hot_pixel_fraction_), CEREAL_NVP(max_event_rate_hz_), CEREAL_NVP(max_events_per_frame_), CEREAL_NVP(use_gpu_),
            CEREAL_NVP(seed_));
  }
};

//}

/* GetEventCameraData //{ */

// packed events: xyp_ = x | y << 16 | brighter << 31, t_us_ = microseconds after base_stamp_
namespace GetEventCameraData
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_event_camera_data)){};

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_));
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_event_camera_data)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_event_camera_data, _status){};

  std::vector<uint32_t> xyp_;
  std::vector<uint32_t> t_us_;

  double base_stamp_ = 0.0;
  double stamp_      = 0.0;  // sim time the batch is complete up to

  uint32_t dropped_ = 0;  // events lost to the readout cap or a full buffer since the last drain

  int width_  = 0;
  int height_ = 0;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(xyp_), CEREAL_NVP(t_us_), CEREAL_NVP(base_stamp_), CEREAL_NVP(stamp_),
            CEREAL_NVP(dropped_), CEREAL_NVP(width_), CEREAL_NVP(height_));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_));
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_event_camera_config)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_event_camera_config, _status){};

  EventCameraConfig config;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(config));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_), CEREAL_NVP(config));
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
  // 2 stereographic (r = 2f*tan(theta/2)), 3 kannala_brandt (r = f*theta*(1 + k1 theta^2 + k2 theta^4 + k3 theta^6 + k4 theta^8), OpenCV fisheye)
  int lens_model_;

  CameraCompression compression_;

  // image circle fitted to the frame corner (fov_ is the diagonal) instead of the short side
  bool full_frame_;

  // with use_custom_ the focal (pixels per radian) and principal point come from here instead of fov_/full_frame_, so a
  // Kalibr/OpenVINS calibration can be rendered as is; fov_ then only caps the rendered angle. OpenCV pixel convention.
  CameraIntrinsics intrinsics_;

  double kb_k1_ = 0.0;
  double kb_k2_ = 0.0;
  double kb_k3_ = 0.0;
  double kb_k4_ = 0.0;

  double capture_rate_ = 0.0;  // Hz in simulated time, 0 = every tick

  template <class Archive>
  void serialize(Archive& archive) {
    archive(CEREAL_NVP(show_debug_camera_), CEREAL_NVP(offset_x_), CEREAL_NVP(offset_y_), CEREAL_NVP(offset_z_), CEREAL_NVP(rotation_pitch_),
            CEREAL_NVP(rotation_yaw_), CEREAL_NVP(rotation_roll_), CEREAL_NVP(fov_), CEREAL_NVP(width_), CEREAL_NVP(height_), CEREAL_NVP(lens_model_),
            CEREAL_NVP(compression_), CEREAL_NVP(full_frame_), CEREAL_NVP(intrinsics_), CEREAL_NVP(kb_k1_), CEREAL_NVP(kb_k2_), CEREAL_NVP(kb_k3_),
            CEREAL_NVP(kb_k4_), CEREAL_NVP(capture_rate_));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_));
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
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(image_), CEREAL_NVP(stamp_));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_));
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_fisheye_camera_config)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_fisheye_camera_config, _status){};

  FisheyeCameraConfig config;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(config));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_), CEREAL_NVP(config));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(device_name_), CEREAL_NVP(offset_x_), CEREAL_NVP(offset_y_),
            CEREAL_NVP(offset_z_), CEREAL_NVP(rotation_pitch_), CEREAL_NVP(rotation_yaw_), CEREAL_NVP(rotation_roll_), CEREAL_NVP(show_mesh_));
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
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(sensor_ids_), CEREAL_NVP(sensor_types_));
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
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(names_), CEREAL_NVP(descriptions_));
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
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(visible));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(visible));
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
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(crashed));
  }
};
}  // namespace GetCrashState

//}

/* GetDepthCameraData //{ */

// Depth in uint16 millimetres (0 = invalid/too close, 65535 = clamp), row-major
// width_ x height_, uncompressed. The Kinect/RealSense convention, so depth
// consumers need no conversion.
namespace GetDepthCameraData
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_depth_camera_data)) {
  }

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_));
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_depth_camera_data)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_depth_camera_data, _status) {
  }

  std::vector<uint16_t> image_;
  int                   width_;
  int                   height_;
  double                stamp_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(image_), CEREAL_NVP(width_), CEREAL_NVP(height_), CEREAL_NVP(stamp_));
  }
};
}  // namespace GetDepthCameraData

//}

/* GetInstanceSegData //{ */

// The instance-segmentation camera: a plugin-owned mesh pass renders each actor's
// 24-bit instance id (little-endian in RGB, 0 = background) with correct occlusion.
// The image is PNG (lossless); decode ids as R + (G<<8) + (B<<16).
namespace GetInstanceSegData
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_instance_seg_data)) {
  }

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_));
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_instance_seg_data)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_instance_seg_data, _status) {
  }

  std::vector<unsigned char> image_;
  double                     stamp_;
  uint32_t                   size_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(image_), CEREAL_NVP(stamp_), CEREAL_NVP(size_));
  }
};
}  // namespace GetInstanceSegData

//}

/* GetInstanceSegMap //{ */

struct InstanceSegMapEntry
{
  int         id;
  std::string actor;
  int         class_id = 0;  // the actor's segmentation class (see get_class_map); 0 unlabelled

  template <class Archive>
  void serialize(Archive& archive) {
    archive(CEREAL_NVP(id), CEREAL_NVP(actor), CEREAL_NVP(class_id));
  }
};

// The id -> actor mapping snapshotted at the same instant as the last requested
// instance-segmentation image, so the two are always consistent.
namespace GetInstanceSegMap
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_instance_seg_map)) {
  }

  int sensor_id_ = -1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_));
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_instance_seg_map)) {
  }
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_instance_seg_map, _status) {
  }

  std::vector<InstanceSegMapEntry> entries_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(entries_));
  }
};
}  // namespace GetInstanceSegMap

/* GetLidarScan //{ */

// one scan of any lidar with the channels asked for: get_lidar_data/get_lidar_seg/get_lidar_int
// each trace separately (and each advance their own Livox cursor), this traces once and hands
// back every channel for the same points
namespace GetLidarScan
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_lidar_scan)){};

  int  sensor_id_      = -1;
  bool with_intensity_ = false;  // physical-material class per point (MaterialIntensity), -1 without a return
  bool with_labels_    = false;  // class_id_ (segmentation stencil, landscape 1, -1 no return) and instance_id_ (0 background)

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_), CEREAL_NVP(with_intensity_), CEREAL_NVP(with_labels_));
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_lidar_scan)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_lidar_scan, _status){};

  double startX = 0.0;
  double startY = 0.0;
  double startZ = 0.0;
  double stamp_ = 0.0;

  std::vector<GetLidarData::LidarData> points_;

  // per point, same order as points_; empty unless requested. instance ids share the instance camera's id space
  std::vector<int>      intensity_;
  std::vector<int>      class_id_;
  std::vector<uint32_t> instance_id_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(startX), CEREAL_NVP(startY), CEREAL_NVP(startZ), CEREAL_NVP(stamp_),
            CEREAL_NVP(points_), CEREAL_NVP(intensity_), CEREAL_NVP(class_id_), CEREAL_NVP(instance_id_));
  }
};
}  // namespace GetLidarScan

//}

/* GetCameraInfo //{ */

// the effective calibration of one camera eye, enough for a ROS CameraInfo + TF; mount offset
// and rotation follow the camera configs' drone-frame convention and units. Pixel convention is
// OpenCV's (a pixel's centre is at its integer index), so an ideal centred camera has cx = W/2 - 0.5.
struct CameraInfo
{
  int sensor_id_   = -1;
  int sensor_type_ = 0;
  int eye_         = 0;  // stereo: 0 left, 1 right

  int width_  = 0;
  int height_ = 0;

  // pinhole | fisheye_equidistant | fisheye_equisolid | fisheye_stereographic | fisheye_kannala_brandt
  std::string projection_;

  // sensor_msgs/CameraInfo.distortion_model: plumb_bob for pinhole, equidistant (Kannala-Brandt) for every fisheye;
  // equisolid and stereographic lenses are reported through their theta-series, exact to the terms KB carries
  std::string distortion_model_;

  double fx_ = 0.0;  // pixels; a fisheye's f is pixels per radian of theta
  double fy_ = 0.0;
  double cx_ = 0.0;
  double cy_ = 0.0;

  std::vector<double> d_;  // plumb_bob: k1 k2 p1 p2 k3; equidistant: k1 k2 k3 k4 - zeros when the lens is ideal, never empty

  double fov_deg_ = 0.0;  // pinhole: horizontal; fisheye: the image circle (diagonal when full-frame)

  double offset_x_ = 0.0;
  double offset_y_ = 0.0;
  double offset_z_ = 0.0;

  double rotation_pitch_ = 0.0;
  double rotation_yaw_   = 0.0;
  double rotation_roll_  = 0.0;

  double baseline_     = 0.0;  // stereo: distance between the eyes, config units
  double readout_time_ = 0.0;  // rolling shutter seconds top-to-bottom; 0 = global shutter

  template <class Archive>
  void serialize(Archive& archive) {
    archive(CEREAL_NVP(sensor_id_), CEREAL_NVP(sensor_type_), CEREAL_NVP(eye_), CEREAL_NVP(width_), CEREAL_NVP(height_), CEREAL_NVP(projection_),
            CEREAL_NVP(distortion_model_), CEREAL_NVP(fx_), CEREAL_NVP(fy_), CEREAL_NVP(cx_), CEREAL_NVP(cy_), CEREAL_NVP(d_), CEREAL_NVP(fov_deg_), CEREAL_NVP(offset_x_),
            CEREAL_NVP(offset_y_), CEREAL_NVP(offset_z_), CEREAL_NVP(rotation_pitch_), CEREAL_NVP(rotation_yaw_), CEREAL_NVP(rotation_roll_),
            CEREAL_NVP(baseline_), CEREAL_NVP(readout_time_));
  }
};

namespace GetCameraInfo
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(static_cast<unsigned short>(MessageType::get_camera_info)){};

  int sensor_id_ = -1;  // -1: every camera on the drone

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(sensor_id_));
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_camera_info)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_camera_info, _status){};

  std::vector<CameraInfo> cameras_;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(cameras_));
  }
};
}  // namespace GetCameraInfo

//}

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
  list_assets             = 24,
  get_world_mesh          = 25,
  get_distance_batch      = 26,
  set_time_dilation       = 27,
  set_fixed_timestep      = 28,
  set_lockstep            = 29,
  step                    = 30,
  get_class_map           = 31,
  load_library            = 32,
  list_library            = 33
};

// Scene-editor poses travel in the same frame as the spawn YAML itself: a right-handed
// (ROS) frame relative to the world origin, metres and degrees. Orientation is roll,
// pitch, yaw.
struct LibraryObject
{
  std::string library;
  std::string id;
  std::string spawn_name;  // what a spawn config's 'asset' field takes
  std::string cls;
  std::string description;
  std::string license;
  std::string thumbnail;  // a path on the simulator's machine

  double scale    = 1.0;
  double height_m = 0.0;
  bool   runtime  = true;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(CEREAL_NVP(library), CEREAL_NVP(id), CEREAL_NVP(spawn_name), CEREAL_NVP(cls), CEREAL_NVP(description), CEREAL_NVP(license), CEREAL_NVP(thumbnail),
            CEREAL_NVP(scale), CEREAL_NVP(height_m), CEREAL_NVP(runtime));
  }
};

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
    archive(CEREAL_NVP(id), CEREAL_NVP(asset), CEREAL_NVP(stencil), CEREAL_NVP(position), CEREAL_NVP(orientation), CEREAL_NVP(scale));
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
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(ports));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(Density_Level));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(Hilly_Level));
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
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(port));
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
      archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z), CEREAL_NVP(MeshName));
    }
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::spawn_drone_at_location)){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::spawn_drone_at_location, _status){};

    int port;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(port));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(port));
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
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(cameraCaptureMode));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(cameraCaptureMode));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(graphicsSettings));
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
      archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(weather_id));
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
      archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(hour), CEREAL_NVP(minute));
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
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(worldLevelName));
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
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(fps));
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
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(api_version_major), CEREAL_NVP(api_version_minor));
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

  // Wall clock, which is what a client synchronising its own clock against the
  // simulator needs; it does not scale with time dilation.
  double time;

  // Simulated seconds since the level began, and the factor relating the two. At a
  // dilation of 0.5 this advances half as fast as the wall clock, which is what lets a
  // perception stack that cannot keep up still see every frame.
  double sim_time = 0.0;
  double time_dilation = 1.0;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(time), CEREAL_NVP(sim_time), CEREAL_NVP(time_dilation));
  }
};
}  // namespace GetTime

/*SetTimeDilation//{*/
namespace SetTimeDilation
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::set_time_dilation){};

  // Simulated seconds per wall second. Below 1 the world runs slow, which buys a client
  // more frames per simulated second; above 1 it runs fast. The engine clamps this, so
  // the response reports what was actually applied.
  double dilation = 1.0;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(dilation));
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::set_time_dilation)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_time_dilation, _status){};

  double applied = 1.0;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(applied));
  }
};
}  // namespace SetTimeDilation
/*//}*/

/* SetFixedTimestep //{ */

// every tick advances sim time by exactly dt (0 = wall clock); adaptive_min_dt > 0 lets it shrink when an event camera aliases
namespace SetFixedTimestep
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::set_fixed_timestep){};

  double dt              = 0.0;
  double adaptive_min_dt = 0.0;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(dt), CEREAL_NVP(adaptive_min_dt));
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::set_fixed_timestep)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_fixed_timestep, _status){};

  double applied = 0.0;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(applied));
  }
};
}  // namespace SetFixedTimestep

/*//}*/

/* SetLockstep / Step //{ */

// lockstep parks the world paused; step runs N ticks and returns once every camera has their frames
namespace SetLockstep
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::set_lockstep){};

  bool enabled = true;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(enabled));
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::set_lockstep)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::set_lockstep, _status){};
};
}  // namespace SetLockstep

namespace Step
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::step){};

  int ticks = 1;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(ticks));
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::step)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::step, _status){};

  double   sim_time   = 0.0;
  uint64_t tick_count = 0;  // ticks run in lockstep since it was enabled

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(sim_time), CEREAL_NVP(tick_count));
  }
};
}  // namespace Step

/*//}*/

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
      archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z));
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
      archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(mutual_visibiliti_enabled));
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
      archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(yaml));
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
      archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(object_ids), CEREAL_NVP(error));
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
      archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(object_id));
    }
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::remove_spawned_object)){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::remove_spawned_object, _status){};

    int removed_count;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(removed_count));
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
      archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(object_id), CEREAL_NVP(position), CEREAL_NVP(orientation),
              CEREAL_NVP(scale));
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
      archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(objects));
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
      archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(yaml));
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
      archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(path_prefix), CEREAL_NVP(name_filter));
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
      archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(assets), CEREAL_NVP(truncated));
    }
  };
}
/*//}*/

/*LoadLibrary//{*/
namespace LoadLibrary
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(MessageType::load_library){};

    // A manifest file or a directory of glTF models, on the simulator's machine.
    std::string path;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(path));
    }
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::load_library)){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::load_library, _status){};

    int         count = 0;
    std::string error;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(count), CEREAL_NVP(error));
    }
  };
}
/*//}*/

/*ListLibrary//{*/
namespace ListLibrary
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(MessageType::list_library){};

    // Empty strings match everything; each word of filter must appear in the id, class or description, ignoring case.
    std::string library;
    std::string cls;
    std::string filter;

    int offset = 0;
    int limit  = 200;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(library), CEREAL_NVP(cls), CEREAL_NVP(filter), CEREAL_NVP(offset), CEREAL_NVP(limit));
    }
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::list_library)){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::list_library, _status){};

    // One page; total counts every match.
    std::vector<LibraryObject> objects;
    int                        total = 0;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(objects), CEREAL_NVP(total));
    }
  };
}
/*//}*/

/*GetWorldMesh//{*/
namespace GetWorldMesh
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(MessageType::get_world_mesh){};

    // Axis-aligned box filter in the scene frame (right-handed metres relative to the
    // world origin). Any extent <= 0 disables the filter on that axis.
    double center_x_ = 0.0;
    double center_y_ = 0.0;
    double center_z_ = 0.0;
    double extent_x_ = 0.0;
    double extent_y_ = 0.0;
    double extent_z_ = 0.0;

    // Hard cap on returned triangles; 0 takes the server default.
    uint32_t max_triangles_ = 0;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(center_x_), CEREAL_NVP(center_y_), CEREAL_NVP(center_z_),
              CEREAL_NVP(extent_x_), CEREAL_NVP(extent_y_), CEREAL_NVP(extent_z_), CEREAL_NVP(max_triangles_));
    }
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_world_mesh)){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_world_mesh, _status){};

    // Triangle soup of the world's static geometry: xyz triplets in the scene frame
    // (right-handed metres relative to the world origin), indices in triples.
    std::vector<float>    vertices_;
    std::vector<uint32_t> indices_;

    // True when max_triangles_ cut the export short.
    bool truncated_ = false;

    // Number of mesh components that contributed.
    int32_t source_count_ = 0;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(vertices_), CEREAL_NVP(indices_), CEREAL_NVP(truncated_),
              CEREAL_NVP(source_count_));
    }
  };
}
/*//}*/

/*GetDistanceBatch//{*/
namespace GetDistanceBatch
{
  struct Request : public Common::NetworkRequest
  {
    Request() : Common::NetworkRequest(MessageType::get_distance_batch){};

    // Query points packed x0,y0,z0,x1,y1,z1,... in the scene frame (right-handed
    // metres relative to the world origin).
    std::vector<float> query_points_;

    // Truncation radius [m]: with no surface within it, the returned distance is
    // exactly max_radius_. Must be > 0.
    double max_radius_ = 5.0;

    // Binary-search tolerance [m].
    double tolerance_ = 0.001;

    // true: distance to terrain only (Landscape or actors tagged "terrain");
    // false: distance to any static collision geometry.
    bool terrain_only_ = false;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkRequest>(this), CEREAL_NVP(query_points_), CEREAL_NVP(max_radius_), CEREAL_NVP(tolerance_),
              CEREAL_NVP(terrain_only_));
    }
  };

  struct Response : public Common::NetworkResponse
  {
    Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_distance_batch)){};
    explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_distance_batch, _status){};

    // One distance [m] per query point; == max_radius_ when truncated; 0.0 when the
    // query point is inside collision geometry.
    std::vector<float> distances_;

    template <class Archive>
    void serialize(Archive& archive) {
      archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(distances_));
    }
  };
}
/*//}*/

/* GetClassMap //{ */

// the segmentation class tree the tagger applies in this world (Config/FlightForgeClasses.yaml plus
// the ini rules); the same ids appear in the instance camera's alpha channel, the instance map, the
// legacy stencil (ids <= 255) and the lidar's class_id_
namespace GetClassMap
{
struct Request : public Common::NetworkRequest
{
  Request() : Common::NetworkRequest(MessageType::get_class_map){};
};

// one node of the class tree: path like "vegetation/tree/conifer", parent_id 0 at the root level.
// Sensors emit leaf ids; collapse through parent_id to the level a task needs.
struct ClassEntry
{
  int         id        = 0;
  int         parent_id = 0;
  std::string path;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(CEREAL_NVP(id), CEREAL_NVP(parent_id), CEREAL_NVP(path));
  }
};

struct Response : public Common::NetworkResponse
{
  Response() : Common::NetworkResponse(static_cast<unsigned short>(MessageType::get_class_map)){};
  explicit Response(bool _status) : Common::NetworkResponse(MessageType::get_class_map, _status){};

  std::vector<ClassEntry> classes_;
  uint64_t                hash_ = 0;  // over ids and names, so a dataset can tell which legend it was written with

  template <class Archive>
  void serialize(Archive& archive) {
    archive(cereal::base_class<Common::NetworkResponse>(this), CEREAL_NVP(classes_), CEREAL_NVP(hash_));
  }
};
}  // namespace GetClassMap

//}

}  // namespace GameMode

/* Registry //{ */

// the one list of every op on the wire: get_meta is built from it and the tests check the MessageType enums against it
namespace Registry
{
enum class Scope : unsigned char
{
  Common   = 0,
  Drone    = 1,
  GameMode = 2
};

template <class Req, class Res>
struct Op
{
  using Request  = Req;
  using Response = Res;
  Scope          scope;
  unsigned short id;
  const char*    name;
};

#define FF_OP(Scope_, Ns, msg) f(Op<Scope_::Ns::Request, Scope_::Ns::Response>{Scope::Scope_, static_cast<unsigned short>(Scope_::MessageType::msg), #msg})

template <class F>
void ForEachOp(F&& f) {
  FF_OP(Common, Ping, ping);
  FF_OP(Common, GetMeta, get_meta);

  FF_OP(Drone, GetLocation, get_location);
  FF_OP(Drone, SetLocation, set_location);
  FF_OP(Drone, GetRgbCameraData, get_rgb_camera_data);
  FF_OP(Drone, GetStereoCameraData, get_stereo_camera_data);
  FF_OP(Drone, GetRotation, get_rotation);
  FF_OP(Drone, SetRotation, set_rotation);
  FF_OP(Drone, SetLocationAndRotation, set_location_and_rotation);
  FF_OP(Drone, GetLidarData, get_lidar_data);
  FF_OP(Drone, GetLidarConfig, get_lidar_config);
  FF_OP(Drone, SetLidarConfig, set_lidar_config);
  FF_OP(Drone, GetRgbCameraConfig, get_rgb_camera_config);
  FF_OP(Drone, SetRgbCameraConfig, set_rgb_camera_config);
  FF_OP(Drone, GetStereoCameraConfig, get_stereo_camera_config);
  FF_OP(Drone, SetStereoCameraConfig, set_stereo_camera_config);
  FF_OP(Drone, GetMoveLineVisible, get_move_line_visible);
  FF_OP(Drone, SetMoveLineVisible, set_move_line_visible);
  FF_OP(Drone, GetRgbSegCameraData, get_rgb_seg_camera_data);
  FF_OP(Drone, GetLidarSegData, get_lidar_seg);
  FF_OP(Drone, SetLocationAndRotationAsync, set_location_and_rotation_async);
  FF_OP(Drone, GetCrashState, get_crash_state);
  FF_OP(Drone, GetLidarIntData, get_lidar_int);
  FF_OP(Drone, GetRangefinderData, get_rangefinder_data);
  FF_OP(Drone, AddSensor, add_sensor);
  FF_OP(Drone, RemoveSensor, remove_sensor);
  FF_OP(Drone, ListSensors, list_sensors);
  FF_OP(Drone, GetEventCameraData, get_event_camera_data);
  FF_OP(Drone, GetEventCameraConfig, get_event_camera_config);
  FF_OP(Drone, SetEventCameraConfig, set_event_camera_config);
  FF_OP(Drone, GetFisheyeCameraData, get_fisheye_camera_data);
  FF_OP(Drone, GetFisheyeCameraConfig, get_fisheye_camera_config);
  FF_OP(Drone, SetFisheyeCameraConfig, set_fisheye_camera_config);
  FF_OP(Drone, AddDevice, add_device);
  FF_OP(Drone, ListDevices, list_devices);
  FF_OP(Drone, GetInstanceSegData, get_instance_seg_data);
  FF_OP(Drone, GetInstanceSegMap, get_instance_seg_map);
  FF_OP(Drone, GetDepthCameraData, get_depth_camera_data);
  FF_OP(Drone, SetPoseTrajectory, set_pose_trajectory);
  FF_OP(Drone, SetPropellerSpeeds, set_propeller_speeds);
  FF_OP(Drone, GetCameraInfo, get_camera_info);
  FF_OP(Drone, GetLidarScan, get_lidar_scan);

  FF_OP(GameMode, GetDrones, get_drones);
  FF_OP(GameMode, SpawnDrone, spawn_drone);
  FF_OP(GameMode, RemoveDrone, remove_drone);
  FF_OP(GameMode, GetCameraCaptureMode, get_camera_capture_mode);
  FF_OP(GameMode, SetCameraCaptureMode, set_camera_capture_mode);
  FF_OP(GameMode, GetFps, get_fps);
  FF_OP(GameMode, GetTime, get_time);
  FF_OP(GameMode, GetApiVersion, get_api_version);
  FF_OP(GameMode, SetGraphicsSettings, set_graphics_settings);
  FF_OP(GameMode, SwitchWorldLevel, switch_world_level);
  FF_OP(GameMode, SetForestDensity, set_forest_density);
  FF_OP(GameMode, SetForestHillyLevel, set_forest_hilly_level);
  FF_OP(GameMode, GetWorldOrigin, get_world_origin);
  FF_OP(GameMode, SpawnDroneAtLocation, spawn_drone_at_location);
  FF_OP(GameMode, SetWeather, set_weather);
  FF_OP(GameMode, SetDaytime, set_daytime);
  FF_OP(GameMode, SetMutualVisibility, set_mutual_visibility);
  FF_OP(GameMode, SpawnObjectsFromYaml, spawn_objects_from_yaml);
  FF_OP(GameMode, RemoveSpawnedObject, remove_spawned_object);
  FF_OP(GameMode, MoveSpawnedObject, move_spawned_object);
  FF_OP(GameMode, ListSpawnedObjects, list_spawned_objects);
  FF_OP(GameMode, ExportScene, export_scene);
  FF_OP(GameMode, ListAssets, list_assets);
  FF_OP(GameMode, GetWorldMesh, get_world_mesh);
  FF_OP(GameMode, GetDistanceBatch, get_distance_batch);
  FF_OP(GameMode, SetTimeDilation, set_time_dilation);
  FF_OP(GameMode, SetFixedTimestep, set_fixed_timestep);
  FF_OP(GameMode, SetLockstep, set_lockstep);
  FF_OP(GameMode, Step, step);
  FF_OP(GameMode, GetClassMap, get_class_map);
  FF_OP(GameMode, LoadLibrary, load_library);
  FF_OP(GameMode, ListLibrary, list_library);
}

#undef FF_OP

// reads one whole reply of `type` from the archive so a client can step past a stale reply; false when the id is not known in scope
template <class Archive>
bool LoadResponse(Archive& archive, Scope scope, unsigned short type) {
  bool found = false;
  ForEachOp([&](auto op) {
    if (found || op.id != type || (op.scope != scope && op.scope != Scope::Common)) {
      return;
    }
    typename decltype(op)::Response response;
    archive(response);
    found = true;
  });
  return found;
}

}  // namespace Registry

//}

}  // namespace Serializable
