# C++ 标准库常用头文件速查

> 定位：**导航参考**，不重复展开用法。每个头文件"讲什么、在哪第一次详细讲"都在这张表里；点链接跳到对应知识目录。
> 已建成部分链接到 `01_language`（C++ 语法主线），规划中部分标注 ⏳（对应 03_stl 的子目录规划）。

## 1. 输入输出

| 头文件 | 提供什么 | 首次出现 / 详解 |
|--------|---------|----------------|
| `<iostream>` | `cout` / `cin` / `cerr` 控制台输入输出 | [01_language/01_basics/00_hello_world](../01_language/01_basics/00_hello_world/README.md)（cout/cin 入门） |
| `<fstream>` | `ifstream` / `ofstream` 文件读写 | ⏳ 03_stl/05_io_streams |
| `<sstream>` | `stringstream` 字符串流 | ⏳ 03_stl/05_io_streams |
| `<iomanip>` | `setw` / `setprecision` 等格式化操纵符 | ⏳ 03_stl/05_io_streams |
| `<cstdio>` | C 风格 `printf` / `FILE` | 不推荐，除非与 C 代码交互 |

## 2. 字符串与文本

| 头文件 | 提供什么 | 首次出现 / 详解 |
|--------|---------|----------------|
| `<string>` | `std::string`（常用操作：拼接/substr/find/互转） | [01_language/01_basics/07_arrays_strings](../01_language/01_basics/07_arrays_strings/README.md) 1.4 |
| `<string_view>` | `std::string_view` 只读视图（C++17，不拷贝） | [01_language/01_basics/07_arrays_strings](../01_language/01_basics/07_arrays_strings/README.md) 1.5 |
| `<charconv>` | `from_chars` / `to_chars` 高性能数字转换 | ⏳ 03_stl/11_string |
| `<format>` | `std::format` / `std::print`（C++20/23） | ⏳ 03_stl/06_format |
| `<regex>` | `std::regex` 正则匹配 | ⏳ 03_stl/09_regex |
| `<cstring>` | C 风格 `strlen` / `strcpy` / `memcpy` | 与 C 代码交互时用 |

## 3. 容器

| 头文件 | 提供什么 | 首次出现 / 详解 |
|--------|---------|----------------|
| `<array>` | `std::array` 定长数组（不退化） | [01_language/01_basics/07_arrays_strings](../01_language/01_basics/07_arrays_strings/README.md) 1.2 |
| `<vector>` | `std::vector` 动态数组（最常用容器） | [01_language/01_basics/07_arrays_strings](../01_language/01_basics/07_arrays_strings/README.md) 1.3 |
| `<deque>` | `std::deque` 双端队列 | ⏳ 03_stl/01_containers |
| `<list>` | `std::list` 双向链表 | ⏳ 03_stl/01_containers |
| `<forward_list>` | `std::forward_list` 单向链表 | ⏳ 03_stl/01_containers |
| `<map>` | `std::map` / `std::multimap` 有序键值对（pybind 项目已实际使用） | ⏳ 03_stl/01_containers |
| `<set>` | `std::set` / `std::multiset` 有序集合 | ⏳ 03_stl/01_containers |
| `<unordered_map>` | `std::unordered_map` 哈希表（O(1) 查找） | ⏳ 03_stl/01_containers |
| `<unordered_set>` | `std::unordered_set` 哈希集合 | ⏳ 03_stl/01_containers |
| `<stack>` / `<queue>` | 栈 / 队列 / 优先队列适配器 | ⏳ 03_stl/01_containers |
| `<span>` | `std::span` 数组视图（C++20） | ⏳ 03_stl/11_string |

## 4. 算法与迭代器

