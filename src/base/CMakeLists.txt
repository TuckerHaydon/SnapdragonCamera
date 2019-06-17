set(TARGET lib_base)

set(SOURCE_FILES
  odometry_buffer_sentry.cc
  moving_average.cc
)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_include_directories(${TARGET} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# target_link_libraries(${TARGET} PUBLIC )

set_target_properties(${TARGET} PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_compile_options(${TARGET} PRIVATE 
  -Wfatal-errors
)
