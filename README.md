# cppcookbook

存放 C++ 语法与实战代码的仓库。每个示例均可独立编译运行。

## 目录导航

| 目录 | 内容 |
|------|------|
| [01_language/](01_language/) | C++ 语言本身：基础 → OOP → 模板 → 现代特性 → 内存 → 专家 |
| [02_algorithms/](02_algorithms/) | 数据结构 / 经典算法 / 设计模式（GoF in Modern C++） |
| [03_stl/](03_stl/) | 标准模板库：容器 / 算法 / 迭代器 / ranges / IO / 并发… |
| [04_libraries/](04_libraries/) | 通用三方库：网络 / 序列化 / 日志 / 数学 / 测试… |
| [05_frameworks/](05_frameworks/) | 领域级完整框架：量化（CTP/vnpy）/ 游戏引擎（flecs/entt）|
| [06_extending/](06_extending/) | 互操作：C ABI / pybind11 / Lua / WebAssembly / JNI |
| [07_systems/](07_systems/) | 系统编程：网络 / IPC / POSIX / 嵌入式 / 高性能服务 |
| [08_hpc/](08_hpc/) | 高性能计算：SIMD / OpenMP / TBB / CUDA / 数值优化 |
| [09_domain/](09_domain/) | 领域应用：游戏 / 图形 / AI 推理 / 音视频 / 量化 / 机器人 |
| [10_ops/](10_ops/) | 工程化：CMake / 调试 / 分析 / 包管理 / CI |

详细目录规划见 [STRUCTURE.md](STRUCTURE.md)，编码习惯见 [CONVENTIONS.md](CONVENTIONS.md)。

## 开发环境

- **OS**：Windows 11 x64
- **IDE**：CLion 2026.0.1
- **Windows SDK**：10.0.26100.0（`D:\Windows Kits\10\`）

---

### 工具链 A — MinGW（CLion 默认）

GCC + GDB 组合，跨平台友好，适合学习语言特性和标准库。
编译器目标三元组：`x86_64-w64-mingw32`。

| 工具 | 完整路径 | 版本 |
|------|----------|------|
| C/C++ 编译器 | `D:\ProgramData\JetBrains\CLion20260101\bin\mingw\bin\g++.exe` | GCC 13.1.0 |
| 调试器 | `D:\ProgramData\JetBrains\CLion20260101\bin\gdb\win\x64\bin\gdb.exe` | GDB 16.3（JetBrains 定制构建 243）|

---

### 工具链 B — MSVC

cl.exe + LLDB 组合，生成原生 Windows COFF 格式目标文件，适合调试 Windows API、测试 ABI 兼容性。
工具集版本号 `14.51`，对应 VS 2022 17.x。

| 工具 | 完整路径 | 版本 |
|------|----------|------|
| C/C++ 编译器 | `D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.51.36231\bin\Hostx64\x64\cl.exe` | 19.51.36247（Hostx64 → Targetx64）|
| 调试器 | `D:\ProgramData\JetBrains\CLion20260101\bin\lldb\win\x64\bin\lldb.exe` | LLDB 9.0（JetBrains 定制构建 445）|

> MSVC 的 cl.exe 依赖 `vcvarsall.bat` 设置的环境变量（头文件路径、库路径、SDK 路径），
> 不能脱离 Visual Studio 环境单独调用。CLion 在启动构建时会自动注入这些变量，无需手动操作。

---

### 独立工具：CMake 与 Ninja

CMake 是构建系统生成器，Ninja 是构建执行器，两者与编译器无关，理论上装一份即可全局使用。
CLion 和 Visual Studio 各自捆绑了副本，版本略有差异；CLion 配置中选"已捆绑"时各用各的。

| 工具 | 来源 | 完整路径 | 版本 |
|------|------|----------|------|
| CMake | CLion 捆绑 | `D:\ProgramData\JetBrains\CLion20260101\bin\cmake\win\x64\bin\cmake.exe` | 4.2.2 |
| CMake | VS 捆绑 | `D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe` | 4.3.1-msvc1 |
| Ninja | CLion 捆绑 | `D:\ProgramData\JetBrains\CLion20260101\bin\ninja\win\x64\ninja.exe` | 1.13.2 |
| Ninja | VS 捆绑 | `D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe` | 1.13.2 |

---

### 辅助工具

| 工具 | 用途 | 完整路径 | 版本 |
|------|------|----------|------|
| vcpkg | C++ 包管理器，安装三方库（相当于 pip） | `D:\software\vcpkg\vcpkg.exe` | 2026-05-27 |
| clang-format | 代码格式化，按 `.clang-format` 统一风格 | `D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\Llvm\x64\bin\clang-format.exe` | 22.1.3 |
| clang-tidy | 静态分析，按 `.clang-tidy` 检查代码问题 | `D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\Llvm\x64\bin\clang-tidy.exe` | 22.1.3 |
| Git | 版本控制 | `D:\Program Files\Git\bin\git.exe` | 2.54.0 |

> clang-format / clang-tidy 不在系统 PATH，命令行调用需使用上表完整路径，
> 或将 `D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\Llvm\x64\bin` 加入 PATH。
> CLion 中可在 `设置 → 编辑器 → 代码样式` 和 `设置 → 语言 & 框架 → C/C++` 配置路径后直接使用。

---

### 如何切换工具链

CLion 中切换：`设置 → 构建、执行、部署 → 工具链`，将目标工具链拖到顶部设为默认，
或在 `CMake` 配置页为每个 Profile（Debug/Release）单独指定工具链。
