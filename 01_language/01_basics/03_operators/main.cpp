// C++20
// 演示：算术、比较、逻辑、位、赋值、自增自减、三目、逗号、运算符优先级
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>
#include <cmath>    // std::abs

// ① 算术运算符
void demo01_arithmetic()
{
    std::cout << "\n① 算术运算符\n";

    int a = 10, b = 3;
    std::cout << "  a+b=" << (a + b) << "  a-b=" << (a - b)
        << "  a*b=" << (a * b) << "\n";
    std::cout << "  a/b=" << (a / b) << "（整数截断：10/3=3）\n";
    std::cout << "  a%b=" << (a % b) << "（取余：10%3=1）\n";

    // 知识点 1.1：负数取余
    // 符号与被除数一致

    std::cout << "  -7%3=" << (-7 % 3) << "（符号同被除数，=-1）\n";
    std::cout << "  7%-3=" << (7 % -3) << "（符号同被除数，=1）\n";

    // 知识点 1.2：浮点除法
    // 至少一个浮点数才得浮点结果

    std::cout << "  10.0/3=" << (10.0 / 3) << "\n";

    // 知识点 1.3：整数提升
    // char/short 参与运算先提升为 int

    char c1 = 100, c2 = 100;
    std::cout << "  char(100)+char(100)=" << (c1 + c2)
        << "（结果是 int，不溢出）\n";
}

// ② 比较运算符
void demo02_comparison()
{
    std::cout << "\n② 比较运算符\n";

    int a = 5, b = 10;
    std::cout << "  " << a << "==" << b << ":" << (a == b)
        << "  " << a << "!=" << b << ":" << (a != b)
        << "  " << a << "<" << b << ":" << (a < b) << "\n";

    // 知识点 2.1：浮点比较陷阱

    double x = 0.1 + 0.2;
    std::cout << "  0.1+0.2==0.3: " << (x == 0.3)
        << "（false！用 |差值|<epsilon 比较）\n";
    std::cout << "  |0.1+0.2-0.3|<1e-9: " << (std::abs(x - 0.3) < 1e-9) << "\n";

    // 知识点 2.2：链式比较陷阱

    int n = 5;
    bool trap = (1 < n < 10); // 先 1<5=1，再 1<10=true，永远 true！
    bool correct = (1 < n && n < 10);
    std::cout << "  陷阱 (1<n<10)=" << trap
        << "  正确写法 (1<n&&n<10)=" << correct << "\n";
}

// ③ 逻辑运算符与短路求值
void demo03_logical()
{
    std::cout << "\n③ 逻辑运算符与短路求值\n";

    std::cout << "  真值表：T&&T=" << (true && true)
        << " T&&F=" << (true && false)
        << " T||F=" << (true || false)
        << " !T=" << (!true) << "\n";

    // 知识点 3.1：短路求值
    // 下面用 lambda（[&]() {...}）记录"右边是否被求值"——lambda 语法在 05_functions 章正式讲

    int counter = 0;
    auto inc = [&]() -> bool
    {
        ++counter;
        return true;
    };

    counter = 0;
    false && inc();
    std::cout << "  false && inc(): counter=" << counter << "（inc 未执行）\n";
    counter = 0;
    true || inc();
    std::cout << "  true  || inc(): counter=" << counter << "（inc 未执行）\n";

    // 知识点 3.2：短路求值防崩溃

    int* p = nullptr;
    int n = 0;
    bool r1 = p && (*p > 0); // p 空时不解引用
    bool r2 = n != 0 && 10 / n > 1; // n=0 时不除
    std::cout << "  nullptr&&(*p>0)=" << r1 << "（安全，不崩溃）\n";
    std::cout << "  0!=0&&10/0>1  =" << r2 << "（安全，不除零）\n";
}

// ④ 位运算符
void demo04_bitwise()
{
    std::cout << "\n④ 位运算符\n";

    unsigned a = 0b1100, b = 0b1010; // 12, 10
    std::cout << "  a=0b1100=12  b=0b1010=10\n";
    std::cout << "  a&b=" << (a & b) << "(0b1000=8 按位与)\n";
    std::cout << "  a|b=" << (a | b) << "(0b1110=14 按位或)\n";
    std::cout << "  a^b=" << (a ^ b) << "(0b0110=6 按位异或)\n";
    std::cout << "  a<<1=" << (a << 1) << "(左移=乘2)\n";
    std::cout << "  a>>1=" << (a >> 1) << "(右移=除2)\n";

    // 知识点 4.1：标志位操作

    enum Flags : unsigned { READ = 1 << 0, WRITE = 1 << 1, EXEC = 1 << 2 };
    unsigned perm = READ | WRITE;
    std::cout << "  权限 READ|WRITE=" << perm << "\n";
    std::cout << "  有写权? " << (bool)(perm & WRITE) << "\n";
    perm &= ~WRITE; // 移除写权限
    std::cout << "  移除WRITE后=" << perm << "  有写权? " << (bool)(perm & WRITE) << "\n";
}

