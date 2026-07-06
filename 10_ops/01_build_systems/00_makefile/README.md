# 00_makefile — 手写 Makefile

手写构建规则，管理多文件项目的增量编译。

> 工具完整路径见 [父目录 README](../README.md)

---

## 命令行 · MinGW（Git Bash）

MinGW 自带 `mingw32-make.exe`，用完整路径调用：

```bash
MAKE="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

"$MAKE"            # 构建（增量）
"$MAKE" run        # 构建并运行
"$MAKE" clean      # 清理
"$MAKE" --dry-run  # 查看将要执行的命令（不实际构建）
```

---

## 命令行 · MSVC（cmd）

MSVC 自带 `nmake.exe`，但本示例 Makefile 使用了 GNU 扩展（模式规则 `%.o: %.cpp`），**nmake 不支持**。  
MSVC 用户请直接从 `01_hello` 起使用 CMake。

如需了解 nmake 语法差异，参考 [Microsoft NMake 文档](https://learn.microsoft.com/cpp/build/reference/nmake-reference)。

---

## CLion IDE

CLion 不直接支持 Makefile 项目，推荐在 Terminal 里运行：

```
View → Tool Windows → Terminal（Alt+F12）
→ "$MAKE" run
```

---

## Makefile 核心语法速记

| 符号 | 含义 |
|------|------|
| `$@` | 当前目标文件名 |
| `$<` | 第一个依赖文件 |
| `$^` | 所有依赖文件 |
| `%.o: %.cpp` | GNU 模式规则（nmake 不支持）|
| `.PHONY` | 声明伪目标 |

---

## Makefile 的局限（CMake 出现的原因）

- 头文件依赖需手动维护，遗漏导致增量编译失效
- GNU make 与 nmake 不完全兼容，跨平台困难
- 复杂项目的依赖安装、打包能力弱
