# Motion model, sensor model and scenarios. No graphics, so exercise tests can link it:
#   add_subdirectory(<path>/tools/imu-sim/core imusim_core)
#   target_link_libraries(<test> PRIVATE imusim_core)
add_library(imusim_core STATIC imusim_core.cpp)
target_include_directories(imusim_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(imusim_core PUBLIC cxx_std_17)
if(MSVC)
  target_compile_options(imusim_core PRIVATE /utf-8)
endif()