| 头文件 | 提供什么 | 首次出现 / 详解 |
|--------|---------|----------------|
| `<algorithm>` | `sort` / `find` / `count_if` / `for_each` 等 | 排序 [01_basics/07_arrays_strings](../01_language/01_basics/07_arrays_strings/README.md) 1.2.4；lambda 配合 [04_modern/01_lambda](../01_language/04_modern/01_lambda/README.md) 1.4 |
| `<numeric>` | `gcd` / `lcm` / `accumulate` / `iota` | ⏳ 03_stl/12_numeric |
| `<iterator>` | `std::size`（C++17）/ 迭代器工具 | [01_basics/02_type_queries](../01_language/01_basics/02_type_queries/README.md) 1.1.3（`std::size`） |
| `<ranges>` | C++20 ranges：`views::reverse` / 管道 | [10_ops/13_enterprise 示例](../10_ops/01_build_systems/13_enterprise/utils/string_utils.cpp) 已用；系统讲解 ⏳ 03_stl/04_ranges |
| `<random>` | `mt19937` / 分布引擎 | ⏳ 03_stl/12_numeric |

## 5. 工具与函数对象

| 头文件 | 提供什么 | 首次出现 / 详解 |
|--------|---------|----------------|
| `<utility>` | `std::move` / `std::swap` / `std::exchange` / `std::pair` | [01_language/02_oop/06_special_members](../01_language/02_oop/06_special_members/README.md) 1.6.3（`std::exchange`） |
| `<functional>` | `std::function` / `std::bind` / `less<>` | [01_language/01_basics/05_functions](../01_language/01_basics/05_functions/README.md) 1.6.5 |
| `<tuple>` | `std::tuple` / `std::tie` / `std::get` | ⏳ 03_stl/10_optional_variant |
| `<optional>` | `std::optional` 可空值（C++17） | ⏳ 03_stl/10_optional_variant |
| `<variant>` | `std::variant` 类型安全联合（C++17） | ⏳ 03_stl/10_optional_variant |
| `<any>` | `std::any` 任意类型（C++17） | ⏳ 03_stl/10_optional_variant |
| `<expected>` | `std::expected` 错误处理（C++23） | ⏳ 03_stl/10_optional_variant |
| `<initializer_list>` | `{}` 初始化列表类型 | 隐式使用（列表初始化语法见 [01_types](../01_language/01_basics/01_types/README.md) 1.6） |
| `<compare>` | `std::strong_ordering` 等三态比较类型（C++20，`<=>` 返回） | [01_language/02_oop/05_operator_overload](../01_language/02_oop/05_operator_overload/README.md) 1.2.1（太空船运算符） |

## 6. 内存管理

| 头文件 | 提供什么 | 首次出现 / 详解 |
|--------|---------|----------------|
| `<memory>` | `unique_ptr` / `shared_ptr` / `weak_ptr` | [01_language/01_basics/06_pointers_refs](../01_language/01_basics/06_pointers_refs/README.md) 1.7 |
| `<new>` | `new` / `delete` / `placement new` | [01_basics/06_pointers_refs](../01_language/01_basics/06_pointers_refs/README.md) 1.6；placement ⏳ 01_language/05_memory |
| `<memory_resource>` | pmr 内存资源（C++17） | ⏳ 01_language/05_memory/03_allocators |

## 7. 类型支持与编译期

