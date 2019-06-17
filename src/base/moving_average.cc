// Author: Tucker Haydon

#include <algorithm>

#include "moving_average.h"

namespace snapdragon_camera {
  MovingAverage::MovingAverage(const Options& options)
    : options_(options) {
      this->buffer_ = new double[this->options_.buffer_size];
    }

  MovingAverage::~MovingAverage() {
    delete[] this->buffer_;
  }

  void MovingAverage::Insert(double val) {
    this->buffer_[this->idx_] = val;
    this->idx_++;
    this->idx_ = this->idx_ % this->options_.buffer_size;

    this->size_++;
    this->size_ = std::min(this->size_, this->options_.buffer_size);
    
  }

  double MovingAverage::Evaluate() const {
    if(this->size_ == 0) {
      return 0.0;
    }

    double sum = 0.0;
    for(size_t jdx = 0; jdx < this->size_; ++jdx) {
      sum += this->buffer_[jdx];
    }
    const double average = sum / this->size_;
    return average;
  }
}
