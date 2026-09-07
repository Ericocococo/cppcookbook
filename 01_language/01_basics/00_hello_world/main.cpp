// C++20
// 演示：程序结构、#include、main、变量、cout、注释、作用域、基本输入
// 构建：cmake -B build-mingw-ninja（详见 README.md）

// 知识点 1.1：#include 引入工具箱
// 把头文件内容复制进来，不写就用不了里面的东西，编译器报"未定义"错误
// #include <iostream> 之后才能用 std::cout，#include <string> 之后才能用 std::string

// 知识点 1.2：std 命名空间
// std 是标准库的命名空间（namespace），所有标准库的东西都装在 std 里面

// 知识点 1.3：标准库常用头文件
// 标准库（Standard Library）：C++ 自带的工具箱，不用额外安装，#include 就能用
//   <iostream>    → cout/cin/cerr（打印和读输入）
//   <string>      → std::string（字符串）
//   <vector>      → std::vector（动态数组，用得最多）
//   <map>         → std::map（键值对，类似 Python dict）
//   <algorithm>   → sort/find/count_if（算法）
//   <cmath>       → sqrt/sin/pow（数学函数）
//   <memory>      → unique_ptr/shared_ptr（智能指针）
//   <functional>  → std::function（存函数/lambda）
//   <fstream>     → ifstream/ofstream（读写文件）
//   <chrono>      → 时间点/时长（计时）

#include <iostream>   // 提供 cout（打印）、cin（读输入）、cerr（错误输出）
#include <string>     // 提供 std::string 字符串类型

// ① 程序结构
void demo01_structure()
{
    std::cout << "\n① 程序结构\n";
    std::cout << "  #include <iostream>  → 引入输入输出工具箱\n";
    std::cout << "  int main()           → 程序入口，有且只有一个\n";
    std::cout << "  return 0;            → 正常退出，返回 0 给操作系统\n";
    std::cout << "  ;（分号）            → 每条语句结束符，缺少会报编译错误\n";
    std::cout << "  {}（花括号）         → 划定作用域边界\n";
}

// ② 注释：给人看的说明，编译器完全忽略
void demo02_comments()
{
    std::cout << "\n② 注释\n";

    // 知识点 2.1：单行注释
    // // 到行尾的内容都被编译器忽略，只给人看

    std::cout << "  // 单行注释：从 // 到行尾都被忽略\n";

    /* 多行注释
       可以跨越多行
    */

    // 知识点 2.2：多行注释
    // /* 到 */ 之间全被忽略，可以跨多行

    std::cout << "  /* 多行注释 */：从 /* 到 */ 之间全被忽略\n";

    // 知识点 2.3：注释写什么
    // 好注释写"为什么"，不重复代码已经表达的"做了什么"
    // int speed = 60;    // 不好：设置速度为60（代码已经说了）
    // int speed = 60;    // 好：限速 60 km/h，超过触发警报（解释了原因）

    std::cout << "  好注释写原因，不重复代码内容\n";
}

// ③ 变量：存储数据的命名空间
void demo03_variables()
{
    std::cout << "\n③ 变量（类型 变量名 = 初始值;）\n";

    // 知识点 3.1：变量声明
    // 格式：类型 变量名 = 初始值;
    // 每个变量必须先声明类型，编译器才知道分配多少内存

    int age = 18;
    double height = 1.75;
    bool passed = true;
    char grade = 'A';             // 单个字符用单引号
    std::string name = "小白";    // 字符串用双引号

    std::cout << "  int    age    = " << age << "\n";
    std::cout << "  double height = " << height << "\n";
    std::cout << "  bool   passed = " << passed << "（1=true）\n";
    std::cout << "  char   grade  = " << grade << "\n";
    std::cout << "  string name   = " << name << "\n";

    // 知识点 3.2：未初始化变量
    // 局部变量不初始化，值是随机的垃圾值，读取它是未定义行为
    // int bad;
    // std::cout << bad;  // 危险：打印随机垃圾值

    // 知识点 3.3：变量命名规则
    // 字母/数字/下划线，不能以数字开头，区分大小写

    int count1 = 1; // 合法
    int Count1 = 2; // 合法（和 count1 是不同变量！大小写不同）
    // int 1count = 3;  // 非法：不能以数字开头
    std::cout << "  count1=" << count1 << "  Count1=" << Count1
        << "（不同变量，大小写敏感）\n";
}

