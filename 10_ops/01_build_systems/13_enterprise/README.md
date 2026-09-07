# 13_enterprise — 企业级多模块项目

`project_options` 接口库统一选项，`add_subdirectory` 管理依赖链，ctest 集成测试

## 1. 目录结构

```
13_enterprise/
├── CMakeLists.txt          # 顶层：定义 project_options，add_subdirectory
├── utils/
│   ├── CMakeLists.txt
│   ├── include/utils/
│   │   └── string_utils.h  # 头文件按 include/<模块>/ 组织
│   └── string_utils.cpp
├── core/
│   ├── CMakeLists.txt      # 依赖 utils
│   ├── include/core/
│   │   └── engine.h
│   └── engine.cpp
├── app/
│   ├── CMakeLists.txt      # 依赖 core（PUBLIC 传播自动拉入 utils）
│   └── main.cpp
└── tests/
    ├── CMakeLists.txt
    ├── test_utils.cpp
    └── test_engine.cpp
```

## 2. 架构要点

| 要点                    | 说明                                                                                                                        |
|-----------------------|---------------------------------------------------------------------------------------------------------------------------|
| `project_options` 接口库 | 以 `INTERFACE` 属性集中声明警告、标准、ASan 等选项，所有子目标 `target_link_libraries(... project_options)` 继承，避免 `target_compile_options` 全局污染 |
| `PUBLIC` 传播依赖链        | `core` 以 `PUBLIC` 链接 `utils`，`app` 链接 `core` 后自动获得 `utils` 的头文件和链接，无需手动重复声明                                               |
| 按模块拆分测试               | `tests/` 下各测试目标只链接被测模块，隔离性好，CTest 可按 `-L` 标签并行运行                                                                          |

## 3. 本目录专属命令（多模块构建 / 测试 / ASan）

- **一次配置构建全部模块**：顶层 `add_subdirectory(utils) → add_subdirectory(core) → add_subdirectory(app) → add_subdirectory(tests)`，顺序即依赖顺序；配置、构建都只需在顶层目录操作，命令本身与单模块工程相同，但产物按模块分子目录存放——exe 在 `build-xxx/app/enterprise_app.exe`（不是 build 根目录）。
- **ctest 照常可用**：`enable_testing()` 在顶层开启，`tests/` 子目录注册 `engine`、`utils` 两组测试；运行命令与 10_ctest 相同：先 `cmake --build`，再 `ctest --test-dir <build 目录> --output-on-failure`。
- **ASan 开关在顶层**：`option(ENABLE_ASAN)` 打开后，`project_options` 接口库统一携带 `-fsanitize=address`（MSVC 为 `/fsanitize=address`），链接了它的所有目标一并生效；建议用独立 build 目录（如 `build-mingw-ninja-asan`）隔离普通 Debug 产物。

## 4. 构建与运行

完整六套方案（MinGW 三方案 / MSVC 三方案，含对比与 CLion）见 [章 README § 5 通用构建流程](../README.md)。本目录 exe 名 `enterprise_app`（在 `app` 子模块，产物多一层 `app/` 前缀）；CLion 打开顶层目录会递归加载子目录，Run/Debug Configurations 中列出 `enterprise_app`、`test_engine`、`test_utils`。日常最常用——Ninja 方案 A（Git Bash）：

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"
NINJA="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

# Debug：配置、构建、运行主程序、跑全部测试
"$CMAKE" -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="$GXX" -DCMAKE_MAKE_PROGRAM="$NINJA" -DCMAKE_BUILD_TYPE=Debug
"$CMAKE" --build build-mingw-ninja
./build-mingw-ninja/app/enterprise_app.exe
ctest --test-dir build-mingw-ninja --output-on-failure

# ASan 变体（内存错误检测）：多一个 -DENABLE_ASAN=ON，独立 build 目录
"$CMAKE" -B build-mingw-ninja-asan -G Ninja -DCMAKE_CXX_COMPILER="$GXX" -DCMAKE_MAKE_PROGRAM="$NINJA" -DCMAKE_BUILD_TYPE=Debug -DENABLE_ASAN=ON
"$CMAKE" --build build-mingw-ninja-asan
./build-mingw-ninja-asan/app/enterprise_app.exe
```

> cmd 版与 MSVC/Linux 版：按通用流程构建，把 exe 名换成 `enterprise_app` 并注意路径多一层 `app\`；执行测试用 `ctest --test-dir <build 目录> --output-on-failure`；ASan 变体在配置命令后追加 `-DENABLE_ASAN=ON`（CLion 则在 Profile 的 **CMake options** 里加）。CLion 执行全部测试：**Run → Run 'All CTest'**。
