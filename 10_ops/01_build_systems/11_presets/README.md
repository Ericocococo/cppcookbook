# 11_presets — CMakePresets.json

预设文件管理多套构建配置，一个命令切换

> 工具完整路径见 [父目录 README](../README.md)

---

## 命令行 · MinGW（Git Bash）

```bash
CMAKE="D:/Program Files/Microsoft Visual Studio/18/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe"
GCC="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/gcc.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"

# 列出所有可用预设
"$CMAKE" --list-presets

# Debug 预设
"$CMAKE" --preset debug && "$CMAKE" --build --preset debug

# Release 预设
"$CMAKE" --preset release && "$CMAKE" --build --preset release

# ASan 预设（AddressSanitizer，检测内存错误）
"$CMAKE" --preset asan && "$CMAKE" --build --preset asan && ./build/asan/presets_demo.exe
```

---

## 命令行 · MSVC（cmd）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

call "%VCVARSALL%" x64

rem 列出所有可用预设
"%CMAKE%" --list-presets

rem Debug 预设
"%CMAKE%" --preset debug && "%CMAKE%" --build --preset debug

rem Release 预设
"%CMAKE%" --preset release && "%CMAKE%" --build --preset release

rem ASan 预设（MSVC 支持 /fsanitize=address，需 VS 2019 16.9+）
"%CMAKE%" --preset asan && "%CMAKE%" --build --preset asan && build\asan\presets_demo.exe
```

---

## CLion IDE

1. **Settings → Build, Execution, Deployment → CMake**，点击 `+` → **From CMakePresets.json**（CLion 检测到 `CMakePresets.json` 后也会自动提示导入）。
2. 导入后每个 `configurePreset` 对应一个 CMake Profile，可在工具栏直接切换。
3. 选择 `asan` Profile 后运行：若程序存在内存错误，ASan 运行时会在控制台打印红色 `ERROR: AddressSanitizer` 报告，精确定位到出错行。

> **MSVC ASan 注意**：需在 VS Installer 中安装 **C++ AddressSanitizer** 组件（Individual Components → C++ AddressSanitizer）。
