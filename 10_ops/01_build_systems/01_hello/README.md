# 01_hello — 最简 CMake 工程

`cmake_minimum_required` + `project` + `add_executable` 三行完成一个可运行工程。

> 工具完整路径见 [父目录 README](../README.md)

---

## 命令行 · MinGW（Git Bash）

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
GCC="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/gcc.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"

# 配置
"$CMAKE" -B build-mingw -G Ninja \
  -DCMAKE_C_COMPILER="$GCC" \
  -DCMAKE_CXX_COMPILER="$GXX"

# 构建
"$CMAKE" --build build-mingw

# 运行
./build-mingw/hello_cmake.exe
```

---

## 命令行 · MSVC（cmd）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

:: 激活 MSVC 环境（cl.exe 必须）
call "%VCVARSALL%" x64

:: 配置（激活后 cmake 自动检测到 cl.exe）
"%CMAKE%" -B build-msvc -G Ninja

:: 构建
"%CMAKE%" --build build-msvc

:: 运行
build-msvc\hello_cmake.exe
```

---

## CLion IDE

1. `File → Open` 选择 `01_hello/` 目录
2. CLion 自动识别 `CMakeLists.txt`，右下角点击 **加载**
3. 工具栏选择工具链（MinGW 或 Visual Studio）和构建类型
4. **构建** `Ctrl+F9`　**运行** `Shift+F10`
