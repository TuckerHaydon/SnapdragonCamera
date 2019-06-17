// Author: Tucker Haydon

#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <grp.h>
#include <pwd.h>
#include <unistd.h>

#include "compressed_image_saver.h"

namespace snapdragon_camera {
  void CompressedImageSaver::SaveImage(camera::ICameraFrame* frame, const std::string& frame_file_path) const {

    // Convert image from NV12 to rgb
    cv::Mat yuv_frame = cv::Mat(
        1.5 * this->options_.true_frame_size.height, 
        this->options_.true_frame_size.width, 
        CV_8UC1, 
        frame->data);

    cv::Mat rgb_frame;
    cv::cvtColor(yuv_frame, rgb_frame, CV_YUV420sp2RGB);
    yuv_frame.release();

    // Crop image, removing green bar
    cv::Rect cropping_rect(0,0,this->options_.frame_size.width, this->options_.frame_size.height);
    cv::Mat rgb_frame_cropped = rgb_frame(cropping_rect);
    rgb_frame.release();

    // Compress image as jpeg and write to disk
    const std::vector<int> imwrite_params = {CV_IMWRITE_JPEG_QUALITY, this->options_.jpeg_compression_quality};
    cv::imwrite(frame_file_path, rgb_frame_cropped, imwrite_params);
    rgb_frame_cropped.release();

    // Set the user/group of the image to linaro:linaro
    {
      const char* linaro = "linaro";

      const struct group* linaro_gr = getgrnam(linaro);
      const gid_t linaro_gid = linaro_gr->gr_gid;

      const struct passwd* linaro_passwd = getpwnam(linaro);
      const uid_t linaro_uid = linaro_passwd->pw_uid;

      if(0 != chown(frame_file_path.c_str(), linaro_uid, linaro_gid)) {
        std::cerr << "CompressedImageSaver::SaveImage() -- Could not change user/group of frame." << std::endl;
        std::exit(EXIT_FAILURE);
      }
    }
  }

  void CompressedImageSaver::Options::Check() {
    // Check dimensions specified
    if(0 == this->frame_size.width || 0 == this->frame_size.height) {
      std::cerr << "CompressedImageSaver::Options::Check() -- ImageSize must be non-zero." << std::endl;
      std::exit(EXIT_FAILURE);
    }

    // Adjust dimensions. Certain image sizes have extra lines of pixels. See
    // doc/notes.md for further details.
    if(3840 == this->frame_size.width && 2160 == this->frame_size.height) {
      this->true_frame_size.width = 3840;
      this->true_frame_size.height = 2176;
    } else if(1920 == this->frame_size.width && 1080 == this->frame_size.height) {
      this->true_frame_size.width = 1920;
      this->true_frame_size.height = 1088;
    } else if(1280 == this->frame_size.width && 720 == this->frame_size.height) {
      this->true_frame_size.width = 1280;
      this->true_frame_size.height = 736;
    } else if(640 == this->frame_size.width && 480 == this->frame_size.height) {
      this->true_frame_size.width = 640;
      this->true_frame_size.height = 480;
    } else {
      std::cerr 
        << "CompressedImageSaver::Options::Check() -- Unknown image size: [" 
        << this->frame_size.width 
        << ", " 
        << this->frame_size.height 
        << "]" << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }
}
