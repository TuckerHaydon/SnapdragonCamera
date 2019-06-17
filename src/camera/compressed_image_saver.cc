// Author: Tucker Haydon

#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "compressed_image_saver.h"

namespace snapdragon_camera {
  void CompressedImageSaver::SaveImage(camera::ICameraFrame* frame, const std::string& frame_file_path) const {

    // Convert image from NV12 to rgb
    cv::Mat yuv_frame = cv::Mat(
        1.5 * this->options_.frame_size.height, 
        this->options_.frame_size.width, 
        CV_8UC1, 
        frame->data);

    cv::Mat rgb_frame;
    cv::cvtColor(yuv_frame, rgb_frame, CV_YUV420sp2RGB);
    yuv_frame.release();

    // Compress image as jpeg and write to disk
    const std::vector<int> imwrite_params = {CV_IMWRITE_JPEG_QUALITY, this->options_.jpeg_compression_quality};
    cv::imwrite(frame_file_path, rgb_frame, imwrite_params);
    rgb_frame.release();
  }

  void CompressedImageSaver::Options::Check() {
    if(0 == this->frame_size.width || 0 == this->frame_size.height) {
      std::cerr << "CompressedImageSaver::Options::Check() -- ImageSize must be non-zero." << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }
}
