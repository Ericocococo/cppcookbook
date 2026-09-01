# 06_pointers_refs — 指针与引用

演示引用、指针、`nullptr`、const 指针四种形式、指针算术、`new`/`delete`、智能指针。

## 1. 文件

| 文件               | 说明                           |
|------------------|------------------------------|
| `main.cpp`       | 6 个 demo 函数，从引用到智能指针逐步讲解     |
| `CMakeLists.txt` | 构建配置，生成可执行文件 `pointers_refs` |

---

## 2. 命令行 · MinGW（Git Bash）

### 方案 A — Ninja

```bash
cmake -G Ninja -B build-mingw-ninja -S .
cmake --build build-mingw-ninja
./build-mingw-ninja/pointers_refs.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-mingw-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-mingw-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-mingw-ninja\pointers_refs.exe
```

</details>

### 方案 B — MinGW Makefiles

```bash
cmake -G "MinGW Makefiles" -B build-mingw-make -S .
cmake --build build-mingw-make
./build-mingw-make/pointers_refs.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "MinGW Makefiles" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-mingw-make -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-mingw-make
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-mingw-make\pointers_refs.exe
```

</details>

### 方案 C — Ninja Multi-Config

```bash
cmake -G "Ninja Multi-Config" -B build-mingw-mc -S .
cmake --build build-mingw-mc --config Release
./build-mingw-mc/Release/pointers_refs.exe
```

<details><summary>cmd 三步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Ninja Multi-Config" -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-mingw-mc -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-mingw-mc --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-mingw-mc\Release\pointers_refs.exe
```

</details>

---

## 3. 命令行 · MSVC（cmd）

### 方案 A — vcvarsall + Ninja

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B build-msvc-ninja -S .
cmake --build build-msvc-ninja
build-msvc-ninja\pointers_refs.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
call "C:\Program Files\Microsoft Visual Studio\2026\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -G Ninja -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-msvc-ninja -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-msvc-ninja
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-msvc-ninja\pointers_refs.exe
```

</details>

### 方案 B — VS Generator

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B build-msvc-vs -S .
cmake --build build-msvc-vs --config Release
build-msvc-vs\Release\pointers_refs.exe
```

<details><summary>cmd 四步版（绝对路径，可直接粘贴）</summary>

```cmd
cmake -G "Visual Studio 18 2026" -A x64 -B D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-msvc-vs -S D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs
cmake --build D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-msvc-vs --config Release
D:\workspace\clion_workspace\cppcookbook\01_language\01_basics\06_pointers_refs\build-msvc-vs\Release\pointers_refs.exe
```

</details>

---

## 4. 命令行 · Linux / WSL

```bash
cmake -G Ninja -B build-linux -S .
cmake --build build-linux
./build-linux/pointers_refs
```

---

## 5. 英文及缩写说明

### 关键字与运算符

| 词汇         | 说明                                       |
|------------|------------------------------------------|
| `&`（取地址）   | address-of operator：取变量的内存地址，结果是指针       |
| `*`（声明指针）  | pointer declarator：声明指针变量，如 `int* p`     |
| `*`（解引用）   | dereference operator：通过指针访问所指地址的值，如 `*p` |
| `nullptr`  | null pointer：空指针（C++11），比 `NULL` 类型安全    |
| `new`      | 在堆（heap）上动态分配内存，返回指针                     |
| `delete`   | 释放 `new` 分配的单个对象内存，不释放 = 内存泄漏            |
| `delete[]` | 释放 `new[]` 分配的数组内存，必须与 `new[]` 配对        |

### 概念

| 概念               | 说明                                                                |
|------------------|-------------------------------------------------------------------|
| reference        | 引用：变量的别名，声明时必须初始化，不能为空                                            |
| pointer          | 指针：存储内存地址的变量，可为 `nullptr`，可改变指向                                   |
| stack            | 栈：函数内声明的变量，函数返回时自动释放                                              |
| heap             | 堆：`new` 分配的内存，程序员负责手动释放                                           |
| memory leak      | 内存泄漏：`new` 后忘记 `delete`，内存无法回收                                    |
| dangling pointer | 悬空指针：指向已释放内存的指针，访问它是未定义行为                                         |
| RAII             | Resource Acquisition Is Initialization：资源获取即初始化，析构时自动释放；智能指针的设计原则 |
| `unique_ptr`     | 独占所有权的智能指针，不能复制，离开作用域自动 `delete`                                  |
| `shared_ptr`     | 共享所有权的智能指针，引用计数归零时释放                                              |
| `make_unique`    | 创建 `unique_ptr` 的工厂函数（C++14），比直接 `new` 更安全                        |
| `make_shared`    | 创建 `shared_ptr` 的工厂函数，一次内存分配效率更高                                  |
| `use_count`      | `shared_ptr` 的引用计数：当前有几个 `shared_ptr` 共享此内存                       |
| `<memory>`       | 包含 `unique_ptr`、`shared_ptr`、`make_unique`、`make_shared`          |
