// Author: Tucker Haydon

#include "odometry_buffer_sentry.h"

namespace snap_cam {

  void OdometryBufferSentry::Read(OdometryBuffer& buffer) const {
    std::lock_guard<std::mutex>(this->mtx_);
    buffer = this->buffer_;
  }

  void OdometryBufferSentry::Write(const OdometryBuffer& buffer) {
    std::lock_guard<std::mutex>(this->mtx_);
    this->buffer_ = buffer;
  }
}
