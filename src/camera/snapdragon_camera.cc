// Author: Tucker Haydon

#include <cstdlib>
#include <iostream>

#include "snapdragon_camera.h"

namespace snap_cam {
  SnapdragonCamera::SnapdragonCamera(const Options& options)
    : options_(options) {
    this->options_.Check();

    // Check cameras are present. If not, something is wrong. Oftentimes, user
    // ran this program without root.
    if (1 > camera::getNumberOfCameras()) {
      std::cerr << "SnapdragonCamera::SnapdragonCamera  -- No cameras detected. Are you using sudo? Exiting." << std::endl;
      std::exit(EXIT_FAILURE);
    }

  }

  SnapdragonCamera::~SnapdragonCamera() {
    this->Stop();
  }

  void SnapdragonCamera::Start() {
    // Check if camera device is already running. This shouldnt happen.
    if(true == this->camera_device_allocated_) {
      std::cerr << "SnapdragonCamera::Start  -- Camera already running. Error. Exiting." << std::endl;
      std::exit(EXIT_FAILURE);
    }

    // Create new camera device
    if(0 != camera::ICameraDevice::createInstance(this->options_.camera_id, &this->camera_device_)) {
      std::cerr << "SnapdragonCamera::Start  -- Could not open camera." << std::endl;
      std::exit(EXIT_FAILURE);
    }
    this->camera_device_allocated_ = true;

    // Add listener
    this->camera_device_->addListener(this->options_.camera_listener.get());

    // Configure parameters
    camera::CameraParams camera_parameters;
    camera_parameters.init(this->camera_device_);

    camera_parameters.setPreviewSize(this->options_.image_size);
    camera_parameters.setVideoSize(this->options_.image_size);
    camera_parameters.setPictureSize(this->options_.image_size);
    camera_parameters.setFocusMode(this->options_.focus_mode);
    camera_parameters.setWhiteBalance(this->options_.white_balance);
    camera_parameters.setISO(this->options_.iso);
    camera_parameters.setSharpness(this->options_.sharpness);
    camera_parameters.setBrightness(this->options_.brightness);
    camera_parameters.setContrast(this->options_.contrast);
    // camera_parameters.setPreviewFpsRange();
    // camera_parameters.setVideoFPS();
    camera_parameters.setPreviewFpsRange(camera_parameters.getSupportedPreviewFpsRanges()[3]);
    camera_parameters.setVideoFPS(camera_parameters.getSupportedVideoFps()[0]);
    camera_parameters.setPreviewFormat(this->options_.preview_format);

    // Unused parameters
    // camera_parameters.setManualExposure(int);
    // camera_parameters.setManualGain(int);
    // camera_parameters.setVerticalFlip(bool);
    // camera_parameters.setHorizontalMirror(bool);

    // Commit the parameters to the camera device
    camera_parameters.commit();
    this->camera_device_->setParameters(camera_parameters);

    // Start the camera
    this->camera_device_->startPreview();
    this->camera_device_->startRecording();

    // Wait for stop
    std::unique_lock<std::mutex> lk(this->mtx_);
    this->cv_.wait(lk, [&]{ return (false == this->ok_); });
  }

  void SnapdragonCamera::Stop() {
    this->ok_ = false;
    this->cv_.notify_all();

    if(true == this->camera_device_allocated_) {
      this->camera_device_->stopRecording();
      this->camera_device_->stopPreview();

      camera::ICameraDevice::deleteInstance(&this->camera_device_);
      this->camera_device_allocated_ = false;
    }
  }

  void SnapdragonCamera::Options::Check() {
    if(nullptr == this->camera_listener) {
      std::cerr << "SnapdragonCamera::Options::Check -- Camera listener is null." << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }

}
