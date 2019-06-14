// Author: Tucker Haydon

#include "frame_output_manager.h"

namespace snap_cam {

  std::string FrameOutputManager::FrameFilePath() const {
    const std::string file_path = 
      this->options_.root_dir + 
      this->options_.output_dir + 
      this->FrameFileName();
    return file_path;
  }

  std::string FrameOutputManager::FrameFileName() const {
    std::stringstream ss;
    ss << std::setw(5) << std::setfill('0') << this->frame_number;
    const std::string file_name = "frame" + ss.str() + this->options_.extension;
    return file_name;
  }

  void FrameOutputManager::Increment() {
    this->frame_number++;
  }

  void FrameOutputManager::Options::Check() {
    // Check root dir exists
    {
      struct stat info;
      if(0 != stat(this->root_dir, &info)) {
        std::cerr 
          << "FrameOutputManager::Options::Configure -- Cannot access root dir: " 
          << this->root_dir 
          << std::endl;
        exit(EXIT_FAILURE);
      } else if(false == (info.st_mode & S_IFDIR)) {
        std::cerr 
          << "FrameOutputManager::Options::Configure -- Root dir does not exist: " 
          << this->root_dir 
          << std::endl;
        exit(EXIT_FAILURE);
      }
    }

    // Create output directory if not specified
    if(true == this->output_dir.empty()) {
      time_t now = time(0);
      tm *ltm = localtime(&now);
      std::string year = std::to_string(1900 + ltm->tm_year);
      std::string month = std::to_string(1 + ltm->tm_mon);
      std::string day = std::to_string(ltm->tm_mday);
      std::string hour = std::to_string(ltm->tm_hour);
      std::string min = std::to_string(ltm->tm_min);
      std::string sec = std::to_string(ltm->tm_sec);
      std::string output_dir_name = year + "-" + month + "-" + day + "-" + hour + "-" + min + "-" + sec + "/";
      std::string output_dir_path = this->options_.root_dir + output_dir_name;
      const int err = mkdir(full_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
      if(-1 == err){
          std::cerr << "FrameOutputManager::Options::Configure -- Could not create output dir!" << std::endl;
          exit(EXIT_FAILURE);
      }
      this->output_dir = output_dir_name;
    }
 
    // Checkout output dir exists
    {
      struct stat info;
      if(0 != stat(this->root_dir + this->output_dir, &info)) {
        std::cerr 
          << "FrameOutputManager::Options::Configure -- Cannot access output dir: " 
          << this->root_dir + this->output_dir 
          << std::endl;
        exit(EXIT_FAILURE);
      } else if(false == (info.st_mode & S_IFDIR)) {
        std::cerr 
          << "FrameOutputManager::Options::Configure -- Output dir does not exist: " 
          << this->root_dir + this->output_dir 
          << std::endl;
        exit(EXIT_FAILURE);
      }
    }
  }
}
