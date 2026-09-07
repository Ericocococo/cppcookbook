# 04_library — 静态库与动态库

`add_library(STATIC/SHARED)` 构建两种库，`target_link_libraries` 消费

## 1. 静态库 vs 动态库

| 对比项     | 静态库（`.a` / `.lib`） | 动态库（`.so` / `.dll`）  |
|---------|--------------------|----------------------|
| 链接时机    | 编译期直接嵌入可执行文件       | 运行期加载                |
| 运行时依赖   | 无（自包含）             | 需要 `.dll` / `.so` 文件 |
| 可执行文件大小 | 较大                 | 较小                   |
| 更新方式    | 需重新编译链接            | 替换库文件即可（ABI 兼容前提下）   |
| 典型场景    | 发布单一可执行文件          | 插件、共享代码、减小内存占用       |

## 2. 构建与运行

完整六套方案（MinGW 三方案 / MSVC 三方案，含对比与 CLion）见 [章 README § 5 通用构建流程](../README.md)。本目录可执行名有两个——`use_static` 与 `use_shared`，把 `<exe_name>` 逐个换成它们即可（CLion 的 Run/Debug Configurations 下拉中同样会出现这两个目标）。日常最常用——Ninja 方案 A（Git Bash 三步）：

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"
NINJA="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

"$CMAKE" -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="$GXX" -DCMAKE_MAKE_PROGRAM="$NINJA"
"$CMAKE" --build build-mingw-ninja
./build-mingw-ninja/use_static.exe
./build-mingw-ninja/use_shared.exe
```

> 本目录专属（动态库）：运行 `use_shared.exe` 时，其 `.dll` 须与可执行文件位于同一目录或处于系统 `PATH` 中——Ninja 构建后 `.dll` 已与 exe 同目录输出，无需复制。
>
> cmd 版与 MSVC/Linux 版：同样把通用流程里的 exe 名换成 `use_static` / `use_shared` 即可。
