# CMakeLists.txt 语法详解

> 所有示例均来自本目录各子工程，从简到繁递进。

---

## 0. CMake 命令行速查

### 0.1 配置阶段（`cmake -B`）

```bash
cmake -B <构建目录> [选项...]
```

| 参数                                   | 说明                                                                                                           |
|--------------------------------------|--------------------------------------------------------------------------------------------------------------|
| `-B build`                           | 指定构建目录（build 文件夹），不存在时自动创建                                                                                   |
| `-S .`                               | 指定源目录（含 CMakeLists.txt），默认为当前目录，通常可省略                                                                        |
| `-G Ninja`                           | 指定构建系统生成器；常用值：`Ninja`（快）、`"MinGW Makefiles"`、`"Visual Studio 18 2026"`                                       |
| `-A x64`                             | 目标平台，**VS Generator 专用**；不指定默认 Win32（32 位）；可选值：`x64`（64 位）、`Win32`（32 位）、`ARM`、`ARM64`、`ARM64EC`             |
| `-DCMAKE_BUILD_TYPE=Debug`           | 构建类型（单配置 Generator 用）：`Debug` / `Release` / `RelWithDebInfo` / `MinSizeRel`；VS Generator 不用此参数，改用 `--config` |
| `-DCMAKE_CXX_COMPILER="路径"`          | 指定 C++ 编译器可执行文件路径；编译器不在 PATH 时必填                                                                             |
| `-DCMAKE_MAKE_PROGRAM="路径"`          | 指定构建工具（ninja.exe / mingw32-make.exe）路径；构建工具不在 PATH 时必填                                                       |
| `-DCMAKE_TOOLCHAIN_FILE=路径`          | 工具链文件（vcpkg 集成时用）                                                                                            |
| `-DCMAKE_INSTALL_PREFIX=路径`          | `cmake --install` 的安装根目录，默认 `/usr/local`（Linux）或 `C:/Program Files`（Windows）                                 |
| `-DCMAKE_CXX_STANDARD=17`            | 强制 C++ 标准（17 / 20 / 23），等价于 CMakeLists.txt 里的 `set(CMAKE_CXX_STANDARD 17)`                                   |
| `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` | 导出 `compile_commands.json`，供 clangd / IDE 静态分析用；**Ninja 支持，VS Generator 不支持**                                |
| `-DCMAKE_VERBOSE_MAKEFILE=ON`        | 构建时打印完整编译命令，排查编译参数问题用                                                                                        |
| `-DFOO=ON`                           | 向 CMakeLists.txt 传递缓存变量，等价于 `set(FOO ON CACHE BOOL "")`                                                      |
| `-Wno-dev`                           | 抑制 CMakeLists.txt 的开发者警告                                                                                     |
| `--fresh`                            | 强制删除旧缓存重新配置（CMake 3.24+）                                                                                     |

```bash
# 完整示例
cmake -B build -G Ninja \
  -DCMAKE_CXX_COMPILER="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe" \
  -DCMAKE_MAKE_PROGRAM="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=./install-root
```

### 0.2 构建阶段（`cmake --build`）

```bash
cmake --build <构建目录> [选项...]
```

| 参数                      | 说明                              |
|-------------------------|---------------------------------|
| `--build build`         | 指定构建目录（与配置时的 `-B` 一致）           |
| `--target <名称>`         | 只构建指定目标；`--target clean` 清理构建产物 |
| `--config Release`      | 多配置生成器（Visual Studio）下指定构建类型    |
| `-j 8` / `--parallel 8` | 并行编译线程数；省略时使用生成器默认值             |
| `--verbose` / `-v`      | 打印实际执行的编译命令，排查编译选项问题时用          |

```bash
cmake --build build -j 8 --verbose    # 8 线程构建并打印完整命令
cmake --build build --target clean    # 只清理
```

### 0.3 安装阶段（`cmake --install`）

```bash
cmake --install <构建目录> [选项...]
```

| 参数                 | 说明                                          |
|--------------------|---------------------------------------------|
| `--install build`  | 指定构建目录                                      |
| `--prefix 路径`      | 覆盖安装根目录（优先级高于配置时的 `CMAKE_INSTALL_PREFIX`）   |
| `--component <名称>` | 只安装指定组件（需 CMakeLists.txt 配合 `COMPONENT` 参数） |
| `--config Release` | 多配置生成器下指定安装哪种构建类型                           |

```bash
cmake --install build --prefix ./dist   # 安装到 ./dist
```

### 0.4 测试（`ctest`）

```bash
ctest --test-dir <构建目录> [选项...]
```

| 参数                    | 说明                                          |
|-----------------------|---------------------------------------------|
| `--test-dir build`    | 指定构建目录（含 CTestTestfile.cmake）               |
| `--output-on-failure` | 只在测试失败时打印输出，通过的不显示                          |
| `-L <标签>`             | 只运行带指定标签的测试（标签在 `set_tests_properties` 里设置） |
| `-R <正则>`             | 按测试名称正则过滤                                   |
| `-j 4`                | 并行运行测试                                      |
| `--rerun-failed`      | 只重跑上次失败的测试                                  |

```bash
ctest --test-dir build --output-on-failure -j 4   # 4 并行，失败时才输出
ctest --test-dir build -L edge --rerun-failed      # 只跑 edge 标签的失败用例
```

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

| 行                                      | 说明                                                                            |
|----------------------------------------|-------------------------------------------------------------------------------|
| `cmake_minimum_required(VERSION 3.28)` | 声明最低 CMake 版本；低于此版本报错停止                                                       |
| `project(hello_cmake LANGUAGES CXX)`   | 定义工程名；`LANGUAGES CXX` 只启用 C++ 工具链，省略时默认启用 `C CXX` 两套，纯 C++ 项目写明可避免多余的 C 编译器探测 |
| `set(CMAKE_CXX_STANDARD 20)`           | 指定 C++ 标准，等价编译器的 `-std=c++20`                                                 |
| `set(CMAKE_CXX_STANDARD_REQUIRED ON)`  | 若编译器不支持该标准则报错，而不是静默降级                                                         |
| `set(CMAKE_CXX_EXTENSIONS OFF)`        | 禁用编译器扩展（`-std=c++20` 而非 `-std=gnu++20`）                                       |
| `add_executable(hello_cmake main.cpp)` | 创建可执行目标，名为 `hello_cmake`，源文件 `main.cpp`                                       |

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

| 值         | 说明             |
|-----------|----------------|
| `C`       | C 编译器          |
| `CXX`     | C++ 编译器        |
| `CUDA`    | CUDA 编译器（nvcc） |
| `Fortran` | Fortran 编译器    |
| `ASM`     | 汇编器            |
| `NONE`    | 不启用任何语言（纯脚本工程） |

---

### 1.2 多源文件与头文件搜索路径（02_sources）

```cmake
add_executable(multi_sources
    main.cpp
    math_utils.cpp
)

target_include_directories(multi_sources PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
```

| 行                             | 说明                             |
|-------------------------------|--------------------------------|
| 多行 `add_executable`           | 源文件列表换行写，不用通配符——CMake 无法感知新增文件 |
| `target_include_directories`  | 为目标添加头文件搜索路径                   |
| `PRIVATE`                     | 传播范围：仅本目标使用，不传给链接它的其他目标        |
| `${CMAKE_CURRENT_SOURCE_DIR}` | 当前 CMakeLists.txt 所在目录，内置变量    |

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

**`add_library(math_static STATIC math_utils.cpp)`**

| 参数               | 说明                                                                |
|------------------|-------------------------------------------------------------------|
| `math_static`    | 库目标名，在整个 CMake 项目中唯一，后续 `target_link_libraries` 通过此名引用            |
| `STATIC`         | 构建静态库：Linux/macOS 生成 `.a`，Windows 生成 `.lib`；链接时整体嵌入可执行文件，运行时无额外依赖 |
| `math_utils.cpp` | 源文件列表，可写多个，空格分隔                                                   |

**`target_include_directories(math_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})`**

| 参数                            | 说明                                                                                  |
|-------------------------------|-------------------------------------------------------------------------------------|
| `math_static`                 | 要设置头文件搜索路径的目标                                                                       |
| `PUBLIC`                      | 传播范围：**本目标编译时**和**所有链接它的目标编译时**都生效；见下方传播范围说明                                        |
| `${CMAKE_CURRENT_SOURCE_DIR}` | 内置变量，当前 `CMakeLists.txt` 所在目录；这里用于让消费者不需要知道头文件的绝对路径，直接 `#include "math_utils.h"` 即可 |

**`add_library(math_shared SHARED math_utils.cpp)`**

| 参数               | 说明                                                                      |
|------------------|-------------------------------------------------------------------------|
| `math_shared`    | 动态库目标名                                                                  |
| `SHARED`         | 构建动态库：Linux/macOS 生成 `.so`，Windows 生成 `.dll` + `.lib`（导入库）；运行时按需加载，可热替换 |
| `math_utils.cpp` | 源文件，与静态库共用相同源文件，CMake 分别编译两份（编译标志可能不同）                                  |

**`set_target_properties(math_shared PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)`**

| 参数                              | 说明                                                                                                                     |
|---------------------------------|------------------------------------------------------------------------------------------------------------------------|
| `math_shared`                   | 要设置属性的目标                                                                                                               |
| `PROPERTIES`                    | 关键字，后跟 `属性名 值` 对，可连续写多组                                                                                                |
| `WINDOWS_EXPORT_ALL_SYMBOLS ON` | **仅 Windows 有效**：让 CMake 自动生成 `.def` 文件并导出所有公开符号，免去在每个函数前写 `__declspec(dllexport)`；Linux/macOS 的 `.so` 默认导出所有符号，不需要此属性 |

> **为什么 Windows 需要显式导出？** Windows 的 DLL 默认隐藏所有符号，只有加了 `__declspec(dllexport)` 的符号才对外可见；Linux/macOS
> 相反，默认全部可见（可用 `-fvisibility=hidden` 收紧）。`WINDOWS_EXPORT_ALL_SYMBOLS` 是"懒人模式"，适合快速移植；生产代码更推荐用
`__declspec(dllexport/dllimport)` 精确控制接口。

