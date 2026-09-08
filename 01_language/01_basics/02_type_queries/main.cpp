// C++20
// 演示：sizeof（大小）、alignof（对齐）、decltype（类型推断）、auto 深入、结构体填充
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>
#include <string>
#include <cstdint>    // uint8_t / uint32_t

// ① sizeof：查字节大小，编译期求值，不执行表达式
void demo01_sizeof()
{
    std::cout << "\n① sizeof\n";

    // 知识点 1.1：基本类型大小

    std::cout << "  sizeof(bool)      = " << sizeof(bool) << "\n";
    std::cout << "  sizeof(char)      = " << sizeof(char) << "\n";
    std::cout << "  sizeof(int)       = " << sizeof(int) << "\n";
    std::cout << "  sizeof(long long) = " << sizeof(long long) << "\n";
    std::cout << "  sizeof(double)    = " << sizeof(double) << "\n";
    std::cout << "  sizeof(void*)     = " << sizeof(void*)
        << "（指针大小 = 地址总线宽度，64位系统=8字节）\n";

    // 知识点 1.2：sizeof 查变量
    // 不执行表达式！

    int n = 0;
    sizeof(++n); // ++n 不会执行，n 仍为 0
    std::cout << "  sizeof(++n) 不执行 ++n，n 仍为 " << n << "\n";

    // 知识点 1.3：sizeof 查数组

    int arr[7];
    std::cout << "  int arr[7]: sizeof=" << sizeof(arr)
        << "  元素数=" << sizeof(arr) / sizeof(arr[0]) << "\n";

    // 知识点 1.4：指针和数组的区别

    int* p = arr;
    std::cout << "  int* p=arr: sizeof(p)=" << sizeof(p)
        << "（指针大小，不是数组大小！）\n";
    std::cout << "  传数组给函数后，数组退化为指针，sizeof 就只有指针大小了\n";
}

// ② alignof + 结构体内存对齐与填充字节
void demo02_alignof_padding()
{
    std::cout << "\n② alignof 与结构体填充\n";

    std::cout << "  alignof(char)      = " << alignof(char) << "\n";
    std::cout << "  alignof(short)     = " << alignof(short) << "\n";
    std::cout << "  alignof(int)       = " << alignof(int) << "\n";
    std::cout << "  alignof(double)    = " << alignof(double) << "\n";
    std::cout << "  （含义：该类型必须放在几的倍数内存地址上）\n";

    // 知识点 2.1：结构体填充
    // 字段顺序不同，大小不同！

    struct Bad
    {
        // 顺序不好
        char a; // 1字节，偏移0
        // ← 3字节填充（为了让 b 对齐到4的倍数）
        int b; // 4字节，偏移4
        char c; // 1字节，偏移8
        // ← 3字节填充（结构体总大小必须是最大对齐数的倍数）
    }; // 总计 12 字节

    struct Good
    {
        // 顺序好（大字段放前面）
        int b; // 4字节，偏移0
        char a; // 1字节，偏移4
        char c; // 1字节，偏移5
        // ← 2字节填充
    }; // 总计 8 字节

    std::cout << "  struct{char,int,char}: sizeof=" << sizeof(Bad)
        << "（字段顺序差，浪费 " << sizeof(Bad) - (1 + 4 + 1) << " 字节填充）\n";
    std::cout << "  struct{int,char,char}: sizeof=" << sizeof(Good)
        << "（字段顺序好，浪费 " << sizeof(Good) - (4 + 1 + 1) << " 字节填充）\n";
    std::cout << "  建议：按对齐从大到小排列字段，减少填充浪费\n";

    // 知识点 2.2：alignas
    // 手动指定对齐要求（C++11）

    struct alignas(16) Vec4
    {
        // 强制16字节对齐（SIMD指令需要）
        float x, y, z, w;
    };
    std::cout << "  alignas(16) Vec4{float*4}: sizeof=" << sizeof(Vec4)
        << "  alignof=" << alignof(Vec4) << "（SIMD 要求16字节对齐）\n";
}

