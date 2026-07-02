# C++ Cookbook 目录设计

> 一个存放 C++ 语法与实战代码的仓库结构设计。
> 核心原则：**顶层按"知识属性"分桶，桶底稳定；面向无限增长时对应大目录，永远有明确落位。**
> 约定：**所有层级目录统一带两位数字前缀（`01_ 02_ ...`），顺序即学习/工程顺序。**

---

## 与 Python Cookbook 的对应关系

| Python Cookbook | C++ Cookbook | 说明 |
|-----------------|--------------|------|
| `01_language` | `01_language` | 语言本身，稳定 |
| `02_algorithms` | `02_algorithms` | 算法与模式，稳定 |
| `03_stdlib` | `03_stl` | 标准库（C++ 叫 STL） |
| `04_libraries` | `04_libraries` | 通用三方库 |
| `05_frameworks` | `05_frameworks` | 领域大型开源框架 |
| `06_extending` | `06_extending` | 互操作与语言扩展（方向相反：Python 用 C++ 加速；C++ 嵌入脚本/导出绑定） |
| `07_backend` | `07_systems` | 无限增长① 系统编程 |
| `08_data` | `08_hpc` | 无限增长② 高性能计算 |
| `09_ai` | `09_domain` | 无限增长③ 领域应用（最大） |
| `10_ops` | `10_ops` | 无限增长④ 工程化（内容大不同：CMake/调试/分析） |

---

## 设计哲学

- **一底六稳（01–06）**：`language / algorithms / stl / libraries / frameworks / extending` 大体稳定，几乎不增长；
  可能例外是 `05_frameworks`（大型领域开源系统 vnpy/ROS2/flecs…）随研究项目增长，依赖差异大，只是序号好写在此处。
- **四顶无限（07–10）**：`systems / hpc / domain / ops` 面向无限增长。
- 新东西来了，先问归属哪个大库，再往里加子目录——不会出现"不知道往哪放"。
- **序号只表示顺序，不表示优先级**；插入新目录就重排序号，幅度大就整体重组。工程约定目录（`tests/`、`.github/`）按惯例不加序号。

---

## 顶层结构

```
cppcookbook/
│
├── 01_language/      # 语言本身      ── 稳定，几乎不增长
├── 02_algorithms/    # 算法与模式    ── 稳定
├── 03_stl/           # 标准模板库    ── 稳定
├── 04_libraries/     # 通用三方库    ── 缓慢增长（网络/序列化/日志/测试…）
├── 05_frameworks/    # 领域大型框架  ── 按领域增长（vnpy/ROS2/flecs…）
├── 06_extending/     # 互操作与扩展  ── 稳定（Python绑定/Lua/Wasm/JNI）
│
├── 07_systems/       # ▲ 系统编程    ── 无限增长桶①
├── 08_hpc/           # ▲ 高性能计算  ── 无限增长桶②
├── 09_domain/        # ▲ 领域应用    ── 无限增长桶③（最大）
└── 10_ops/           # ▲ 工程化      ── 无限增长桶④
```

---

## 完整目录树

