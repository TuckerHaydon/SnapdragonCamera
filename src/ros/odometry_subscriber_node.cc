// Author: Tucker Haydon

#include "odometry_subscriber_node.h"

namespace snap_cam {
  OdometrySubscriberNode::OdometrySubscriberNode(
      const std::string& topic, 
      std::shared_ptr<OdometryBufferSentry> odometry_buffer_sentry) {
    this->node_handle_ = ros::NodeHandle("/");
    this->odometry_buffer_sentry_ = odometry_buffer_sentry;
    this->subscriber_ = node_handle_.subscribe(
        topic, 
        1, 
        &OdometrySubscriberNode::SubscriberCallback, 
        this);
  }

  void OdometrySubscriberNode::SubscriberCallback(const nav_msgs::Odometry& msg) {
    OdometryBuffer buffer;
    buffer.position = Eigen::Vector3d(
        msg.pose.pose.position.x,
        msg.pose.pose.position.y,
        msg.pose.pose.position.z
        );
    buffer.velocity = Eigen::Vector3d(
        msg.twist.twist.linear.x,
        msg.twist.twist.linear.y,
        msg.twist.twist.linear.z
        );
    buffer.orientation = Eigen::Quaterniond(
        msg.pose.pose.orientation.w,
        msg.pose.pose.orientation.x,
        msg.pose.pose.orientation.y,
        msg.pose.pose.orientation.z
        );
    buffer.angular_rate = Eigen::Vector3d(
        msg.twist.twist.angular.x,
        msg.twist.twist.angular.y,
        msg.twist.twist.angular.z
        );
    this->odometry_buffer_sentry_->Write(buffer);
  }
}
