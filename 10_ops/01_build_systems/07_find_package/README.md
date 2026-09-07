# 07_find_package — 查找系统依赖

`Module 模式 vs Config 模式`，可选依赖处理

## 1. Module 模式 vs Config 模式

| 对比项  | Module 模式（`FindXxx.cmake`）                            | Config 模式（`XxxConfig.cmake`）                   |
|------|-------------------------------------------------------|------------------------------------------------|
| 查找文件 | CMake 内置 `FindXxx.cmake` 或 `CMAKE_MODULE_PATH` 中的同名文件 | 库安装时生成的 `XxxConfig.cmake` / `xxx-config.cmake` |
| 适用场景 | 系统库、无 CMake 支持的老库                                     | 现代库（通过 install(EXPORT) 导出）                     |
| 调用示例 | `find_package(ZLIB MODULE)`                           | `find_package(nlohmann_json CONFIG)`           |
| 变量风格 | `ZLIB_INCLUDE_DIRS` / `ZLIB_LIBRARIES`                | Imported Target：`ZLIB::ZLIB`                   |
| 推荐程度 | 兼容旧项目                                                 | **推荐**，与现代 CMake target 理念一致                   |

## 2. 构建与运行

完整六套方案（MinGW 三方案 / MSVC 三方案，含对比与 CLion）见 [章 README § 5 通用构建流程](../README.md)，本目录只需把 `<exe_name>` 换成 `app`。日常最常用——Ninja 方案 A（Git Bash 三步）：

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"
NINJA="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

"$CMAKE" -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="$GXX" -DCMAKE_MAKE_PROGRAM="$NINJA"
"$CMAKE" --build build-mingw-ninja
./build-mingw-ninja/app.exe
```

> 本目录专属：cmake **配置阶段**（`cmake -B` 时）终端会打印 `ZLIB found` 或 `ZLIB NOT found`，对应 CMakeLists.txt 中可选依赖的两条代码路径（CLion 中可在 CMake 工具窗口查看同一份配置日志，含 `find_package` 的完整查找过程）。
>
> cmd 版与 MSVC/Linux 版：同样把通用流程里的 exe 名换成 `app` 即可。
