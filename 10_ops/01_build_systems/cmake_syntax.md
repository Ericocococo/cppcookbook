# CMakeLists.txt 语法详解

> 所有示例均来自本目录各子工程，从简到繁递进。

---

## 1. 逐例解析

### 1.1 最小工程（01_hello）

```cmake
cmake_minimum_required(VERSION 3.28)
project(hello_cmake LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(hello_cmake main.cpp)
```

| 行 | 说明 |
|----|------|
| `cmake_minimum_required(VERSION 3.28)` | 声明最低 CMake 版本；低于此版本报错停止 |
| `project(hello_cmake LANGUAGES CXX)` | 定义工程名；`LANGUAGES CXX` 只启用 C++ 工具链，省略时默认启用 `C CXX` 两套，纯 C++ 项目写明可避免多余的 C 编译器探测 |
| `set(CMAKE_CXX_STANDARD 20)` | 指定 C++ 标准，等价编译器的 `-std=c++20` |
| `set(CMAKE_CXX_STANDARD_REQUIRED ON)` | 若编译器不支持该标准则报错，而不是静默降级 |
| `set(CMAKE_CXX_EXTENSIONS OFF)` | 禁用编译器扩展（`-std=c++20` 而非 `-std=gnu++20`） |
| `add_executable(hello_cmake main.cpp)` | 创建可执行目标，名为 `hello_cmake`，源文件 `main.cpp` |

**`LANGUAGES` 与 `-DCMAKE_CXX_COMPILER` 是两件独立的事**：

- `LANGUAGES CXX` — 告诉 CMake **启用哪种语言**
- `-DCMAKE_CXX_COMPILER` — 告诉 CMake **用哪个具体的编译器程序**

`LANGUAGES CXX` 只是说"我要用 C++"，CMake 仍会去 PATH 里自动查找 `g++`。编译器不在 PATH 时（本项目场景），需显式指定：

```bash
cmake -B build \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_MAKE_PROGRAM="$NINJA" \
  -G Ninja
```

> `-DCMAKE_C_COMPILER` 是 C 编译器路径变量；已声明 `LANGUAGES CXX` 不启用 C 时，此参数多余，不需要加。

可选的 `LANGUAGES` 值：

| 值 | 说明 |
|----|------|
| `C` | C 编译器 |
| `CXX` | C++ 编译器 |
| `CUDA` | CUDA 编译器（nvcc） |
| `Fortran` | Fortran 编译器 |
| `ASM` | 汇编器 |
| `NONE` | 不启用任何语言（纯脚本工程） |

---

### 1.2 多源文件与头文件搜索路径（02_sources）

```cmake
add_executable(multi_sources
    main.cpp
    math_utils.cpp
)

target_include_directories(multi_sources PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
```

| 行 | 说明 |
|----|------|
| 多行 `add_executable` | 源文件列表换行写，不用通配符——CMake 无法感知新增文件 |
| `target_include_directories` | 为目标添加头文件搜索路径 |
| `PRIVATE` | 传播范围：仅本目标使用，不传给链接它的其他目标 |
| `${CMAKE_CURRENT_SOURCE_DIR}` | 当前 CMakeLists.txt 所在目录，内置变量 |

---

### 1.3 静态库与动态库（03_library）

```cmake
add_library(math_static STATIC math_utils.cpp)
target_include_directories(math_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(math_shared SHARED math_utils.cpp)
set_target_properties(math_shared PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_executable(use_static main_static.cpp)
target_link_libraries(use_static PRIVATE math_static)
```

| 行 | 说明 |
|----|------|
| `add_library(... STATIC ...)` | 静态库（`.a` / `.lib`），编译进可执行文件，无运行时依赖 |
| `add_library(... SHARED ...)` | 动态库（`.so` / `.dll`），运行时加载 |
| `PUBLIC` | 本目标和所有链接它的目标都继承此属性 |
| `WINDOWS_EXPORT_ALL_SYMBOLS ON` | Windows 下自动导出所有符号，免写 `__declspec(dllexport)` |
| `target_link_libraries(... PRIVATE ...)` | 链接库；`PRIVATE` = 链接关系不传播 |