**`add_executable(use_static main_static.cpp)`**

同 [1.1 节](#11-最小工程01_hello)，创建可执行目标。

**`target_link_libraries(use_static PRIVATE math_static)`**

| 参数            | 说明                                               |
|---------------|--------------------------------------------------|
| `use_static`  | 要链接的目标（消费者）                                      |
| `PRIVATE`     | 传播范围：链接关系**不传播**给链接 `use_static` 的其他目标；见下方传播范围说明 |
| `math_static` | 被链接的目标（库）；CMake 自动处理链接顺序和传递依赖                    |

**传播范围（PRIVATE / PUBLIC / INTERFACE）速查**

| 关键字         | 本目标编译时生效 | 消费者编译时生效 | 典型用途                 |
|-------------|:--------:|:--------:|----------------------|
| `PRIVATE`   |    ✓     |    ✗     | 实现细节，不暴露给外部（如内部依赖的库） |
| `PUBLIC`    |    ✓     |    ✓     | 接口与实现都需要（如库的公开头文件路径） |
| `INTERFACE` |    ✗     |    ✓     | 纯头文件库，只传属性不参与编译      |

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

| 关键字         | 本目标使用 | 消费者继承 | 适用场景                      |
|-------------|-------|-------|---------------------------|
| `PRIVATE`   | ✓     | ✗     | 实现细节，不暴露给外部               |
| `PUBLIC`    | ✓     | ✓     | 接口与实现都需要（如 `include/` 路径） |
| `INTERFACE` | ✗     | ✓     | 纯头文件库，只传属性不参与编译           |

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

> 入门简单写法（`if(MSVC)` + `add_compile_options`）见 § 2.20；本节演示进阶的生成器表达式写法。

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

| 表达式                         | 含义                |
|-----------------------------|-------------------|
| `$<CXX_COMPILER_ID:MSVC>`   | 编译器是 MSVC 时为真     |
| `$<NOT:$<...>>`             | 取反                |
| `$<AND:$<...>,$<...>>`      | 与                 |
| `$<CONFIG:Debug>`           | 当前构建类型是 Debug 时为真 |
| `$<BUILD_INTERFACE:path>`   | 构建时使用此路径（安装后不用）   |
| `$<INSTALL_INTERFACE:path>` | 安装后使用此路径（构建时不用）   |

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

| 参数                 | 说明                                                |
|--------------------|---------------------------------------------------|
| `REQUIRED`         | 找不到则报错停止                                          |
| `CONFIG`           | 强制 Config 模式，查找包自带的 `XxxConfig.cmake`             |
| 省略 `CONFIG`        | 先 Module 模式（CMake 内置 `FindXxx.cmake`），再 Config 模式 |
| `Threads::Threads` | 导入目标（imported target），自动携带正确的编译/链接标志              |

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

| 变量                         | 默认值       |
|----------------------------|-----------|
| `CMAKE_INSTALL_BINDIR`     | `bin`     |
| `CMAKE_INSTALL_LIBDIR`     | `lib`     |
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

### 1.10 生成器表达式（10_generator_expr）

```cmake
target_compile_options(app PRIVATE
    # 编译器分支：MSVC 用 /W4，其他用 -Wall -Wextra
    $<IF:$<CXX_COMPILER_ID:MSVC>,/W4,-Wall -Wextra>
)

target_compile_definitions(app PRIVATE
    # 配置分支：Debug 时定义 DEBUG_BUILD
    $<$<CONFIG:Debug>:DEBUG_BUILD>
    # 多配置：Release 时定义 NDEBUG（等价于 -DNDEBUG）
    $<$<CONFIG:Release>:NDEBUG>
    # 自定义 option 控制的宏
    $<$<BOOL:${ENABLE_FEATURE}>:FEATURE_ENABLED>
)

target_include_directories(mylib PUBLIC
    # 构建时用源码目录，安装后用安装目录
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# 调试生成器表达式：求值后写入文件查看
file(GENERATE
    OUTPUT "${CMAKE_BINARY_DIR}/debug_flags_$<CONFIG>.txt"
    CONTENT "compile options: $<TARGET_PROPERTY:app,COMPILE_OPTIONS>\n"
)
```

| 模式       | 语法                                                | 说明                           |
|----------|---------------------------------------------------|------------------------------|
| 条件（布尔求值） | `$<condition>`                                    | condition 为真时展开为 `1`，否则为 `0` |
| 条件取值     | `$<condition:value>`                              | condition 为真时展开为 value，否则为空  |
| IF-ELSE  | `$<IF:cond,true_val,false_val>`                   | 三元运算符                        |
| 编译器判断    | `$<CXX_COMPILER_ID:MSVC>`                         | 编译器匹配时为 `1`                  |
| 构建类型     | `$<CONFIG:Release>`                               | 当前配置匹配时为 `1`                 |
| 目标属性     | `$<TARGET_PROPERTY:tgt,PROP>`                     | 读取目标属性值                      |
| 目标文件路径   | `$<TARGET_FILE:tgt>`                              | 目标输出文件完整路径                   |
| 构建/安装接口  | `$<BUILD_INTERFACE:v>` / `$<INSTALL_INTERFACE:v>` | 区分构建期和安装后的路径                 |

---

### 1.11 CMakePresets.json（11_presets）

```json
{
  "version": 6,
  "configurePresets": [
    {
      "name": "base",
      "hidden": true,
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build/${presetName}"
    },
    {
      "name": "debug",
      "displayName": "Debug",
      "inherits": "base",
      "cacheVariables": { "CMAKE_BUILD_TYPE": "Debug" }
    },
    {
      "name": "release",
      "displayName": "Release",
      "inherits": "base",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release",
        "CMAKE_INTERPROCEDURAL_OPTIMIZATION": "ON"
      }
    },
    {
      "name": "asan",
      "displayName": "Debug + ASan",
      "inherits": "base",
      "cacheVariables": { "CMAKE_BUILD_TYPE": "Debug", "ENABLE_ASAN": "ON" }
    }
  ],
  "buildPresets": [
    { "name": "debug",   "configurePreset": "debug"   },
    { "name": "release", "configurePreset": "release" },
    { "name": "asan",    "configurePreset": "asan"    }
  ]
}
```

使用：

```bash
cmake --list-presets                              # 列出所有预设
cmake --preset debug && cmake --build --preset debug    # 配置 + 构建
cmake --preset asan  && cmake --build --preset asan     # ASan 预设
```

| 字段               | 说明                                                                |
|------------------|-------------------------------------------------------------------|
| `version`        | Presets 格式版本，6 = CMake 3.25+；3 = CMake 3.21+（最低可用版本）              |
| `hidden: true`   | 该预设仅作为基类，不出现在 `--list-presets` 列表                                 |
| `inherits`       | 继承另一个预设的所有字段，可多重继承（数组形式）                                          |
| `binaryDir`      | 构建目录；`${sourceDir}` 是 CMakePresets.json 所在目录，`${presetName}` 是预设名 |
| `cacheVariables` | 等价于命令行 `-DVAR=value`，覆盖 CMakeLists.txt 里的默认值                      |
| `generator`      | 构建系统生成器，等价于 `-G`                                                  |
| `toolchainFile`  | 工具链文件路径，等价于 `-DCMAKE_TOOLCHAIN_FILE`                              |
| `environment`    | 设置环境变量（仅对该预设有效）                                                   |

---

### 1.12 企业级多模块（12_enterprise）

```cmake
# 顶层 CMakeLists.txt
cmake_minimum_required(VERSION 3.28)
project(enterprise LANGUAGES CXX)

# 接口库：集中声明所有子目标共享的编译选项
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)
target_compile_options(project_options INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /utf-8 /permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Werror>
)

# ASan 支持（通过 option 控制）
option(ENABLE_ASAN "开启 AddressSanitizer" OFF)
if(ENABLE_ASAN)
    target_compile_options(project_options INTERFACE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(project_options INTERFACE -fsanitize=address)
endif()

# 子目录（CMake 自动处理链接依赖顺序）
add_subdirectory(utils)   # 定义 utils_lib
add_subdirectory(core)    # 定义 core_lib，依赖 utils_lib
add_subdirectory(app)     # 定义 enterprise_app，依赖 core_lib
add_subdirectory(tests)   # 测试目标

# utils/CMakeLists.txt
add_library(utils_lib STATIC src/utils.cpp)
target_include_directories(utils_lib PUBLIC include/)
target_link_libraries(utils_lib PUBLIC project_options)

# core/CMakeLists.txt
add_library(core_lib STATIC src/core.cpp)
target_include_directories(core_lib PUBLIC include/)
target_link_libraries(core_lib
    PUBLIC  utils_lib        # PUBLIC：app 链接 core_lib 后自动获得 utils_lib
    PRIVATE project_options
)

# app/CMakeLists.txt
add_executable(enterprise_app main.cpp)
target_link_libraries(enterprise_app
    PRIVATE core_lib         # 自动传递：core_lib → utils_lib → project_options
)

# tests/CMakeLists.txt
enable_testing()
add_executable(test_utils test_utils.cpp)
target_link_libraries(test_utils PRIVATE utils_lib)
add_test(NAME utils_test COMMAND test_utils)
set_tests_properties(utils_test PROPERTIES LABELS "unit" TIMEOUT 10)
```

**架构要点**：

| 要点                           | 说明                                                                                 |
|------------------------------|------------------------------------------------------------------------------------|
| `project_options` 接口库        | 以 `INTERFACE` 集中管理编译选项，所有目标 `target_link_libraries(... project_options)` 继承，避免全局污染 |
| `PUBLIC` 传播依赖链               | `core_lib PUBLIC utils_lib`：链接 `core_lib` 的消费者自动获得 `utils_lib` 的头文件和链接，无需重复声明      |
| `PRIVATE project_options`    | 编译选项不对外暴露（外部不需要用同样的警告标志编译）                                                         |
| ASan 用 `target_link_options` | ASan 既需要编译标志（`-fsanitize=address`）也需要链接标志，两个都要设                                    |

---

## 2. 语法完整参考

### 2.1 project() 与 cmake_minimum_required() 完整参数

```cmake
cmake_minimum_required(VERSION 3.28)
cmake_minimum_required(VERSION 3.20...3.28)   # 范围写法：兼容 3.20+，以 3.28 策略运行
```

| 参数                    | 说明                                    |
|-----------------------|---------------------------------------|
| `VERSION 3.28`        | 要求最低 CMake 版本；低于此版本报错停止               |
| `VERSION 3.20...3.28` | 策略范围：最低 3.20，最高用 3.28 的 policy；推荐现代写法 |

```cmake
project(MyApp
    VERSION      1.2.3
    DESCRIPTION  "示例项目"
    HOMEPAGE_URL "https://example.com"
    LANGUAGES    CXX C
)
```

| 参数                | 说明                                                         |
|-------------------|------------------------------------------------------------|
| `MyApp`           | 工程名，设置 `PROJECT_NAME` 变量                                   |
| `VERSION 1.2.3`   | 设置 `PROJECT_VERSION` / `PROJECT_VERSION_MAJOR/MINOR/PATCH` |
| `DESCRIPTION`     | 设置 `PROJECT_DESCRIPTION`，供 CPack 等工具读取                     |
| `HOMEPAGE_URL`    | 设置 `PROJECT_HOMEPAGE_URL`                                  |
| `LANGUAGES CXX C` | 启用的语言；仅 `CXX` 时跳过 C 编译器探测，加快配置速度                           |

---

### 2.2 变量

```cmake
set(VAR value)                 # 定义普通变量
set(VAR value CACHE STRING "说明")  # 缓存变量（可在 cmake-gui / -D 覆盖）
set(ENV{PATH} "/usr/bin")      # 设置环境变量

unset(VAR)                     # 删除变量

message(STATUS "${VAR}")       # 输出变量值（STATUS = 前缀 "--"）
message(WARNING "...")
message(FATAL_ERROR "...")      # 报错并停止

# 统一所有目标的输出目录（放在顶层 CMakeLists.txt）
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)  # .exe / .dll
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)  # .so / .dylib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)  # .a / .lib

# 调试：一次性打印多个变量（CMake 3.18+，比手写 message 方便）
include(CMakePrintHelpers)
cmake_print_variables(CMAKE_CXX_COMPILER CMAKE_BUILD_TYPE PROJECT_VERSION)
# 输出：CMAKE_CXX_COMPILER="..." ; CMAKE_BUILD_TYPE="Debug" ; PROJECT_VERSION="1.2.3"
```

**常用内置变量**：

| 变量                               | 含义                                                                                     |
|----------------------------------|----------------------------------------------------------------------------------------|
| `CMAKE_SOURCE_DIR`               | 顶层 CMakeLists.txt 所在目录（始终固定）                                                           |
| `CMAKE_CURRENT_SOURCE_DIR`       | 当前正在处理的 CMakeLists.txt 所在目录                                                            |
| `CMAKE_CURRENT_LIST_DIR`         | 当前正在处理的**文件**所在目录；在 `include()` 引入的 `.cmake` 文件里与 `CMAKE_CURRENT_SOURCE_DIR` 不同        |
| `CMAKE_BINARY_DIR`               | 顶层构建目录                                                                                 |
| `CMAKE_CURRENT_BINARY_DIR`       | 当前目标的构建目录                                                                              |
| `PROJECT_NAME`                   | `project()` 设置的工程名                                                                     |
| `PROJECT_VERSION`                | `project(... VERSION x.y.z)` 设置的版本                                                     |
| `CMAKE_BUILD_TYPE`               | `Debug` / `Release` / `RelWithDebInfo` / `MinSizeRel`                                  |
| `CMAKE_CXX_COMPILER`             | 实际使用的 C++ 编译器完整路径（由 CMake 自动检测或 `-DCMAKE_CXX_COMPILER` 指定）                             |
| `CMAKE_CXX_COMPILER_ID`          | `MSVC` / `GNU` / `Clang` / `AppleClang`                                                |
| `CMAKE_INSTALL_PREFIX`           | `cmake --install` 的安装根目录；默认 `/usr/local`（Linux）或 `C:/Program Files/<project>`（Windows） |
| `CMAKE_RUNTIME_OUTPUT_DIRECTORY` | 统一所有目标的可执行文件 / DLL 输出目录（覆盖各目标的默认位置）                                                    |
| `CMAKE_LIBRARY_OUTPUT_DIRECTORY` | 统一共享库（`.so` / `.dylib`）输出目录                                                            |
| `CMAKE_ARCHIVE_OUTPUT_DIRECTORY` | 统一静态库（`.a` / `.lib`）输出目录                                                               |

> **`CMAKE_CURRENT_LIST_DIR` vs `CMAKE_CURRENT_SOURCE_DIR`**：在 CMakeLists.txt 里两者相同；但在 `include(my_utils.cmake)`
> 引入的文件里，`CMAKE_CURRENT_SOURCE_DIR` 仍指向调用方的目录，`CMAKE_CURRENT_LIST_DIR` 指向 `my_utils.cmake` 所在目录。引用
`.cmake` 文件自身路径时应使用 `CMAKE_CURRENT_LIST_DIR`。

### 2.3 option()

```cmake
option(ENABLE_TESTS  "构建并运行单元测试" ON)
option(ENABLE_ASAN   "开启 AddressSanitizer" OFF)
option(USE_SHARED    "构建动态库而非静态库" OFF)

if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
```

| 语法                              | 说明                                                       |
|---------------------------------|----------------------------------------------------------|
| `option(<变量> "<说明>" <默认值>)`     | 声明一个布尔缓存变量；默认值 `ON` / `OFF`                              |
| 命令行覆盖                           | `cmake -B build -DENABLE_TESTS=OFF` 覆盖默认值                |
| 与 `set(... CACHE BOOL ...)` 的区别 | `option` 是简化写法，等价于 `set(VAR OFF CACHE BOOL "说明")`，但只支持布尔 |

> **注意**：`option` 的值一旦写入 `CMakeCache.txt` 就不会被下次 cmake 命令的默认值覆盖——必须显式 `-DVAR=新值` 或删除缓存。

---

### 2.4 条件与循环

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

### 2.5 函数与宏

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

**`cmake_parse_arguments()` — 为自定义函数/宏解析具名参数：**

```cmake
function(my_target_setup target)
    cmake_parse_arguments(
        ARG                          # 变量前缀
        "WARNINGS;WERROR"            # 布尔选项（无值）
        "STANDARD"                   # 单值选项
        "SOURCES;INCLUDES"           # 多值选项（列表）
        ${ARGN}                      # 剩余参数
    )
    # 使用：ARG_WARNINGS / ARG_STANDARD / ARG_SOURCES
    if(ARG_WARNINGS)
        target_compile_options(${target} PRIVATE -Wall -Wextra)
    endif()
    if(ARG_STANDARD)
        set_target_properties(${target} PROPERTIES CXX_STANDARD ${ARG_STANDARD})
    endif()
    target_sources(${target} PRIVATE ${ARG_SOURCES})
    target_include_directories(${target} PRIVATE ${ARG_INCLUDES})
endfunction()

# 调用
my_target_setup(app WARNINGS STANDARD 20 SOURCES main.cpp INCLUDES include/)
```

| 参数                       | 说明                                          |
|--------------------------|---------------------------------------------|
| 变量前缀                     | 所有解析结果以此为前缀，如 `ARG_WARNINGS`、`ARG_STANDARD` |
| 布尔选项                     | 出现则为 `TRUE`，不出现则为 `FALSE`                   |
| 单值选项                     | 后跟一个值                                       |
| 多值选项                     | 后跟多个值，结果为列表                                 |
| `ARG_UNPARSED_ARGUMENTS` | 未被任何选项匹配的剩余参数                               |

### 2.6 目标命令

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

target_link_options(name PRIVATE|PUBLIC|INTERFACE -Wl,--as-needed)  # 链接选项
target_compile_features(name PRIVATE|PUBLIC|INTERFACE cxx_std_20)   # C++ 标准特性

# FILE_SET：头文件集合（CMake 3.23+），支持 C++20 模块
target_sources(mylib
    PUBLIC FILE_SET HEADERS
    BASE_DIRS include/
    FILES     include/mylib/api.h include/mylib/types.h
)
# install 时头文件自动安装到正确位置
install(TARGETS mylib FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
```

**`target_compile_features` vs `set(CMAKE_CXX_STANDARD 20)`：**

| 方式                                                      | 作用范围      | 推荐场景         |
|---------------------------------------------------------|-----------|--------------|
| `target_compile_features(tgt PRIVATE cxx_std_20)`       | 仅该目标      | 库开发，避免污染全局设置 |
| `set(CMAKE_CXX_STANDARD 20)`                            | 整个项目      | 顶层项目统一设置     |
| `set_target_properties(tgt PROPERTIES CXX_STANDARD 20)` | 仅该目标，语义等价 | 同上，写法不同      |

### 2.7 add_subdirectory()

```cmake
# 顶层 CMakeLists.txt
add_subdirectory(utils)          # 进入 utils/ 目录，执行其 CMakeLists.txt
add_subdirectory(core)           # core 可以 target_link_libraries(core PRIVATE utils_lib)
add_subdirectory(app)
add_subdirectory(tests EXCLUDE_FROM_ALL)   # 不纳入默认构建，需显式 cmake --build --target tests
```

| 参数                 | 说明                                               |
|--------------------|--------------------------------------------------|
| `<子目录>`            | 相对于当前 CMakeLists.txt 的路径，子目录须有自己的 CMakeLists.txt |
| `EXCLUDE_FROM_ALL` | 该子目录的目标不加入默认构建目标（ALL），只在显式指定时才构建                 |

**子目录之间的依赖顺序**：`add_subdirectory` 按顺序执行，但目标的实际链接依赖由 `target_link_libraries` 决定，CMake
自动排序，无需手动保证顺序。

---

### 2.8 include() 与常用模块

```cmake
include(GNUInstallDirs)      # 提供标准安装目录变量
include(FetchContent)        # 在线下载依赖
include(CPack)               # 打包支持
include(CTest)               # 测试支持（等价于 enable_testing()）
include(CheckCXXCompilerFlag)  # 检测编译器是否支持某 flag
include(GenerateExportHeader)  # 生成 DLL 导出宏头文件
include(CMakePackageConfigHelpers)  # 生成 find_package 配置文件

include(my_utils.cmake)      # 引入自定义 .cmake 脚本

# 防止 .cmake 文件被重复 include（类似 C++ #pragma once）
include_guard()              # 放在 .cmake 文件顶部；全局作用域
include_guard(DIRECTORY)     # 仅在当前目录作用域生效
```

| 模块                     | 作用                                                                  |
|------------------------|---------------------------------------------------------------------|
| `GNUInstallDirs`       | 定义 `CMAKE_INSTALL_BINDIR` / `LIBDIR` / `INCLUDEDIR` 等标准目录           |
| `FetchContent`         | CMake 3.11+，配置阶段下载外部依赖（见 2.14 节）                                    |
| `CPack`                | 生成安装包（.zip / .deb / .msi 等），`include(CPack)` 后即可用 `cpack` 命令        |
| `CheckCXXCompilerFlag` | `check_cxx_compiler_flag("-std=c++20" HAS_CXX20)` 检测编译器 flag 支持     |
| `GenerateExportHeader` | 为动态库自动生成 `dllexport/dllimport` 宏头文件，替代 `WINDOWS_EXPORT_ALL_SYMBOLS` |

---

### 2.9 文件操作

```cmake
file(GLOB srcs "*.cpp")                       # 展开通配符（不推荐用于 SRCS）
file(GLOB_RECURSE srcs "src/*.cpp")           # 递归展开
file(READ path content)                       # 读文件内容到变量
file(WRITE path "content")                    # 写文件
file(COPY src DESTINATION dst)                # 复制文件/目录
configure_file(input.h.in output.h)           # 替换 @VAR@ 生成头文件

# 在生成阶段将生成器表达式求值后写入文件（用于调试 $<...>）
file(GENERATE
    OUTPUT  "${CMAKE_BINARY_DIR}/flags_$<CONFIG>.txt"
    CONTENT "$<TARGET_PROPERTY:my_app,COMPILE_OPTIONS>"
)
```

> `file(GENERATE)` 在 `cmake -B`（配置阶段）之后、`cmake --build`（构建阶段）之前执行，是调试生成器表达式的标准手段——把
`$<CONFIG>`、`$<TARGET_PROPERTY:...>` 等写入文件查看实际值。

### 2.10 list() 操作

```cmake
set(SRCS main.cpp utils.cpp)

list(APPEND SRCS extra.cpp)          # 追加元素
list(REMOVE_ITEM SRCS utils.cpp)     # 删除元素
list(LENGTH SRCS len)                # 获取长度 → len=2
list(GET SRCS 0 first)               # 按索引取值 → first=main.cpp
list(FIND SRCS extra.cpp idx)        # 查找 → idx=1（-1 表示不存在）
list(SORT SRCS)                      # 排序（原地）
list(REVERSE SRCS)                   # 反转
list(FILTER SRCS INCLUDE REGEX ".*\.cpp")  # 正则过滤（保留匹配项）
list(FILTER SRCS EXCLUDE REGEX "test_.*")  # 正则过滤（排除匹配项）
list(JOIN SRCS ";" result)           # 用分隔符拼接成字符串
list(TRANSFORM SRCS PREPEND "src/")  # 每个元素加前缀
```

---

### 2.11 string() 操作

```cmake
string(TOUPPER "hello" upper)          # → HELLO
string(TOLOWER "HELLO" lower)          # → hello
string(LENGTH "hello" len)             # → 5
string(SUBSTRING "hello" 1 3 sub)      # → ell（从位置1取3个字符）
string(REPLACE "o" "0" result "hello") # → hell0
string(REGEX MATCH "[0-9]+" num "abc123def")   # → 123（第一个匹配）
string(REGEX MATCHALL "[0-9]+" nums "a1b2c3")  # → 1;2;3（所有匹配，结果为列表）
string(REGEX REPLACE "([0-9]+)" "(\\1)" result "a1b2")  # → a(1)b(2)
string(STRIP "  hello  " trimmed)      # 去首尾空白 → hello
string(CONFIGURE "${template}" output) # 替换 ${VAR} 和 @VAR@
```

---

### 2.12 execute_process()

```cmake
# 在配置阶段（cmake -B）执行命令，结果存入变量
execute_process(
    COMMAND git rev-parse --short HEAD
    OUTPUT_VARIABLE GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)
message(STATUS "Git commit: ${GIT_HASH}")
target_compile_definitions(app PRIVATE GIT_HASH="${GIT_HASH}")

# 执行多条命令（管道）
execute_process(
    COMMAND python3 --version
    OUTPUT_VARIABLE PY_VER
    ERROR_VARIABLE  PY_VER     # Python 2 把版本号输出到 stderr
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE PY_RESULT  # 退出码
)
if(NOT PY_RESULT EQUAL 0)
    message(FATAL_ERROR "未找到 Python 3")
endif()
```

| 参数                                 | 说明                              |
|------------------------------------|---------------------------------|
| `COMMAND`                          | 要执行的命令及参数，不经过 shell 解析（无管道/重定向） |
| `OUTPUT_VARIABLE`                  | 捕获标准输出到变量                       |
| `ERROR_VARIABLE`                   | 捕获标准错误到变量                       |
| `RESULT_VARIABLE`                  | 捕获退出码（0 = 成功）                   |
| `OUTPUT_STRIP_TRAILING_WHITESPACE` | 去除输出末尾换行，常用                     |
| `ERROR_QUIET`                      | 忽略错误输出，不显示到终端                   |
| `WORKING_DIRECTORY`                | 指定命令的工作目录                       |
| `TIMEOUT`                          | 超时秒数，超时后命令被终止                   |

> **与 `add_custom_command` 的区别**：`execute_process` 在**配置阶段**运行（`cmake -B`），结果是 CMake 变量；
`add_custom_command` 在**构建阶段**运行（`cmake --build`），结果是文件或构建动作。

---

### 2.13 add_custom_command() / add_custom_target()

```cmake
# 在构建某目标之前 / 之后执行命令
add_custom_command(
    TARGET my_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:my_app> ${CMAKE_BINARY_DIR}/output/
    COMMENT "复制可执行文件到 output 目录"
)

# 生成文件（源码生成场景）
add_custom_command(
    OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/generated.cpp
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/codegen.py
            -o ${CMAKE_CURRENT_BINARY_DIR}/generated.cpp
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/codegen.py
            ${CMAKE_CURRENT_SOURCE_DIR}/schema.json
    COMMENT "运行代码生成器"
)
add_library(mylib generated.cpp other.cpp)   # generated.cpp 自动触发上面的命令

# 自定义目标（不生成文件，按需执行）
add_custom_target(format
    COMMAND clang-format -i ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    COMMENT "格式化源码"
)
# cmake --build build --target format
```

| 参数                                         | 说明                                                            |
|--------------------------------------------|---------------------------------------------------------------|
| `TARGET xxx PRE_BUILD/PRE_LINK/POST_BUILD` | 绑定到目标构建的某个阶段；`POST_BUILD` 最常用                                 |
| `OUTPUT 文件`                                | 声明命令生成的文件；有依赖此文件的目标时自动触发                                      |
| `DEPENDS 文件/目标`                            | 输入依赖；依赖变化时重新执行命令                                              |
| `COMMENT`                                  | 构建时显示的进度文字                                                    |
| `${CMAKE_COMMAND} -E`                      | CMake 跨平台文件操作（`copy`、`make_directory`、`remove` 等），避免 shell 差异 |

---

### 2.14 FetchContent（在线下载依赖）

```cmake
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
FetchContent_Declare(
    nlohmann_json
    URL      https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    URL_HASH SHA256=d6c65aca6b1ed68e7a182f4757257b107ae403032760ed6ef121c9d55e81757d
)

FetchContent_MakeAvailable(googletest nlohmann_json)

# 之后直接使用下载的目标
target_link_libraries(my_test PRIVATE GTest::gtest_main)
target_link_libraries(app PRIVATE nlohmann_json::nlohmann_json)
```

| 命令                           | 说明                                                         |
|------------------------------|------------------------------------------------------------|
| `FetchContent_Declare`       | 声明依赖来源（GIT / URL / SVN），不立即下载                              |
| `FetchContent_MakeAvailable` | 下载并添加到构建（等价于 `FetchContent_Populate` + `add_subdirectory`） |
| `GIT_TAG`                    | 推荐使用 tag 或 commit hash，避免每次构建拉取最新                          |
| `URL_HASH`                   | 校验下载包完整性，防止篡改                                              |

> **FetchContent vs vcpkg**：FetchContent 在 cmake 配置阶段下载源码并编译，无需额外工具；vcpkg 预编译缓存，速度更快。大型依赖（如
> Qt、Boost）优先用 vcpkg。

---

### 2.15 target_precompile_headers()

```cmake
# 创建预编译头（PCH）
target_precompile_headers(my_lib
    PRIVATE
        <vector>
        <string>
        <unordered_map>
        "my_common.h"
)

# 复用其他目标的 PCH（避免重复编译）
target_precompile_headers(my_app REUSE_FROM my_lib)
```

| 参数                         | 说明                            |
|----------------------------|-------------------------------|
| `PRIVATE/PUBLIC/INTERFACE` | 传播范围，通常用 `PRIVATE`（PCH 是实现细节） |
| `<header>`                 | 系统头文件用尖括号；自定义头文件用引号           |
| `REUSE_FROM <目标>`          | 复用已有 PCH，节省重复编译；两个目标的编译标志须兼容  |

> **适用场景**：频繁 include 的重量级头文件（STL、Boost、Qt 头文件等）收益明显；小项目或头文件变动频繁时收益有限。

---

### 2.16 生成器表达式速查

| 表达式                         | 含义            |
|-----------------------------|---------------|
| `$<CXX_COMPILER_ID:MSVC>`   | 编译器是 MSVC     |
| `$<CONFIG:Release>`         | 构建类型是 Release |
| `$<NOT:$<...>>`             | 取反            |
| `$<AND:$<...>,$<...>>`      | 与             |
| `$<OR:$<...>,$<...>>`       | 或             |
| `$<BUILD_INTERFACE:path>`   | 仅构建时生效        |
| `$<INSTALL_INTERFACE:path>` | 仅安装后生效        |
| `$<TARGET_FILE:name>`       | 目标输出文件的完整路径   |

### 2.17 find_library() / find_program() / find_path() / find_file()

```cmake
# find_library — 查找库文件（.a / .lib / .so / .dll）
find_library(
    ZLIB_LIB          # 结果变量（找到时为完整路径，未找到为 <VAR>-NOTFOUND）
    NAMES z zlib      # 按顺序尝试的库名（不含前缀 lib 和后缀）
    PATHS /usr/lib /usr/local/lib
    PATH_SUFFIXES x64 # 在搜索路径下附加子目录
)
if(ZLIB_LIB)
    target_link_libraries(app PRIVATE ${ZLIB_LIB})
endif()

# find_program — 查找可执行文件
find_program(CLANG_FORMAT clang-format
    HINTS /usr/local/bin
)
if(CLANG_FORMAT)
    message(STATUS "clang-format: ${CLANG_FORMAT}")
endif()

# find_path — 查找包含某个文件的目录（常用于头文件目录）
find_path(ZLIB_INCLUDE_DIR zlib.h
    HINTS /usr/include /usr/local/include
)

# find_file — 查找某个具体文件的完整路径
find_file(ZLIB_H zlib.h
    HINTS /usr/include
)
```

| 命令             | 用途        | 结果变量      |
|----------------|-----------|-----------|
| `find_library` | 查找库文件     | 库完整路径     |
| `find_program` | 查找可执行文件   | 可执行文件完整路径 |
| `find_path`    | 查找头文件所在目录 | 目录路径      |
| `find_file`    | 查找任意文件    | 文件完整路径    |

**通用搜索顺序**（从高到低）：

1. `CMAKE_PREFIX_PATH` 指定的目录
2. `HINTS` 指定的目录（优先于系统路径）
3. 系统默认路径（`/usr/lib`、`%ProgramFiles%` 等）
4. `PATHS` 指定的目录（低于系统路径）

> `find_package` 是这四个命令的高层封装，会自动查找库路径、头文件目录、导入目标等；`find_library`
> 等是更底层的单一职责工具，适合手动组装自定义查找逻辑。

---

### 2.18 install() 完整语法

```cmake
include(GNUInstallDirs)   # 提供标准目录变量

# 安装目标（可执行文件、库）
install(TARGETS myapp mylib
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}      # .exe / 可执行
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}      # .so / .dylib
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}      # .a / .lib
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}  # 头文件目录（仅记录，不拷贝）
)

