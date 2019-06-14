// Author: Tucker Haydon

#include <cstdlib>
#include <iostream>

#include "camera_listener.h"

namespace snap_cam {
  void CameraListener::onError() {
    // No implementation
    return;
  }

  void CameraListener::onControl(const camera::ControlEvent& control) {
    // No implementation
    return;
  }

  void CameraListener::onPreviewFrame(camera::ICameraFrame* frame) {
    // No implementation
    return;
  }

  void CameraListener::onVideoFrame(camera::ICameraFrame* frame) {
    if(false == this->occupied_) {
      this->occupied_ = true;
    } else {
      return;
    }

    const std::string frame_file_name = this->options_.frame_output_manager->FrameFileName();
    const std::string frame_file_path = this->options_.frame_output_manager->FrameFilePath();

    this->options_.metadata_logger->LogMetadata(frame_file_name);
    this->options_.image_saver->SaveImage(frame, frame_file_path);

    this->options_.frame_output_manager->Increment();

    this->occupied_ = false;
  }

  void CameraListener::onPictureFrame(camera::ICameraFrame* frame) {
    // No implementation
    return;
  }

  void CameraListener::onMetadataFrame(camera::ICameraFrame* frame) {
    // No implementation
    return;
  }

  void CameraListener::Options::Check() {
    if(nullptr == this->image_saver) {
      std::cerr << "CameraListener::Options::Check() -- Must specify an ImageSaver." << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }
}