---

### 1.4 现代 target 属性传播（04_targets）

```cmake
# 接口库（header-only）：不编译，只传播属性
add_library(utils_iface INTERFACE)
target_include_directories(utils_iface INTERFACE include/)
target_compile_definitions(utils_iface INTERFACE UTILS_VERSION=2)

# 静态库，PUBLIC 传播 include 路径
add_library(core STATIC core.cpp)
target_include_directories(core PUBLIC include/)
target_compile_options(core PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /utf-8>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
)

# 可执行文件链接 core，include 路径自动继承
add_executable(app main.cpp)
target_link_libraries(app PRIVATE core utils_iface)
```

**传播范围三关键字**：

| 关键字 | 本目标使用 | 消费者继承 | 适用场景 |
|--------|-----------|-----------|---------|
| `PRIVATE` | ✓ | ✗ | 实现细节，不暴露给外部 |
| `PUBLIC` | ✓ | ✓ | 接口与实现都需要（如 `include/` 路径） |
| `INTERFACE` | ✗ | ✓ | 纯头文件库，只传属性不参与编译 |

实际执行（`app` 链接 `core` 时）：
```bash
# core 编译时
g++ -Wall -Wextra -Iinclude/ -c core.cpp -o core.o

# app 编译时（自动继承 core 的 PUBLIC include 路径）
g++ -Iinclude/ -c main.cpp -o main.o

# app 链接时
g++ main.o core.a -o app
```

---

### 1.5 编译选项与生成器表达式（05_compile_options）

```cmake
target_compile_options(app PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /utf-8 /permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

target_compile_options(app PRIVATE
    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<CONFIG:Debug>>:-O0 -g3>
    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<CONFIG:Release>>:-O3 -DNDEBUG>
)

target_compile_definitions(app PRIVATE
    APP_VERSION="1.0.0"
    $<$<CONFIG:Debug>:DEBUG_BUILD>
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```

生成器表达式在 **构建时** 展开（configure 阶段不展开），格式 `$<条件:值>`：

| 表达式 | 含义 |
|--------|------|
| `$<CXX_COMPILER_ID:MSVC>` | 编译器是 MSVC 时为真 |
| `$<NOT:$<...>>` | 取反 |
| `$<AND:$<...>,$<...>>` | 与 |
| `$<CONFIG:Debug>` | 当前构建类型是 Debug 时为真 |
| `$<BUILD_INTERFACE:path>` | 构建时使用此路径（安装后不用） |
| `$<INSTALL_INTERFACE:path>` | 安装后使用此路径（构建时不用） |

---

### 1.6 查找依赖（06_find_package）

```cmake
find_package(Threads REQUIRED)           # 内置模块（FindThreads.cmake）
find_package(nlohmann_json CONFIG REQUIRED)  # Config 模式（包自带配置文件）
find_package(ZLIB)                       # 可选依赖

target_link_libraries(app PRIVATE Threads::Threads)

if(ZLIB_FOUND)
    target_link_libraries(app PRIVATE ZLIB::ZLIB)
    target_compile_definitions(app PRIVATE HAS_ZLIB)
    message(STATUS "ZLIB 已找到，版本：${ZLIB_VERSION_STRING}")
else()
    message(STATUS "ZLIB 未找到，相关功能禁用")
endif()
```

| 参数 | 说明 |
|------|------|
| `REQUIRED` | 找不到则报错停止 |
| `CONFIG` | 强制 Config 模式，查找包自带的 `XxxConfig.cmake` |
| 省略 `CONFIG` | 先 Module 模式（CMake 内置 `FindXxx.cmake`），再 Config 模式 |
| `Threads::Threads` | 导入目标（imported target），自动携带正确的编译/链接标志 |

---

### 1.7 vcpkg 集成（07_vcpkg）