# 安装头文件
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES     mylib.h  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# 安装 CMake 包配置文件（让其他项目 find_package 找到本库）
install(EXPORT mylibTargets
    FILE      mylibTargets.cmake
    NAMESPACE mylib::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mylib
)

# 按组件安装
install(TARGETS myapp COMPONENT runtime)
install(TARGETS mylib COMPONENT devel)
# cmake --install build --component runtime   # 只安装 runtime 组件
```

| 关键字         | 说明                                                        |
|-------------|-----------------------------------------------------------|
| `RUNTIME`   | Windows 的 `.exe` / `.dll`，Linux 的可执行文件                    |
| `LIBRARY`   | Linux/macOS 的共享库 `.so` / `.dylib`；Windows DLL 归 `RUNTIME` |
| `ARCHIVE`   | 静态库 `.a` / `.lib`；Windows DLL 的导入库 `.lib` 也归这里            |
| `COMPONENT` | 分组安装，`cmake --install --component` 时过滤                    |
| `NAMESPACE` | 导出目标的命名空间前缀，消费者用 `mylib::mylib` 引用                        |

---

### 2.19 enable_testing() / add_test() / set_tests_properties()

```cmake
enable_testing()   # 或 include(CTest)，启用 CTest 支持

add_executable(test_basic test_basic.cpp)
target_link_libraries(test_basic PRIVATE mylib)

