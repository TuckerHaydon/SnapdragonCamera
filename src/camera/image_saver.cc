// Author: Tucker Haydon

#include "image_saver.h"

namespace snap_cam {
  void ImageSaver::SaveImage(camera::ICameraFrame* frame, const std::string& frame_file_path) const {
    std::fstream frame_file(frame_file_path, std::ios::out | std::ios::binary);
    frame_file.write((char*)frame->data, frame->size);
    frame_file.close();
  }
}
