// Author: Tucker Haydon

#pragma once

#include <Eigen/Geometry>

namespace snap_cam {
  // POD struct containing odometry data
  struct OdometryBuffer {
    Eigen::Vector3d position;
    Eigen::Vector3d velocity;
    Eigen::Quaterniond orientation;
    Eigen::Vector3d angular_rate;

    OdometryBuffer() {}
  };
}
