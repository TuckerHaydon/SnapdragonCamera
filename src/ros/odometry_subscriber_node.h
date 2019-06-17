// Author: Tucker Haydon

#pragma once

#include <ros/ros.h>
#include <memory>
#include "nav_msgs/Odometry.h"
#include "odometry_buffer_sentry.h"

namespace snapdragon_camera {
  // Class encapsulating a single ROS odometry subscriber. The subscriber
  // converts the ROS message data into an internal data structure protected by
  // a sentry object for safe asynchronous access.
  class OdometrySubscriberNode {
    private:
      // ROS node handle
      ros::NodeHandle node_handle_;

      // ROS subscriber
      ros::Subscriber subscriber_;

      // Odometry buffer sentry
      std::shared_ptr<OdometryBufferSentry> odometry_buffer_sentry_;

      // Callback. Extract message and pass to odometry buffer sentry
      void SubscriberCallback(const nav_msgs::Odometry& msg);
      
    public:
      // Constructor.
      //
      // Note parameters are intentionally copied.
      OdometrySubscriberNode(
          const std::string& topic, 
          std::shared_ptr<OdometryBufferSentry> odometry_buffer_sentry);
  };
}
