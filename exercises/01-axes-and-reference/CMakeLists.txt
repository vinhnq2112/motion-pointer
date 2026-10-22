# Exercise 01 — mentor-owned. Builds student.cpp + tests, linked to the imu-sim physics core.
cmake_minimum_required(VERSION 3.20)
project(ex01_axes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
  add_compile_options(/utf-8)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../tools/imu-sim/core imusim_core)

add_executable(test_axes tests/test_axes.cpp student.cpp)
target_include_directories(test_axes PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(test_axes PRIVATE imusim_core)

enable_testing()
foreach(t turnLeft tiltDown twistRight lookUp)
  add_test(NAME ${t} COMMAND test_axes ${t})
endforeach()
add_test(NAME stretch-rotateAbout COMMAND test_axes rotateAbout)
