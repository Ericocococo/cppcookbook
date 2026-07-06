# 03_stl — 标准模板库

STL = Standard Template Library，C++ 标准库的核心部分，包含容器、算法、迭代器等。

| 目录 | 内容 |
|------|------|
| 01_containers | vector/deque/list/array、map/set、unordered_map/set、stack/queue |
| 02_algorithms | sort/find/transform/reduce/partition 等 `<algorithm>` 函数 |
| 03_iterators | 五类迭代器、反向迭代器、插入迭代器、自定义迭代器 |
| 04_ranges | C++20 Ranges、views 管道、自定义 view |
| 05_io_streams | iostream/fstream/stringstream、格式化标志 |
| 06_format | `std::format`/`std::print`（C++20/23）|
| 07_filesystem | `std::filesystem`（C++17）路径、目录操作 |
| 08_chrono | 时间点、时长、日历（C++20）|
| 09_regex | `std::regex` 匹配、搜索、替换 |
| 10_optional_variant | `optional`/`variant`/`any`/`expected`（C++23）|
| 11_string | `string`/`string_view`/`span`/`byte` |
| 12_numeric | `<numeric>` gcd/lcm/complex/bit |
| 13_thread | `thread`/`mutex`/`condition_variable`/`latch`/`barrier` |
| 14_atomic | `atomic`/`memory_order`/lock-free 基础 |
| 15_coroutines | C++20 协程基础（generator/task 模型）|

## 术语速查

| 术语 | 全称 / 含义 |
|------|-------------|
| STL | Standard Template Library — 标准模板库 |
| CTAD | Class Template Argument Deduction — 类模板参数推导（C++17）|
| SSO | Small String Optimization — 短字符串优化，避免短字符串的堆分配 |
| COW | Copy-On-Write — 写时复制，共享数据直到修改时才真正复制 |
| ADL | Argument-Dependent Lookup — 参数依赖查找，根据参数类型自动搜索命名空间 |
