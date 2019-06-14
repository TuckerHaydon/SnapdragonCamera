// Author: Tucker Haydon

#include <cstdlib>
#include <unistd.h>

#include "boost/program_options.hpp" 
#include "odometry_subscriber_node.h"
#include "camera_listener.h"
#include "frame_output_manager.h"
#include "image_saver.h"
#include "metadata_logger.h"

using namespace snap_cam;

int main(int argc, char** argv) {
  // Variables to be parsed
  std::string odom_topic;

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
      ("odom_topic", po::value<std::string>(&odom_topic)->required(), "Full path to ROS topic providing odometry data")
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
      std::cerr << "Unknown error -- capture_raw::main()" << std::endl;
      return EXIT_FAILURE;
  }

  // Program must be run as root to use the cameras.
  if(0 != getuid()) {
    std::cerr << "Only root may use the cameras! Exiting." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  auto odometry_buffer_sentry = std::make_shared<OdometryBufferSentry>();
  OdometrySubscriberNode odometry_subscriber_node(
      odom_topic,
      odometry_buffer_sentry);

  auto frame_output_manager = std::make_shared<FrameOutputManager>(FrameOutputManager::Options());

  MetadataLogger::Options metadata_logger_options;
  metadata_logger_options.odometry_buffer_sentry = odometry_buffer_sentry;
  metadata_logger_options.log_file_directory = frame_output_manager->OutputDirectoryPath();
  auto metadata_logger = std::make_shared<MetadataLogger>(metadata_logger_options);

  auto image_saver = std::make_shared<ImageSaver>(ImageSaver::Options());

  CameraListener::Options camera_listener_options;
  camera_listener_options.frame_output_manager = frame_output_manager;
  camera_listener_options.image_saver = image_saver;
  auto camera_listener = std::make_shared<CameraListener>(camera_listener_options);


  return EXIT_SUCCESS;
}
