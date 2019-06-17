// Author: Tucker Haydon

#pragma once

#include <camera.h>
#include <string>

namespace snap_cam {
  // Interface for saving images
  class ImageSaver {
    public:
      virtual void SaveImage(camera::ICameraFrame* frame, const std::string& frame_file_path) const = 0;
  };
}
