// C++20
// 演示：程序结构、#include、main、变量、cout、注释、作用域、基本输入
// 构建：cmake -B build-mingw（详见 README.md）

// #include：引入工具箱（头文件）
// 不写就用不了里面的东西，编译器报"未定义"错误
#include <iostream>   // 提供 cout（打印）、cin（读输入）、cerr（错误输出）
#include <string>     // 提供 std::string 字符串类型

// ① 程序入口：main 函数
// int main()：程序从这里开始执行，有且只有一个
// int：表示 main 向操作系统返回一个整数
// return 0：表示"程序正常结束"；非零表示出错
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

    // 单行注释：// 到行尾
    std::cout << "  // 单行注释：从 // 到行尾都被忽略\n";

    /* 多行注释
       可以跨越多行
    */
    std::cout << "  /* 多行注释 */：从 /* 到 */ 之间全被忽略\n";

    // 注释写"为什么"，不写"做了什么"
    // int speed = 60;    // 不好：设置速度为60（代码已经说了）
    // int speed = 60;    // 好：限速 60 km/h，超过触发警报（解释了原因）
    std::cout << "  好注释写原因，不重复代码内容\n";
}

// ③ 变量：存储数据的命名空间
void demo03_variables()
{
    std::cout << "\n③ 变量（类型 变量名 = 初始值;）\n";

    // 声明变量：类型 变量名 = 初始值
    int age = 18;
    double height = 1.75;
    bool passed = true;
    char grade = 'A'; // 单个字符用单引号
    std::string name = "小白"; // 字符串用双引号

    std::cout << "  int    age    = " << age << "\n";
    std::cout << "  double height = " << height << "\n";
    std::cout << "  bool   passed = " << passed << "（1=true）\n";
    std::cout << "  char   grade  = " << grade << "\n";
    std::cout << "  string name   = " << name << "\n";

    // 未初始化的局部变量：值是随机的！不要用
    // int bad;
    // std::cout << bad;  // 危险：打印随机垃圾值

    // 变量名规则：字母/数字/下划线，不能以数字开头，区分大小写
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

    // std::cout：标准输出流
    // std:: = "去标准库里找"；cout = console output（控制台输出）
    // << = 流插入运算符，把右边内容送进输出流

    std::cout << "  文字字符串\n"; // 打印文字
    std::cout << "  数字: " << 42 << "\n"; // 打印数字
    std::cout << "  混合: " << "age=" << 18 << "\n"; // 链式

    // 换行方式
    std::cout << "  行1\n"; // \n：换行符（推荐，快）
    std::cout << "  行2" << "\n"; // 同上
    std::cout << "  行3" << std::endl; // std::endl：换行+刷新缓冲区（慢一点）

    // 打印特殊字符
    std::cout << "  转义字符：\n";
    std::cout << "    \\n  换行\n";
    std::cout << "    \\t  制表符：[" << '\t' << "]\n";
    std::cout << "    \\\\  反斜杠：\\\n";
    std::cout << "    \\'  单引号：'\n";
    std::cout << "    \\\"  双引号：\"\n";
    std::cout << "    \\0  空字符（字符串结束标志）\n";

    // cerr：错误输出（不走缓冲，立即输出）
    std::cerr << "  cerr（错误输出，立即刷新）\n";
}

// ⑤ 作用域：变量的"有效范围"
void demo05_scope()
{
    std::cout << "\n⑤ 作用域\n";

    int outer = 10;
    std::cout << "  外层 outer=" << outer << "\n";

    {
        // 内层 {} 里声明的变量，出了 {} 就消失
        int inner = 20;
        std::cout << "  内层 inner=" << inner << "（只在这个{}内有效）\n";
        std::cout << "  内层也能访问外层 outer=" << outer << "\n";

        // 内层可以重新声明同名变量（屏蔽外层）
        int outer = 999; // 这个是新的 outer，屏蔽外层的
        std::cout << "  内层重声明 outer=" << outer << "（屏蔽外层）\n";
    }
    // 这里 inner 不存在，outer 恢复为外层的值
    std::cout << "  出{}后 outer=" << outer << "（外层的）\n";
    // std::cout << inner;  // 编译错误：inner 已超出作用域
}

// ⑥ 基本输入（cin）
void demo06_cin()
{
    std::cout << "\n⑥ cin（从键盘读输入）\n";

    // >> 运算符：读单个整数，遇空白停止
    int age;
    std::cout << "  请输入年龄（整数）: ";
    std::cin >> age;
    std::cout << "  你输入了: " << age << "\n";

    // >> 读单词（遇空格停止）
    std::string name;
    std::cout << "  请输入姓名（单词）: ";
    std::cin >> name;
    std::cout << "  你输入了: " << name << "\n";

    // cin >> 读完后缓冲区留有换行符，getline 前需要清掉
    std::cin.ignore();

    // getline：读整行（含空格，回车结束）
    std::string line;
    std::cout << "  请输入一句话（含空格）: ";
    std::getline(std::cin, line);
    std::cout << "  你输入了: \"" << line << "\"\n";
}

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
