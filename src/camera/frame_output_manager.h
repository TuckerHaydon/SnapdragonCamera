// Author: Tucker Haydon

#pragma once

namespace snap_cam {
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

      std::string FrameFileName() const;
      std::string FrameFilePath() const;
      void Increment();

    private:
      Options options_;

      size_t frame_number{0};
  };
}
