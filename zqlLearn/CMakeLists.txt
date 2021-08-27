# CMakeLists.txt
cmake_minimum_required(VERSION 3.20.2)

# vcpkg
set(VCPKG_PATH_ C:/Program1)
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_PATH_}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")
  
project(zqlLearn)

# 包含子项目。
add_subdirectory (cmake/cmake_vcpkg_test)
add_subdirectory (cmake/cmake_lib)
add_subdirectory (example)