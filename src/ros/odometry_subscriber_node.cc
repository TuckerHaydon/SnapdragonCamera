// Author: Tucker Haydon

#include "odometry_subscriber_node.h"

namespace snap_cam {
  OdometrySubscriberNode::OdometrySubscriberNode(
      const std::string& topic, 
      std::shared_ptr<OdometryBufferSentry> odometry_buffer_sentry) {
    this->node_handle_ = ros::NodeHandle("~");
    this->odometry_buffer_sentry_ = odometry_buffer_sentry;
    this->subscriber_ = node_handle_.subscribe(
        topic, 
        1, 
        &OdometrySubscriberNode::SubscriberCallback, 
        this);
  }

  void OdometrySubscriberNode::SubscriberCallback(const nav_msgs::Odometry& msg) {
    OdometryBuffer buffer;

    buffer.position.x = msg.pose.pose.position.x;
    buffer.position.y = msg.pose.pose.position.y;
    buffer.position.z = msg.pose.pose.position.z;

    buffer.velocity.x = msg.twist.twist.linear.x;
    buffer.velocity.y = msg.twist.twist.linear.y;
    buffer.velocity.z = msg.twist.twist.linear.z;

    buffer.orientation.w = msg.pose.pose.orientation.w;
    buffer.orientation.x = msg.pose.pose.orientation.x;
    buffer.orientation.y = msg.pose.pose.orientation.y;
    buffer.orientation.z = msg.pose.pose.orientation.z;

    buffer.angular_rate.x =  msg.twist.twist.angular.x;
    buffer.angular_rate.y =  msg.twist.twist.angular.y;
    buffer.angular_rate.z =  msg.twist.twist.angular.z;

    for(size_t idx = 0; idx < 36; ++idx) {
      buffer.pose_covariance[idx] = msg.pose.covariance[idx];
    }
    
    this->odometry_buffer_sentry_->Write(buffer);
  }
}
