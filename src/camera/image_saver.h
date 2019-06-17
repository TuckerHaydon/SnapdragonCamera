// Author: Tucker Haydon

#pragma once

#include <camera.h>
#include <string>

namespace snapdragon_camera {
  // Interface for saving images
  class ImageSaver {
    public:
      virtual void SaveImage(camera::ICameraFrame* frame, const std::string& frame_file_path) const = 0;
  };
}
