// Author: Tucker Haydon

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>

#include "moving_average.h"
#include "snapdragon_camera_listener.h"

namespace snapdragon_camera {
  void SnapdragonCameraListener::onError() {
    // No implementation
    return;
  }

  void SnapdragonCameraListener::onControl(const camera::ControlEvent& control) {
    // No implementation
    return;
  }

  void SnapdragonCameraListener::onPreviewFrame(camera::ICameraFrame* frame) {
    // No implementation
    return;
  }

  void SnapdragonCameraListener::onVideoFrame(camera::ICameraFrame* frame) {
    if(false == this->occupied_) {
      this->occupied_ = true;
    } else {
      return;
    }

    // Save image and log metadata
    const std::string frame_file_name = this->options_.frame_output_manager->FrameFileName();
    const std::string frame_file_path = this->options_.frame_output_manager->FrameFilePath();

    this->options_.metadata_logger->Log(frame_file_name);
    this->options_.image_saver->SaveImage(frame, frame_file_path);

    this->options_.frame_output_manager->Increment();

    // Calculate the average FPS and report it. Report when an image is saved.
    // The average fps in this case is defined as the inverse of the moving
    // average of the duration between the last N=20 frames.
    static std::chrono::system_clock::time_point last_time = std::chrono::system_clock::now();
    const std::chrono::system_clock::time_point now_time = std::chrono::system_clock::now();
    const double duration = std::chrono::duration_cast<std::chrono::duration<double>>(now_time - last_time).count();
    last_time = now_time;

    static MovingAverage::Options moving_average_options;
    static MovingAverage moving_average(moving_average_options);
    moving_average.Insert(duration);
    const double avg_fps = 1.0 / moving_average.Evaluate();

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << avg_fps;
    std::string avg_fps_str = ss.str();

    std::cout << frame_file_name + " saved! Average fps: " << avg_fps_str << std::endl;

    this->occupied_ = false;
  }

  void SnapdragonCameraListener::onPictureFrame(camera::ICameraFrame* frame) {
    // No implementation
    return;
  }

  void SnapdragonCameraListener::onMetadataFrame(camera::ICameraFrame* frame) {
    // No implementation
    return;
  }

  void SnapdragonCameraListener::Options::Check() {
    if(nullptr == this->image_saver) {
      std::cerr << "SnapdragonCameraListener::Options::Check() -- Must specify an ImageSaver." << std::endl;
      std::exit(EXIT_FAILURE);
    }

    if(nullptr == this->frame_output_manager) {
      std::cerr << "SnapdragonCameraListener::Options::Check() -- Must specify a FrameOutputManager." << std::endl;
      std::exit(EXIT_FAILURE);
    }

    if(nullptr == this->metadata_logger) {
      std::cerr << "SnapdragonCameraListener::Options::Check() -- Must specify a MetadataLogger." << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }
}
