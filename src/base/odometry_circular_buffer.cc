// Author: Tucker Haydon

#include "odometry_circular_buffer.h"
#include <algorithm>
#include <iostream>

namespace snapdragon_camera {

  void OdometryCircularBuffer::Options::Check() {
    if(0 >= this->buffer_size) {
      std::cerr << "OdometryCircularBuffer::Options::Check() -- Buffer must have positive size." << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }

  bool OdometryCircularBuffer::Cycle(const OdometryBuffer& input, OdometryBuffer& output) {
    std::lock_guard<std::mutex>(this->mtx_);
    bool buffer_filled = false;

    // If the circular buffer is filled, output the oldest OdometryBuffer
    if(this->current_buffer_size_ != this->options_.buffer_size) {
      buffer_filled = false;
    } else {
      output = this->circular_buffer_[this->buffer_idx_];
      buffer_filled = true;
    }

    // Write the incoming OdometryBuffer to the circular buffer
    this->circular_buffer_[this->buffer_idx_] = input;

    this->buffer_idx_++;
    this->buffer_idx_ = this->buffer_idx_ % this->options_.buffer_size;

    this->current_buffer_size_++;
    this->current_buffer_size_ = std::min(this->current_buffer_size_, this->options_.buffer_size);

    // Return
    return buffer_filled;
  }
}
