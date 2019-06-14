// Author: Tucker Haydon

#pragma once

#include <camera.h>
#include <string>

namespace snap_cam {
  // Class in charge of saving images to disk
  class ImageSaver {
    public:
      struct Options {
        Options() {}
        void Check();
      };

      ImageSaver(const Options& options)
        : options_{options} { this->options_.Check(); }

      void SaveImage(camera::ICameraFrame* frame, const std::string& frame_file_path) const;

    private:
      Options options_;
  };
}
