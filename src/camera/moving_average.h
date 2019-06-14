// Author: Tucker Haydon

#pragma once

namespace snap_cam {
  class MovingAverage {
    public:
      struct Options {
        // Number of elements in moving average
        size_t buffer_size = 20;
      };

      MovingAverage(const Options& options);
      ~MovingAverage();

      void Insert(double val);
      double Evaluate() const;

    private:
      Options options_;
      double* buffer_{nullptr};
      size_t idx_{0};
      size_t size_{0};
  };
}
