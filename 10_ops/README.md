# 10_ops — 工程化

横切所有模块的工程活动：构建系统、测试、静态分析、包管理、CI/CD、调试、性能分析。

| 目录 | 内容 |
|------|------|
| [01_build_systems](01_build_systems/README.md) | 构建系统演进：手动编译 → Makefile → CMake → vcpkg → 企业级 |
| 02_testing | ctest 集成、代码覆盖率、模糊测试（libFuzzer/AFL）|
| 03_lint_format | clang-tidy/clang-format/cppcheck、编译警告治理 |
| 04_packaging | vcpkg/conan/CPM/ExternalProject/FetchContent |
| 05_ci_cd | GitHub Actions/Jenkins C++ CI 流水线 |
| 06_debugging | GDB/LLDB/AddressSanitizer/UBSan/Valgrind/rr |
| 07_profiling | perf/VTune/Tracy/Instruments/gprof |
| 08_cross_compile | 交叉编译/嵌入式工具链/sysroot/QEMU 仿真 |

## 术语速查

| 术语 | 全称 / 含义 |
|------|-------------|
| ASan | AddressSanitizer — 内存错误检测工具，检测越界/UAF/double-free |
| UBSan | UndefinedBehaviorSanitizer — 未定义行为检测工具 |
| TSan | ThreadSanitizer — 数据竞争检测工具 |
| LTO | Link-Time Optimization — 链接期优化，跨编译单元内联 |
| IPO | Interprocedural Optimization — 过程间优化，同 LTO |
| PGO | Profile-Guided Optimization — 基于运行时 profile 的优化 |
| CI/CD | Continuous Integration / Continuous Deployment — 持续集成/持续部署 |