// ⑤ 自增自减
void demo05_increment()
{
    std::cout << "\n⑤ 自增自减\n";

    int a = 5;
    int b = ++a; // 前缀：先加1，b得到加后的值
    std::cout << "  b=++a -> a=" << a << " b=" << b << "（前缀：先加再用）\n";

    int c = a++; // 后缀：先用，c得到加前的值，再加
    std::cout << "  c=a++ -> a=" << a << " c=" << c << "（后缀：先用再加）\n";

    int d = --a;
    std::cout << "  d=--a -> a=" << a << " d=" << d << "（前缀减）\n";
    int e = a--;
    std::cout << "  e=a-- -> a=" << a << " e=" << e << "（后缀减）\n";

    std::cout << "  性能：++i 比 i++ 快（对迭代器等非基本类型，i++ 需临时拷贝）\n";
}

// ⑥ 三目运算符
void demo06_ternary()
{
    std::cout << "\n⑥ 三目运算符\n";

    int a = 8, b = 3;
    std::cout << "  max(8,3)=" << ((a > b) ? a : b) << "\n";

    // 知识点 6.1：用三目运算符初始化 const（if-else 不能做到）

    const int score = 85;
    const std::string grade = score >= 90 ? "优" : score >= 75 ? "良" : score >= 60 ? "及格" : "不及格";
    std::cout << "  score=" << score << " -> " << grade << "\n";

    // 知识点 6.2：两分支类型不同时会提升

    auto x = true ? 1 : 3.14; // int 提升为 double
    std::cout << "  true?1:3.14 -> " << sizeof(x) << "字节(double)\n";
}

// ⑦ 运算符优先级
void demo07_precedence()
{
    std::cout << "\n⑦ 运算符优先级（从高到低）\n";
    std::cout << "  1. ()[]->.\n";
    std::cout << "  2. 一元前缀: ++ -- ~ ! + -\n";
    std::cout << "  3. 乘除取余: * / %\n";
    std::cout << "  4. 加减: + -\n";
    std::cout << "  5. 位移: << >>\n";
    std::cout << "  6. 比较: < <= > >=\n";
    std::cout << "  7. 相等: == !=\n";
    std::cout << "  8-10. 位: & ^ |\n";
    std::cout << "  11-12. 逻辑: && ||\n";
    std::cout << "  13. 三目: ?:\n";
    std::cout << "  14. 赋值: = += -= ...\n";

    std::cout << "  2+3*4   =" << (2 + 3 * 4) << "（*先）\n";
    std::cout << "  (2+3)*4 =" << ((2 + 3) * 4) << "（括号改变）\n";

    // 知识点 7.1：位运算优先级陷阱
    // 位运算优先级低于比较

    std::cout << "  陷阱 1|2==3=" << (1 | 2 == 3)
        << "（先算2==3=0，再1|0=1；应写(1|2)==3=" << ((1 | 2) == 3) << "）\n";
    std::cout << "  原则：不确定时加括号，让意图清晰\n";
}

// ⑧ 逗号运算符
void demo08_comma()
{
    std::cout << "\n⑧ 逗号运算符\n";

    int a = (1, 2, 3); // 从左到右求值，整体值是最后一个
    std::cout << "  a=(1,2,3)=" << a << "（整体值是最后一个）\n";

    std::cout << "  for(i=0,j=10;i<3;++i,--j): ";
    for (int i = 0, j = 10; i < 3; ++i, --j)
        std::cout << "(" << i << "," << j << ") ";
    std::cout << "\n";
}

int main()
{
    std::cout << "=== 03_operators: 运算符 ===";
    demo01_arithmetic();
    demo02_comparison();
    demo03_logical();
    demo04_bitwise();
    demo05_increment();
    demo06_ternary();
    demo07_precedence();
    demo08_comma();
    std::cout << "\n完成。\n";
    return 0;
}