# 注册测试（NAME 唯一标识，COMMAND 为要运行的可执行文件及参数）
add_test(NAME basic_test    COMMAND test_basic)
add_test(NAME basic_verbose COMMAND test_basic --verbose)
add_test(NAME py_test       COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/test.py)

# 设置测试属性
set_tests_properties(basic_test PROPERTIES
    TIMEOUT        10           # 超时秒数，超时视为失败
    LABELS         "unit;fast"  # 标签，ctest -L 过滤用
    PASS_REGULAR_EXPRESSION "PASSED"   # 输出匹配此正则才算通过
    FAIL_REGULAR_EXPRESSION "FAILED"   # 输出匹配此正则则失败
    ENVIRONMENT    "MY_VAR=1"          # 设置测试运行时的环境变量
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# 批量设置属性（CMake 3.28+）
set_property(TEST basic_test basic_verbose PROPERTY LABELS "unit")
```

| 属性                        | 说明                                       |
|---------------------------|------------------------------------------|
| `TIMEOUT`                 | 超时秒数；CI 环境必设，防止死循环测试卡住流水线                |
| `LABELS`                  | 分号分隔的标签列表；`ctest -L unit` 只跑带 unit 标签的测试 |
| `PASS_REGULAR_EXPRESSION` | 输出需匹配此正则才算通过；适合脚本测试                      |
| `FAIL_REGULAR_EXPRESSION` | 输出匹配此正则即失败（优先于 PASS）                     |
| `ENVIRONMENT`             | 测试运行的额外环境变量                              |
| `DEPENDS`                 | 本测试依赖的其他测试名（先跑依赖项）                       |
| `RUN_SERIAL`              | `ON` 时不并行执行（`ctest -j` 对此测试无效）           |

---

### 2.20 add_compile_options() / add_link_options()

```cmake
# 全局编译选项：作用于当前目录及所有子目录的所有目标
add_compile_options(-Wall -Wextra)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/W4>)

