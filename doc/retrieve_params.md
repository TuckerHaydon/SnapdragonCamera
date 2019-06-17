# Retrieving Camera Params

## Code
```
std::vector<camera::ImageSize> supported_preview_sizes = camera_parameters.getSupportedPreviewSizes();
std::vector<camera::ImageSize> supported_video_sizes = camera_parameters.getSupportedVideoSizes();
std::vector<camera::ImageSize> supported_picture_sizes = camera_parameters.getSupportedPictureSizes();
std::vector<std::string> supported_focus_modes = camera_parameters.getSupportedFocusModes();
std::vector<std::string> supported_white_balance_modes = camera_parameters.getSupportedWhiteBalance();
std::vector<std::string> supported_isos = camera_parameters.getSupportedISO();
camera::Range supported_sharpness = camera_parameters.getSupportedSharpness();
camera::Range supported_brightness = camera_parameters.getSupportedBrightness();
camera::Range supported_contrast = camera_parameters.getSupportedContrast();
std::vector<camera::Range> supported_preview_fps_ranges = camera_parameters.getSupportedPreviewFpsRanges();
std::vector<camera::VideoFPS> supported_video_fps = camera_parameters.getSupportedVideoFps();
std::vector<std::string> supported_preview_formats = camera_parameters.getSupportedPreviewFormats();

std::cout << "Supported Preview Sizes" << std::endl;
std::cout << "[width, height]" << std::endl;
for(const auto& is: supported_preview_sizes) {
  std::cout << "[" << is.width << ", " << is.height << "]" << std::endl;
}
std::cout << ""  << std::endl;

std::cout << "Supported Video Sizes" << std::endl;
std::cout << "[width, height]" << std::endl;
for(const auto& is: supported_video_sizes) {
  std::cout << "[" << is.width << ", " << is.height << "]" << std::endl;
}
std::cout << ""  << std::endl;

std::cout << "Supported Picture Sizes" << std::endl;
std::cout << "[width, height]" << std::endl;
for(const auto& is: supported_picture_sizes) {
  std::cout << "[" << is.width << ", " << is.height << "]" << std::endl;
}
std::cout << ""  << std::endl;

std::cout << "Supported Focus Modes" << std::endl;
for(const auto& fm: supported_focus_modes) {
  std::cout << fm << std::endl;
}
std::cout << ""  << std::endl;

std::cout << "Supported White Balance Modes" << std::endl;
for(const auto& wb: supported_white_balance_modes) {
  std::cout << wb << std::endl;
}
std::cout << ""  << std::endl;

std::cout << "Supported ISOs" << std::endl;
for(const auto& iso: supported_isos) {
  std::cout << iso << std::endl;
}
std::cout << ""  << std::endl;

std::cout << "Supported Sharpness" << std::endl;
std::cout << "[min, max, step]" << std::endl;
std::cout << "[" << supported_sharpness.min << ", " << supported_sharpness.max << ", " << supported_sharpness.step << "]" << std::endl;
std::cout << ""  << std::endl;

std::cout << "Supported Brightness" << std::endl;
std::cout << "[min, max, step]" << std::endl;
std::cout << "[" << supported_brightness.min << ", " << supported_brightness.max << ", " << supported_brightness.step << "]" << std::endl;
std::cout << ""  << std::endl;

std::cout << "Supported Contrast" << std::endl;
std::cout << "[min, max, step]" << std::endl;
std::cout << "[" << supported_contrast.min << ", " << supported_contrast.max << ", " << supported_contrast.step << "]" << std::endl;
std::cout << ""  << std::endl;

std::cout << "Supported Preview FPS Ranges" << std::endl;
std::cout << "[min, max, step]" << std::endl;
for(const auto& p: supported_preview_fps_ranges) {
  std::cout << "[" << p.min << ", " << p.max << ", " << p.step << "]" << std::endl;
}
std::cout << ""  << std::endl;

std::cout << "Supported VideoFPS" << std::endl;
for(const auto& p: supported_video_fps) {
  std::cout << p << std::endl;
}
std::cout << ""  << std::endl;


std::cout << "Supported Preview Formats" << std::endl;
for(const auto& p: supported_preview_formats) {
  std::cout << p << std::endl;
}

std::exit(EXIT_FAILURE);
```

## Output
Supported Preview Sizes
[width, height]
[4208, 3120]
[4096, 2160]
[3840, 2160]
[2560, 1440]
[2104, 1560]
[1920, 1080]
[1280, 720]
[864, 480]
[800, 480]
[800, 400]
[768, 432]
[720, 480]
[640, 480]
[640, 240]
[1280, 480]
[576, 432]
[480, 320]
[384, 288]
[352, 288]
[320, 240]
[240, 160]
[176, 144]

Supported Video Sizes
[width, height]
[4208, 3120]
[4096, 2160]
[3840, 2160]
[2560, 1440]
[2104, 1560]
[1920, 1080]
[1280, 720]
[864, 480]
[800, 480]
[720, 480]
[640, 480]
[640, 240]
[1280, 480]
[480, 320]
[352, 288]
[320, 240]
[176, 144]

Supported Picture Sizes
[width, height]
[4208, 3120]
[4128, 3096]
[4128, 2322]
[4096, 2160]
[3840, 2160]
[4000, 3000]
[3200, 2400]
[2592, 1944]
[2104, 1560]
[2048, 1536]
[1920, 1080]
[1600, 1200]
[1280, 768]
[1280, 720]
[1024, 768]
[800, 600]
[864, 480]
[800, 480]
[720, 480]
[640, 480]
[640, 240]
[1280, 480]
[352, 288]
[320, 240]

Supported Focus Modes
fixed

Supported White Balance Modes
auto
incandescent
fluorescent
warm-fluorescent
daylight
cloudy-daylight
twilight
shade
manual-cct

Supported ISOs
auto
ISO_HJR
ISO100
ISO200
ISO400
ISO800
ISO1600
ISO3200

Supported Sharpness
[min, max, step]
[0, 36, 6]

Supported Brightness
[min, max, step]
[0, 6, 1]

Supported Contrast
[min, max, step]
[0, 10, 1]

Supported Preview FPS Ranges
[min, max, step]
[15000, 15000, 1]
[24000, 24000, 1]
[30000, 30000, 1]
[60000, 60000, 1]
[90000, 90000, 1]
[7500, 120000, 1]
[120000, 120000, 1]

Supported VideoFPS
2
3
4
1

Supported Preview Formats
yuv420sp
yuv420p
nv12-venus
bayer-rggb
