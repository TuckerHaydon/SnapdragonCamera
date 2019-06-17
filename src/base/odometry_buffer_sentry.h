// Author: Tucker Haydon

#pragma once

#include <mutex>
#include "odometry_buffer.h"

namespace snapdragon_camera {
  // Sentry object protecting an OdometryBuffer, enabling safe asynchronous
  // read/write access.
  class OdometryBufferSentry {
    private:
      mutable std::mutex mtx_;
      OdometryBuffer buffer_;

    public:
      OdometryBufferSentry() {}
      void Read(OdometryBuffer& buffer) const;
      void Write(const OdometryBuffer& buffer);
  };
}
