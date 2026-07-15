// C++20
// 演示：算术、比较、逻辑、位、赋值、自增自减、三目、sizeof 运算符
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>

// ① 算术运算符：加减乘除取余
void demo01_arithmetic() {
    std::cout << "\n① 算术运算符\n";

    int a = 10, b = 3;
    std::cout << "  a=" << a << ", b=" << b << "\n";
    std::cout << "  a + b = " << (a + b) << "\n";
    std::cout << "  a - b = " << (a - b) << "\n";
    std::cout << "  a * b = " << (a * b) << "\n";

    // 整数除法：结果也是整数，小数部分直接丢弃（不是四舍五入）
    std::cout << "  a / b = " << (a / b) << "  （整数除法：10/3=3，小数丢弃）\n";
    std::cout << "  a % b = " << (a % b) << "  （取余：10除3余1）\n";

    // 想得到小数结果，至少有一个操作数是浮点数
    std::cout << "  (double)a / b = " << (static_cast<double>(a) / b)
              << "  （强制转为 double 再除）\n";
}

// ② 比较运算符：结果只有 true(1) 或 false(0)
void demo02_comparison() {
    std::cout << "\n② 比较运算符\n";

    int a = 5, b = 10;
    std::cout << "  a=" << a << ", b=" << b << "\n";
    std::cout << "  a == b  （等于）: " << (a == b) << "\n";
    std::cout << "  a != b  （不等）: " << (a != b) << "\n";
    std::cout << "  a <  b  （小于）: " << (a <  b) << "\n";
    std::cout << "  a >  b  （大于）: " << (a >  b) << "\n";
    std::cout << "  a <= b （小等于）: " << (a <= b) << "\n";
    std::cout << "  a >= b （大等于）: " << (a >= b) << "\n";
    std::cout << "  （0=false，1=true）\n";
}

// ③ 逻辑运算符：组合多个条件
void demo03_logical() {
    std::cout << "\n③ 逻辑运算符\n";

    bool t = true, f = false;
    // &&（与）：两边都为 true 才是 true
    std::cout << "  true  && true  = " << (t && t) << "  （都真才真）\n";
    std::cout << "  true  && false = " << (t && f) << "  （有假则假）\n";
    // ||（或）：有一个 true 就是 true
    std::cout << "  true  || false = " << (t || f) << "  （有真则真）\n";
    std::cout << "  false || false = " << (f || f) << "  （都假才假）\n";
    // !（非）：取反
    std::cout << "  !true          = " << (!t)     << "\n";
    std::cout << "  !false         = " << (!f)     << "\n";

    // 短路求值：&& 左边是 false，右边不会执行（避免无意义或危险的计算）
    int n = 0;
    // n != 0 为 false，所以 10/n 不会执行，不会除以零
    bool safe = (n != 0) && (10 / n > 1);
    std::cout << "  (n!=0) && (10/n>1) = " << safe
              << "  （短路：左边false则右边不执行，避免除零）\n";
}

// ④ 位运算符：直接操作二进制位
void demo04_bitwise() {
    std::cout << "\n④ 位运算符（操作二进制位）\n";

    // 0b 前缀表示二进制字面量（C++14）
    unsigned int a = 0b1100;   // 十进制 12
    unsigned int b = 0b1010;   // 十进制 10
    std::cout << "  a = 0b1100 = " << a << "\n";
    std::cout << "  b = 0b1010 = " << b << "\n";

    // & 按位与：同位都是1才是1（用于检查/清除某个位）
    std::cout << "  a & b  = " << (a & b)  << "  （0b1000=8，同位都1才1）\n";
    // | 按位或：同位有1就是1（用于设置某个位）
    std::cout << "  a | b  = " << (a | b)  << "  （0b1110=14，有1就1）\n";
    // ^ 按位异或：同位不同才是1（用于翻转某个位）
    std::cout << "  a ^ b  = " << (a ^ b)  << "  （0b0110=6，不同才1）\n";
    // << 左移：相当于乘以 2 的 n 次方
    std::cout << "  a << 1 = " << (a << 1) << "  （左移1位=乘2：12*2=24）\n";
    // >> 右移：相当于除以 2 的 n 次方
    std::cout << "  a >> 1 = " << (a >> 1) << "  （右移1位=除2：12/2=6）\n";
}

