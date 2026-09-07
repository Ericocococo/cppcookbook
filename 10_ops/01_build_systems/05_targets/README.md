# 05_targets — 现代 target 属性传播

`PRIVATE / PUBLIC / INTERFACE` 精确控制属性传播范围

## 1. PRIVATE / PUBLIC / INTERFACE 对比

| 关键字         | 影响本目标 | 传播给消费者 | 典型用途                   |
|-------------|:-----:|:------:|------------------------|
| `PRIVATE`   |   ✓   |   ✗    | 内部实现细节（不泄漏接口）          |
| `PUBLIC`    |   ✓   |   ✓    | 库同时自用且对外暴露的接口          |
| `INTERFACE` |   ✗   |   ✓    | 纯头文件库 / header-only 接口 |

> 本目录 CMakeLists.txt 里的 `$<$<CXX_COMPILER_ID:MSVC>:...>` 是**生成器表达式**（MSVC 编译时用 `/W4`，其他编译器用 `-Wall`）。这里先理解用途即可，完整语法在 06_compile_options（首次进阶）与 11_generator_expr（专章）展开。

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

> 本目录专属：构建完成后，可在 Build 输出窗口查看实际编译命令，验证 `-I`（include 路径）是否只按预期向被链接的消费者传播。
>
> cmd 版与 MSVC/Linux 版：同样把通用流程里的 exe 名换成 `app` 即可。
