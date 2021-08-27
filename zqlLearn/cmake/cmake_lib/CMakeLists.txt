# 父目录下的CMakeLists.txt
cmake_minimum_required(VERSION 3.20.2)

project(cmake_lib)

# 添加包含文件
include_directories(sub)

# 添加子项目
add_subdirectory(sub output) 

# 添加源文件
add_executable(${PROJECT_NAME} "main.cpp")

# 链接子项目
target_link_libraries(${PROJECT_NAME} sub)