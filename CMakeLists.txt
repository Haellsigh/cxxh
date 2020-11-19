cmake_minimum_required(VERSION 3.19)

# Vcpkg as a submodule
set(VCPKG_TARGET_TRIPLET "x64-windows-static")
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

project(cxxh LANGUAGES CXX)

add_executable(cxxh "")

target_sources(cxxh PRIVATE
  main.cc
  utilities.hh
  utilities.cc
  )


target_include_directories(cxxh PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

##### Compilation options
# C++20
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
if (MSVC)
  target_compile_options(${PROJECT_NAME} PUBLIC /Zc:__cplusplus)
endif()
# Enable Link Time Optimization if available
if(ipo_supported)
  set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
# Warnings
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /WX /W4)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra -Wconversion -Wsign-conversion)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS program_options locale REQUIRED)
target_link_libraries(cxxh PRIVATE ${Boost_LIBRARIES})

find_package(Microsoft.GSL CONFIG REQUIRED)
target_link_libraries(cxxh PRIVATE Microsoft.GSL::GSL)

find_package(cxxopts CONFIG REQUIRED)
target_link_libraries(cxxh PRIVATE cxxopts::cxxopts)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(cxxh PRIVATE fmt::fmt)
