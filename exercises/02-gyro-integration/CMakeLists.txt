# Exercise 02 — mentor-owned. Builds student.cpp + tests, linked to the imu-sim physics core and runner.
cmake_minimum_required(VERSION 3.20)
project(ex02_gyro LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
  add_compile_options(/utf-8)
endif()

set(SIM ${CMAKE_CURRENT_SOURCE_DIR}/../../tools/imu-sim)
set(STUDENT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/student.cpp CACHE FILEPATH "The learner's file (mentor checks only)")
add_subdirectory(${SIM}/core imusim_core)

add_executable(test_gyro tests/test_gyro.cpp ${STUDENT_SRC} ${SIM}/src/runner.cpp)
target_include_directories(test_gyro PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${SIM}/include ${SIM}/src)
target_link_libraries(test_gyro PRIVATE imusim_core)
target_compile_definitions(test_gyro PRIVATE IMUSIM_PLUGIN_NAME="student.cpp")

enable_testing()
foreach(t oneSample-yaw oneSample-pitch yawSweep pitchSweep sampleRate plugin)
  add_test(NAME ${t} COMMAND test_gyro ${t})
endforeach()
