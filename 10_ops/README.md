# 10_ops — 工程化

横切所有模块的工程活动：构建系统、测试、静态分析、包管理、CI/CD、调试、性能分析。

| 目录                                             | 内容                                                                 | 状态 |
|------------------------------------------------|----------------------------------------------------------------------|------|
| [01_build_systems](01_build_systems/README.md) | 构建系统演进：手动编译 → Makefile → CMake → vcpkg → 企业级（14 个知识目录） | ✅ 建成 |
| 02_testing                                     | 代码覆盖率、模糊测试（libFuzzer/AFL）；ctest 集成已在 01_build_systems 教学，不重复 | ⏳ 空目录 |
| [03_lint_format](03_lint_format/README.md)    | clang-tidy/clang-format/cppcheck、编译警告治理                             | 📄 规划中 |
| 04_packaging                                   | vcpkg/conan/CPM/ExternalProject/FetchContent（vcpkg 使用见 01_build_systems） | ⏳ 空目录 |
| 05_ci_cd                                       | GitHub Actions/Jenkins C++ CI 流水线                                  | ⏳ 空目录 |
| 06_debugging                                   | GDB/LLDB/AddressSanitizer/UBSan/Valgrind/rr                          | ⏳ 空目录 |
| 07_profiling                                   | perf/VTune/Tracy/Instruments/gprof                                 | ⏳ 空目录 |
| 08_cross_compile                               | 交叉编译/嵌入式工具链/sysroot/QEMU 仿真                                     | ⏳ 空目录 |

> 状态图例：✅ 建成 · 📄 规划文档已建 · ⏳ 空目录待填充。本章很多流程知识（ctest、vcpkg、presets、生成器表达式）已并入 `01_build_systems` 各知识目录，其他章不重复展开，只放"独立工程流程"。

## 术语速查（本章高频）

| 术语 | 全称 / 含义 |
|------|------------|
| generator | CMake 生成器：Ninja / MinGW Makefiles / "Visual Studio 18 2026" |
| toolchain | 工具链：编译器 + 链接器 + 库的组合（MinGW / MSVC） |
| triplet | vcpkg 目标三元组，如 `x64-windows`（架构-平台） |
| preset | CMakePresets.json 里预设的配置/构建参数组 |
| vcvarsall | VS 环境脚本：注入 cl.exe 与 INCLUDE/LIB 环境变量 |
| CMake 语言 | CMakeLists.txt 的脚本语法（详见 01_build_systems/cmake_syntax.md） |
| ASan / UBSan / TSan | Address/UndefinedBehavior/Thread Sanitizer — 内存/UB/数据竞争检测工具 |
| CI/CD | Continuous Integration / Continuous Deployment — 持续集成/持续部署 |