// ⑤ 赋值与复合赋值
void demo05_assignment() {
    std::cout << "\n⑤ 赋值与复合赋值\n";

    int a = 10;
    std::cout << "  初始 a = " << a << "\n";

    // 复合赋值：a op= b  等价于  a = a op b
    a += 5;  std::cout << "  a += 5  -> a = " << a << "\n";
    a -= 3;  std::cout << "  a -= 3  -> a = " << a << "\n";
    a *= 2;  std::cout << "  a *= 2  -> a = " << a << "\n";
    a /= 4;  std::cout << "  a /= 4  -> a = " << a << "\n";
    a %= 3;  std::cout << "  a %= 3  -> a = " << a << "\n";
}

// ⑥ 自增自减：注意前缀和后缀的区别
void demo06_increment() {
    std::cout << "\n⑥ 自增自减\n";

    int a = 5;
    std::cout << "  初始 a = " << a << "\n";

    // 前缀 ++a：先加1，再使用（b得到加1后的值）
    int b = ++a;
    std::cout << "  b = ++a  ->  a=" << a << "  b=" << b << "  （先加再用）\n";

    // 后缀 a++：先使用当前值（c得到加1前的值），再加1
    int c = a++;
    std::cout << "  c = a++  ->  a=" << a << "  c=" << c << "  （先用再加）\n";

    // 减法同理
    int d = --a;
    std::cout << "  d = --a  ->  a=" << a << "  d=" << d << "  （先减再用）\n";
    int e = a--;
    std::cout << "  e = a--  ->  a=" << a << "  e=" << e << "  （先用再减）\n";
}

// ⑦ 三目运算符：简化 if-else
void demo07_ternary() {
    std::cout << "\n⑦ 三目运算符：条件 ? 真值 : 假值\n";

    int a = 8, b = 3;
    // 等价于：if(a>b) max=a; else max=b;
    int max = (a > b) ? a : b;
    std::cout << "  a=" << a << " b=" << b << "\n";
    std::cout << "  (a>b) ? a : b = " << max << "（取较大值）\n";

    int score = 85;
    // 根据分数判断等级
    std::string grade = (score >= 90) ? "优" : (score >= 60) ? "及格" : "不及格";
    std::cout << "  score=" << score << " -> " << grade << "\n";
}

// ⑧ sizeof：查询类型或变量占多少字节
void demo08_sizeof() {
    std::cout << "\n⑧ sizeof（单位：字节，1字节=8位）\n";

    std::cout << "  sizeof(bool)      = " << sizeof(bool)      << "\n";
    std::cout << "  sizeof(char)      = " << sizeof(char)      << "\n";
    std::cout << "  sizeof(short)     = " << sizeof(short)     << "\n";
    std::cout << "  sizeof(int)       = " << sizeof(int)       << "\n";
    std::cout << "  sizeof(long long) = " << sizeof(long long) << "\n";
    std::cout << "  sizeof(float)     = " << sizeof(float)     << "\n";
    std::cout << "  sizeof(double)    = " << sizeof(double)    << "\n";

    // 数组：sizeof 返回整个数组的字节数
    int arr[5];
    std::cout << "  sizeof(int[5])    = " << sizeof(arr) << "（整个数组）\n";
    std::cout << "  元素个数          = " << sizeof(arr)/sizeof(arr[0]) << "\n";
}

int main() {
    std::cout << "=== 02_operators: 运算符 ===";
    demo01_arithmetic();
    demo02_comparison();
    demo03_logical();
    demo04_bitwise();
    demo05_assignment();
    demo06_increment();
    demo07_ternary();
    demo08_sizeof();
    std::cout << "\n完成。\n";
    return 0;
}
