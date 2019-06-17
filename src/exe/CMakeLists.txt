
find_package( Boost 1.40 COMPONENTS program_options REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )

add_executable(capture capture.cc)
target_link_libraries(capture
  lib_ros
  lib_camera
  ${Boost_LIBRARIES}
)

set_target_properties(capture
  PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_compile_options(capture PRIVATE 
  -Wfatal-errors
)
