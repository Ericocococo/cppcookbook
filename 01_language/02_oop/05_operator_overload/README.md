# 05_operator_overload — 运算符重载全形式

## 1. 文件

| 文件 | 说明 |
|------|------|
| `main.cpp` | 演示代码 |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `operator_overload` |

---

## 2. 命令行 · MinGW（Git Bash）

### 方案 A — Ninja

```bash
cmake -G Ninja -B build-mingw-ninja -S .
cmake --build build-mingw-ninja
./build-mingw-ninja/operator_overload.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-mingw-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-mingw-ninja\operator_overload.exe
```

</details>

### 方案 B — MinGW Makefiles

```bash
cmake -G "MinGW Makefiles" -B build-mingw-make -S .
cmake --build build-mingw-make
./build-mingw-make/operator_overload.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-mingw-make
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-mingw-make\operator_overload.exe
```

</details>

### 方案 C — Ninja Multi-Config

```bash
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .
cmake --build build-mingw-mc --config Release
./build-mingw-mc/Release/operator_overload.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-mingw-mc --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-mingw-mc\Release\operator_overload.exe
```

</details>

---

## 3. 命令行 · MSVC（cmd）

### 方案 A — vcvarsall + Ninja

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B build-msvc-ninja -S .
cmake --build build-msvc-ninja
build-msvc-ninja\operator_overload.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-msvc-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-msvc-ninja\operator_overload.exe
```

</details>

### 方案 B — VS Generator

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .
cmake --build build-msvc-vs --config Release
build-msvc-vs\Release\operator_overload.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-msvc-vs --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\02_oop\05_operator_overload\build-msvc-vs\Release\operator_overload.exe
```

</details>

---

## 4. 命令行 · Linux / WSL

```bash
cmake -G Ninja -B build-linux -S .
cmake --build build-linux
./build-linux/operator_overload
```
