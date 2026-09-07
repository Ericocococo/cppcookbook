# 10_ctest — CTest 单元测试集成

`enable_testing()` + `add_test()` 接入 CTest

## 1. 为什么需要 ctest？

构建完会生成被测静态库 calc 和两个测试程序 exe，ctest 统一调度测试程序：

```
build/
├── test_basic.exe      ← 注册名 basic，add_test(NAME basic COMMAND test_basic)
└── test_edge.exe       ← 注册名 edge_cases，带 LABELS "edge"、TIMEOUT 5
```

`ctest` 的价值：统一运行所有测试、统计通过/失败数、CI 里自动拦截失败。**随手验证直接运行 `test_basic.exe` 即可，ctest 是工程化/CI 阶段才用的。**

## 2. 本目录专属命令（ctest 流程）

- 测试在**配置阶段注册**（`enable_testing()` + `add_test()`），运行前必须先 `cmake --build` 成功——ctest 只负责运行，不会触发编译。
- `ctest --test-dir <build 目录> --output-on-failure`：`--test-dir` 指向 build 目录（测试清单 `CTestTestfile.cmake` 在配置时生成于此）；`--output-on-failure` 让失败的测试打印自己的输出，否则只看得到 Failed 摘要，无法排查原因。
- **标签筛选**：`set_tests_properties(... LABELS "edge")` 给测试分组后，`-L <标签>` 只运行带该标签的测试（本目录即 `-L edge`），`-LE <标签>` 排除该标签。`TIMEOUT 5` 是单测超时上限，超过 5 秒判失败。
- 多配置构建目录（Ninja Multi-Config / VS Generator）要加 `-C Debug` / `-C Release` 选配置，否则找不到对应配置的测试程序。

## 3. 构建与运行

完整六套方案（MinGW 三方案 / MSVC 三方案，含对比与 CLion）见 [章 README § 5 通用构建流程](../README.md)。本目录构建出的程序是测试程序而非普通 app，构建后以 ctest 验证（顺手单测可直接跑 `test_basic.exe`）。日常最常用——Ninja 方案 A（Git Bash）：

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"
NINJA="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

"$CMAKE" -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="$GXX" -DCMAKE_MAKE_PROGRAM="$NINJA"
"$CMAKE" --build build-mingw-ninja

# 运行全部测试
ctest --test-dir build-mingw-ninja --output-on-failure

# 只运行 edge 标签的测试
ctest --test-dir build-mingw-ninja --output-on-failure -L edge
```

> cmd 版与 MSVC/Linux 版：按通用流程构建后，同样执行 `ctest --test-dir <build 目录> --output-on-failure`（MSVC 的 Ninja/VS Generator 目录视配置加 `-C Debug`）即可。

CLion 中：**Run → Run 'All CTest'** 在 Test Runner 窗口看每条用例的通过/失败状态，**Run → Rerun Failed Tests** 只重跑失败用例；`LABELS` 属性可在 CLion 测试筛选器中按标签过滤。
