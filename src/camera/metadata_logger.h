// Author: Tucker Haydon

#pragma once

#include <memory>
#include <string>
#include <fstream>

#include "odometry_buffer_sentry.h"

namespace snapdragon_camera {
  class MetadataLogger {
    public:
      struct Options {
        // Required: Sentry protecting access to the most current odom data
        std::shared_ptr<OdometryBufferSentry> odometry_buffer_sentry = nullptr;
        // Required: Log file directory. Must terminate with forward slash.
        std::string log_file_directory = "";
        // Name of log file
        std::string log_file_name = "metadata.log";

        Options() {}
        void Check();
      };

      // Constructor. Opens logging file. RAII.
      MetadataLogger(const Options& options);

      // Destructor. Closes logging file. RAII.
      ~MetadataLogger();

      // Write a new metadata line to the log file
      void Log(const std::string& frame_file_name) const;

    private:
      Options options_;

      // Pointer to log file stream
      std::shared_ptr<std::ofstream> log_file_;
  };
}
