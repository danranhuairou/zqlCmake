# zqlCmake.txt: TimeRecorderConsole 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 将源代码添加到此项目的可执行文件。
project (TimeRecorderConsoleOld)
add_executable (TimeRecorder "Monitor.cpp" "sql/sqlite3.c" "main.cpp" "SQLOperator.cpp" "test.cpp" "sql/tinyLog.cpp" "sql/tinyLog.h")

# TODO: 如有需要，请添加测试并安装目标。
