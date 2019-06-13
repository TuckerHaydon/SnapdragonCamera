// Author: Tucker Haydon

#include <cstdlib>

#include "boost/program_options.hpp" 
#include "odometry_subscriber_node.h"

using namespace snap_cam;

int main(int argc, char** argv) {

  std::string odom_topic;

  // Parse command line options with boost
  try { 
    // Define and parse the program options 
    namespace po = boost::program_options; 
    po::options_description desc("Options"); 
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

  auto odometry_buffer_sentry = std::make_shared<OdometryBufferSentry>();

  OdometrySubscriberNode odometry_subscriber_node(
      odom_topic,
      odometry_buffer_sentry);

  return EXIT_SUCCESS;
}
