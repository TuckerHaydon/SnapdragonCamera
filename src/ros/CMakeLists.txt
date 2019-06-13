set(TARGET lib_ros)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  message_generation
  nav_msgs
)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

set(SOURCE_FILES
  odometry_subscriber_node.cc
  odometry_buffer_sentry.cc
)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_include_directories(${TARGET} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${catkin_INCLUDE_DIRS}
)

target_link_libraries(${TARGET} PUBLIC 
  Eigen3::Eigen
  ${catkin_LIBRARIES}
)

add_dependencies(${TARGET} nav_msgs_generate_messages_cpp)

set_target_properties(${TARGET} PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_compile_options(${TARGET} PRIVATE 
  -Wfatal-errors
)
