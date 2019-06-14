// Author: Tucker Haydon

#include <cstdlib>
#include <iostream>

#include "snapdragon_camera_listener.h"

namespace snap_cam {
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

    const std::string frame_file_name = this->options_.frame_output_manager->FrameFileName();
    const std::string frame_file_path = this->options_.frame_output_manager->FrameFilePath();

    this->options_.metadata_logger->Log(frame_file_name);
    this->options_.image_saver->SaveImage(frame, frame_file_path);

    this->options_.frame_output_manager->Increment();

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