// ④ cout：向终端打印
void demo04_cout()
{
    std::cout << "\n④ cout 打印\n";

    // 知识点 4.1：std::cout 输出流
    // std:: = "去标准库里找"
    // cout = character output（字符输出），向终端打印内容
    // << = 流插入运算符，把右边内容送进输出流

    // 知识点 4.2：<< 链式调用
    // << 可以连续写任意多个，每个 << 返回 std::cout 本身，所以能继续接下一个 <<
    // std::cout << "a" << 1 << "\n";  →  先输出 "a"，再输出 1，最后换行

    std::cout << "  文字字符串\n";                         // 打印文字
    std::cout << "  数字: " << 42 << "\n";                // 打印数字
    std::cout << "  混合: " << "age=" << 18 << "\n";      // 链式

    // 知识点 4.3：\n vs std::endl
    // "\n" 只换行，内容攒在缓冲区里，攒够一批再一次性写到屏幕（快）
    // std::endl = 换行 + 强制刷新输出缓冲区，每次都立即写屏幕（慢）
    // 循环大量打印时差距明显
    // 结论：日常用 "\n" 就够了，debug 怕崩溃丢输出时才用 std::endl

    std::cout << "  行1\n";                    // \n：换行符（推荐，快）
    std::cout << "  行2" << "\n";              // 同上
    std::cout << "  行3" << std::endl;         // std::endl：换行+刷新缓冲区

    // 知识点 4.4：转义字符
    // 反斜杠 \ 开头的特殊字符，在字符串里表示不可见字符或特殊含义

    std::cout << "  转义字符：\n";
    std::cout << "    \\n  换行\n";
    std::cout << "    \\t  制表符：[" << '\t' << "]\n";
    std::cout << "    \\\\  反斜杠：\\\n";
    std::cout << "    \\'  单引号：'\n";
    std::cout << "    \\\"  双引号：\"\n";
    std::cout << "    \\0  空字符（字符串结束标志）\n";

    // 知识点 4.5：cerr 错误输出
    // cerr = character error（字符错误输出）
    // 和 cout 一样用 <<，但不走缓冲区，立即输出
    // 用于输出错误信息，确保程序崩溃前错误信息不会丢

    std::cerr << "  cerr（错误输出，立即刷新）\n";
}

// ⑤ 作用域：变量的"有效范围"
void demo05_scope()
{
    std::cout << "\n⑤ 作用域\n";

    // 知识点 5.1：作用域 {}
    // 变量只在声明它的 {} 内有效，出了 {} 就消失（内存被回收）
    // 内层可以访问外层变量，外层不能访问内层变量

    int outer = 10;
    std::cout << "  外层 outer=" << outer << "\n";

    {
        int inner = 20;
        std::cout << "  内层 inner=" << inner << "（只在这个{}内有效）\n";
        std::cout << "  内层也能访问外层 outer=" << outer << "\n";

        // 知识点 5.2：变量屏蔽（shadowing）
        // 内层可以重新声明同名变量，屏蔽外层的同名变量
        // 出了内层 {} 后，外层变量恢复

        int outer = 999;
        std::cout << "  内层重声明 outer=" << outer << "（屏蔽外层）\n";
    }

    std::cout << "  出{}后 outer=" << outer << "（外层的）\n";
    // std::cout << inner;  // 编译错误：inner 已超出作用域
}

// ⑥ 基本输入（cin）
void demo06_cin()
{
    std::cout << "\n⑥ cin（从键盘读输入）\n";

    // 知识点 6.1：cin >> 读取输入
    // cin = character input（字符输入），从键盘读数据
    // >> 是流提取运算符，从输入流读取数据存入变量
    // 遇到空白字符（空格/换行/Tab）停止读取

    int age;
    std::cout << "  请输入年龄（整数）: ";
    std::cin >> age;
    std::cout << "  你输入了: " << age << "\n";

    std::string name;
    std::cout << "  请输入姓名（单词）: ";
    std::cin >> name;
    std::cout << "  你输入了: " << name << "\n";

    // 知识点 6.2：cin.ignore() 清缓冲区
    // cin >> 读完后缓冲区留有换行符，getline 会直接读到这个换行符导致空行
    // cin.ignore() 丢弃缓冲区里的一个字符（那个残留的换行符）

    std::cin.ignore();

    // 知识点 6.3：getline 读整行
    // std::getline(std::cin, 变量) 读取一整行（含空格），遇回车结束
    // 和 cin >> 的区别：cin >> 遇空格就停，getline 遇回车才停

    std::string line;
    std::cout << "  请输入一句话（含空格）: ";
    std::getline(std::cin, line);
    std::cout << "  你输入了: \"" << line << "\"\n";
}

// 知识点 1.4：main 函数
// int main()：程序入口，有且只有一个，操作系统从这里开始执行
// int：表示 main 向操作系统返回一个整数
// return 0：表示"程序正常结束"；非零表示出错

int main()
{
    std::cout << "=== 00_hello_world: 程序结构 ===";
    demo01_structure();
    demo02_comments();
    demo03_variables();
    demo04_cout();
    demo05_scope();
    demo06_cin();
    std::cout << "\n完成。\n";
    return 0;
}
