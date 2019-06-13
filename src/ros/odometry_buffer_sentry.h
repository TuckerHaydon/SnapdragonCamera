// Author: Tucker Haydon

#include <mutex>
#include "odometry_buffer.h"

namespace snap_cam {
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
