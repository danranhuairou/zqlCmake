# 如何在cmake中使用vcpkg

* 参考资料：https://github.com/microsoft/vcpkg
* 使用前请把 CMakeLists.txt 中 VCPKG_PATH_ 变量改为 vcpkg 的安装目录
* 已经测试可以在不用其他配置的情况下，可以在已安装 win10 msvc2019 cmake vcpkg 的环境下运行，vcpkg中需要有 sqlite3 glog 的64位包，使用 vscode 或 vs2019 右键打开文件夹 运行。
* 如果遇到cmake无法构建的情况，请尝试检查 CMakeLists.txt 的变量名命名情况，根据 cmake 发出的警告不是能很好的发现错误所在
* 在更改 VCPKG_PATH_ 后请尝试重新构建或重启 vscode, 不然即使您 CMakeLists.txt 文件编写正确，可能也无法运行
* 如果使用 add_subdirectory 将此项目作为其他项目的子项目，请在顶级 CMakeLists.txt 中添加 VCPKG_PATH_ 及 CMAKE_TOOLCHAIN_FILE 否则会报错无法找到包
* 注意，请使用类似此项目的结构，使用两个 CMakeLists.txt 文件，不然可能由于未知原因无法找到包