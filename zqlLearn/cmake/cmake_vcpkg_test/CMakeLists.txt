# CMakeLists.txt
cmake_minimum_required(VERSION 3.20.2)

# vcpkg 的安装路径
set(VCPKG_PATH_ C:/Program1)
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_PATH_}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")

project(cmake_vcpkg_test)

# 导入包
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(glog CONFIG REQUIRED) 

# 添加源文件
add_executable(${PROJECT_NAME} "main.cpp")

# 链接包
target_link_libraries(${PROJECT_NAME} PRIVATE glog::glog)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::sqlite3::sqlite3)

