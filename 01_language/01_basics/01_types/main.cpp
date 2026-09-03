// C++20
// 演示：内置类型、字面量后缀、初始化方式、类型别名、const/constexpr/volatile
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>
#include <climits>    // INT_MAX / INT_MIN 等范围常量
#include <cfloat>     // FLT_MAX / DBL_MAX 等浮点范围常量
#include <cstdint>    // int8_t / uint32_t 等定宽整数类型
#include <string>

// ① 整数类型：大小、范围、符号
void demo01_integer_types()
{
    std::cout << "\n① 整数类型\n";

    // 知识点 1.1：有符号整数
    // 可以存负数，范围 -2^(n-1) ~ 2^(n-1)-1

    std::cout << "  --- 有符号整数 ---\n";
    std::cout << "  char:      " << sizeof(char) << " 字节  "
        << (int)CHAR_MIN << " ~ " << (int)CHAR_MAX << "\n";
    std::cout << "  short:     " << sizeof(short) << " 字节  "
        << SHRT_MIN << " ~ " << SHRT_MAX << "\n";
    std::cout << "  int:       " << sizeof(int) << " 字节  "
        << INT_MIN << " ~ " << INT_MAX << "\n";
    std::cout << "  long:      " << sizeof(long) << " 字节  "
        << LONG_MIN << " ~ " << LONG_MAX << "\n";
    std::cout << "  long long: " << sizeof(long long) << " 字节  "
        << LLONG_MIN << " ~ " << LLONG_MAX << "\n";

    // 知识点 1.2：无符号整数
    // 不能存负数，范围 0 ~ 2^n-1（正数范围翻倍）

    std::cout << "  --- 无符号整数 ---\n";
    std::cout << "  unsigned char:      " << sizeof(unsigned char)
        << " 字节  0 ~ " << (unsigned)UCHAR_MAX << "\n";
    std::cout << "  unsigned int:       " << sizeof(unsigned int)
        << " 字节  0 ~ " << UINT_MAX << "\n";
    std::cout << "  unsigned long long: " << sizeof(unsigned long long)
        << " 字节  0 ~ " << ULLONG_MAX << "\n";

    // 知识点 1.3：整数溢出

    unsigned char uc = 255;
    ++uc; // 无符号溢出：取模 256 = 0（合法的定义行为）
    std::cout << "  无符号溢出：255u+1 = " << (unsigned)uc
        << "（回绕到 0，取模 256，合法）\n";
}

// ② 浮点类型：精度与范围
void demo02_float_types()
{
    std::cout << "\n② 浮点类型\n";

    std::cout << "  float:  " << sizeof(float) << " 字节  精度约7位有效数字\n";
    std::cout << "  double: " << sizeof(double) << " 字节  精度约15位有效数字（推荐）\n";

    // 知识点 2.1：浮点精度问题
    // 不能用 == 比较
    // 原因：计算机用二进制存储小数，0.1、0.2 在二进制里是无限循环小数（类似 1/3 在十进制是 0.333...）
    // 存进有限位数时被截断，产生微小误差；两个误差相加后不等于 0.3 的截断值
    // 正确做法：比较两数之差是否足够小，如 std::abs(a - 0.3) < 1e-9

    double a = 0.1 + 0.2;
    std::cout << "  0.1+0.2 == 0.3 : " << (a == 0.3)
        << "（0=false！浮点有精度误差）\n";

    // 知识点 2.2：浮点特殊值

    float inf = FLT_MAX * 2.0F; // 溢出 → +∞
    float nan = 0.0F / 0.0F; // 0/0 → NaN
    std::cout << "  FLT_MAX*2 = " << inf << "（正无穷）\n";
    std::cout << "  0.0f/0.0f = " << nan << "（NaN，任何比较都返回 false）\n";
    std::cout << "  NaN==NaN  = " << (nan == nan) << "（NaN 不等于自身！）\n";
}

// ③ bool 和 char
void demo03_bool_char()
{
    std::cout << "\n③ bool 与 char\n";

    // 知识点 3.1：bool 类型

    std::cout << "  true=" << true << "  false=" << false << "（1/0）\n";
    std::cout << "  bool(42)=" << (bool)42 << "  bool(0)=" << (bool)0
        << "  bool(-1)=" << (bool)-1 << "（非零即 true）\n";

    // 知识点 3.2：char 类型
    // 字符本质是整数

    char c = 'A';
    std::cout << "  'A' = " << c << "  ASCII=" << (int)c << "\n";
    std::cout << "  'A'+1 = '" << (char)('A' + 1) << "'（B）\n";
    std::cout << "  'a'-'A' = " << ('a' - 'A') << "（大小写差32）\n";

    // 知识点 3.3：转义字符

    std::cout << "  转义：\\n换行 \\t制表 \\\\反斜杠 \\'单引号 \\\"双引号 \\0空字符\n";
}

