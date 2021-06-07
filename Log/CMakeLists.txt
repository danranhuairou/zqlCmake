# zql Log Interface and implementation
#
cmake_minimum_required (VERSION 3.8)

project(Log)

# 添加项目文件
add_executable(LogTest "log.h" "log.cpp" "test.cpp")

add_library(Log "log.cpp")
