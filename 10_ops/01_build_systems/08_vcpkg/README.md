# 08_vcpkg — vcpkg 包管理集成

`vcpkg.json` 声明依赖，cmake 配置阶段自动安装

## 1. 手动安装依赖

如需单独预装包（无需依赖 cmake 配置触发），可在终端执行：

```bash
# MinGW triplet
D:/software/vcpkg/vcpkg.exe install nlohmann-json:x64-mingw-dynamic fmt:x64-mingw-dynamic

# MSVC triplet
D:/software/vcpkg/vcpkg.exe install nlohmann-json:x64-windows fmt:x64-windows
```

| triplet              | 适用编译器     | 库类型     |
|----------------------|-----------|---------|
| `x64-mingw-dynamic`  | MinGW/GCC | 动态库     |
| `x64-mingw-static`   | MinGW/GCC | 静态库     |
| `x64-windows`        | MSVC      | 动态库（默认） |
| `x64-windows-static` | MSVC      | 静态库     |

## 2. 构建与运行

完整六套方案（MinGW 三方案 / MSVC 三方案，含对比与 CLion）见 [章 README § 5 通用构建流程](../README.md)，本目录把 `<exe_name>` 换成 `app`，并在每条**配置命令**上追加 toolchain 参数（见下方专属说明）。日常最常用——Ninja 方案 A（Git Bash 三步）：

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"
NINJA="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

"$CMAKE" -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="$GXX" -DCMAKE_MAKE_PROGRAM="$NINJA" -DCMAKE_TOOLCHAIN_FILE=D:/software/vcpkg/scripts/buildsystems/vcpkg.cmake
"$CMAKE" --build build-mingw-ninja
./build-mingw-ninja/app.exe
```

> 本目录专属（vcpkg 工具链注入）：六套方案的**每次 cmake 配置都必须追加**
> `-DCMAKE_TOOLCHAIN_FILE=D:/software/vcpkg/scripts/buildsystems/vcpkg.cmake`，cmake 由此进入 vcpkg 模式，按 `vcpkg.json`
> 自动安装依赖（第一次配置即触发，耗时取决于网络和编译速度）。CLion 中把该参数填入 **Settings → Build, Execution,
> Deployment → CMake → CMake options**，点击 OK 后自动重新配置并安装。
>
> cmd 版与 MSVC/Linux 版：同样把通用流程里的 exe 名换成 `app`，并追加上面那条 toolchain 参数即可。