```
cppcookbook/                                  # 顶层仓库（可运行的合集）
│
├── README.md                               # 总索引：每个系列一句话导航
├── CMakeLists.txt                          # 主构建文件（对应 pyproject.toml）
│                                           #   ① 项目元数据  ② add_subdirectory 统一管理
│                                           #   ③ 工具链配置（clang-tidy/asan 等）
├── CMakePresets.json                       # 预置构建变体（debug/release/asan/wasm）
├── vcpkg.json                              # ★新增: 依赖声明（换机 vcpkg install 一键装）
├── .clang-format                           # ★新增: 统一代码格式
├── .clang-tidy                             # ★新增: 静态分析规则
├── .gitignore                              # 忽略 build//.venv/密钥/大文件
├── Makefile                               # 快捷方式：make build/test/lint/clean
├── CONTRIBUTING.md                         # ★新增: 贡献风格与提交约定
├── .github/workflows/                      # ★新增: CI（安装依赖→构建→测试→lint）
└── tests/                                  # ★新增: 全库集成/回归测试
│
├── 01_language/                            # ─── C++ 语言本身（按难度递进）───
│   ├── 01_basics/                          # 基础
│   │   ├── types_values.cpp                    # 内置类型/值类别(lvalue/rvalue/xvalue)
│   │   ├── references_pointers.cpp             # 引用/指针/nullptr
│   │   ├── control_flow.cpp                    # if/for/while/switch/goto
│   │   ├── functions.cpp                       # 函数/重载/默认参数/内联/constexpr
│   │   ├── arrays_strings.cpp                  # 原生数组/C字符串/string
│   │   └── namespaces.cpp                      # 命名空间/using/ADL
│   ├── 02_oop/                             # 面向对象
│   │   ├── classes_objects.cpp                 # 类/成员/访问控制/友元
│   │   ├── constructors_raii.cpp               # 构造/析构/拷贝/移动/RAII
│   │   ├── inheritance.cpp                     # 继承/虚函数/override/final
│   │   ├── polymorphism.cpp                    # 多态/纯虚/抽象类/虚析构
│   │   ├── operator_overload.cpp               # 运算符重载
│   │   └── special_members.cpp                 # Rule of 0/3/5
│   ├── 03_templates/                       # 模板
│   │   ├── function_templates.cpp              # 函数模板/实例化/推导
│   │   ├── class_templates.cpp                 # 类模板/成员模板
│   │   ├── specialization.cpp                  # 全特化/偏特化
│   │   ├── variadic_templates.cpp              # 变参模板/fold 表达式
│   │   └── sfinae_tag_dispatch.cpp             # SFINAE/标签分发（C++20 前）
│   ├── 04_modern/                          # 现代 C++（C++11-23 核心特性）
│   │   ├── move_semantics.cpp                  # 移动语义/std::move/完美转发
│   │   ├── lambda.cpp                          # lambda/捕获/泛型lambda
│   │   ├── structured_bindings.cpp             # 结构化绑定（C++17）
│   │   ├── if_switch_init.cpp                  # if/switch 初始化语句（C++17）
│   │   ├── concepts.cpp                        # Concepts（C++20）
│   │   ├── modules_demo.cpp                    # Modules（C++20）
│   │   └── ranges_brief.cpp                    # Ranges 入门（C++20，详见 03_stl）
│   ├── 05_memory/                          # 内存管理
│   │   ├── smart_pointers.cpp                  # unique_ptr/shared_ptr/weak_ptr
│   │   ├── memory_layout.cpp                   # 对齐/padding/sizeof
│   │   ├── allocators.cpp                      # 自定义分配器/pmr
│   │   ├── memory_model.cpp                    # C++ 内存模型/happens-before
│   │   └── placement_new.cpp                   # placement new/对象生命期
│   └── 06_expert/                          # 专家（元编程/底层/陷阱）
│       ├── type_traits.cpp                     # type_traits/条件类型
│       ├── constexpr_consteval.cpp             # constexpr/consteval/constinit
│       ├── reflection_demo.cpp                 # ★ 静态反射（C++26 草案/workaround）
│       ├── ub_and_traps.cpp                    # UB/悬挂引用/严格别名/常见陷阱
│       └── abi_linkage.cpp                     # ABI/name mangling/extern "C"
│
├── 02_algorithms/                          # ─── ★新增: 算法与设计模式 ───
│   ├── 01_data_structures/                 # 链表/树/图/堆/并查集/跳表/布隆过滤器
│   ├── 02_classic/                         # 排序/搜索/DP/贪心/回溯/分治
│   └── 03_patterns/                        # 设计模式 in Modern C++
│                                           #   创建型: singleton/factory/builder
│                                           #   结构型: decorator/proxy/flyweight
│                                           #   行为型: observer/strategy/visitor/CRTP
│
├── 03_stl/                                 # ─── 标准模板库（查手册最多的地方）───
│   │                                       #   每个库一个子目录，内部按功能若干 .cpp
│   ├── 01_containers/                      # 顺序: vector/deque/list/array/forward_list
│   │                                       # 关联: map/set/multimap/multiset
│   │                                       # 无序: unordered_map/unordered_set
│   │                                       # 适配: stack/queue/priority_queue
│   ├── 02_algorithms/                      # <algorithm> sort/stable_sort/find/count
│   │                                       # transform/reduce/partition/merge/set_*
│   ├── 03_iterators/                       # 五类迭代器/反向迭代器/插入迭代器/自定义
│   ├── 04_ranges/                          # ★新增: C++20 ranges/views/管道/自定义view
│   ├── 05_io_streams/                      # iostream/fstream/stringstream/ios flags
│   ├── 06_format/                          # ★新增: std::format/std::print（C++20/23）
│   ├── 07_filesystem/                      # ★新增: std::filesystem（C++17）
│   ├── 08_chrono/                          # 时间点/时长/time_point/日历（C++20）
│   ├── 09_regex/                           # std::regex/match/search/replace
│   ├── 10_optional_variant/                # ★新增: optional/variant/any/expected（C++23）
│   ├── 11_string/                          # string/string_view/span/byte
│   ├── 12_numeric/                         # numeric/gcd/lcm/complex/valarray/bit
│   ├── 13_thread/                          # ★新增: thread/mutex/condition_variable/latch/barrier
│   ├── 14_atomic/                          # ★新增: atomic/memory_order/lock-free 基础
│   └── 15_coroutines/                      # ★新增: C++20 协程基础（generator/task 模型）
│
├── 04_libraries/                           # ─── 通用三方库（pip 装完即用的 C++ 等价物）───
│   ├── 01_network/                             # asio/cpp-httplib/libcurl/beast
│   ├── 02_serialization/                       # nlohmann-json/protobuf/yaml-cpp/toml++/msgpack
│   ├── 03_cli/                                 # CLI11/lyra/args/rang（颜色）
│   ├── 04_logging/                             # spdlog/quill/glog
│   ├── 05_math/                                # Eigen/glm/Armadillo/blaze
│   ├── 06_db/                                  # SQLite3-cpp/libpq-cpp/hiredis/SOCI
│   ├── 07_parsing/                             # re2/PEGTL/lexy/rapidxml
│   ├── 08_compression/                         # zlib/lz4/zstd/brotli
│   ├── 09_crypto_auth/                         # OpenSSL/libsodium/mbedTLS/jwt-cpp
│   │                                           #  （jwt 库主体在此；认证流程见 07_systems）
│   ├── 10_testing/                             # gtest/catch2/doctest/benchmark/fakeit
│   │                                           #  （测试工程化流程见 10_ops/02_testing）
│   └── 11_misc/                                # ★新增: {fmt}/magic_enum/nameof/expected/frozen
│
├── 05_frameworks/                          # ─── 开源应用框架（面向平台，学架构+使用+二次开发）───
│   │                                       #   ★重定义: 这里不放通用网络/web 框架
│   │                                       #   ——drogon/crow/oatpp 已迁至 07_systems/05_server；
│   │                                       #   这里只放"领域级完整系统":学其架构+使用+二次开发
│   │                                       #   组织: 领域 → 具体框架 → 架构笔记/使用/二次示例
│   ├── 01_quant/                           # 量化交易
│   │   ├── 01_ctp/                             # 上期 CTP 期货接口（柜台/行情/报单）
│   │   └── 02_vnpy_core/                       # vnpy C++ 核心分析（事件引擎/网关）
│   └── 02_game/                            # 游戏引擎/框架
│       ├── 01_flecs/                            # Flecs ECS 框架（世界/实体/系统/查询）
│       └── 02_entt/                             # EnTT ECS（Registry/View/Signal）
│                                           #   其他领域/框架到来时追加 03_ 04_ …
│
├── 06_extending/                           # ─── 互操作与扩展（C++ ↔ 其他语言）───
│   ├── 01_c_abi/                               # C ABI/extern "C"/动态库/.so/.dll 导出
│   ├── 02_pybind11/                            # C++ → Python（含 CMake + setup.py + 示例）
│   ├── 03_nanobind/                            # pybind11 新一代替代（更快/更小）
│   ├── 04_lua/                                 # C++ ↔ Lua 脚本嵌入（sol2/lua C API）
│   ├── 05_wasm/                                # C++ → WebAssembly（Emscripten 工具链/bindgen/DOM/WebGL 应用）
│   └── 06_jni/                                 # C++ ↔ Java/Android（JNI 基础）
│
├── 07_systems/                             # ─── ▲ 系统编程 ───
│   ├── 01_network/                             # socket/TCP/UDP/epoll/kqueue/io_uring
│   ├── 02_ipc/                                 # pipe/shm/mmap/信号/消息队列/semaphore
│   ├── 03_posix/                               # POSIX API/系统调用/文件描述符/进程管理
│   ├── 04_embedded/                            # 嵌入式技巧/裸机/RTOS接口/volatile/位操作
│   ├── 05_server/                              # 高性能服务器（Reactor/Proactor/drogon/crow）
│   └── 06_security/                            # ★新增: 漏洞类型（缓冲区溢出/UAF）/安全编码规范
│
├── 08_hpc/                                 # ─── ▲ 高性能计算（C++ 最显著优势区）───
│   ├── 01_simd/                                # SIMD/SSE/AVX2/AVX512/NEON 向量化
│   ├── 02_openmp/                              # OpenMP 共享内存并行（#pragma omp）
│   ├── 03_tbb/                                 # Intel TBB/parallel_for/pipeline/流图
│   ├── 04_gpu/                                 # CUDA/OpenCL/SYCL（含 thrust/CUB）
│   ├── 05_numerical/                           # 数值计算/线性代数/FFT/稀疏矩阵/ODE
│   └── 06_memory_opt/                          # 缓存友好/内存布局/NUMA/对象池/tcmalloc
│
├── 09_domain/                              # ─── ▲ 领域应用（最大，独立依赖重）───
│   ├── 01_game/                                # 游戏开发（碰撞物理/AI行为树/网络对战）
│   ├── 02_graphics/                            # 图形渲染（OpenGL/Vulkan/Metal/光追/PBR）
│   ├── 03_ai_inference/                        # AI 推理部署（ONNX Runtime/TensorRT/llama.cpp）
│   ├── 04_audio_video/                         # 音视频（FFmpeg API/WebRTC/GStreamer）
│   ├── 05_quant_trading/                       # 量化交易（超低延迟/行情处理/回测引擎）
│   └── 06_robotics/                            # 机器人（ROS2/DDS/传感器融合/SLAM）
│                                           #   其他领域来了就追加 07_ 08_ …
│
└── 10_ops/                                 # ─── ▲ 工程化（横切；只放"纯工程活动"）───
    ├── 01_cmake/                               # CMake（基础/target/generator/find_package/install/cpack）
    ├── 02_testing/                             # ctest 集成/代码覆盖率/模糊测试（libFuzzer/AFL）
    ├── 03_lint_format/                         # clang-tidy/clang-format/cppcheck/编译警告治理
    ├── 04_packaging/                           # vcpkg/conan/CPM/ExternalProject/FetchContent
    ├── 05_ci_cd/                               # GitHub Actions/Jenkins C++ CI 流水线
    ├── 06_debugging/                           # GDB/LLDB/AddressSanitizer/UBSan/Valgrind/rr
    ├── 07_profiling/                           # perf/VTune/Tracy/Instruments/gprof
    └── 08_cross_compile/                       # 交叉编译/嵌入式工具链/sysroot/QEMU 仿真
```