```cmake
find_package(nlohmann_json CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

target_link_libraries(app PRIVATE
    nlohmann_json::nlohmann_json
    fmt::fmt
)
```

vcpkg 通过 `CMAKE_TOOLCHAIN_FILE` 注入，三种方式：

```bash
# 方式一：命令行
cmake -B build -DCMAKE_TOOLCHAIN_FILE=D:/software/vcpkg/scripts/buildsystems/vcpkg.cmake

# 方式二：CMakePresets.json（推荐）
# 方式三：CLion 工具链设置
```

---

### 1.8 安装规则（08_install）

```cmake
include(GNUInstallDirs)   # 提供标准目录变量

add_library(mylib STATIC mylib.cpp)
target_include_directories(mylib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS mylib myapp
    EXPORT mylibTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT mylibTargets
    FILE mylibTargets.cmake
    NAMESPACE mylib::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mylib
)
```

| 变量 | 默认值 |
|------|--------|
| `CMAKE_INSTALL_BINDIR` | `bin` |
| `CMAKE_INSTALL_LIBDIR` | `lib` |
| `CMAKE_INSTALL_INCLUDEDIR` | `include` |

---

### 1.9 测试（09_ctest）

```cmake
enable_testing()

add_executable(test_basic test_basic.cpp)
target_link_libraries(test_basic PRIVATE calc)
add_test(NAME basic COMMAND test_basic)

set_tests_properties(edge_cases PROPERTIES
    TIMEOUT 5
    LABELS "edge"
)
```

运行：
```bash
cmake --build build
ctest --build-dir build --output-on-failure
ctest --build-dir build -L edge   # 只运行 edge 标签的测试
```

---

## 2. 语法完整参考

### 2.1 变量

```cmake
set(VAR value)                 # 定义普通变量
set(VAR value CACHE STRING "说明")  # 缓存变量（可在 cmake-gui / -D 覆盖）
set(ENV{PATH} "/usr/bin")      # 设置环境变量

unset(VAR)                     # 删除变量

message(STATUS "${VAR}")       # 输出变量值（STATUS = 前缀 "--"）
message(WARNING "...")
message(FATAL_ERROR "...")      # 报错并停止
```

**常用内置变量**：

| 变量 | 含义 |
|------|------|
| `CMAKE_SOURCE_DIR` | 顶层 CMakeLists.txt 所在目录 |
| `CMAKE_CURRENT_SOURCE_DIR` | 当前 CMakeLists.txt 所在目录 |
| `CMAKE_BINARY_DIR` | 顶层构建目录 |
| `CMAKE_CURRENT_BINARY_DIR` | 当前目标的构建目录 |
| `PROJECT_NAME` | `project()` 设置的工程名 |
| `PROJECT_VERSION` | `project(... VERSION x.y.z)` 设置的版本 |
| `CMAKE_BUILD_TYPE` | `Debug` / `Release` / `RelWithDebInfo` / `MinSizeRel` |
| `CMAKE_CXX_COMPILER_ID` | `MSVC` / `GNU` / `Clang` / `AppleClang` |

### 2.2 目标命令

```cmake
add_executable(name src1.cpp src2.cpp)
add_library(name STATIC|SHARED|INTERFACE src.cpp)

target_include_directories(name PRIVATE|PUBLIC|INTERFACE dir/)
target_link_libraries(name PRIVATE|PUBLIC|INTERFACE lib)
target_compile_options(name PRIVATE|PUBLIC|INTERFACE -Wall)
target_compile_definitions(name PRIVATE|PUBLIC|INTERFACE FOO=1)
target_sources(name PRIVATE extra.cpp)         # 追加源文件

set_target_properties(name PROPERTIES
    OUTPUT_NAME "myapp"                        # 输出文件名
    CXX_STANDARD 20
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)
get_target_property(val name OUTPUT_NAME)      # 读取属性
```

### 2.3 条件与循环