# 全局链接选项
add_link_options(-Wl,--as-needed)
```

**与 `target_compile_options` 的关键区别：**

|      | `add_compile_options` | `target_compile_options`           |
|------|-----------------------|------------------------------------|
| 作用范围 | 当前目录 + 所有子目录的**全部目标** | 仅指定的单个目标                           |
| 传播控制 | 无（全局生效）               | `PRIVATE` / `PUBLIC` / `INTERFACE` |
| 推荐程度 | ⚠ 谨慎使用，容易污染第三方库       | ✓ 推荐，精确控制                          |
| 典型用途 | 顶层设置所有目标共享的基础选项       | 库/可执行文件的独立选项                       |

> **最佳实践**：优先用 `target_compile_options`；如需全局设置，考虑用接口库（`project_options` 模式，见 1.11 节）代替
`add_compile_options`，避免意外影响 `add_subdirectory` 引入的第三方库。

**跨编译器写法：`if(MSVC)` vs 生成器表达式**

MSVC（`cl.exe`）和 GCC/Clang 的选项前缀不兼容（MSVC 用 `/`，GCC 用 `-`），需要区分处理。有两种写法：

```cmake
# 写法 A：if(MSVC) — 简单直观，适合单目标的入门工程（本项目各子目录用此写法）
# if(MSVC) 在 CMake 配置阶段（cmake -B build 时）判断
if(MSVC)
    add_compile_options(
        /W4        # 警告级别 4（最高常用级，/W0=无，/W1-/W4 递增）
        /utf-8     # 源文件和执行字符集均为 UTF-8，避免中文注释乱码
    )
else()             # MinGW / GCC / Clang
    add_compile_options(
        -Wall      # 高价值警告集合（非全部，名字有误导）
        -Wextra    # Wall 之外的额外警告
        -Wpedantic # 严格 C++ 标准，报所有编译器扩展用法
    )
endif()

# 写法 B：生成器表达式 — 精细控制，可同时区分编译器类型 + Debug/Release
# 生成器表达式在构建阶段（ninja/make 运行时）展开，能区分 Debug/Release；if(MSVC) 不行
target_compile_options(app PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /utf-8>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)
```

| 对比项                | `if(MSVC)`       | 生成器表达式               |
|--------------------|------------------|----------------------|
| 判断时机               | 配置阶段（`cmake -B`） | 构建阶段（`ninja`/`make`） |
| 能否区分 Debug/Release | ❌                | ✅                    |
| 可读性                | ✅ 直观             | ⚠ 嵌套深，初学难读           |
| 适合场景               | 简单工程、入门          | 多配置工程、需精细控制          |

---

### 2.21 跨平台条件配置

根据编译器、操作系统、构建类型的不同，向目标传入不同的编译选项。

**两种主要写法对比：**

| 写法              | 判断时机                 | 能否区分 Debug/Release  | 推荐场景             |
|-----------------|----------------------|---------------------|------------------|
| `if(MSVC)` 等条件块 | 配置阶段（`cmake -B`）     | ❌ 仅单配置 Generator 有效 | 简单工程，只按编译器/OS 区分 |
| 生成器表达式 `$<...>` | 构建阶段（`ninja`/`make`） | ✅ 多/单配置均有效          | 需同时区分编译器 + 构建类型  |

---

#### 层次一：最简 — MSVC vs 其他

最常见场景：MSVC（`/`前缀选项）和 GCC/Clang（`-`前缀选项）不兼容，必须分支。

```cmake
# 作用于当前目录所有目标（放顶层 CMakeLists.txt）
if(MSVC)
    add_compile_options(/W4 /utf-8)
else()  # MinGW / GCC / Clang
    add_compile_options(-Wall -Wextra)
endif()

