// Author: Tucker Haydon

#include <fstream>
#include <iostream>
#include <grp.h>
#include <pwd.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "raw_image_saver.h"

namespace snapdragon_camera {
  void RawImageSaver::SaveImage(camera::ICameraFrame* frame, const std::string& frame_file_path) const {

    // Use the O_SYNC flag to force filesystem synchronization after each write.
    // Sacrifices kernel buffering and potential optimizations therewith, but
    // ensures that images are written to the disk at a predicatble and uniform
    // rate. With the kernel buffering, images tend to queue up and be written
    // all at once, blocking the camera from capturing images and causing video
    // jumps.
    int fd = open(frame_file_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_SYNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(-1 == fd) {
      std::cerr << "RawImageSaver::SaveImage() -- Error opening file: " << frame_file_path << std::endl;
      std::exit(EXIT_FAILURE);
    }

    size_t num_bytes_written = write(fd, reinterpret_cast<const void*>(frame->data), frame->size);
    if(frame->size != num_bytes_written) {
      std::cerr << "RawImageSaver::SaveImage() -- Error writing data!" << std::endl;
      std::exit(EXIT_FAILURE);
    }

    if(-1 == close(fd)) {
      std::cerr << "RawImageSaver::SaveImage() -- Error closing file: " << frame_file_path << std::endl;
      std::exit(EXIT_FAILURE);
    }

    // Set the user/group of the image to linaro:linaro
    {
      const char* linaro = "linaro";

      const struct group* linaro_gr = getgrnam(linaro);
      const gid_t linaro_gid = linaro_gr->gr_gid;

      const struct passwd* linaro_passwd = getpwnam(linaro);
      const uid_t linaro_uid = linaro_passwd->pw_uid;

      if(0 != chown(frame_file_path.c_str(), linaro_uid, linaro_gid)) {
        std::cerr << "RawImageSaver::SaveImage() -- Could not change user/group of frame." << std::endl;
        std::exit(EXIT_FAILURE);
      }
    }
  }

  void RawImageSaver::Options::Check() {

  }
}
