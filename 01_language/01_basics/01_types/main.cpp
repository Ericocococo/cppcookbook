// C++20
// 演示：内置数据类型、字面量后缀、auto/decltype、const/constexpr
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>
#include <climits>   // INT_MAX / INT_MIN 等范围常量

// ① 基本类型：每种类型能存什么、占多少内存
void demo01_basic_types() {
    std::cout << "\n① 基本类型大小与取值\n";

    // bool：只有 true(1) 和 false(0) 两个值
    bool flag = true;
    std::cout << "  bool   : " << sizeof(bool)   << " 字节  值=" << flag << "\n";

    // char：存一个字符，本质是一个小整数（ASCII 码）
    char ch = 'A';
    std::cout << "  char   : " << sizeof(char)   << " 字节  值=" << ch
              << "  ASCII码=" << static_cast<int>(ch) << "\n";

    // int：最常用的整数类型
    int n = 42;
    std::cout << "  int    : " << sizeof(int)    << " 字节  值=" << n << "\n";
    std::cout << "    int 最大值=" << INT_MAX << "  最小值=" << INT_MIN << "\n";

    // long long：需要存非常大的整数时用
    long long big = 9000000000LL;   // LL 后缀表示 long long 字面量
    std::cout << "  long long: " << sizeof(long long) << " 字节  值=" << big << "\n";

    // double：小数，精度比 float 高
    double pi = 3.141592653589793;
    std::cout << "  double : " << sizeof(double) << " 字节  值=" << pi << "\n";

    // float：精度较低的小数，占内存小
    float pif = 3.14F;             // F 后缀表示 float 字面量
    std::cout << "  float  : " << sizeof(float)  << " 字节  值=" << pif << "\n";
}

// ② 字面量后缀：告诉编译器这个数字是什么类型
void demo02_literals() {
    std::cout << "\n② 字面量后缀\n";

    // 没有后缀时，整数默认是 int，小数默认是 double
    auto a = 42;      // int
    auto b = 42LL;    // long long（加 LL）
    auto c = 42U;     // unsigned int（加 U，不能存负数，但正数范围翻倍）
    auto d = 3.14;    // double
    auto e = 3.14F;   // float（加 F）

    std::cout << "  42    -> int:          " << sizeof(a) << " 字节\n";
    std::cout << "  42LL  -> long long:    " << sizeof(b) << " 字节\n";
    std::cout << "  42U   -> unsigned int: " << sizeof(c) << " 字节\n";
    std::cout << "  3.14  -> double:       " << sizeof(d) << " 字节\n";
    std::cout << "  3.14F -> float:        " << sizeof(e) << " 字节\n";

    // 不同进制的整数字面量
    int hex = 0xFF;    // 0x 开头 = 十六进制，0xFF = 255
    int bin = 0b1010;  // 0b 开头 = 二进制（C++14），0b1010 = 10
    std::cout << "  0xFF   = " << hex << "（十六进制）\n";
    std::cout << "  0b1010 = " << bin << "（二进制）\n";
}

// ③ 初始化方式：C++ 有多种写法，推荐用 {}
void demo03_initialization() {
    std::cout << "\n③ 初始化方式\n";

    int a = 10;    // 拷贝初始化（最传统的写法，C 语言风格）
    int b(20);     // 直接初始化
    int c{30};     // 列表初始化（C++11，推荐）—— 更安全，防止精度丢失

    std::cout << "  int a = 10  -> " << a << "（拷贝初始化）\n";
    std::cout << "  int b(20)   -> " << b << "（直接初始化）\n";
    std::cout << "  int c{30}   -> " << c << "（列表初始化，推荐）\n";

    // 列表初始化的安全性：会拒绝会丢失精度的转换
    // int d{3.14};  // 编译错误！double -> int 会丢失精度，列表初始化不允许
    int d = 3.14;    // 这行编译通过，但悄悄丢掉了小数部分（不安全）
    std::cout << "  int d = 3.14 -> " << d << "（悄悄丢失了小数，列表初始化{}会报错）\n";
}

// ④ auto：让编译器自动推断类型，不用手写
void demo04_auto() {
    std::cout << "\n④ auto 类型推断\n";

    // 编译器根据右边的值推断变量类型
    auto i = 10;          // int
    auto d = 3.14;        // double
    auto c = 'A';         // char
    auto b = true;        // bool
    auto s = std::string("hello");  // std::string

    std::cout << "  auto i = 10   -> " << sizeof(i) << " 字节 (int),    值=" << i << "\n";
    std::cout << "  auto d = 3.14 -> " << sizeof(d) << " 字节 (double), 值=" << d << "\n";
    std::cout << "  auto c = 'A'  -> " << sizeof(c) << " 字节 (char),   值=" << c << "\n";
    std::cout << "  auto b = true -> " << sizeof(b) << " 字节 (bool),   值=" << b << "\n";
    std::cout << "  auto s = string(\"hello\") -> 值=" << s << "\n";

    // decltype：根据表达式推断类型，但不执行表达式
    int x = 5;
    decltype(x) y = 100;         // y 和 x 类型相同（int）
    decltype(x + 0.5) z = 1.5;   // x+0.5 是 double，所以 z 是 double
    std::cout << "  decltype(x) y    -> " << sizeof(y) << " 字节 (int)\n";
    std::cout << "  decltype(x+0.5) z-> " << sizeof(z) << " 字节 (double)\n";
}

// ⑤ const 与 constexpr：不允许修改的变量
void demo05_const_constexpr() {
    std::cout << "\n⑤ const 与 constexpr\n";

    // const：运行时常量，值不能修改
    const int MAX_SCORE = 100;
    // MAX_SCORE = 200;  // 编译错误：const 变量不能修改
    std::cout << "  const int MAX_SCORE = " << MAX_SCORE << "（不可修改）\n";

    // constexpr：编译期常量，编译时就计算好了，效率更高
    // 用于需要编译期已知值的场景（数组长度、模板参数等）
    constexpr int BOARD_SIZE = 8 * 8;  // 编译时计算，结果是 64
    std::cout << "  constexpr int BOARD_SIZE = " << BOARD_SIZE << "（编译时算好）\n";

    // constexpr 可以直接用作数组大小
    int board[BOARD_SIZE];
    std::cout << "  int board[BOARD_SIZE] 大小=" << sizeof(board)/sizeof(board[0]) << "\n";
}

int main() {
    std::cout << "=== 01_types: 数据类型 ===";
    demo01_basic_types();
    demo02_literals();
    demo03_initialization();
    demo04_auto();
    demo05_const_constexpr();
    std::cout << "\n完成。\n";
    return 0;
}