# 精确控制单个目标（推荐）
if(MSVC)
    target_compile_options(app PRIVATE /W4 /utf-8)
else()
    target_compile_options(app PRIVATE -Wall -Wextra)
endif()
```

> **本项目各子目录**均采用此简化写法。

---

#### 层次二：三路编译器区分

| 条件                                      | 匹配对象                   | `CMAKE_CXX_COMPILER_ID` 值  |
|-----------------------------------------|------------------------|----------------------------|
| `MSVC`                                  | Visual Studio 的 cl.exe | `"MSVC"`                   |
| `CMAKE_CXX_COMPILER_ID STREQUAL "GNU"`  | GCC / MinGW-w64        | `"GNU"`                    |
| `CMAKE_CXX_COMPILER_ID MATCHES "Clang"` | Clang 和 AppleClang 均匹配 | `"Clang"` / `"AppleClang"` |

```cmake
if(MSVC)
    target_compile_options(app PRIVATE /W4 /WX /utf-8 /permissive-)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(app PRIVATE -Wall -Wextra -Wpedantic -Werror)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(app PRIVATE -Weverything -Wno-c++98-compat)
endif()
```

> `if(MSVC)` 等价于 `if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")`，`MSVC` 是 CMake 内置快捷变量。

---

#### 层次三：操作系统区分

| 条件                   | 匹配对象                      | 注意             |
|----------------------|---------------------------|----------------|
| `WIN32`              | Windows（MSVC 或 MinGW 均匹配） | 含 64 位 Windows |
| `APPLE`              | macOS / iOS               | `UNIX` 同时也为真   |
| `UNIX AND NOT APPLE` | 纯 Linux                   | 排除 macOS       |
| `UNIX`               | Linux + macOS             | 两者都匹配          |

```cmake
if(WIN32)
    target_compile_definitions(app PRIVATE PLATFORM_WINDOWS)
    target_link_libraries(app PRIVATE ws2_32)          # Windows Socket
elseif(APPLE)
    target_compile_definitions(app PRIVATE PLATFORM_MACOS)
    find_library(CORE_FOUNDATION CoreFoundation)
    target_link_libraries(app PRIVATE ${CORE_FOUNDATION})
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(app PRIVATE PLATFORM_LINUX)
    target_link_libraries(app PRIVATE pthread)
endif()
```

---

#### 层次四：Debug / Release 区分

**单配置 Generator（Ninja、MinGW Makefiles）** 在 `cmake -B` 时确定类型，可用 `if()`；
**多配置 Generator（Visual Studio）** 构建时才确定，必须用生成器表达式。

```cmake
# ✅ 推荐：生成器表达式（两种 Generator 均有效）
target_compile_options(app PRIVATE
    $<$<CONFIG:Debug>:-O0 -g3>           # Debug：不优化，最全调试信息
    $<$<CONFIG:Release>:-O3>             # Release：最大优化
)

target_compile_definitions(app PRIVATE
    $<$<CONFIG:Debug>:DEBUG_BUILD>
    $<$<CONFIG:Release>:NDEBUG>
)

# ⚠ 仅单配置 Generator 有效（VS Generator 下 CMAKE_BUILD_TYPE 为空）
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(app PRIVATE DEBUG_BUILD)
endif()
```

---

#### 层次五：组合条件（编译器 + 构建类型）

`if()` 无法同时区分编译器和构建类型；生成器表达式可以嵌套 `$<AND:...>` 实现。

```cmake
target_compile_options(app PRIVATE
    # MSVC：所有构建类型
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /utf-8>
    # GCC/Clang Debug：不优化 + 调试符号
    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<CONFIG:Debug>>:-O0 -g3>
    # GCC/Clang Release：最大优化
    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<CONFIG:Release>>:-O3>
)

# 更清晰的三元写法（CMake 3.15+）
target_compile_options(app PRIVATE
    $<IF:$<CXX_COMPILER_ID:MSVC>,/W4 /utf-8,$<IF:$<CONFIG:Debug>,-O0 -g3,-O3>>
)
```

---

#### 层次六：用户 option 开关

```cmake
option(ENABLE_ASAN    "开启 AddressSanitizer" OFF)
option(ENABLE_FEATURE "启用某功能"             OFF)

if(ENABLE_ASAN)
    # ASan 必须同时设编译标志和链接标志
    target_compile_options(app PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(app    PRIVATE -fsanitize=address)
endif()

if(ENABLE_FEATURE)
    target_compile_definitions(app PRIVATE FEATURE_ENABLED)
    target_sources(app PRIVATE feature.cpp)
endif()

# 等价的生成器表达式写法（option 变量是布尔值）
target_compile_definitions(app PRIVATE
    $<$<BOOL:${ENABLE_FEATURE}>:FEATURE_ENABLED>
)
```

```bash
cmake -B build -DENABLE_ASAN=ON -DENABLE_FEATURE=ON
```

---

#### 层次七：编译器版本区分

```cmake
# if() 写法（配置阶段判断）
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
   AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "12")
    target_compile_options(app PRIVATE -fcoroutines)
endif()

if(MSVC AND MSVC_VERSION GREATER_EQUAL 1930)   # VS 2022 = 1930
    target_compile_options(app PRIVATE /Zc:__cplusplus)
endif()

# 生成器表达式（可与 CONFIG 嵌套组合）
target_compile_options(app PRIVATE
    $<$<AND:$<CXX_COMPILER_ID:GNU>,$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,12>>:-fcoroutines>
)
```

| 变量                           | 示例值        | 说明                                        |
|------------------------------|------------|-------------------------------------------|
| `CMAKE_CXX_COMPILER_VERSION` | `"13.2.0"` | 完整版本字符串，用 `VERSION_GREATER_EQUAL` 比较      |
| `MSVC_VERSION`               | `1930`     | MSVC 内部版本号（VS 2022 = 193x，VS 2019 = 192x） |
| `MSVC_TOOLSET_VERSION`       | `143`      | 工具集版本（v143 = VS 2022，v142 = VS 2019）      |

---

#### 速查表

**常用条件变量：**

| 变量 / 条件                      | 含义                    | 典型值                                                   |
|------------------------------|-----------------------|-------------------------------------------------------|
| `MSVC`                       | 编译器是 cl.exe           | `TRUE` / `FALSE`                                      |
| `WIN32`                      | 目标平台是 Windows（含 64 位） | `TRUE` / `FALSE`                                      |
| `APPLE`                      | 目标平台是 macOS 或 iOS     | `TRUE` / `FALSE`                                      |
| `UNIX`                       | 目标平台是 Linux 或 macOS   | `TRUE` / `FALSE`                                      |
| `CMAKE_CXX_COMPILER_ID`      | 编译器 ID 字符串            | `MSVC` / `GNU` / `Clang` / `AppleClang`               |
| `CMAKE_CXX_COMPILER_VERSION` | 编译器完整版本号              | `13.2.0` / `17.0.1`                                   |
| `CMAKE_BUILD_TYPE`           | 构建类型（单配置 Generator）   | `Debug` / `Release` / `RelWithDebInfo` / `MinSizeRel` |
| `MSVC_VERSION`               | MSVC 内部版本号            | `1930`（VS 2022）                                       |

**生成器表达式速查（用于 `target_compile_options` 等）：**

| 表达式                                                   | 含义                     |
|-------------------------------------------------------|------------------------|
| `$<CXX_COMPILER_ID:MSVC>`                             | 编译器是 MSVC              |
| `$<CXX_COMPILER_ID:GNU>`                              | 编译器是 GCC               |
| `$<CONFIG:Debug>`                                     | 当前构建类型是 Debug          |
| `$<NOT:$<CXX_COMPILER_ID:MSVC>>`                      | 编译器不是 MSVC             |
| `$<AND:$<A>,$<B>>`                                    | A 且 B 同时为真             |
| `$<IF:cond,yes,no>`                                   | 三元：cond 为真取 yes，否则取 no |
| `$<BOOL:${VAR}>`                                      | 将 CMake 变量转成布尔值        |
| `$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,12>` | 编译器版本 ≥ 12             |

---

### 2.22 math(EXPR ...)

```cmake
# 基本运算（结果存入变量）
math(EXPR result "1 + 2")           # → 3
math(EXPR result "10 - 3")          # → 7
math(EXPR result "4 * 5")           # → 20
math(EXPR result "10 / 3")          # → 3（整除）
math(EXPR result "10 % 3")          # → 1（取模）

# 位运算
math(EXPR result "0xFF & 0x0F")     # → 15
math(EXPR result "1 << 4")          # → 16

# 引用变量
set(N 8)
math(EXPR half "${N} / 2")          # → 4

# 十六进制输出（CMake 3.13+）
math(EXPR result "255" OUTPUT_FORMAT HEXADECIMAL)  # → 0xff
```

| 运算符                                  | 说明                  |
|--------------------------------------|---------------------|
| `+ - * /`                            | 四则运算（整除）            |
| `%`                                  | 取模                  |
| `& \| ^ ~`                           | 位与、位或、位异或、位非        |
| `<< >>`                              | 左移、右移               |
| `OUTPUT_FORMAT DECIMAL\|HEXADECIMAL` | 指定输出格式（CMake 3.13+） |

---

### 2.23 find_package() 完整参数

```cmake
# Module 模式：CMake 查找内置 FindXxx.cmake 或 CMAKE_MODULE_PATH 里的同名文件
find_package(ZLIB)                          # 可选（找不到不报错）
find_package(ZLIB REQUIRED)                 # 必须找到，否则报错停止
find_package(OpenSSL 3.0 REQUIRED)          # 最低版本 3.0
find_package(OpenSSL 3.0 EXACT REQUIRED)    # 精确版本匹配

# Config 模式：查找库安装时生成的 XxxConfig.cmake / xxx-config.cmake
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Boost 1.82 REQUIRED COMPONENTS filesystem system)
find_package(Qt6    REQUIRED COMPONENTS Core Gui Widgets)

# 搜索路径控制
find_package(MyLib
    HINTS   /opt/mylib /usr/local/mylib    # 优先搜索（高于系统路径）
    PATHS   /custom/path                   # 额外路径（低于系统路径）
    PATH_SUFFIXES lib/cmake                # 在各路径下附加子目录
    NO_DEFAULT_PATH                        # 禁用所有默认路径，只用上面指定的
)
```

