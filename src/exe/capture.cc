// Author: Tucker Haydon

#include <cstdlib>
#include <unistd.h>
#include <csignal>
#include <thread>

#include "boost/program_options.hpp" 
#include "odometry_subscriber_node.h"
#include "snapdragon_camera_listener.h"
#include "frame_output_manager.h"
#include "raw_image_saver.h"
#include "compressed_image_saver.h"
#include "metadata_logger.h"
#include "snapdragon_camera.h"

using namespace snapdragon_camera;

namespace { 
  // Signal variable and handler
  volatile std::sig_atomic_t kill_program;
  void SigIntHandler(int sig) {
    kill_program = 1;
  }

  // Helper function. Interprets a string as an camera::ImageSize. Returns
  // false if invalid.
  void ImageSizeStringToCameraImageSize(const std::string& image_size_string, camera::ImageSize* image_size) {
    if("4k" == image_size_string) {
      *image_size = SnapdragonCamera::ImageSizes::FourK();
    } else if("1080p" == image_size_string) {
      *image_size = SnapdragonCamera::ImageSizes::FHD();
    } else if("720p" == image_size_string) {
      *image_size = SnapdragonCamera::ImageSizes::HD();
    } else if("vga" == image_size_string) {
      *image_size = SnapdragonCamera::ImageSizes::VGA();
    } else {
      std::cerr << "capture -- ImageSizeStringToCameraImageSize -- Invalid image size string: " << image_size_string << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  // Helper function. Interprets a string as an image [width, height]. Returns
  // false if invalid.
  void ImageSizeStringToWidthHeight(const std::string& image_size_string, size_t image_size[2]) {
    camera::ImageSize camera_image_size;
    ImageSizeStringToCameraImageSize(image_size_string, &camera_image_size);
    image_size[0] = camera_image_size.width;
    image_size[1] = camera_image_size.height;
  }
}

int main(int argc, char** argv) {
  // Configure sigint handler
  std::signal(SIGINT, SigIntHandler);

  // Start ROS
  ros::init(argc, argv, "SnapdragonCamera", ros::init_options::NoSigintHandler | ros::init_options::AnonymousName);

  // Variables to be parsed
  std::string odom_topic;
  bool compress_images;
  std::string root_dir;
  std::string output_dir;
  std::string image_size;

  // Parse command line options with boost
  try { 
    // Define and parse the program options 
    namespace po = boost::program_options; 
    po::options_description desc(R"V0G0N(
capture_raw: capture images from the Snapdragon Flight cameras and save them to disk.

Notes: Must run this program as root. Can only access the cameras as root.

Usage)V0G0N"); 
    desc.add_options() 
      ("help,h", "Print help messages") 
      ("odom_topic", po::value<std::string>(&odom_topic)->required(), "Full path to ROS topic providing odometry data.")
      ("image_size", po::value<std::string>(&image_size)->default_value("4k"), "Size of images to be saved [4k, 1080p, 720p, vga].")
      ("compress_images", po::value<bool>(&compress_images)->default_value(false), "Compress images into jpeg format, else save raw.")
      ("root_dir", po::value<std::string>(&root_dir)->default_value("/mnt/storage/images/"), "Directory in which image directory will be placed.")
      ("output_dir", po::value<std::string>(&output_dir)->default_value(""), "Directory relative to root_dir in which images will be placed. Will be created if it does not exist.")
      ; 

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return EXIT_FAILURE;
    }

    po::notify(vm);
  }
  catch(std::exception& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return EXIT_FAILURE;
  }
  catch(...) {
      std::cerr << "Unknown error -- capture::main()" << std::endl;
      return EXIT_FAILURE;
  }

  // Program must be run as root to use the cameras.
  if(0 != getuid()) {
    std::cerr << "Only root may use the cameras! Exiting." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  // Odometry buffer sentry enabling thread-safe read/write access
  auto odometry_buffer_sentry = std::make_shared<OdometryBufferSentry>();
  OdometrySubscriberNode odometry_subscriber_node(
      odom_topic,
      odometry_buffer_sentry);

  // Declare components of camera pipeline
  std::shared_ptr<FrameOutputManager> frame_output_manager;
  std::shared_ptr<MetadataLogger> metadata_logger;
  std::shared_ptr<ImageSaver> image_saver;
  std::shared_ptr<SnapdragonCameraListener> camera_listener;
  std::shared_ptr<SnapdragonCamera> camera;

  FrameOutputManager::Options frame_output_manager_options;
  MetadataLogger::Options metadata_logger_options;
  SnapdragonCameraListener::Options camera_listener_options;
  SnapdragonCamera::Options camera_options;

  // Configure
  if(true == compress_images) {
    frame_output_manager_options.extension = ".jpg";

    size_t image_dimensions[2];
    ImageSizeStringToWidthHeight(image_size, image_dimensions);

    CompressedImageSaver::Options image_saver_options;
    image_saver_options.frame_size.width = image_dimensions[0];
    image_saver_options.frame_size.height = image_dimensions[1];
    image_saver = std::make_shared<CompressedImageSaver>(image_saver_options);
  } else {
    frame_output_manager_options.extension = ".yuv";

    RawImageSaver::Options image_saver_options;
    image_saver = std::make_shared<RawImageSaver>(image_saver_options);
  }

  frame_output_manager_options.root_dir = root_dir;
  frame_output_manager_options.output_dir = output_dir;
  frame_output_manager = std::make_shared<FrameOutputManager>(frame_output_manager_options);

  metadata_logger_options.odometry_buffer_sentry = odometry_buffer_sentry;
  metadata_logger_options.log_file_directory = frame_output_manager->OutputDirectoryPath();
  metadata_logger = std::make_shared<MetadataLogger>(metadata_logger_options);

  camera_listener_options.frame_output_manager = frame_output_manager;
  camera_listener_options.image_saver = image_saver;
  camera_listener_options.metadata_logger = metadata_logger;
  camera_listener = std::make_shared<SnapdragonCameraListener>(camera_listener_options);

  camera::ImageSize camera_image_size;
  ImageSizeStringToCameraImageSize(image_size, &camera_image_size);

  camera_options.camera_listener = camera_listener;
  camera_options.camera_id = SnapdragonCamera::CameraID::FORWARD;
  camera_options.image_size = camera_image_size;
  camera = std::make_shared<SnapdragonCamera>(camera_options);

  // Start the camera
  std::thread camera_thread([&](){ camera->Start(); });

  // Kill program thread. This thread sleeps for a bit and then checks if the
  // 'kill_program' variable has been set. If it has, it shuts ros down and
  // sends stop signals to any other threads that might be running.
  std::thread kill_thread(
      [&]() {
        while(true) {
          if(true == kill_program) {
            break;
          } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
          }
        }
        std::cout << "Killing the program..." << std::endl;
        camera->Stop();
        ros::shutdown();
      });

  // Spin for ros subscribers
  ros::spin();

  // Wait for program termination via ctl-c
  kill_thread.join();

  // Wait for other threads to die
  camera_thread.join();

  return EXIT_SUCCESS;
}
