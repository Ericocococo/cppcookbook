# 09_install — 安装规则与包导出

`install()` 定义安装位置，导出包配置让其他项目 find_package

## 1. 为什么需要 install？

构建完 exe 在 `build/` 下，目录结构杂乱（混有中间文件）。`cmake --install` 把产物整理成规范结构：

```
build/          →    install-root/
├── CMakeFiles/       ├── bin/myapp.exe
├── myapp.exe         ├── lib/libmylib.a
├── mylib.a           └── include/mylib.h
└── ...
```

用途：发布给用户、打包成安装包、或让其他 CMake 项目通过 `find_package` 找到这个库。**日常开发调试不需要此步骤。**

## 2. 本目录专属命令（install 流程）

构建命令与通用流程一致，本目录额外两步：

1. **配置时加 `-DCMAKE_INSTALL_PREFIX=<目录>`**：安装根目录，不指定时用系统默认位置（Windows 上是 `C:/Program Files/<工程名>` 等），学习演示一律覆盖为本地目录 `./install-root`。其他通用配置参数见 [cmake_syntax.md § 0.1 配置阶段](../cmake_syntax.md)。
2. **构建后执行 `cmake --install <build 目录>`**：把构建产物复制到安装根目录，构建目录本身原样保留（不删除，仍可继续开发）。`install()` 实际生成一个 install 目标，`cmake --install` 等价于构建该目标；多配置构建目录（Ninja Multi-Config / VS Generator）须带 `--config Debug` / `--config Release` 指明装哪个配置。

CLion 中执行安装：构建成功后 **Tools → CMake → Install**（安装前缀在 Profile 的 **CMake options** 里设置 `-DCMAKE_INSTALL_PREFIX=路径`）。

## 3. 构建与运行

完整六套方案（MinGW 三方案 / MSVC 三方案，含对比与 CLion）见 [章 README § 5 通用构建流程](../README.md)。本目录可执行目标是 `myapp`（另有静态库 `mylib`，由 myapp 链接），exe 不安装也能在 build 目录直接运行；安装流程是验证重点。日常最常用——Ninja 方案 A（Git Bash）：

```bash
CMAKE="D:/ProgramData/JetBrains/CLion20260101/bin/cmake/win/x64/bin/cmake.exe"
GXX="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/g++.exe"
NINJA="D:/ProgramData/JetBrains/CLion20260101/bin/ninja/win/x64/ninja.exe"

"$CMAKE" -B build-mingw-ninja -G Ninja -DCMAKE_CXX_COMPILER="$GXX" -DCMAKE_MAKE_PROGRAM="$NINJA" -DCMAKE_INSTALL_PREFIX=./install-root
"$CMAKE" --build build-mingw-ninja
"$CMAKE" --install build-mingw-ninja
```

验证：`./build-mingw-ninja/myapp.exe` 直接运行构建产物；`./install-root/bin/myapp.exe` 运行安装后的 exe。两者都能跑，后者证明安装规则生效——重装只需删掉 `install-root/` 再执行一次 `--install`。

> cmd 版与 MSVC/Linux 版：在通用流程的配置命令后追加 `-DCMAKE_INSTALL_PREFIX=./install-root`，最后加一条 `"$CMAKE" --install <build 目录>` 即可，exe 名换成 `myapp`。

## 4. 安装后目录结构

```
install-root/
├── bin/
│   └── myapp.exe                # 可执行文件
├── lib/
│   ├── libmylib.a               # 静态库（MinGW）或 mylib.lib（MSVC）
│   └── cmake/
│       └── mylib/
│           ├── mylibTargets.cmake
│           └── mylibConfigVersion.cmake
└── include/
    └── mylib/
        └── mylib.h             # 公开头文件
```

其他项目可通过 `-DCMAKE_PREFIX_PATH=<install-root 路径>` 让 `find_package(mylib CONFIG)` 找到此安装包。