// ③ auto：让编译器推断类型
void demo03_auto()
{
    std::cout << "\n③ auto（类型推断）\n";

    // 知识点 3.1：基本推断

    auto i = 42; // int
    auto d = 3.14; // double
    auto c = 'A'; // char
    auto b = true; // bool
    auto s = std::string("hello"); // std::string

    std::cout << "  auto i=42     -> " << sizeof(i) << "字节(int)\n";
    std::cout << "  auto d=3.14   -> " << sizeof(d) << "字节(double)\n";
    std::cout << "  auto c='A'    -> " << sizeof(c) << "字节(char)\n";
    std::cout << "  auto s=string -> " << s << "\n";

    // 知识点 3.2：auto 剥掉 const 和引用

    const int ci = 10;
    auto v1 = ci; // v1 是 int（剥掉了 const）
    v1 = 99; // 可以修改，说明 const 被剥掉了
    std::cout << "  auto v1=ci(const int): v1=" << v1
        << "（const 被剥掉，可修改）\n";

    // 知识点 3.3：保留 const/引用要显式写

    const auto v2 = ci; // const int
    auto& v3 = ci; // const int&（引用的 const 保留）
    // v2 = 99;             // 错误
    // v3 = 99;             // 错误
    std::cout << "  const auto v2=ci: 不可修改\n";
    std::cout << "  auto& v3=ci:      引用，不可修改\n";

    // 知识点 3.4：auto 在范围 for 中的用法
    // 范围 for 的语法在 04_control_flow 章正式讲，这里只演示 auto 的三种写法

    int arr[] = {1, 2, 3, 4, 5};
    std::cout << "  auto  遍历（副本）: ";
    for (auto x : arr) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // auto&：引用遍历，修改 x 会直接改原数组元素
    for (auto& x : arr) {
        x *= 10;
    }
    std::cout << "  auto& 遍历后原数组: ";
    for (auto x : arr) {
        std::cout << x << " ";
    }
    std::cout << "（×10，说明 auto& 改的就是原数组）\n";

    // const auto&：只读引用，不复制也不修改（大对象遍历推荐）
    std::cout << "  const auto& 遍历: ";
    for (const auto& x : arr) {
        std::cout << x << " ";
    }
    std::cout << "（只读，改不了）\n";
}

// ④ decltype：推断类型，保留 const 和引用，不执行表达式
void demo04_decltype()
{
    std::cout << "\n④ decltype（推断类型，保留 const/引用）\n";

    int a = 5;
    double b = 3.14;

    decltype(a) x = 10; // int
    decltype(b) y = 1.5; // double
    decltype(a + b) z = 8.14; // double（int+double推断为double）

    std::cout << "  decltype(a)     x -> " << sizeof(x) << "字节(int)\n";
    std::cout << "  decltype(b)     y -> " << sizeof(y) << "字节(double)\n";
    std::cout << "  decltype(a+b)   z -> " << sizeof(z) << "字节(double)\n";

    // 知识点 4.1：decltype vs auto 的关键区别
    // const 和引用的保留

    const int ci = 42;
    int& ri = a;

    auto av = ci; // int（剥掉 const）
    decltype(ci) dv = ci; // const int（保留 const）

    auto ar = ri; // int（剥掉引用，拷贝）
    decltype(ri) dr = a; // int&（保留引用）

    std::cout << "  const int ci=42:\n";
    std::cout << "    auto       av=ci -> int      (剥掉const，可改)\n";
    std::cout << "    decltype   dv=ci -> const int (保留const，不可改)\n";
    std::cout << "  int& ri=a:\n";
    std::cout << "    auto       ar=ri -> int  (拷贝，改 ar 不影响 a)\n";
    std::cout << "    decltype   dr=a  -> int& (引用，改 dr 影响 a)\n";
    dr = 999;
    std::cout << "    dr=999 后，a=" << a << "（dr 是引用，修改了 a）\n";
    (void)av;
    (void)dv;
    (void)ar;

    // 知识点 4.2：decltype(auto)（C++14）
    // 兼具 auto 的简洁和 decltype 的保留能力

    decltype(auto) da = ci; // const int（完整保留）
    std::cout << "  decltype(auto) da=ci -> const int（C++14，完整保留）\n";
    (void)da;
}

// ⑤ 实际使用场景
void demo05_practical()
{
    std::cout << "\n⑤ 实际使用场景\n";

    // 知识点 5.1：用 sizeof 计算数组元素数（C 风格的安全做法）

    int arr[] = {10, 20, 30, 40, 50};
    int len = sizeof(arr) / sizeof(arr[0]);
    std::cout << "  数组元素数 = " << len << "\n";

    // 知识点 5.2：decltype 推断函数返回类型（泛型编程）
    // 这里的 lambda（[](auto a, auto b) {...}）与泛型参数语法在 05_functions 章正式讲，
    // 此处只演示 decltype 的用途：根据参数类型推断返回类型

    auto add = [](auto a, auto b) -> decltype(a + b)
    {
        return a + b;
    };
    std::cout << "  add(1, 2.5) = " << add(1, 2.5)
        << "（decltype 推断返回 double）\n";

    // 知识点 5.3：验证结构体对齐

    struct Msg
    {
        uint8_t type; // 1字节
        uint32_t length; // 4字节
        uint8_t data[3]; // 3字节
    };
    std::cout << "  网络消息结构体 sizeof=" << sizeof(Msg)
        << "（含填充字节，序列化前需注意）\n";
}

int main()
{
    std::cout << "=== 02_type_queries: 类型查询运算符 ===";
    demo01_sizeof();
    demo02_alignof_padding();
    demo03_auto();
    demo04_decltype();
    demo05_practical();
    std::cout << "\n完成。\n";
    return 0;
}
