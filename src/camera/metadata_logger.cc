// Author: Tucker Haydon

#include "metadata_logger.h"

#include <iostream>
#include <cstdlib>
#include <ios>
#include <sys/stat.h>
#include <iomanip>
#include <grp.h>
#include <pwd.h>
#include <unistd.h>
#include <cstring>

namespace snapdragon_camera {

  void MetadataLogger::Options::Check() {
    // Check odometry_buffer_sentry not null
    if(nullptr == this->odometry_buffer_sentry) {
      std::cerr << "MetadataLogger::Options::Check -- odometry_buffer_sentry is null." << std::endl;
      exit(EXIT_FAILURE);
    }

    // Checkout log_file_directory exists
    {
      struct stat info;
      if(0 != stat((this->log_file_directory).c_str(), &info)) {
        std::cerr 
          << "MetadataLogger::Options::Check -- Cannot access log_file_directory: " 
          << this->log_file_directory
          << std::endl;
        exit(EXIT_FAILURE);
      } else if(false == (info.st_mode & S_IFDIR)) {
        std::cerr 
          << "MetadataLogger::Options::Check -- log_file_directory does not exist: " 
          << this->log_file_directory
          << std::endl;
        exit(EXIT_FAILURE);
      }
    }

    if(true == this->log_file_name.empty()) {
      std::cerr << "MetadataLogger::Options::Check -- Log file name is empty." << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  MetadataLogger::MetadataLogger(const Options& options)
    : options_(options) {
    this->options_.Check();

    // Create odometry circular buffer
    this->odometry_circular_buffer_ 
      = std::make_shared<OdometryCircularBuffer>(this->options_.odometry_circular_buffer_options);

    // Create log file
    const std::string log_file_path = this->options_.log_file_directory + this->options_.log_file_name;
    this->log_file_ = std::make_shared<std::ofstream>(log_file_path, std::ios_base::trunc);

    // Set the user/group of the log file to linaro:linaro
    {
      const char* linaro = "linaro";

      const struct group* linaro_gr = getgrnam(linaro);
      const gid_t linaro_gid = linaro_gr->gr_gid;

      const struct passwd* linaro_passwd = getpwnam(linaro);
      const uid_t linaro_uid = linaro_passwd->pw_uid;

      if(0 != chown(log_file_path.c_str(), linaro_uid, linaro_gid)) {
        std::cerr << "MetadataLogger::MetadataLogger() -- Could not change user/group of log file." << std::endl;
        std::exit(EXIT_FAILURE);
      }
    }

    // Set the floating point precision
    *(this->log_file_) << std::setprecision(10);

    // Write header
    *(this->log_file_) << "# Log File Information:" << std::endl;
    *(this->log_file_) << "#   Each entry in this log file is a new entry." << std::endl;
    *(this->log_file_) << "#   The format of each row is described in the next section." << std::endl;
    *(this->log_file_) << "# Log Row Format:" << std::endl;
    *(this->log_file_) << "#   File name" << std::endl;
    *(this->log_file_) << "#   Position (X Y Z)" << std::endl;
    *(this->log_file_) << "#   Orientation (W X Y Z) " << std::endl;
    *(this->log_file_) << "#   Timing (ros_sec ros_nsec cam_time_stamp) " << std::endl;
    *(this->log_file_) << "#   PoseCovariance (36 elements, [position, orientation], row-major)" << std::endl;
  }

  MetadataLogger::~MetadataLogger() {
    // RAII
    this->log_file_->close();
  }

  void MetadataLogger::Log(const std::string& frame_file_name, const uint64_t& time_stamp) const {
    // Retrieve most recent pose
    OdometryBuffer recent_odometry_buffer;
    this->options_.odometry_buffer_sentry->Read(recent_odometry_buffer);

    // Cycle the circular buffer and retrieve the synchonized odom data
    OdometryBuffer synchronized_odometry_buffer;
    bool synchronized = this->odometry_circular_buffer_->Cycle(recent_odometry_buffer, synchronized_odometry_buffer);

    // If not yet synchronized, write comment line with a zero'd out odom buffer
    if(false == synchronized) {
      std::memset(&synchronized_odometry_buffer, 0, sizeof(synchronized_odometry_buffer));  
      *(this->log_file_) << "# ";
    }

    // Log data
    *(this->log_file_)
      << frame_file_name << " "
      << synchronized_odometry_buffer.position.x << " "
      << synchronized_odometry_buffer.position.y << " "
      << synchronized_odometry_buffer.position.z << " "
      << synchronized_odometry_buffer.orientation.w << " "
      << synchronized_odometry_buffer.orientation.x << " "
      << synchronized_odometry_buffer.orientation.y << " "
      << synchronized_odometry_buffer.orientation.z << " "
      << synchronized_odometry_buffer.time.sec << " "
      << synchronized_odometry_buffer.time.nsec << " "
      << time_stamp << " ";

    // 36 element pose covariance
    for(size_t idx = 0; idx < 36; ++idx) {
      *(this->log_file_) << synchronized_odometry_buffer.pose_covariance[idx];
      if(35 != idx) {
        *(this->log_file_) << " ";
      }
    }

    *(this->log_file_) << std::endl;
    this->log_file_->flush();
  }
}
