cmake_minimum_required(VERSION 3.28)
project(hello LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# /W4 = MSVC 最高实用警告级别；/utf-8 = 源码和输出都用 UTF-8
# -Wall -Wextra -Wpedantic = GCC/Clang 的全面警告
# pybind11_add_module 不会自动设这些（它只管头文件路径、.pyd 后缀等）
if (MSVC)
    add_compile_options(/W4 /utf-8)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

# find_package：让 CMake 去电脑上找 pybind11，找到了才能继续；REQUIRED：没找到就报错退出
# 找到后创建 pybind11::headers 目标，内部已配好 include 路径：
#   D:/ProgramData/anaconda3/envs/quant311/Lib/site-packages/pybind11/include/

# CMake 模块（pybind11_add_module 定义的地方）在：
#   D:/ProgramData/anaconda3/envs/quant311/Lib/site-packages/pybind11/share/cmake/pybind11/pybind11Config.cmake
#   （实际实现在旁边的 pybind11Tools.cmake）
# 注意：CMake 的 find_package 找的是 .cmake 模块文件，不是 C++ 的 .h 头文件——两套机制

find_package(pybind11 REQUIRED)

# pybind11_add_module：pybind11 提供的 cmake 函数，本质是 add_library(MODULE) 的封装。
# 第一个参数 = Python import 时的模块名，后面 = 源文件列表。

# MODULE 参数被内部写死，不需要（也不能）再手写——它是三种库类型之一：
#   STATIC → .a/.lib 静态库，链接时拷进 exe
#   SHARED → .dll/.so 动态库，运行时加载
#   MODULE → .pyd/.so Python 扩展，运行时被 Python 加载（pybind11 要的就是这种）

# 它一行搞定下面手写 5 行的事（自动设好头文件路径、链接、.pyd 后缀等）：
#
#   add_library(hello MODULE hello.cpp)                      # ① 声明动态库
#   target_include_directories(hello PRIVATE ${PYBIND11_INCLUDE_DIR})  # ② 头文件路径
#   target_link_libraries(hello PRIVATE pybind11::headers)   # ③ 链接 pybind11
#   set_target_properties(hello PROPERTIES
#       PREFIX ""                                            # ④ 去掉 lib 前缀（Python 不认识 libhello.pyd）
#       SUFFIX ".cp311-win_amd64.pyd"                       # ⑤ 设正确的 .pyd 后缀
#   )

# ① 声明库：把 hello.cpp 编译成 MODULE 动态库（默认产物 hello.dll / libhello.so）

# ② 头文件路径：告诉编译器去哪找 <pybind11/pybind11.h>，PRIVATE = 只有 hello 自己用
#   源码里写 #include <xxx> 时，编译器必须知道 xxx 在哪，否则报 No such file or directory
#   等价于编译命令加 -ID:/.../site-packages/pybind11/include/
#   其他例子：target_include_directories(app PRIVATE ./include)            # 自己的头文件
#            target_include_directories(app PRIVATE C:/libs/json/include)  # nlohmann/json

# ③ 链接 pybind11：pybind11 是 header-only（只有 .h，没有 .lib），传的主要是编译选项
#   （/utf-8、警告级别、PYBIND11_* 宏定义都打包在 pybind11::headers 目标里）
#   普通库例子：target_link_libraries(app PRIVATE curl)     → 链接期把 libcurl.lib 接进 exe
#              target_link_libraries(app PRIVATE opencv_core)

#   ② vs ③：
#     ② 解决"找到声明"（预处理/编译阶段，影响能否生成 .o）
#     ③ 解决"找到实现"（链接阶段，拼 .exe）
#   缺 ② → 报错 No such file；缺 ③ → 报错 undefined reference / LNK2019
#   精确到 C++ 编译流程的 4 步：
#     预处理（展开 #include，-I 在这找头文件）→ 编译（读头文件内容）→ 汇编（生成 .o）→ 链接（拼 .exe，③ 的库在这里用）

#   注意这里的"编译/链接"都是 C++ 编译流程里的子阶段，不是 CMake 的配置期/构建期：
#     CMake 分两期：配置期（cmake -B 生成构建文件）、构建期（cmake --build 执行上面的 4 步）
#     target_link_libraries 只是声明"这个目标依赖谁"，CMake 在构建时
#     把依赖的库/参数拼进编译命令，真正做链接动作的仍是编译流程的第四步

# ④ PREFIX "": CMake 默认加 lib 前缀（libhello.pyd），Python import 要求文件名就是 hello.pyd
# ⑤ SUFFIX ".cp311-win_amd64.pyd": Python 要求扩展名带 CPython 版本和平台标记，
#   后缀不对 import hello 直接失败（cp311 = Python 3.11，win_amd64 = Windows 64 位）

# 这 5 步每一步都不能少：
#   ①-③ 缺了 → 编译期失败：没目标 / 找不到 pybind11.h / 缺编译参数
#   ④-⑤ 缺了 → 编译能过，但运行期 import hello 直接失败（名字/后缀不对）
# 即：①-③ 保证"能编译出来"，④-⑤ 保证"能被 Python 认出来"。

# ② 里的 PYBIND11_INCLUDE_DIR 是 CMake 变量（不是 C++ 常量），${} 是取值语法。
# 赋值发生在 find_package 内部——pybind11Tools.cmake 里执行 set(...)，
# 所以自己的 CMakeLists.txt 里看不到赋值语句。

# 完整链路：
#   find_package(pybind11)  →  创建 pybind11::headers（含 include 路径）
#                                       ↓
#   pybind11_add_module()   →  内部自动链接 pybind11::headers
#                                       ↓
#   编译器加 -I.../pybind11/include/  →  #include <pybind11/pybind11.h> 就能找到了
pybind11_add_module(hello hello.cpp)
