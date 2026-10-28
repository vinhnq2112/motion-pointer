cmake_minimum_required(VERSION 3.20)
project(imu_sim LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
  add_compile_options(/utf-8)
endif()

option(IMUSIM_GUI "Build the window app (downloads raylib on the first build)" ON)
set(IMUSIM_PLUGIN "plugin/placeholder.cpp" CACHE STRING
    "The .cpp file with onImuSample (absolute, or relative to tools/imu-sim)")

get_filename_component(IMUSIM_PLUGIN_ABS "${IMUSIM_PLUGIN}" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
if(NOT EXISTS "${IMUSIM_PLUGIN_ABS}")
  message(FATAL_ERROR "IMUSIM_PLUGIN file not found: ${IMUSIM_PLUGIN_ABS}")
endif()
get_filename_component(IMUSIM_PLUGIN_NAME "${IMUSIM_PLUGIN_ABS}" NAME)
# The plug-in's own folder is on the include path, so an exercise file can #include "student.h".
get_filename_component(IMUSIM_PLUGIN_DIR "${IMUSIM_PLUGIN_ABS}" DIRECTORY)
message(STATUS "imu-sim plug-in: ${IMUSIM_PLUGIN_ABS}")

add_subdirectory(core)

# Headless runner: scenarios → numbers (and CSV). No graphics.
add_executable(imu-sim-headless src/headless_main.cpp src/runner.cpp "${IMUSIM_PLUGIN_ABS}")
target_include_directories(imu-sim-headless PRIVATE include src "${IMUSIM_PLUGIN_DIR}")
target_link_libraries(imu-sim-headless PRIVATE imusim_core)
target_compile_definitions(imu-sim-headless PRIVATE IMUSIM_PLUGIN_NAME="${IMUSIM_PLUGIN_NAME}")

# Self-test of the simulator physics.
add_executable(imu-sim-selftest tests/selftest.cpp)
target_link_libraries(imu-sim-selftest PRIVATE imusim_core)

enable_testing()
add_test(NAME physics COMMAND imu-sim-selftest)
add_test(NAME headless-smoke COMMAND imu-sim-headless --scenario all --noise 0.5 --bias 0.2 --tremor 0.1)

if(IMUSIM_GUI)
  include(FetchContent)
  set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
  set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
  FetchContent_MakeAvailable(raylib)

  add_executable(imu-sim src/gui_main.cpp src/runner.cpp "${IMUSIM_PLUGIN_ABS}")
  target_include_directories(imu-sim PRIVATE include src "${IMUSIM_PLUGIN_DIR}")
  target_link_libraries(imu-sim PRIVATE imusim_core raylib)
  target_compile_definitions(imu-sim PRIVATE IMUSIM_PLUGIN_NAME="${IMUSIM_PLUGIN_NAME}")
endif()
