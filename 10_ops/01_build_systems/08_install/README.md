# 08_install — 安装规则与包导出

`install()` 定义安装位置，导出包配置让其他项目 find_package

> 工具完整路径见 [父目录 README](../README.md)

---

## 命令行 · MinGW（Git Bash）

```bash
CMAKE="D:/Program Files/Microsoft Visual Studio/18/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe"
GCC="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/gcc.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"

"$CMAKE" -B build-mingw -G Ninja -DCMAKE_C_COMPILER="$GCC" -DCMAKE_CXX_COMPILER="$GXX" -DCMAKE_INSTALL_PREFIX=./install-root && "$CMAKE" --build build-mingw && "$CMAKE" --install build-mingw
```

---

## 命令行 · MSVC（cmd）

```bat
set CMAKE=D:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe
set VCVARSALL=D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat

call "%VCVARSALL%" x64
"%CMAKE%" -B build-msvc -G Ninja -DCMAKE_INSTALL_PREFIX=.\install-root && "%CMAKE%" --build build-msvc && "%CMAKE%" --install build-msvc
```

---

## CLion IDE

构建完成后执行安装有两种方式：

1. **菜单**：Tools → CMake → Install
2. **Terminal**：在 CLion 内置终端执行 `cmake --install build-mingw`（或 `build-msvc`）

> 安装前缀在 Profile 的 **CMake options** 里设置 `-DCMAKE_INSTALL_PREFIX=路径`。

### 安装后目录结构

```
install-root/
├── bin/
│   └── install_demo.exe        # 可执行文件
├── lib/
│   ├── libmylib.a              # 静态库（MinGW）或 mylib.lib（MSVC）
│   └── cmake/
│       └── mylib/
│           ├── mylibConfig.cmake
│           └── mylibConfigVersion.cmake
└── include/
    └── mylib/
        └── mylib.h             # 公开头文件
```

其他项目可通过 `-DCMAKE_PREFIX_PATH=<install-root 路径>` 让 `find_package(mylib CONFIG)` 找到此安装包。
