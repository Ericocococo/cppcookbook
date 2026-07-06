# 命令快捷表。不负责 C++ 构建（构建由各子目录 CMake 负责）。
# 用法：make <target>

CLANG_FORMAT := "D:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/Llvm/x64/bin/clang-format.exe"
CLANG_TIDY   := "D:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/Llvm/x64/bin/clang-tidy.exe"

# 查找所有 .cpp / .h（排除 build 目录）
CPP_FILES := $(shell find . -name "*.cpp" -o -name "*.h" | grep -v "/build/")

.PHONY: fmt lint help

fmt: ## 格式化所有 .cpp / .h 文件
	@echo "格式化 $(words $(CPP_FILES)) 个文件..."
	@echo $(CPP_FILES) | xargs $(CLANG_FORMAT) -i
	@echo "完成"

lint: ## 静态分析（需先在目标目录 cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON 生成 compile_commands.json）
	@echo "分析 $(words $(CPP_FILES)) 个文件..."
	@echo $(CPP_FILES) | xargs $(CLANG_TIDY) --quiet
	@echo "完成"

help: ## 显示此帮助
	@grep -E '^[a-zA-Z_-]+:.*?##' $(MAKEFILE_LIST) \
		| awk 'BEGIN {FS = ":.*?## "}; {printf "  %-10s %s\n", $$1, $$2}'
