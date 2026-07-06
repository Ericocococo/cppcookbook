cmake_minimum_required(VERSION 3.28)
project(cppcookbook LANGUAGES CXX)

# 仓库根标记。
# 各子目录均为独立 CMake 工程，在对应子目录下单独打开/构建，互不依赖。
# 此根文件仅供 CLion 识别仓库根目录，以及全局工具（clang-format/clang-tidy）的配置入口。