---

## 根级文件说明

| 文件 / 目录 | 作用 | 说明 |
|-------------|------|------|
| `README.md` | 总索引 | 每个系列一句话导航，新人从这里进 |
| `CMakeLists.txt` | **项目配置中心** | ① 项目元数据 ② `add_subdirectory` 统一管理 ③ 工具链 ④ 安装规则。**不绑定编译器** |
| `CMakePresets.json` | 构建变体预置 | `debug` / `release` / `asan` / `wasm` 等一键切换 |
| `vcpkg.json` | **依赖声明** | 换机后 `vcpkg install` 一键装；版本固定防漂移 |
| `.clang-format` | 统一格式 | 存档一套风格，`make fmt` 全格式化 |
| `.clang-tidy` | 静态分析规则 | 与 CI 打通，`make lint` 本地验证 |
| `.gitignore` | git 忽略 | `build/` / `.cache/` / `*.o` / `*.pdb` / 大二进制 |
| `Makefile` | **命令快捷表** | `make build/test/lint/clean/fmt`。只是命令运行器，不认语言、不做 C++ 构建 |
| `CONTRIBUTING.md` | 贡献约定 | 提交风格与 PR 规范 |
| `.github/workflows/` | CI | 推代码自动：安装依赖 → 构建 → 测试 → lint |
| `tests/` | 全库回归测试 | 跨模块的整体验证 |

