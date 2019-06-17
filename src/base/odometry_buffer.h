// Author: Tucker Haydon

#pragma once

#include <vector>

namespace snap_cam {
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

    Vector3D position;
    Vector3D velocity;
    Quaternion orientation;
    Vector3D angular_rate;

    // 36-element, row-major covariance
    double pose_covariance[36];

    OdometryBuffer() {}
  };
}
