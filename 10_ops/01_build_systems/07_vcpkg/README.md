# 07_vcpkg — vcpkg 包管理集成

`vcpkg.json` 声明依赖，cmake 配置阶段自动安装

> 工具完整路径见 [父目录 README](../README.md)

---

## 命令行 · MinGW（Git Bash）

```bash
CMAKE="D:/Program Files/Microsoft Visual Studio/18/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe"
GCC="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/gcc.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"

"$CMAKE" -B build-mingw -G Ninja \
  -DCMAKE_C_COMPILER="$GCC" \
  -DCMAKE_CXX_COMPILER="$GXX" \
  -DCMAKE_TOOLCHAIN_FILE=D:/software/vcpkg/scripts/buildsystems/vcpkg.cmake \
  && "$CMAKE" --build build-mingw && ./build-mingw/vcpkg_demo.exe
```

第一次运行 cmake 配置时，vcpkg 自动按 `vcpkg.json` 安装 `nlohmann-json` 和 `fmt`，耗时取决于网络和编译速度。

---

## 命令行 · MSVC（cmd）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

call "%VCVARSALL%" x64
"%CMAKE%" -B build-msvc -G Ninja -DCMAKE_TOOLCHAIN_FILE=D:\software\vcpkg\scripts\buildsystems\vcpkg.cmake && "%CMAKE%" --build build-msvc && build-msvc\vcpkg_demo.exe
```

---

## CLion IDE

1. **Settings → Build, Execution, Deployment → CMake**，选中当前 Profile。
2. 在 **CMake options** 中填入：

```
-DCMAKE_TOOLCHAIN_FILE=D:/software/vcpkg/scripts/buildsystems/vcpkg.cmake
```

3. 点击 **OK** 后 CLion 自动重新配置，vcpkg 开始安装依赖。配置完成后正常 Run 即可。

### 手动安装依赖

如需单独预装包（无需依赖 cmake 配置触发），可在终端执行：

```bash
# MinGW triplet
D:/software/vcpkg/vcpkg.exe install nlohmann-json:x64-mingw-dynamic fmt:x64-mingw-dynamic

# MSVC triplet
D:/software/vcpkg/vcpkg.exe install nlohmann-json:x64-windows fmt:x64-windows
```

| triplet | 适用编译器 | 库类型 |
|---------|-----------|--------|
| `x64-mingw-dynamic` | MinGW/GCC | 动态库 |
| `x64-mingw-static` | MinGW/GCC | 静态库 |
| `x64-windows` | MSVC | 动态库（默认） |
| `x64-windows-static` | MSVC | 静态库 |
