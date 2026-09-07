# 传统手写 Makefile —— 演示核心语法与增量编译原理
# 用法：make        构建
#       make clean  清理
#       make run    构建并运行

CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2
TARGET   := app

# ① 源文件列表（显式，不用通配符——原因同 CMake：工具不能自动感知新文件）
SRCS := main.cpp math_utils.cpp logger.cpp

# ② 目标文件列表：把 .cpp 替换为 .o
OBJS := $(SRCS:.cpp=.o)

# ③ 默认目标：链接所有 .o → 可执行文件
#    $@  = 当前目标名（app）
#    $^  = 所有依赖（全部 .o）
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# ④ 模式规则：任意 .cpp → .o
#    $<  = 第一个依赖（对应的 .cpp）
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# ⑤ 头文件依赖（手动维护，这是 Makefile 最大的痛点）
main.o:       main.cpp math_utils.h logger.h
math_utils.o: math_utils.cpp math_utils.h
logger.o:     logger.cpp logger.h

# ⑥ 伪目标：不对应文件，避免与同名文件冲突
.PHONY: clean run

run: $(TARGET)
	./$(TARGET)

# Windows 下 mingw32-make 默认用 sh 执行命令，但 rm 不在 PATH 里；
# 显式切换到 cmd 并用 del，避免依赖 sh 环境；Unix 保持 rm -f
ifeq ($(OS),Windows_NT)
SHELL = cmd
clean:
	if exist *.o del /Q *.o
	if exist $(TARGET).exe del /Q $(TARGET).exe
else
clean:
	rm -f $(OBJS) $(TARGET)
endif