| 参数                       | 说明                              |
|--------------------------|---------------------------------|
| `REQUIRED`               | 找不到报错停止                         |
| `QUIET`                  | 找不到不打印提示；与 `REQUIRED` 同用时仍报错    |
| `VERSION x.y`            | 最低版本要求                          |
| `EXACT`                  | 精确版本匹配                          |
| `COMPONENTS comp1 comp2` | 指定需要的组件；组件找不到视为整个包未找到           |
| `OPTIONAL_COMPONENTS`    | 可选组件，找不到不报错                     |
| `CONFIG` / `NO_MODULE`   | 强制 Config 模式，跳过 Module 模式       |
| `HINTS`                  | 优先搜索路径（高于系统默认路径）                |
| `PATHS`                  | 额外搜索路径（低于系统默认路径）                |
| `NO_DEFAULT_PATH`        | 禁用所有默认搜索路径，只用 `HINTS` / `PATHS` |

---

### 2.24 add_dependencies()

```cmake
# 强制构建顺序：my_app 构建前先构建 codegen_target
add_custom_target(codegen
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/codegen.py
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/generated.h
)

add_executable(my_app main.cpp ${CMAKE_CURRENT_BINARY_DIR}/generated.h)
add_dependencies(my_app codegen)   # 确保 codegen 先于 my_app 构建
```

**与 `target_link_libraries` 的区别：**

|      | `target_link_libraries`      | `add_dependencies`          |
|------|------------------------------|-----------------------------|
| 作用   | 链接 + 构建顺序 + 属性传播             | 仅保证构建顺序                     |
| 适用对象 | 库目标（STATIC/SHARED/INTERFACE） | 任意目标，包括 `add_custom_target` |
| 推荐程度 | ✓ 首选                         | 只在无法用链接表达依赖时使用              |

> **何时必须用 `add_dependencies`**：依赖一个 `add_custom_target`（它不是库，无法链接）；或依赖一个不需要链接但必须先构建的目标。

---

### 2.25 cmake_path()（CMake 3.20+；LAST_ONLY 需 3.24+）

```cmake
set(p "/path/to/mylib.so.1.2")

# 分解路径
cmake_path(GET p FILENAME   fname)       # → mylib.so.1.2
cmake_path(GET p STEM       stem)        # → mylib（去掉最后一个扩展名）
cmake_path(GET p STEM LAST_ONLY s_last)  # → mylib.so.1（去掉最后一个 .2）
cmake_path(GET p EXTENSION  ext)         # → .1.2（从第一个 . 开始）
cmake_path(GET p EXTENSION LAST_ONLY e)  # → .2（只取最后一个扩展名）
cmake_path(GET p PARENT_PATH parent)     # → /path/to
cmake_path(GET p ROOT_NAME   root)       # Windows: C:；Unix: 空

# 拼接路径
cmake_path(APPEND result "/base" "sub" "file.txt")  # → /base/sub/file.txt
cmake_path(APPEND_STRING result "/base/sub" "/extra")

# 规范化 / 转换
cmake_path(NORMAL_PATH p OUTPUT_VARIABLE norm)   # 消除 .. / . / 多余斜杠
cmake_path(NATIVE_PATH p OUTPUT_VARIABLE native) # 转为本机路径分隔符（Windows: \）

# 查询
cmake_path(IS_ABSOLUTE p is_abs)         # → TRUE / FALSE
cmake_path(HAS_EXTENSION p has_ext)      # → TRUE / FALSE
cmake_path(RELATIVE_PATH p BASE_DIRECTORY "/path" OUTPUT_VARIABLE rel)
```

> **为什么优先用 `cmake_path` 而不是 `string(REGEX ...)`**：`cmake_path` 正确处理 Windows 盘符、UNC 路径、混合斜杠等边缘情况；
`string(REGEX)` 手写路径解析容易出 bug。

---

### 2.26 get_property() / set_property()

```cmake
# 目标属性（等价于 get/set_target_property，但语法更统一）
get_property(opts TARGET mylib PROPERTY COMPILE_OPTIONS)
set_property(TARGET mylib PROPERTY POSITION_INDEPENDENT_CODE ON)

# 批量设置多个目标
set_property(TARGET lib1 lib2 lib3 PROPERTY CXX_STANDARD 20)

# 目录属性（作用于当前目录）
get_property(incs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS DEBUG_MODE)

# 全局属性（跨目录读取所有目标列表）
get_property(all_targets GLOBAL PROPERTY BUILDSYSTEM_TARGETS)
message(STATUS "所有目标: ${all_targets}")

# 测试属性（批量设置多个测试，set_tests_properties 只能逐个设）
set_property(TEST test1 test2 test3 PROPERTY LABELS "unit")
set_property(TEST test1 test2 test3 APPEND PROPERTY LABELS "fast")  # APPEND 追加不覆盖
```

| 作用域         | 语法                                                  | 常用属性                                                       |
|-------------|-----------------------------------------------------|------------------------------------------------------------|
| `TARGET`    | `get/set_property(TARGET <tgt> PROPERTY <name>)`    | `COMPILE_OPTIONS` / `INCLUDE_DIRECTORIES` / `CXX_STANDARD` |
| `DIRECTORY` | `get/set_property(DIRECTORY <dir> PROPERTY <name>)` | `INCLUDE_DIRECTORIES` / `COMPILE_DEFINITIONS`              |
| `GLOBAL`    | `get/set_property(GLOBAL PROPERTY <name>)`          | `BUILDSYSTEM_TARGETS` / 自定义全局状态                            |
| `TEST`      | `get/set_property(TEST <name> PROPERTY <name>)`     | `TIMEOUT` / `LABELS` / `ENVIRONMENT`                       |
| `SOURCE`    | `get/set_property(SOURCE <file> PROPERTY <name>)`   | `COMPILE_FLAGS` / `GENERATED`                              |

---

### 2.27 try_compile() / try_run()

```cmake
# try_compile：测试一段代码能否编译通过（常用于特性检测）
try_compile(
    HAS_CXX20_RANGES        # 结果变量（TRUE = 编译成功）
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/test_ranges.cpp
    CXX_STANDARD 20
    OUTPUT_VARIABLE compile_log   # 捕获编译器输出（排错用）
)
if(HAS_CXX20_RANGES)
    target_compile_definitions(app PRIVATE HAS_CXX20_RANGES)
endif()

# 简短形式（直接内联代码，CMake 3.25+）
try_compile(HAS_STD_FORMAT
    SOURCE_FROM_CONTENT test.cpp
    "#include <format>\nint main(){auto s=std::format(\"{}\",42);}"
    CXX_STANDARD 23
)

# try_run：编译并运行，获取输出（交叉编译时不可用，需提供 fallback）
try_run(
    run_result       # 运行退出码
    compile_result   # 编译是否成功
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/detect_endian.cpp
    RUN_OUTPUT_VARIABLE endian   # 捕获程序输出
)
message(STATUS "字节序: ${endian}")
```

> **注意**：`try_run` 在交叉编译时无法运行目标平台代码，需配合 `CMAKE_TRY_RUN_<VAR>_DEFAULT` 或
`CMAKE_CROSSCOMPILING_EMULATOR` 处理。

---

### 2.28 cmake_host_system_information()

```cmake
# 获取主机系统信息（配置阶段，始终返回构建机信息，非目标机）
cmake_host_system_information(RESULT cpu_count   QUERY NUMBER_OF_PHYSICAL_CORES)
cmake_host_system_information(RESULT total_mem   QUERY TOTAL_PHYSICAL_MEMORY)   # MB
cmake_host_system_information(RESULT os_name     QUERY OS_NAME)         # Windows / Linux / macOS
cmake_host_system_information(RESULT os_platform QUERY OS_PLATFORM)     # x86_64 / arm64
cmake_host_system_information(RESULT hostname    QUERY HOSTNAME)

message(STATUS "CPU: ${cpu_count} 核，内存: ${total_mem} MB，OS: ${os_name}/${os_platform}")

# 常见用途：自动设置并行编译线程数
cmake_host_system_information(RESULT jobs QUERY NUMBER_OF_LOGICAL_CORES)
set(CMAKE_BUILD_PARALLEL_LEVEL ${jobs} CACHE STRING "并行构建线程数")
```

| QUERY 值                     | 说明         |
|-----------------------------|------------|
| `NUMBER_OF_PHYSICAL_CORES`  | 物理 CPU 核数  |
| `NUMBER_OF_LOGICAL_CORES`   | 逻辑核数（含超线程） |
| `TOTAL_PHYSICAL_MEMORY`     | 总物理内存（MB）  |
| `AVAILABLE_PHYSICAL_MEMORY` | 可用物理内存（MB） |
| `OS_NAME`                   | 操作系统名称     |
| `OS_PLATFORM`               | 处理器架构      |
| `HOSTNAME`                  | 主机名        |

---

### 2.29 cmake_policy()

```cmake
# 一次性启用某版本的所有策略（推荐写在 cmake_minimum_required 之后）
cmake_policy(VERSION 3.28)

# 单独设置某条策略
cmake_policy(SET CMP0077 NEW)   # NEW = 推荐行为；OLD = 兼容旧行为（不推荐）
cmake_policy(GET CMP0077 val)   # 读取当前值 → NEW / OLD / ""

# 策略作用域保存/恢复（用于函数、include 等场景）
cmake_policy(PUSH)
cmake_policy(SET CMP0054 NEW)
# ... 中间代码 ...
cmake_policy(POP)
```

**常见策略速查：**

