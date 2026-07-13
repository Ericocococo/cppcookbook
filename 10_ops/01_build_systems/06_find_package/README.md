# 06_find_package — 查找系统依赖

Module 模式 vs Config 模式，可选依赖处理

> 工具完整路径见 [父目录 README](../README.md)

---

## 命令行 · MinGW（Git Bash）

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
GCC="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/gcc.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"

"$CMAKE" -B build-mingw -G Ninja -DCMAKE_C_COMPILER="$GCC" -DCMAKE_CXX_COMPILER="$GXX" && "$CMAKE" --build build-mingw && ./build-mingw/find_package_demo.exe
```

cmake 配置阶段终端会打印 `ZLIB found` 或 `ZLIB NOT found`，对应可选依赖的不同代码路径。

---

## 命令行 · MSVC（cmd）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

call "%VCVARSALL%" x64
"%CMAKE%" -B build-msvc -G Ninja && "%CMAKE%" --build build-msvc && build-msvc\find_package_demo.exe
```

cmake 配置阶段同样打印 ZLIB 查找结果。

---

## CLion IDE

打开 `06_find_package` 目录，CLion 自动配置。在 **CMake 工具窗口**（View → Tool Windows → CMake）可查看完整配置日志，包括 `find_package` 的查找过程和结果。

### Module 模式 vs Config 模式

| 对比项 | Module 模式（`FindXxx.cmake`） | Config 模式（`XxxConfig.cmake`） |
|--------|-------------------------------|--------------------------------|
| 查找文件 | CMake 内置 `FindXxx.cmake` 或 `CMAKE_MODULE_PATH` 中的同名文件 | 库安装时生成的 `XxxConfig.cmake` / `xxx-config.cmake` |
| 适用场景 | 系统库、无 CMake 支持的老库 | 现代库（通过 install(EXPORT) 导出） |
| 调用示例 | `find_package(ZLIB MODULE)` | `find_package(nlohmann_json CONFIG)` |
| 变量风格 | `ZLIB_INCLUDE_DIRS` / `ZLIB_LIBRARIES` | Imported Target：`ZLIB::ZLIB` |
| 推荐程度 | 兼容旧项目 | **推荐**，与现代 CMake target 理念一致 |