```cmake
if(VAR)             # VAR 非空且不为 0/FALSE/OFF/NO/IGNORE
elseif(A AND B)
elseif(A OR B)
elseif(NOT A)
elseif(A STREQUAL "str")
elseif(A VERSION_GREATER "1.0")
elseif(TARGET mylib)     # 目标是否存在
elseif(EXISTS path)      # 文件/目录是否存在
endif()

foreach(item IN LISTS mylist)
    message(STATUS "${item}")
endforeach()

foreach(i RANGE 0 9)     # 0 1 2 ... 9
endforeach()

while(condition)
endwhile()
```

### 2.4 函数与宏

```cmake
function(my_func arg1 arg2)
    message(STATUS "arg1=${arg1}")
    # 函数有独立作用域，set() 不影响外部
endfunction()

macro(my_macro arg1)
    # 宏在调用处展开，无独立作用域
endmacro()

my_func("hello" "world")
```

### 2.5 文件操作

```cmake
file(GLOB srcs "*.cpp")                       # 展开通配符（不推荐用于 SRCS）
file(GLOB_RECURSE srcs "src/*.cpp")           # 递归展开
file(READ path content)                       # 读文件内容到变量
file(WRITE path "content")                    # 写文件
file(COPY src DESTINATION dst)                # 复制文件/目录
configure_file(input.h.in output.h)           # 替换 @VAR@ 生成头文件
```

### 2.6 生成器表达式速查

| 表达式 | 含义 |
|--------|------|
| `$<CXX_COMPILER_ID:MSVC>` | 编译器是 MSVC |
| `$<CONFIG:Release>` | 构建类型是 Release |
| `$<NOT:$<...>>` | 取反 |
| `$<AND:$<...>,$<...>>` | 与 |
| `$<OR:$<...>,$<...>>` | 或 |
| `$<BUILD_INTERFACE:path>` | 仅构建时生效 |
| `$<INSTALL_INTERFACE:path>` | 仅安装后生效 |
| `$<TARGET_FILE:name>` | 目标输出文件的完整路径 |

### 2.7 常用构建命令详解

#### 2.7.1 配置阶段（`cmake -B`）

```bash
cmake -B <构建目录> [选项...]
```

| 参数 | 说明 |
|------|------|
| `-B build` | 指定构建目录（build 文件夹），不存在时自动创建 |
| `-S .` | 指定源目录（含 CMakeLists.txt），默认为当前目录，通常可省略 |
| `-G Ninja` | 指定构建系统生成器；常用值：`Ninja`（快）、`"MinGW Makefiles"`、`"Visual Studio 18 2026"` |
| `-A x64` | 目标平台，**VS Generator 专用**；不指定默认 Win32（32 位）；可选值：`x64`（64 位）、`Win32`（32 位）、`ARM`、`ARM64`、`ARM64EC` |
| `-DCMAKE_BUILD_TYPE=Debug` | 构建类型（单配置 Generator 用）：`Debug` / `Release` / `RelWithDebInfo` / `MinSizeRel`；VS Generator 不用此参数，改用 `--config` |
| `-DCMAKE_CXX_COMPILER="路径"` | 指定 C++ 编译器可执行文件路径；编译器不在 PATH 时必填 |
| `-DCMAKE_MAKE_PROGRAM="路径"` | 指定构建工具（ninja.exe / mingw32-make.exe）路径；构建工具不在 PATH 时必填 |
| `-DCMAKE_TOOLCHAIN_FILE=路径` | 工具链文件（vcpkg 集成时用） |
| `-DCMAKE_INSTALL_PREFIX=路径` | `cmake --install` 的安装根目录，默认 `/usr/local`（Linux）或 `C:/Program Files`（Windows） |
| `-DCMAKE_CXX_STANDARD=17` | 强制 C++ 标准（17 / 20 / 23），等价于 CMakeLists.txt 里的 `set(CMAKE_CXX_STANDARD 17)` |
| `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` | 导出 `compile_commands.json`，供 clangd / IDE 静态分析用；**Ninja 支持，VS Generator 不支持** |
| `-DCMAKE_VERBOSE_MAKEFILE=ON` | 构建时打印完整编译命令，排查编译参数问题用 |
| `-DFOO=ON` | 向 CMakeLists.txt 传递缓存变量，等价于 `set(FOO ON CACHE BOOL "")` |
| `-Wno-dev` | 抑制 CMakeLists.txt 的开发者警告 |
| `--fresh` | 强制删除旧缓存重新配置（CMake 3.24+） |

