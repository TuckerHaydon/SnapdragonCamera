// Author: Tucker Haydon

#include <fstream>
#include <iostream>
#include <grp.h>
#include <pwd.h>
#include <unistd.h>

#include "raw_image_saver.h"

namespace snapdragon_camera {
  void RawImageSaver::SaveImage(camera::ICameraFrame* frame, const std::string& frame_file_path) const {
    std::fstream frame_file(frame_file_path, std::ios::out | std::ios::binary);
    frame_file.write((char*)frame->data, frame->size);
    frame_file.close();

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
