// Author: Tucker Haydon

#pragma once

#include <camera.h>
#include <camera_parameters.h>
#include <memory>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace snapdragon_camera {
  // Class that encapsulates camera configuration and functions. Configure the
  // camera with the Options structure and run the camera on its own thread
  // using the Start()/Stop() functions. Uses RAII to prevent resource leaks.
  class SnapdragonCamera {
    public:
      enum CameraID { 
        FORWARD=1, 
        DOWN=0 
      };

      struct ImageSizes {
        static camera::ImageSize FourK()        { return camera::ImageSize(3840, 2160); };
        static camera::ImageSize FHD()          { return camera::ImageSize(1920, 1080); };
        static camera::ImageSize HD()           { return camera::ImageSize(1280, 720); }; 
        static camera::ImageSize VGA()          { return camera::ImageSize(640, 480); };
      };

      struct PreviewFPSRanges {
        static camera::Range FPS15()        { return camera::Range(15000,15000,1); };
        static camera::Range FPS24()        { return camera::Range(24000,24000,1); };
        static camera::Range FPS30()        { return camera::Range(30000,30000,1); };
        static camera::Range FPS60()        { return camera::Range(60000,60000,1); };
        static camera::Range FPS90()        { return camera::Range(90000,90000,1); };
        static camera::Range FPSFloat()     { return camera::Range(7500,120000,1); };
        static camera::Range FPS120()       { return camera::Range(120000,120000,1); };
      };

      struct FocusModes {
        static std::string Fixed()               { return "fixed"; };

        // Originally these were present, but no longer.
        // static std::string Auto()               { return "auto"; };
        // static std::string Infinity()           { return "infinity"; };
        // static std::string Macro()              { return "macro"; };
        // static std::string ContinuousVideo()    { return "continuous-video"; };
        // static std::string ContinuousPicture()  { return "continuous-picture"; };
        // static std::string Manual()             { return "manual"; };
      };

      struct WhiteBalances {
        static std::string Auto()             { return "auto"; };
        static std::string Incandescent()     { return "incandescent"; };
        static std::string Fluorescent()      { return "fluorescent"; };
        static std::string WarmFluorescent()  { return "warm-fluorescent"; };
        static std::string Daylight()         { return "daylight"; };
        static std::string CloudyDaylight()   { return "cloudy-daylight"; };
        static std::string Twilight()         { return "twilight"; };
        static std::string Shade()            { return "shade"; };
        static std::string ManualCCT()        { return "manual-cct"; };
      };

      struct ISOs {
        static std::string Auto()       { return "auto"; };
        static std::string ISOHJR()     { return "ISO_HJR"; };
        static std::string ISO100()     { return "ISO100"; };
        static std::string ISO200()     { return "ISO200"; };
        static std::string ISO400()     { return "ISO400"; };
        static std::string ISO800()     { return "ISO800"; };
        static std::string ISO1600()    { return "ISO1600"; };
        static std::string ISO3200()    { return "ISO3200"; };
      };

      struct PreviewFormats {
        static std::string YUV420SP()   { return "yuv420sp"; };
        static std::string YUV420P()    { return "yuv420p"; };
        static std::string NV12Venus()  { return "nv12-venus"; };
        static std::string BayerRGGB()  { return "bayer-rggb"; };
      };

      struct Options {
        // Camera listener to attach to the camera device
        std::shared_ptr<camera::ICameraListener> camera_listener = nullptr;
        // Which camera to use. Defaults to the front camera.
        CameraID camera_id = CameraID::FORWARD;
        // Preview/Video/Picture size. Defaults to FourK.
        camera::ImageSize image_size = ImageSizes::FourK();
        // Focus mode. Unless you know what you're doing, default to auto.
        std::string focus_mode = FocusModes::Fixed();
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
        // FPS of preview stream.
        camera::Range preview_fps = PreviewFPSRanges::FPS60();
        // Sharpness. Unless you know what you're doing, default to 18. Set in
        // range [0, 36, step_size=6].
        int sharpness = 18;
        // Brightness. Unless you know what you're doing, default to 3. Set in
        // range [0,6]
        int brightness = 3;
        // Constrast. Unless you know what you're doing, default to 5. Set in
        // range [0,10]
        int contrast = 5;
        // Manual exposure. Unused. Only for the ov7251 sensor (downward
        // camera). Set in range [0,65535].
        // int exposure = 100;
        // Manual gain. Unused. Only for the ov7251 sensor (downward camera).
        // Set in range [0,255]
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
