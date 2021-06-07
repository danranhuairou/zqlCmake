# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

# vcpkg 的安装路径
set(VCPKG_PATH_ C:/Program1)
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_PATH_}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")
  
project ("zqlCmake")

# 包含子项目。
add_subdirectory (weather)
add_subdirectory (TimeRecorderConsole)
add_subdirectory (Log)
