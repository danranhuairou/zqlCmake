# vscode 的使用
## 格式化设置
* 参考资料 https://blog.csdn.net/core571/article/details/82867932
* 打开 setting json，搜索 C_Cpp.clang_format_fallbackStyle
* 我的设置
"{ BasedOnStyle: LLVM, IndentWidth: 4, AccessModifierOffset: -4, PointerAlignment: Left }"

* qt 使用llvm 

BasedOnStyle: LLVM
IndentWidth: 4
AccessModifierOffset: -4
PointerBindsToType: true