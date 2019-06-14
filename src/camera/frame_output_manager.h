// Author: Tucker Haydon

#pragma once

#include <string>

namespace snap_cam {
  // The FrameOutputManager maintains state variables related to image capture.
  // Provides access to the file path and file name of the next frame. Manages
  // creation and maintenance of the directories where images will be saved.
  class FrameOutputManager {
    public:
      struct Options {
        // Filepath extension
        std::string extension = ".yuv";
        // Directory containing the output_dir. Absolute path. Must be
        // terminated with a forward slash.
        std::string root_dir = "/mnt/storage/images/";
        // Directory in which images will be placed. Relative to the root dir.
        // Must be terminated with a forward-slash. If left empty, a new
        // directory in root_dir will be created with the current date.
        std::string output_dir = "";

        Options() {}
        void Check();
      };

      FrameOutputManager(const Options& options)
        : options_{options} { this->options_.Check(); }

      // Return the file name of the current image
      std::string FrameFileName() const;

      // Return the file path of the current image
      std::string FrameFilePath() const;

      // Return the absolute path to the output directory
      std::string OutputDirectoryPath() const;

      // Increment to the next image
      void Increment();

    private:
      Options options_;

      size_t frame_number{0};
  };
}
