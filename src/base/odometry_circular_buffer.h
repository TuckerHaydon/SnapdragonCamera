// Author: Tucker Haydon

#pragma once

#include "odometry_buffer.h"

#include <mutex>

namespace snapdragon_camera {
  // A circular buffer for the odometry data. It was observed that the frames
  // arriving from the snapdragon's camera were 8 frames lates. The odometry
  // data must be preserved in a buffer so that the odometry data is correctly
  // time-synchronized and written with the corrresponding image.
  class OdometryCircularBuffer {
    public:
      struct Options {
        // Hypothesis: Camera frames are late by 8
        size_t buffer_size = 8;

        void Check();
      };

      // Constructor
      OdometryCircularBuffer(const Options& options)
        : options_(options) {
        this->options_.Check();
        this->circular_buffer_ = new OdometryBuffer[this->options_.buffer_size];
      }

      ~OdometryCircularBuffer() {
        delete[] this->circular_buffer_;
      }

      // Get the oldest OdometryBuffer object from the circular buffer and
      // return true. If the buffer is not yet full, return false. Push a new
      // OdometryBuffer onto the circular buffer.
      bool Cycle(const OdometryBuffer& input, OdometryBuffer& output);

    private:
      Options options_;
      // Mutex to ensure safe multi-threaded read-write access
      mutable std::mutex mtx_;
      // Internal array buffer
      OdometryBuffer* circular_buffer_;
      // Index of the next buffer slot to be written to. Is also the index of
      // the oldest OdometryBuffer object.
      size_t buffer_idx_{0};
      // Current size of the buffer. Bounded by Options::buffer_size.
      size_t current_buffer_size_{0};
  };
}
