// Author: Tucker Haydon

// #include <camera_parameters.h>
#include <camera.h>
#include <memory>
#include <atomic>

#include "frame_output_manager.h"
#include "image_saver.h"

namespace snap_cam {
  class CameraListener : public camera::ICameraListener {
    public:
      // Options to configure this class
      struct Options {
        // Required. Pointer to object that will save an image.
        std::shared_ptr<ImageSaver> image_saver = nullptr;

        // Required. Point to object that manages the frame names and file
        // paths
        std::shared_ptr<FrameOutputManager> frame_output_manager = nullptr;

        Options() {}
        void Check();
      };

      // Constructor
      CameraListener(const Options& options)
        : options_(options) { this->options_.Check(); }

      // Virtual functions from ICameraListener
      // Only the onVideoFrame function is implemented
      virtual void onError() override;
      virtual void onControl(const camera::ControlEvent& control) override;
      virtual void onPreviewFrame(camera::ICameraFrame* frame) override;
      virtual void onVideoFrame(camera::ICameraFrame* frame) override;
      virtual void onPictureFrame(camera::ICameraFrame* frame) override;
      virtual void onMetadataFrame(camera::ICameraFrame* frame) override;

    private:
      Options options_;

      // Atomic fence that prevents multiple images from being written to the
      // disk at the same time
      std::atomic<bool> occupied_{false};
  };
}
