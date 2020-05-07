cmake_minimum_required(VERSION 3.16)

project(cxxh LANGUAGES CXX)

add_executable(cxxh "")

target_sources(cxxh PRIVATE
  main.cpp
  utilities.hh
  utilities.cpp
  parsers/identifiers.hh
  parsers/identifiers.cpp
  creators/utilities.hh
  creators/utilities.cpp
  creators/header.hh
  creators/header.cpp
  creators/source.hh
  creators/source.cpp
  creators/class.hh
  creators/class.cpp
  )

### Compilation options
# C++20
target_compile_features(cxxh PUBLIC cxx_std_20)
set_target_properties(cxxh PROPERTIES CXX_EXTENSIONS OFF)
if (MSVC)
    add_compile_options(/std:c++latest /Zc:__cplusplus)
endif()
# Enable Link Time Optimization if available
include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
  set_target_properties(cxxh PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
  message("Enabled Link Time optimization")
endif()

find_package(cxxopts CONFIG REQUIRED)
target_link_libraries(cxxh PRIVATE cxxopts::cxxopts)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(cxxh PRIVATE fmt::fmt fmt::fmt-header-only)

find_package(Boost REQUIRED COMPONENTS program_options locale)
target_link_libraries(cxxh PRIVATE ${Boost_LIBRARIES})

find_package(Microsoft.GSL CONFIG REQUIRED)
target_link_libraries(cxxh PRIVATE Microsoft.GSL::GSL)

target_include_directories(cxxh PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
