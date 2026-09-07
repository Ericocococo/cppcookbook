# 12_presets — CMakePresets.json

预设文件管理多套构建配置，一个命令切换

## 1. 三个预设（debug / release / asan）

`CMakePresets.json` 用隐藏的 `base` 预设固定 `"generator": "Ninja"` 和 `"binaryDir": "${sourceDir}/build/${presetName}"`，其余 preset 继承它并只覆盖 `cacheVariables`；配套 `buildPresets` 让 `cmake --build --preset` 也能按名字选中同配置。因此使用 preset 时不用手动传 `-G`、`-B`、`-DCMAKE_BUILD_TYPE`。

|                                    | debug 预设      | release 预设      | asan 预设      |
|------------------------------------|---------------|-----------------|--------------|
| build 目录                           | `build/debug` | `build/release` | `build/asan` |
| CMAKE_BUILD_TYPE                   | Debug         | Release         | Debug        |
| CMAKE_INTERPROCEDURAL_OPTIMIZATION | 否             | ON（LTO）         | 否            |
| ENABLE_ASAN                        | 否             | 否               | ON           |

## 2. 本目录专属命令（preset 流程）

- `cmake --list-presets`：列出可用的 configurePreset。
- `cmake --preset <名字>`：按预设配置到 `build/<名字>/`，等于把预设展开成 `-G Ninja -B build/<名字> -DCMAKE_BUILD_TYPE=...` 等参数；切换配置只需换 preset 名，无需记参数。
- `cmake --build --preset <名字>`：构建对应 preset（名字与 configurePreset 同名）。

两点注意：

1. 预设只简化参数，**不负责激活编译器**：generator 固定为 Ninja（单配置），MSVC 下须先按章 README §5.2 激活工具链（vcvarsall / Native Tools），VS Generator 方案在 preset 里不适用。
2. `asan` 预设把 `ENABLE_ASAN` 打开（main.cpp 里按它挂 AddressSanitizer 编译选项），运行后若程序存在内存错误（如堆越界），ASan 会在控制台打印红色 `ERROR: AddressSanitizer` 报告并精确定位到出错行；MSVC 下需 VS Installer 里装 **C++ AddressSanitizer** 组件（VS 2019 16.9+，Individual Components → C++ AddressSanitizer）。

## 3. 构建与运行

本目录用 preset 代替手动传参，构建命令不再是章 README §5 的 `-B/-G` 形式（但工具链激活、CLion 用法仍见 [章 README § 5 通用构建流程](../README.md)）。exe 名 `app`，产物在 `build/<preset 名>/` 下。日常最常用——Debug 预设（Git Bash，cmd 同理）：

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"

"$CMAKE" --preset debug
"$CMAKE" --build --preset debug
./build/debug/app.exe
```

换 Release / ASan 验证：把上面 preset 名换成 `release` / `asan` 重跑即可，三者产物互不干扰（`build/release/app.exe`、`build/asan/app.exe`）；用 `"$CMAKE" --list-presets` 查看可用名字。

CLion 中导入：**Settings → Build, Execution, Deployment → CMake** 点 `+` → **From CMakePresets.json**，导入后每个 preset 对应一个 Profile，工具栏直接切换；选 `asan` Profile 运行即可看 ASan 报告。

## 4. CMakePresets.json 与直接写 -D 参数的区别

| 对比项    | CMakePresets.json                | 命令行 -D 参数   |
|--------|----------------------------------|-------------|
| 配置共享   | 提交到 git，团队共享                     | 每人本地命令，不统一  |
| 切换配置   | `cmake --preset debug` 一条命令      | 需记住所有参数     |
| IDE 集成 | CLion / VS Code / VS 自动识别        | 需手动配置每个 IDE |
| 个人本地配置 | CMakeUserPresets.json（gitignore） | 本地脚本        |
