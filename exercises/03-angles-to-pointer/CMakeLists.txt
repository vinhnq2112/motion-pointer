# Exercise 03 — mentor-owned. Builds student.cpp + tests, linked to the imu-sim physics core and runner.
cmake_minimum_required(VERSION 3.20)
project(ex03_pointer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
  add_compile_options(/utf-8)
endif()

set(SIM ${CMAKE_CURRENT_SOURCE_DIR}/../../tools/imu-sim)
set(STUDENT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/student.cpp CACHE FILEPATH "The learner's file (mentor checks only)")
add_subdirectory(${SIM}/core imusim_core)

add_executable(test_pointer tests/test_pointer.cpp ${STUDENT_SRC} ${SIM}/src/runner.cpp)
target_include_directories(test_pointer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${SIM}/include ${SIM}/src)
target_link_libraries(test_pointer PRIVATE imusim_core)
target_compile_definitions(test_pointer PRIVATE IMUSIM_PLUGIN_NAME="student.cpp")

enable_testing()
foreach(t integrate clamp center direction bound plugin-sweep plugin-wild)
  add_test(NAME ${t} COMMAND test_pointer ${t})
  set_tests_properties(${t} PROPERTIES LABELS core)
endforeach()
add_test(NAME stretch-relative COMMAND test_pointer stretch-relative)
set_tests_properties(stretch-relative PROPERTIES LABELS stretch)
