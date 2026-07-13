# 12_enterprise — 企业级多模块项目

project_options 接口库统一选项，add_subdirectory 管理依赖链，ctest 集成测试

> 工具完整路径见 [父目录 README](../README.md)

---

## 命令行 · MinGW（Git Bash）

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
GCC="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/gcc.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"

# Debug 构建
"$CMAKE" -B build-mingw -G Ninja -DCMAKE_C_COMPILER="$GCC" -DCMAKE_CXX_COMPILER="$GXX" -DCMAKE_BUILD_TYPE=Debug && "$CMAKE" --build build-mingw

# 运行主程序
./build-mingw/app/enterprise_app.exe

# 运行 CTest
ctest --test-dir build-mingw --output-on-failure

# 开启 ASan（内存错误检测）
"$CMAKE" -B build-mingw-asan -G Ninja -DCMAKE_C_COMPILER="$GCC" -DCMAKE_CXX_COMPILER="$GXX" -DCMAKE_BUILD_TYPE=Debug -DENABLE_ASAN=ON && "$CMAKE" --build build-mingw-asan && ./build-mingw-asan/app/enterprise_app.exe
```

---

## 命令行 · MSVC（cmd）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

call "%VCVARSALL%" x64

rem Debug 构建
"%CMAKE%" -B build-msvc -G Ninja -DCMAKE_BUILD_TYPE=Debug && "%CMAKE%" --build build-msvc

rem 运行主程序
build-msvc\app\enterprise_app.exe

rem 运行 CTest
ctest --test-dir build-msvc --output-on-failure

rem 开启 ASan
"%CMAKE%" -B build-msvc-asan -G Ninja -DCMAKE_BUILD_TYPE=Debug -DENABLE_ASAN=ON && "%CMAKE%" --build build-msvc-asan && build-msvc-asan\app\enterprise_app.exe
```

---

## CLion IDE

打开 `12_enterprise` 根目录，CLion 递归加载所有子目录的 `CMakeLists.txt`。**Run/Debug Configurations** 下拉列表将列出：

- `enterprise_app` — 主程序
- `test_engine` — 核心模块单元测试
- `test_utils` — 工具模块单元测试

执行所有测试：菜单 **Run → Run 'All CTest'**，Test Runner 窗口显示各模块测试结果。

开启 ASan：在 Profile 的 **CMake options** 里加 `-DENABLE_ASAN=ON`，重新配置后运行即可。

### 目录结构

```
12_enterprise/
├── CMakeLists.txt          # 顶层：定义 project_options，add_subdirectory
├── utils/
│   ├── CMakeLists.txt
│   ├── include/utils/
│   └── src/
├── core/
│   ├── CMakeLists.txt      # 依赖 utils
│   ├── include/core/
│   └── src/
├── app/
│   ├── CMakeLists.txt      # 依赖 core（PUBLIC 传播自动拉入 utils）
│   └── main.cpp
└── tests/
    ├── CMakeLists.txt
    ├── test_utils.cpp
    └── test_engine.cpp
```

### 架构要点

| 要点 | 说明 |
|------|------|
| `project_options` 接口库 | 以 `INTERFACE` 属性集中声明警告、标准、ASan 等选项，所有子目标 `target_link_libraries(... project_options)` 继承，避免 `target_compile_options` 全局污染 |
| `PUBLIC` 传播依赖链 | `core` 以 `PUBLIC` 链接 `utils`，`app` 链接 `core` 后自动获得 `utils` 的头文件和链接，无需手动重复声明 |
| 按模块拆分测试 | `tests/` 下各测试目标只链接被测模块，隔离性好，CTest 可按 `-L` 标签并行运行 |
