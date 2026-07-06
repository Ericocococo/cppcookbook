# 09_ctest — CTest 单元测试集成

`enable_testing()` + `add_test()` 接入 CTest

> 工具完整路径见 [父目录 README](../README.md)

---

## 命令行 · MinGW（Git Bash）

```bash
CMAKE="D:/Program Files/Microsoft Visual Studio/18/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe"
GCC="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/gcc.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"

"$CMAKE" -B build-mingw -G Ninja -DCMAKE_C_COMPILER="$GCC" -DCMAKE_CXX_COMPILER="$GXX" && "$CMAKE" --build build-mingw

# 运行所有测试
ctest --test-dir build-mingw --output-on-failure

# 只运行 edge 标签的测试
ctest --test-dir build-mingw --output-on-failure -L edge
```

---

## 命令行 · MSVC（cmd）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

call "%VCVARSALL%" x64
"%CMAKE%" -B build-msvc -G Ninja && "%CMAKE%" --build build-msvc

rem 运行所有测试
ctest --test-dir build-msvc --output-on-failure

rem 只运行 edge 标签的测试
ctest --test-dir build-msvc --output-on-failure -L edge
```

---

## CLion IDE

CLion 原生集成 CTest：

1. 菜单 **Run → Run 'All CTest'**（或工具栏 Run Configurations 下拉选择 `All CTest`）。
2. 测试执行后在 **Test Runner 窗口**显示每条测试的通过 / 失败状态。
3. 失败的测试会展开输出，便于定位原因。
4. **Run → Rerun Failed Tests** 只重跑失败用例。

> 提示：在 `add_test()` 后通过 `set_tests_properties` 设置 `LABELS` 属性，可在 CLion 的测试筛选器中按标签过滤。
