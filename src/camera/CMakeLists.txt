set(TARGET lib_camera)

find_library(CAMERA_LIBS NAMES camera PATHS /usr/lib)

if (NOT CAMERA_LIBS)
  message(FATAL_ERROR "Could not find camera libraries! Are you sure they are
  installed? Stopping CMake.")
endif()

find_package( OpenCV REQUIRED )

set(SOURCE_FILES
  snapdragon_camera_listener.cc
  frame_output_manager.cc
  metadata_logger.cc
  snapdragon_camera.cc
  compressed_image_saver.cc
  raw_image_saver.cc
)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_include_directories(${TARGET} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${TARGET} PUBLIC 
  lib_base
  lib_ros
  ${CAMERA_LIBS}
  ${OpenCV_LIBS}
)

set_target_properties(${TARGET} PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_compile_options(${TARGET} PRIVATE 
  -Wfatal-errors
)