```bash
# 完整示例
cmake -B build -G Ninja \
  -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=./install-root
```

#### 2.7.2 构建阶段（`cmake --build`）

```bash
cmake --build <构建目录> [选项...]
```

| 参数 | 说明 |
|------|------|
| `--build build` | 指定构建目录（与配置时的 `-B` 一致） |
| `--target <名称>` | 只构建指定目标；`--target clean` 清理构建产物 |
| `--config Release` | 多配置生成器（Visual Studio）下指定构建类型 |
| `-j 8` / `--parallel 8` | 并行编译线程数；省略时使用生成器默认值 |
| `--verbose` / `-v` | 打印实际执行的编译命令，排查编译选项问题时用 |

```bash
cmake --build build -j 8 --verbose    # 8 线程构建并打印完整命令
cmake --build build --target clean    # 只清理
```

#### 2.7.3 安装阶段（`cmake --install`）

```bash
cmake --install <构建目录> [选项...]
```

| 参数 | 说明 |
|------|------|
| `--install build` | 指定构建目录 |
| `--prefix 路径` | 覆盖安装根目录（优先级高于配置时的 `CMAKE_INSTALL_PREFIX`） |
| `--component <名称>` | 只安装指定组件（需 CMakeLists.txt 配合 `COMPONENT` 参数） |
| `--config Release` | 多配置生成器下指定安装哪种构建类型 |

```bash
cmake --install build --prefix ./dist   # 安装到 ./dist
```

#### 2.7.4 测试（`ctest`）

```bash
ctest --test-dir <构建目录> [选项...]
```

| 参数 | 说明 |
|------|------|
| `--test-dir build` | 指定构建目录（含 CTestTestfile.cmake） |
| `--output-on-failure` | 只在测试失败时打印输出，通过的不显示 |
| `-L <标签>` | 只运行带指定标签的测试（标签在 `set_tests_properties` 里设置） |
| `-R <正则>` | 按测试名称正则过滤 |
| `-j 4` | 并行运行测试 |
| `--rerun-failed` | 只重跑上次失败的测试 |

```bash
ctest --test-dir build --output-on-failure -j 4   # 4 并行，失败时才输出
ctest --test-dir build -L edge --rerun-failed      # 只跑 edge 标签的失败用例
```

---

## 3. build 目录文件说明

### 3.1 所有 Generator 通用

| 文件 / 目录 | 说明 |
|---|---|
| `CMakeFiles/` | CMake 内部工作目录，存放配置日志、中间状态、目标中间文件（`.o` / `.obj`） |
| `CMakeFiles/CMakeConfigureLog.yaml` | 配置阶段详细日志，记录编译器探测结果、feature 检测等，排查配置失败时看这里 |
| `CMakeFiles/<version>/` | CMake 版本号目录（如 `4.2.2/`），存放编译器探测脚本和结果 |
| `CMakeFiles/CMakeScratch/` | 配置阶段临时文件，探测完即废弃 |
| `CMakeFiles/pkgRedirects/` | 包重定向配置（find_package 相关） |
| `CMakeFiles/<target>.dir/` | 各构建目标的中间文件目录（`.o`、依赖文件等），目标名即 `add_executable` / `add_library` 的名称 |
| `CMakeCache.txt` | 所有 cmake 变量的缓存，修改后须重跑 cmake 才生效；删除此文件等同于清空配置缓存 |
| `cmake_install.cmake` | `cmake --install` 的安装规则脚本，由 CMakeLists.txt 里的 `install()` 生成 |
| `InstallScripts.json` | 安装脚本索引 |
| `TargetDirectories.txt` | 记录所有构建目标的输出目录路径 |