| 头文件 | 提供什么 | 首次出现 / 详解 |
|--------|---------|----------------|
| `<climits>` | `INT_MAX` / `LLONG_MIN` 等整数范围常量 | [01_language/01_basics/01_types](../01_language/01_basics/01_types/README.md) 1.2 |
| `<cfloat>` | `FLT_MAX` / `DBL_MAX` 等浮点常量 | [01_types](../01_language/01_basics/01_types/README.md) 1.3 |
| `<cstdint>` | `int8_t` / `uint32_t` 定宽整数 | [01_types](../01_language/01_basics/01_types/README.md) 1.8 |
| `<cstddef>` | `size_t` / `ptrdiff_t` / `nullptr_t` | [01_types](../01_language/01_basics/01_types/README.md) 1.9.2（`size_t`） |
| `<limits>` | `std::numeric_limits<T>::max()` | ⏳ 03_stl/12_numeric |
| `<typeinfo>` | `typeid` / `type_info`（运行时类型） | [01_language/01_basics/12_typeid_pack](../01_language/01_basics/12_typeid_pack/README.md) 1.1 |
| `<type_traits>` | `is_integral_v` / `remove_const_t` 编译期查询 | [12_typeid_pack](../01_language/01_basics/12_typeid_pack/README.md) 1.4 |
| `<concepts>` | C++20 concept 定义（`requires`） | ⏳ 01_language/04_modern/05_concepts（[01_basics/10_templates](../01_language/01_basics/10_templates/README.md) 1.5 已用 is_xxx_v 约束） |
| `<bit>` | 位操作：`popcount` / `bit_cast`（C++20） | ⏳ 03_stl/12_numeric |

## 8. 数学与数值

| 头文件 | 提供什么 | 首次出现 / 详解 |
|--------|---------|----------------|
| `<cmath>` | `sqrt` / `sin` / `pow` / `abs` / `isnan` | [01_language/01_basics/01_types](../01_language/01_basics/01_types/README.md) 1.3（浮点判断） |
| `<cstdlib>` | `atoi` / `rand` / `qsort` / `system` | C 兼容工具 |
| `<complex>` | `std::complex` 复数 | ⏳ 03_stl/12_numeric |
| `<valarray>` | `std::valarray` 数值数组 | ⏳ 03_stl/12_numeric |

## 9. 错误处理

| 头文件 | 提供什么 | 首次出现 / 详解 |
|--------|---------|----------------|
| `<exception>` | `std::exception` 基类 / `std::terminate` | [01_language/01_basics/11_exceptions](../01_language/01_basics/11_exceptions/README.md) 1.2 |
| `<stdexcept>` | `std::runtime_error` / `logic_error` 等标准异常 | [11_exceptions](../01_language/01_basics/11_exceptions/README.md) 1.2 |

## 10. 时间与并发

| 头文件 | 提供什么 | 首次出现 / 详解 |
|--------|---------|----------------|
| `<chrono>` | 时间点 / 时长 / 日历（C++20） | ⏳ 03_stl/08_chrono |
| `<thread>` | `std::thread` / `jthread` | ⏳ 03_stl/13_thread |
| `<mutex>` | `std::mutex` / `lock_guard` / `unique_lock` | `lock_guard` 模式见 [02_oop/02_constructors_raii](../01_language/02_oop/02_constructors_raii/README.md) 1.4.2；详解 ⏳ 03_stl/13_thread |
| `<shared_mutex>` | `std::shared_mutex` 读写锁（C++17） | ⏳ 03_stl/13_thread |
| `<condition_variable>` | 条件变量（线程同步） | ⏳ 03_stl/13_thread |
| `<latch>` / `<barrier>` / `<semaphore>` | C++20 同步原语（一次性闩锁/复用屏障/信号量） | ⏳ 03_stl/13_thread |
| `<future>` | `std::async` / `promise` / `future` 异步任务 | ⏳ 03_stl/13_thread |
| `<atomic>` | `std::atomic` / `memory_order` | ⏳ 03_stl/14_atomic |

## 11. 文件系统与杂项

| 头文件 | 提供什么 | 首次出现 / 详解 |
|--------|---------|----------------|
| `<filesystem>` | 路径 / 目录遍历（C++17） | ⏳ 03_stl/07_filesystem |
| `<coroutine>` | 协程原语（C++20） | ⏳ 03_stl/15_coroutines |

> 使用原则：**需要什么包含什么**，不要图省事 `#include <bits/stdc++.h>`（非标准头，编译慢、可移植性差）。
> 头文件里各类型/函数的具体用法在对应的知识目录与 03_stl 子目录详解，本表只做"在哪学"的索引。
