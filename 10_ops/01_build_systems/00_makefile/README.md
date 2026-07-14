# 00_makefile — 手写 Makefile

手写构建规则，管理多文件项目的增量编译。

> 工具完整路径见 [父目录 README](../README.md)

---

## 1. 命令行 · MinGW（Git Bash）

MinGW 自带 `mingw32-make.exe`，用完整路径调用：

```bash
MAKE="D:/ProgramData/JetBrains/CLion20260101/bin/mingw/bin/mingw32-make.exe"

"$MAKE"            # 构建（增量）
"$MAKE" run        # 构建并运行
"$MAKE" clean      # 清理
"$MAKE" --dry-run  # 查看将要执行的命令（不实际构建）
```

---

## 2. 命令行 · MSVC（cmd）

MSVC 自带 `nmake.exe`，但本示例 Makefile 使用了 GNU 扩展（模式规则 `%.o: %.cpp`），**nmake 不支持**。  
MSVC 用户请直接从 `01_hello` 起使用 CMake。

如需了解 nmake 语法差异，参考 [Microsoft NMake 文档](https://learn.microsoft.com/cpp/build/reference/nmake-reference)。

---

## 3. CLion IDE

CLion 不直接支持 Makefile 项目，推荐在 Terminal 里运行：

```
View → Tool Windows → Terminal（Alt+F12）
→ "$MAKE" run
```

---

## 4. 本示例 Makefile 逐行解析

```makefile
CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2
TARGET   := app
```

`:=` 立即展开赋值。`$(CXX)` / `$(TARGET)` 是引用语法，展开为变量值。

```makefile
SRCS := main.cpp math_utils.cpp logger.cpp
```

显式列出源文件，不用通配符——新增文件时能被构建系统感知。

```makefile
OBJS := $(SRCS:.cpp=.o)
```

替换引用：把 `SRCS` 里每个词的 `.cpp` 后缀换成 `.o`，结果为 `main.o math_utils.o logger.o`。  
等价于 `$(patsubst %.cpp,%.o,$(SRCS))`。

```makefile
$(TARGET): $(OBJS)
    $(CXX) $(CXXFLAGS) -o $@ $^
```

**默认目标**（文件里第一个目标，`make` 不带参数时执行）。  
`$@` = 目标名（`app`），`$^` = 所有依赖（所有 `.o`）。实际执行：

```bash
g++ -std=c++20 -Wall -Wextra -O2 -o app main.o math_utils.o logger.o
```

```makefile
%.o: %.cpp
    $(CXX) $(CXXFLAGS) -c -o $@ $<
```

**模式规则**：`%` 匹配任意文件基名，`$<` = 第一个依赖（对应的 `.cpp`）。  
make 为每个 `.o` 自动套用此规则，无需对每个文件单独写。实际执行（以 `main.cpp` 为例）：

```bash
g++ -std=c++20 -Wall -Wextra -O2 -c -o main.o main.cpp
```

```makefile
main.o:       main.cpp math_utils.h logger.h
math_utils.o: math_utils.cpp math_utils.h
logger.o:     logger.cpp logger.h
```

显式头文件依赖，只写依赖不写命令——命令继承上面的模式规则。  
头文件变动时 make 会重新编译对应的 `.o`。

```makefile
.PHONY: clean run
```

声明 `clean` / `run` 是伪目标，不对应真实文件，防止与同名文件冲突。

```makefile
ifeq ($(OS),Windows_NT)
SHELL = cmd
clean:
    if exist *.o del /Q *.o
    if exist $(TARGET).exe del /Q $(TARGET).exe
else
clean:
    rm -f $(OBJS) $(TARGET)
endif
```

`$(OS)` 在 Windows 下为 `Windows_NT`。`mingw32-make` 默认用 `sh` 执行命令，`SHELL = cmd` 切换为 `cmd`，使 `if exist` / `del` 可用；Unix 保持 `rm -f`。

---

## 5. 语法完整参考

### 5.1 变量赋值与引用

**引用语法**：`$(VAR)` — 取变量值。多字符变量必须加括号，否则 `$TARGET` 被解析为 `$T` + 字面量 `ARGET`。单字符自动变量（`$@` `$<` `$^`）可省略括号。

| 写法 | 展开时机 | 说明 |
|------|---------|------|
| `VAR = val` | **使用时**展开（递归） | 可引用后面才定义的变量 |
| `VAR := val` | **定义时**立即展开 | 推荐，行为可预期 |
| `VAR ?= val` | 定义时，仅当未定义才赋值 | 允许外部覆盖默认值 |
| `VAR += val` | 追加到已有值 | 常用于累积 flags |

### 5.2 自动变量

| 变量 | 含义 |
|------|------|
| `$@` | 当前目标文件名 |
| `$<` | 第一个依赖文件 |
| `$^` | 所有依赖文件（去重） |
| `$*` | 模式规则中 `%` 匹配的部分（文件基名） |

### 5.3 替换引用与常用函数

**替换引用**：`$(VAR:旧后缀=新后缀)` — 只能替换后缀，是 `patsubst` 的简写。

```makefile
OBJS := $(SRCS:.cpp=.o)                        # 替换后缀
OBJS := $(patsubst %.cpp,%.o,$(SRCS))          # 等价，更通用
OBJS := $(patsubst src/%.cpp,obj/%.o,$(SRCS))  # 同时替换路径前缀+后缀

# wildcard：展开文件系统通配符
ALL_SRCS := $(wildcard *.cpp)   # 自动发现 .cpp，但不推荐用于 SRCS

# shell：执行 shell 命令，捕获输出为变量值
GIT_HASH := $(shell git rev-parse --short HEAD)
CXXFLAGS += -DGIT_HASH=\"$(GIT_HASH)\"   # 把 git hash 编译进程序
```

### 5.4 规则

```makefile
# 普通规则
目标: 依赖列表
    命令（必须用 Tab 缩进）

# 模式规则：% 匹配任意非空字符串
%.o: %.cpp
    $(CXX) -c -o $@ $<

# 只声明依赖，命令继承已有模式规则
main.o: main.cpp math_utils.h logger.h
```

### 5.5 伪目标

```makefile
.PHONY: clean run all

clean:
    rm -f $(OBJS) $(TARGET)
```

> 不加 `.PHONY`：若目录里存在名为 `clean` 的文件，make 认为目标已最新而跳过。

### 5.6 命令前缀

| 前缀 | 作用 |
|------|------|
| `@` | 不回显命令本身，只显示输出 |
| `-` | 忽略命令失败（非零退出码），继续执行 |

```makefile
clean:
    @echo "清理中..."   # 只显示 "清理中..."，不显示 echo 命令本身
    -rm -f $(OBJS)     # rm 失败也继续
```

### 5.7 条件判断与跨平台

```makefile
ifeq ($(OS),Windows_NT)   # Windows 下 $(OS) = "Windows_NT"
    SHELL = cmd
else
    ...
endif

ifneq ($(CC),gcc)   # 不等于
ifdef  VAR          # VAR 已定义（值非空）
ifndef VAR          # VAR 未定义
```

---

## 6. Makefile 的局限（CMake 出现的原因）

- 头文件依赖需手动维护，遗漏导致增量编译失效
- GNU make 与 nmake 不完全兼容，跨平台困难
- 复杂项目的依赖安装、打包能力弱
