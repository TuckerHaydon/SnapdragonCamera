// Author: Tucker Haydon

#pragma once

#include <camera.h>
#include <string>

#include "image_saver.h"

namespace snap_cam {
  // Class in charge of saving raw images to disk
  class RawImageSaver : public ImageSaver {
    public:
      struct Options {
        Options() {}
        void Check();
      };

      RawImageSaver(const Options& options)
        : options_{options} { this->options_.Check(); }

      virtual void SaveImage(camera::ICameraFrame* frame, const std::string& frame_file_path) const override;

    private:
      Options options_;
  };
}