> **关于 `CMakeLists.txt` vs `Makefile`**：
> `CMakeLists.txt` 是 **C++ 构建的核心**，控制编译/链接/安装；
> `Makefile` 只是 **命令别名**（`make test` → `cmake --build build && ctest`），两者共存，职责不重叠。

---

## 各大库速记

| 库 | 属性 | 一句话 |
|----|------|--------|
| `01_language` | 稳定 | C++ 语法本身，basics→oop→templates→modern→memory→expert 递进 |
| `02_algorithms` | 稳定 | ★数据结构/经典算法/设计模式（GoF in Modern C++）|
| `03_stl` | 稳定 | 装机自带，查手册最多的角落 |
| `04_libraries` | 缓慢增长 | vcpkg/conan 装完即用，跨领域工具库 |
| `05_frameworks` | 按领域增长 | ★领域级完整系统（vnpy/ROS2/flecs…），学架构+使用+二次开发；通用 web 框架在 backend |
| `06_extending` | 稳定 | C++ ↔ Python/Lua/Wasm/JNI 互操作，作为被调用方或调用方 |
| `07_systems` | 无限增长 | 系统编程：网络/IPC/POSIX/嵌入式/高性能服务 |
| `08_hpc` | 无限增长 | 高性能计算：SIMD/OpenMP/TBB/CUDA/数值/内存优化 |
| `09_domain` | 无限增长 | 领域应用：游戏/图形/AI推理/音视频/量化/机器人 |
| `10_ops` | 无限增长 | 工程化横切（纯工程活动）：CMake/调试/分析/包管理 |

