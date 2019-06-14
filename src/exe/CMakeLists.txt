
find_package( Boost 1.40 COMPONENTS program_options REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )

add_executable(capture_raw capture_raw.cc)
target_link_libraries(capture_raw
  lib_ros
  lib_camera
  ${Boost_LIBRARIES}
)

set_target_properties(capture_raw
  PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_compile_options(capture_raw PRIVATE 
  -Wfatal-errors
)
