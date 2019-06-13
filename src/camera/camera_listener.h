// Author: Tucker Haydon

// System libraries to interface with the camera
#include <camera.h>
#include <camera_parameters.h>

namespace snap_cam {

  class CameraListener : public camera::ICameraListener {
    public:
      // Options to configure this class
      struct Options {

      };

      // Constructor
      CameraListener(const Options& options = Options())
        : options_(options) {}

      // Virtual functions from ICameraListener
      virtual void onError() override;
      virtual void onControl(const camera::ControlEvent& control) override;
      virtual void onPreviewFrame(camera::ICameraFrame* frame) override;
      virtual void onVideoFrame(camera::ICameraFrame* frame) override;
      virtual void onPictureFrame(camera::ICameraFrame* frame) override;
      virtual void onMetadataFrame(camera::ICameraFrame* frame) override;

    private:
      Options options_;

  };
}
