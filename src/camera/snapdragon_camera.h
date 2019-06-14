// Author: Tucker Haydon

#pragma once

#include <camera.h>
#include <camera_parameters.h>
#include <memory>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace snap_cam {
  class SnapdragonCamera {
    public:
      enum CameraID { 
        FORWARD=1, 
        DOWN=0 
      };

      struct ImageSizes {
        static camera::ImageSize FourK()        { static camera::ImageSize is(4096, 2160); return is; };
        static camera::ImageSize UHD()          { static camera::ImageSize is(3840, 2160); return is; };
        static camera::ImageSize FHD()          { static camera::ImageSize is(1920, 1080); return is; };
        static camera::ImageSize HD()           { static camera::ImageSize is(1280, 720);  return is; };
        static camera::ImageSize VGA()          { static camera::ImageSize is(640, 480);   return is; };
        static camera::ImageSize StereoVGA()    { static camera::ImageSize is(1280, 480);  return is; };
        static camera::ImageSize QVGA()         { static camera::ImageSize is(320, 240);   return is; };
        static camera::ImageSize StereoQVGA()   { static camera::ImageSize is(640, 240);   return is; };
      };

      struct FocusModes {
        static std::string Auto()       { return "auto"; };
      };

      struct WhiteBalances {
        static std::string Auto()       { return "auto"; };
      };

      struct ISOs {
        static std::string ISO3200()    { return "ISO3200"; };
      };

      struct PreviewFormats {
        static std::string YUV420SP()   { return "yuv420sp"; };
      };

      struct Options {
        // Camera listener to attach to the camera device
        std::shared_ptr<camera::ICameraListener> camera_listener = nullptr;
        // Which camera to use. Defaults to the front camera.
        CameraID camera_id = CameraID::FORWARD;
        // Preview/Video/Picture size. Defaults to UHD.
        camera::ImageSize image_size = ImageSizes::UHD();
        // Focus mode. Unless you know what you're doing, default to auto.
        std::string focus_mode = FocusModes::Auto();
        // White balance. Unless you know what you're doing, default to auto.
        std::string white_balance = WhiteBalances::Auto();
        // ISOs. Unless you know what you're doing, use ISO3200. This is the
        // highest ISO setting. Won't perform very well in low-light settings,
        // but allows the sensor to saturate and return and image much faster in
        // high-light settings, reducing the effect of vibrations and motion
        // blur.
        std::string iso = ISOs::ISO3200();
        // Preview format. Experiment with different modes and update this
        // documentation. As far as I remember, the forward-facing camera only
        // works with yuv420sp. I don't remember what the downward-facing camera
        // uses.
        std::string preview_format = PreviewFormats::YUV420SP();
        // Sharpness. Unless you know what you're doing, default to 18
        int sharpness = 18;
        // Brightness. Unless you know what you're doing, default to 3
        int brightness = 3;
        // Constrast. Unless you know what you're doing, default to 5
        int contrast = 5;
        // Manual exposure. Unused.
        // int exposure = 100;
        // Manual gain. Unused.
        // int gain = 50;

        Options() {}
        void Check();
      };


      SnapdragonCamera(const Options& options);
      ~SnapdragonCamera();

      void Start();
      void Stop();

    private:
      Options options_;
      camera::ICameraDevice* camera_device_;
      std::atomic<bool> camera_device_allocated_{false};

      std::mutex mtx_;
      std::condition_variable cv_;
      volatile std::atomic<bool> ok_{true};
  };
}
