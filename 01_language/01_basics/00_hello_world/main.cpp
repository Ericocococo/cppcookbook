// C++20
// 演示：程序结构——#include、main、变量声明、cout 打印、注释
// 构建：cmake -B build-mingw（详见 README.md）

// #include：引入工具箱。不写这行就用不了 cout / string
// iostream = input/output stream（输入输出流），提供 cout / cin
#include <iostream>
// string 工具箱，提供 std::string 字符串类型
#include <string>

// ① 程序入口：每个程序有且只有一个 main，操作系统从这里开始执行
// int 表示 main 返回一个整数；return 0 表示"正常结束"
void demo01_structure() {
    std::cout << "\n① 程序结构说明\n";
    std::cout << "  #include <iostream> ——引入输入输出工具箱\n";
    std::cout << "  int main()          ——程序入口，有且只有一个\n";
    std::cout << "  return 0;           ——正常退出，返回0给操作系统\n";
    std::cout << "  ;                   ——每条语句结尾必须有分号\n";
}

// ② 注释：给人看的说明，编译器完全忽略
void demo02_comments() {
    std::cout << "\n② 注释\n";

    // 这是单行注释：// 到行尾都被忽略
    std::cout << "  // 单行注释：// 到行尾都被忽略\n";

    /* 这是多行注释
       可以跨越多行 */
    std::cout << "  /* 多行注释：从 /* 到 */ 之间都被忽略 */\n";
}

// ③ 变量：给数据起名字，方便后续使用
// 格式：类型 变量名 = 初始值;
void demo03_variables() {
    std::cout << "\n③ 变量声明（类型 变量名 = 初始值;）\n";

    int    age    = 18;          // int：整数
    double height = 1.75;        // double：小数
    bool   passed = true;        // bool：只有 true / false
    char   grade  = 'A';         // char：单个字符，用单引号
    std::string name = "小白";   // string：一段文字，用双引号

    std::cout << "  int    age    = " << age    << "\n";
    std::cout << "  double height = " << height << "\n";
    std::cout << "  bool   passed = " << passed << "  （1=true，0=false）\n";
    std::cout << "  char   grade  = " << grade  << "\n";
    std::cout << "  string name   = " << name   << "\n";
}

// ④ cout：向终端打印内容
// std:: 表示"去标准库里找"；cout = console output（控制台输出）
// << 是"流入"运算符，把右边的内容送进输出流
void demo04_cout() {
    std::cout << "\n④ cout 打印\n";

    int x = 42;
    std::string msg = "Hello";

    // 打印文字
    std::cout << "  " << msg << "\n";

    // 打印变量
    std::cout << "  x = " << x << "\n";

    // 拼接多个内容：链式 << 依次输出
    std::cout << "  " << msg << ", x=" << x << "!\n";

    // "\n" 换行（推荐）；std::endl 也换行但额外刷新缓冲区（慢一点）
    std::cout << "  第一行\n";
    std::cout << "  第二行\n";
}

// ⑤ 大括号 {}：划定"作用域"，变量只在自己的 {} 内有效
void demo05_scope() {
    std::cout << "\n⑤ 大括号与作用域\n";

    int outer = 10;
    std::cout << "  外层 outer = " << outer << "\n";

    {
        // 内层 {} 里声明的变量，出了 {} 就消失
        int inner = 20;
        std::cout << "  内层 inner = " << inner << "（只在这个{}里有效）\n";
        std::cout << "  内层也能访问外层 outer = " << outer << "\n";
    }
    // 这里 inner 已不存在，访问会编译错误
    // std::cout << inner;  // 错误！inner 已超出作用域

    std::cout << "  出了{}后，outer 仍然存在 = " << outer << "\n";
}

int main() {
    std::cout << "=== 00_hello_world: 程序结构 ===";
    demo01_structure();
    demo02_comments();
    demo03_variables();
    demo04_cout();
    demo05_scope();
    std::cout << "\n完成。\n";
    return 0;
}
