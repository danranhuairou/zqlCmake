# zql Log Interface and implementation
#
cmake_minimum_required (VERSION 3.8)

find_package(GTest CONFIG REQUIRED)

project(Log)

# 添加项目文件
add_executable(LogTest "log.h" "log.cpp" "test.cpp")

# 添加项目文件
add_executable(Log "log.h" "log.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