---

## 贯穿全库的约定

1. **可运行 `.cpp` + `main()` 展示结果**；非代码演示（架构笔记/部署/复杂框架）用 `.md` 记录（如 ROS2、Vulkan 初始化）。
2. **文件顶部注释写明 C++ 标准版本 + 依赖**——三方库 API 变动大（如 Asio standalone vs Boost.Asio）。
3. **依赖统一写进 `vcpkg.json`**——换机 `vcpkg install` 全装。
4. **所有层级目录统一带两位数字前缀 `01_ 02_ ...`**——顺序即导航；插入新目录就重排序号，幅度大就整体重组。
5. **`04_libraries` 与 `10_ops` 分工**——"某个库怎么用"（含 cli/log/test 工具库）归 `04_libraries`；"纯工程活动/流程"（CMake 构建/CI/静态分析/交叉编译）归 `10_ops`，两边不重复。
6. **`05_frameworks` 只放领域级完整系统**——通用 web 框架（drogon/crow）在 `07_systems/05_server`；序列化/CLI 等工具库在 `04_libraries`。

---

## 扩展策略（负荷失控时）

当 `09_domain/` 将来某个子领域（如 `02_graphics/`）自己就长到几百文件、依赖几十 GB 时——**分拆为多仓库（polyrepo）**：

- `ccookbook` → 只保留 `language/algorithms/stl/libraries/extending`（**稳定核心**）
- `graphics-cookbook`、`robotics-cookbook`、`hpc-cookbook` → 各自独立 repo，独立依赖、独立 CI

**依赖差异大到相互累积时，就是该分的信号。**
