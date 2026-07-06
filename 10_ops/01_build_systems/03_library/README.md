# 03_library — 静态库与动态库

`add_library(STATIC/SHARED)` 构建两种库，`target_link_libraries` 消费

> 工具完整路径见 [父目录 README](../README.md)

---

## 命令行 · MinGW（Git Bash）

```bash
CMAKE="D:/Program Files/Microsoft Visual Studio/18/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe"
GCC="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/gcc.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"

"$CMAKE" -B build-mingw -G Ninja -DCMAKE_C_COMPILER="$GCC" -DCMAKE_CXX_COMPILER="$GXX" && "$CMAKE" --build build-mingw
./build-mingw/use_static.exe
./build-mingw/use_shared.exe
```

---

## 命令行 · MSVC（cmd）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

call "%VCVARSALL%" x64
"%CMAKE%" -B build-msvc -G Ninja && "%CMAKE%" --build build-msvc
build-msvc\use_static.exe
build-msvc\use_shared.exe
```

> **注意（动态库）**：运行 `use_shared.exe` 时，对应的 `.dll` 文件须与可执行文件位于同一目录或处于系统 `PATH` 中。Ninja 构建后 `.dll` 通常已输出到 `build-msvc\`，无需额外复制。

---

## CLion IDE

打开 `03_library` 目录，CLion 自动加载 `CMakeLists.txt`。工具栏 **Run/Debug Configurations** 下拉列表中会出现两个目标：

- `use_static` — 链接静态库
- `use_shared` — 链接动态库

分别选中后点击运行按钮即可。

### 静态库 vs 动态库

| 对比项 | 静态库（`.a` / `.lib`） | 动态库（`.so` / `.dll`） |
|--------|------------------------|------------------------|
| 链接时机 | 编译期直接嵌入可执行文件 | 运行期加载 |
| 运行时依赖 | 无（自包含） | 需要 `.dll` / `.so` 文件 |
| 可执行文件大小 | 较大 | 较小 |
| 更新方式 | 需重新编译链接 | 替换库文件即可（ABI 兼容前提下） |
| 典型场景 | 发布单一可执行文件 | 插件、共享代码、减小内存占用 |