| 策略        | 含义                                 | NEW 行为                                   |
|-----------|------------------------------------|------------------------------------------|
| `CMP0077` | `option()` 是否尊重已有缓存变量              | 尊重（`set` 在 `option` 之前有效）                |
| `CMP0048` | `project()` 是否必须声明 VERSION         | 可不声明（`PROJECT_VERSION` 为空字符串）            |
| `CMP0076` | `target_sources` 相对路径处理            | 转为绝对路径                                   |
| `CMP0135` | FetchContent URL 下载时间戳             | 使用提取时间（每次重新构建依赖）                         |
| `CMP0167` | `find_package(Boost)` 首选 Config 模式 | 跳过 FindBoost.cmake，直接找 BoostConfig.cmake |

> 遇到 CMake 警告 `Policy CMP00xx is not set` 时，在 `cmake_minimum_required` 后加 `cmake_policy(VERSION x.y)`
> 即可消除全部警告（推荐）；也可单独 `SET` 某条策略。

---

### 2.30 CMakePresets.json（CMake 3.19+）

```json
{
  "version": 6,
  "cmakeMinimumRequired": { "major": 3, "minor": 25 },

  "configurePresets": [
    {
      "name": "base",
      "hidden": true,
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build/${presetName}",
      "cacheVariables": {
        "CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
      }
    },
    {
      "name": "debug",
      "displayName": "Debug",
      "inherits": "base",
      "cacheVariables": { "CMAKE_BUILD_TYPE": "Debug" }
    },
    {
      "name": "release",
      "displayName": "Release",
      "inherits": "base",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release",
        "CMAKE_INTERPROCEDURAL_OPTIMIZATION": "ON"
      }
    },
    {
      "name": "asan",
      "displayName": "Debug + ASan",
      "inherits": "base",
      "cacheVariables": { "CMAKE_BUILD_TYPE": "Debug", "ENABLE_ASAN": "ON" }
    },
    {
      "name": "vcpkg",
      "displayName": "Debug + vcpkg",
      "inherits": "debug",
      "toolchainFile": "D:/software/vcpkg/scripts/buildsystems/vcpkg.cmake"
    }
  ],

  "buildPresets": [
    { "name": "debug",   "configurePreset": "debug"   },
    { "name": "release", "configurePreset": "release" },
    { "name": "asan",    "configurePreset": "asan"    }
  ],

  "testPresets": [
    {
      "name": "default",
      "configurePreset": "debug",
      "output": { "outputOnFailure": true },
      "execution": { "jobs": 4 }
    }
  ]
}
```

```bash
cmake --list-presets                               # 列出所有可用预设
cmake --preset debug                               # 配置（等价于 cmake -B build/debug -G Ninja ...）
cmake --build --preset debug                       # 构建
ctest --preset default                             # 测试
```

**顶层字段：**

| 字段                     | 说明                                           |
|------------------------|----------------------------------------------|
| `version`              | 格式版本；3 = CMake 3.21+（最低），6 = CMake 3.25+（推荐） |
| `cmakeMinimumRequired` | 要求使用的最低 CMake 版本                             |
| `configurePresets`     | 配置预设列表（对应 `cmake -B ... -G ... -D...`）       |
| `buildPresets`         | 构建预设列表（对应 `cmake --build`）                   |
| `testPresets`          | 测试预设列表（对应 `ctest`）                           |
| `include`              | 引入其他 JSON 文件（模块化拆分预设）                        |

**configurePreset 常用字段：**

| 字段               | 说明                                                               |
|------------------|------------------------------------------------------------------|
| `name`           | 预设唯一标识，命令行用此名称引用                                                 |
| `displayName`    | 显示名称（IDE 展示用）                                                    |
| `hidden`         | `true` = 仅作为基类，不出现在 `--list-presets`                             |
| `inherits`       | 继承另一个预设；字符串或数组（多重继承）                                             |
| `generator`      | 等价于 `-G`                                                         |
| `binaryDir`      | 构建目录；支持变量 `${sourceDir}` / `${presetName}` / `${hostSystemName}` |
| `cacheVariables` | 等价于 `-DVAR=value`，值可以是字符串或 `{"type":"BOOL","value":"ON"}`        |
| `toolchainFile`  | 等价于 `-DCMAKE_TOOLCHAIN_FILE`                                     |
| `environment`    | 设置进程环境变量（仅对该预设有效）                                                |
| `condition`      | 条件表达式，不满足时该预设不可用（CMake 3.22+）                                    |

> **CMakeUserPresets.json**：本地个人预设放在 `CMakeUserPresets.json`（与 `CMakePresets.json` 同目录），应加入 `.gitignore`
> ，避免本地路径提交到仓库。

---

## 3. build 目录文件说明

### 3.1 所有 Generator 通用

| 文件 / 目录                             | 说明                                                                 |
|-------------------------------------|--------------------------------------------------------------------|
| `CMakeFiles/`                       | CMake 内部工作目录，存放配置日志、中间状态、目标中间文件（`.o` / `.obj`）                     |
| `CMakeFiles/CMakeConfigureLog.yaml` | 配置阶段详细日志，记录编译器探测结果、feature 检测等，排查配置失败时看这里                          |
| `CMakeFiles/<version>/`             | CMake 版本号目录（如 `4.2.2/`），存放编译器探测脚本和结果                               |
| `CMakeFiles/CMakeScratch/`          | 配置阶段临时文件，探测完即废弃                                                    |
| `CMakeFiles/pkgRedirects/`          | 包重定向配置（find_package 相关）                                            |
| `CMakeFiles/<target>.dir/`          | 各构建目标的中间文件目录（`.o`、依赖文件等），目标名即 `add_executable` / `add_library` 的名称 |
| `CMakeCache.txt`                    | 所有 cmake 变量的缓存，修改后须重跑 cmake 才生效；删除此文件等同于清空配置缓存                     |
| `cmake_install.cmake`               | `cmake --install` 的安装规则脚本，由 CMakeLists.txt 里的 `install()` 生成       |
| `InstallScripts.json`               | 安装脚本索引                                                             |
| `TargetDirectories.txt`             | 记录所有构建目标的输出目录路径                                                    |

### 3.2 Ninja 方案特有

| 文件                       | 说明                                        |
|--------------------------|-------------------------------------------|
| `build.ninja`            | **主构建文件**，ninja 读取它执行所有构建规则               |
| `CMakeFiles/rules.ninja` | 通用编译规则（响应规则、默认标志等），被 `build.ninja` 包含     |
| `.ninja_deps`            | ninja 依赖数据库，记录每个源文件依赖的头文件，增量构建时判断哪些文件需要重编 |
| `.ninja_log`             | 构建日志，记录每条命令的开始/结束时间，`ninja -t restat` 用它  |

### 3.3 MinGW Makefiles 方案特有

| 文件                                           | 说明                                   |
|----------------------------------------------|--------------------------------------|
| `Makefile`                                   | **主构建文件**，mingw32-make 读取它执行构建       |
| `CMakeFiles/Makefile.cmake`                  | CMake 生成的 Makefile 片段，被主 Makefile 包含 |
| `CMakeFiles/Makefile2`                       | 内部 Makefile，处理目标依赖顺序                 |
| `CMakeFiles/progress.marks`                  | 记录构建进度（用于显示 `[xx%]`）                 |
| `CMakeFiles/CMakeDirectoryInformation.cmake` | 当前目录的路径信息                            |

### 3.4 Ninja Multi-Config 方案特有

| 文件 / 目录                                                                    | 说明                                       |
|----------------------------------------------------------------------------|------------------------------------------|
| `Debug/` / `Release/` / `RelWithDebInfo/`                                  | 各配置的独立输出目录，exe 在对应子目录下                   |
| `build-Debug.ninja` / `build-Release.ninja` / `build-RelWithDebInfo.ninja` | 各配置的入口 ninja 文件，`--config Debug` 时读取对应文件 |
| `CMakeFiles/common.ninja`                                                  | 所有配置共用的编译规则（目标定义、路径等）                    |
| `CMakeFiles/impl-Debug.ninja` 等                                            | 各配置的具体实现（编译标志差异、输出路径等）                   |

### 3.5 MSVC 特有（相比 MinGW）

| 文件 / 目录                       | 说明                                                                      |
|-------------------------------|-------------------------------------------------------------------------|
| `<target>.pdb`                | **调试符号文件**（Program Debug Database），VS 调试器 / WinDbg 读取它，大小通常远超 exe       |
| `<target>.ilk`                | **增量链接文件**（Incremental Link），存储上次链接的中间数据，加速下次链接；Debug 构建才生成             |
| `CMakeFiles/ShowIncludes/`    | 解析 MSVC `/showIncludes` 输出，实现头文件依赖跟踪（GCC 用 `-MF` 生成 `.d` 文件，MSVC 改用此目录） |
| `CMakeFiles/<version>-msvc1/` | MSVC 版本标识目录（如 `4.3.1-msvc1/`）                                           |

### 3.6 VS Generator 特有

| 文件 / 目录                            | 说明                                                 |
|------------------------------------|----------------------------------------------------|
| `<project>.slnx`                   | **VS 解决方案文件**（新格式，VS 2022 17.x+），双击用 VS 打开，包含所有项目  |
| `<target>.vcxproj`                 | 主项目文件，MSBuild 读取它编译对应目标                            |
| `ALL_BUILD.vcxproj`                | 构建所有目标，VS 里 Build Solution 触发的就是它                  |
| `ZERO_CHECK.vcxproj`               | 监控 CMakeLists.txt 变化，变化时自动重新运行 cmake 刷新 `.vcxproj` |
| `*.vcxproj.filters`                | 项目筛选器，控制文件在 VS Solution Explorer 里的分组显示            |
| `*.vcxproj.user`                   | 用户本地配置（调试启动参数、工作目录等），不应提交 git                      |
| `.vs/`                             | VS IDE 本地配置目录，不应提交 git                             |
| `Debug/` / `Release/`              | 各配置的输出目录，exe 在对应子目录下                               |
| `x64/`                             | x64 平台的中间文件目录                                      |
| `CMakeFiles/generate.stamp`        | ZERO_CHECK 用来判断是否需要重新生成 `.vcxproj` 的时间戳            |
| `CMakeFiles/generate.stamp.depend` | 触发重新生成的文件列表（CMakeLists.txt 及所有 `.cmake` 文件）        |
