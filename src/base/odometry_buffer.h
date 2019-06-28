// Author: Tucker Haydon

#pragma once

#include <vector>
#include <cstdint>

namespace snapdragon_camera {
  // POD struct containing odometry data
  struct OdometryBuffer {
    struct Vector3D {
      double x;
      double y;
      double z;
    };

    struct Quaternion {
      double w;
      double x;
      double y;
      double z;
    };

    struct Time {
      uint32_t sec;
      uint32_t nsec;
    };

    // Data from ROS message
    Vector3D position;
    Vector3D velocity;
    Quaternion orientation;
    Vector3D angular_rate;

    // Time stamp from ROS message
    Time time;

    // 36-element, row-major covariance
    double pose_covariance[36];

    OdometryBuffer() {}
  };
}
