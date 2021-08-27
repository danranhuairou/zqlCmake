# 父目录下的CMakeLists.txt
cmake_minimum_required(VERSION 3.20.2)

project(example)

# 导入包
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(glog CONFIG REQUIRED) 

# 添加源文件
add_executable(example_glog "example_glog.cpp")
add_executable(example_sqlite "example_sqlite.c")

# 链接包
target_link_libraries(example_glog PRIVATE glog::glog)
target_link_libraries(example_sqlite PRIVATE unofficial::sqlite3::sqlite3)