// ④ 字面量：写法与后缀
void demo04_literals()
{
    std::cout << "\n④ 字面量写法\n";

    // 知识点 4.1：不同进制

    std::cout << "  255  = " << 255 << "（十进制）\n";
    std::cout << "  0xFF = " << 0xFF << "（十六进制）\n";
    std::cout << "  0377 = " << 0377 << "（八进制）\n";
    std::cout << "  0b11111111 = " << 0b11111111 << "（二进制，C++14）\n";

    // 知识点 4.2：数字分隔符（C++14）

    int million = 1'000'000;
    std::cout << "  1'000'000 = " << million << "（单引号分隔，增加可读性）\n";

    // 知识点 4.3：科学计数法

    std::cout << "  3.14e2 = " << 3.14e2 << "（= 314）\n";
    std::cout << "  1.5e-3 = " << 1.5e-3 << "（= 0.0015）\n";

    // 知识点 4.4：字面量后缀

    std::cout << "  42   -> int:       " << sizeof(42) << " 字节\n";
    std::cout << "  42LL -> long long: " << sizeof(42LL) << " 字节\n";
    std::cout << "  42U  -> uint:      " << sizeof(42U) << " 字节\n";
    std::cout << "  3.14F-> float:     " << sizeof(3.14F) << " 字节\n";
}

// ⑤ 初始化方式
void demo05_initialization()
{
    std::cout << "\n⑤ 初始化方式\n";

    int a = 10; // 拷贝初始化（C 风格）
    int b(20); // 直接初始化
    int c{30}; // 列表初始化（推荐，C++11）
    int d{}; // 值初始化（= 0）
    std::cout << "  a=" << a << "  b=" << b << "  c=" << c << "  d{}=" << d << "\n";

    // 知识点 5.1：列表初始化防窄化
    // int bad{3.14};  // 编译错误：double→int 窄化

    int bad = 3.14; // 允许但丢精度
    std::cout << "  int bad=3.14 -> " << bad << "（悄悄截断，{}会报错）\n";

    // 知识点 5.2：数组零初始化

    int arr[5]{};
    std::cout << "  int arr[5]{}: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "（全零）\n";

    // 知识点 5.3：结构体部分初始化

    struct Point
    {
        int x, y, z;
    };
    Point p{1, 2, 0}; // 显式写 0；省略时编译器同样补 0，但会触发 warning
    std::cout << "  Point{1,2}: x=" << p.x << " y=" << p.y
        << " z=" << p.z << "（未指定的补 0）\n";
}

// ⑥ const / constexpr / volatile
void demo06_qualifiers()
{
    std::cout << "\n⑥ const / constexpr / volatile\n";

    const int MAX = 100; // 运行时常量
    constexpr int SIZE = 10 * 10; // 编译期常量
    int arr[SIZE]; // constexpr 才保证可作数组大小
    std::cout << "  const MAX=" << MAX << "（不可修改，运行时确定）\n";
    std::cout << "  constexpr SIZE=" << SIZE << "（编译时确定，可作数组大小）\n";
    std::cout << "  arr 元素数=" << sizeof(arr) / sizeof(arr[0]) << "\n";

    volatile int reg = 0; // 禁止编译器缓存，用于硬件寄存器/多线程
    reg = 1;
    std::cout << "  volatile reg=" << reg << "（每次都从内存读写）\n";
}

// ⑦ 定宽整数类型（<cstdint>）
void demo07_fixed_width()
{
    std::cout << "\n⑦ 定宽整数类型（<cstdint>）\n";

    int8_t i8 = 127;
    int16_t i16 = 32767;
    int32_t i32 = 2147483647;
    int64_t i64 = 9223372036854775807LL;
    uint8_t u8 = 255;
    uint32_t u32 = 4294967295U;

    std::cout << "  int8_t  = " << (int)i8 << "（8位，精确）\n";
    std::cout << "  int16_t = " << i16 << "（16位，精确）\n";
    std::cout << "  int32_t = " << i32 << "（32位，精确）\n";
    std::cout << "  int64_t = " << i64 << "（64位，精确）\n";
    std::cout << "  uint8_t = " << (unsigned)u8 << "  uint32_t=" << u32 << "\n";
    std::cout << "  （用于网络协议/文件格式/跨平台，需要精确位数时用）\n";
}

// ⑧ 类型别名
void demo08_type_alias()
{
    std::cout << "\n⑧ 类型别名\n";

    using Score = int; // using（C++11，推荐）
    typedef double Price; // typedef（传统写法）

    Score s = 95;
    Price p = 9.99;
    std::cout << "  using Score=int;    Score s=" << s << "\n";
    std::cout << "  typedef double Price; Price p=" << p << "\n";

    // 知识点 8.1：size_t
    // 无符号整数，专门表示大小/索引

    size_t len = sizeof(int);
    std::cout << "  size_t len=sizeof(int)=" << len
        << "（size_t：无符号，专门表示大小和索引）\n";
}

int main()
{
    std::cout << "=== 01_types: 内置类型 ===";
    demo01_integer_types();
    demo02_float_types();
    demo03_bool_char();
    demo04_literals();
    demo05_initialization();
    demo06_qualifiers();
    demo07_fixed_width();
    demo08_type_alias();
    std::cout << "\n完成。\n";
    return 0;
}