### 3.2 Ninja 方案特有

| 文件 | 说明 |
|---|---|
| `build.ninja` | **主构建文件**，ninja 读取它执行所有构建规则 |
| `CMakeFiles/rules.ninja` | 通用编译规则（响应规则、默认标志等），被 `build.ninja` 包含 |
| `.ninja_deps` | ninja 依赖数据库，记录每个源文件依赖的头文件，增量构建时判断哪些文件需要重编 |
| `.ninja_log` | 构建日志，记录每条命令的开始/结束时间，`ninja -t restat` 用它 |

### 3.3 MinGW Makefiles 方案特有

| 文件 | 说明 |
|---|---|
| `Makefile` | **主构建文件**，mingw32-make 读取它执行构建 |
| `CMakeFiles/Makefile.cmake` | CMake 生成的 Makefile 片段，被主 Makefile 包含 |
| `CMakeFiles/Makefile2` | 内部 Makefile，处理目标依赖顺序 |
| `CMakeFiles/progress.marks` | 记录构建进度（用于显示 `[xx%]`） |
| `CMakeFiles/CMakeDirectoryInformation.cmake` | 当前目录的路径信息 |

### 3.4 Ninja Multi-Config 方案特有

| 文件 / 目录 | 说明 |
|---|---|
| `Debug/` / `Release/` / `RelWithDebInfo/` | 各配置的独立输出目录，exe 在对应子目录下 |
| `build-Debug.ninja` / `build-Release.ninja` / `build-RelWithDebInfo.ninja` | 各配置的入口 ninja 文件，`--config Debug` 时读取对应文件 |
| `CMakeFiles/common.ninja` | 所有配置共用的编译规则（目标定义、路径等） |
| `CMakeFiles/impl-Debug.ninja` 等 | 各配置的具体实现（编译标志差异、输出路径等） |

### 3.5 MSVC 特有（相比 MinGW）

| 文件 / 目录 | 说明 |
|---|---|
| `<target>.pdb` | **调试符号文件**（Program Debug Database），VS 调试器 / WinDbg 读取它，大小通常远超 exe |
| `<target>.ilk` | **增量链接文件**（Incremental Link），存储上次链接的中间数据，加速下次链接；Debug 构建才生成 |
| `CMakeFiles/ShowIncludes/` | 解析 MSVC `/showIncludes` 输出，实现头文件依赖跟踪（GCC 用 `-MF` 生成 `.d` 文件，MSVC 改用此目录） |
| `CMakeFiles/<version>-msvc1/` | MSVC 版本标识目录（如 `4.3.1-msvc1/`） |

### 3.6 VS Generator 特有

| 文件 / 目录 | 说明 |
|---|---|
| `<project>.slnx` | **VS 解决方案文件**（新格式，VS 2022 17.x+），双击用 VS 打开，包含所有项目 |
| `<target>.vcxproj` | 主项目文件，MSBuild 读取它编译对应目标 |
| `ALL_BUILD.vcxproj` | 构建所有目标，VS 里 Build Solution 触发的就是它 |
| `ZERO_CHECK.vcxproj` | 监控 CMakeLists.txt 变化，变化时自动重新运行 cmake 刷新 `.vcxproj` |
| `*.vcxproj.filters` | 项目筛选器，控制文件在 VS Solution Explorer 里的分组显示 |
| `*.vcxproj.user` | 用户本地配置（调试启动参数、工作目录等），不应提交 git |
| `.vs/` | VS IDE 本地配置目录，不应提交 git |
| `Debug/` / `Release/` | 各配置的输出目录，exe 在对应子目录下 |
| `x64/` | x64 平台的中间文件目录 |
| `CMakeFiles/generate.stamp` | ZERO_CHECK 用来判断是否需要重新生成 `.vcxproj` 的时间戳 |
| `CMakeFiles/generate.stamp.depend` | 触发重新生成的文件列表（CMakeLists.txt 及所有 `.cmake` 文件） |
