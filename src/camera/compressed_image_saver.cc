// Author: Tucker Haydon

#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "compressed_image_saver.h"

namespace snap_cam {
  void CompressedImageSaver::SaveImage(camera::ICameraFrame* frame, const std::string& frame_file_path) const {

    cv::Mat yuv_frame = cv::Mat(
        1.5 * this->options_.frame_size.height, 
        this->options_.frame_size.width, 
        CV_8UC1, 
        frame->data);

    cv::Mat bgr_frame;
    cv::cvtColor(yuv_frame, bgr_frame, CV_YUV420sp2BGR);
    yuv_frame.release();

    const std::vector<int> imwrite_params = {CV_IMWRITE_JPEG_QUALITY, 100};
    cv::imwrite(frame_file_path, bgr_frame, imwrite_params);
    bgr_frame.release();

    // std::fstream frame_file(frame_file_path, std::ios::out | std::ios::binary);
    // frame_file.write((char*)frame->data, frame->size);
    // frame_file.close();
  }

  void CompressedImageSaver::Options::Check() {
    if(0 == this->frame_size.width || 0 == this->frame_size.height) {
      std::cerr << "CompressedImageSaver::Options::Check() -- ImageSize must be non-zero." << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }
}
