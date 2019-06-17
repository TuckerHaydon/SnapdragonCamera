// Author: Tucker Haydon

#pragma once

#include <camera.h>
#include <string>

#include "image_saver.h"

namespace snapdragon_camera {
  // Class in charge of compressing and saving images to disk
  class CompressedImageSaver : public ImageSaver {
    public:
      struct Options {
        struct FrameSize {
          size_t width = 0;
          size_t height = 0;
        };

        // Reported size of the compressed image in pixels. Set this.
        FrameSize frame_size;
      
        // Actual size of frame based on experiments. Set automatically. No not
        // set this.
        FrameSize true_frame_size;

        // Quality of jpeg compression [0,100], best=100
        int jpeg_compression_quality = 95;

        Options() {}
        void Check();
      };

      CompressedImageSaver(const Options& options)
        : options_{options} { this->options_.Check(); }

      virtual void SaveImage(camera::ICameraFrame* frame, const std::string& frame_file_path) const override;

    private:
      Options options_;
  };
}
