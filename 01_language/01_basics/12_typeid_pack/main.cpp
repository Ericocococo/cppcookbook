// C++20
// 演示：typeid（运行时类型）、sizeof...（变参包大小）、变参模板基础
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>
#include <typeinfo>     // typeid
#include <string>
#include <type_traits>  // is_same_v 等

// ① typeid：查运行时类型
void demo01_typeid()
{
    std::cout << "\n① typeid（运行时类型查询）\n";

    int n = 42;
    double d = 3.14;
    std::string s = "hello";

    // typeid().name()：返回类型名字符串（格式因编译器而异）
    std::cout << "  typeid(int).name()    = \"" << typeid(int).name() << "\"\n";
    std::cout << "  typeid(double).name() = \"" << typeid(double).name() << "\"\n";
    std::cout << "  typeid(n).name()      = \"" << typeid(n).name() << "\"\n";
    std::cout << "  typeid(s).name()      = \"" << typeid(s).name() << "\"\n";
    std::cout << "  （GCC 输出缩写如 i=int，MSVC 输出 int，格式不统一）\n";

    // 比较类型是否相同
    std::cout << "  typeid(n)==typeid(int)    : " << (typeid(n) == typeid(int)) << "\n";
    std::cout << "  typeid(n)==typeid(double) : " << (typeid(n) == typeid(double)) << "\n";

    // typeid 在多态中的真正用途：查实际类型（运行期）
    struct Base
    {
        virtual ~Base()
        {
        }
    };
    struct Derived : Base
    {
    };
    Base* bp = new Derived();
    std::cout << "  Base* 指向 Derived:\n";
    std::cout << "    typeid(*bp).name() = \"" << typeid(*bp).name()
        << "\"（实际是 Derived！）\n";
    std::cout << "    typeid(Base).name()= \"" << typeid(Base).name() << "\"\n";
    std::cout << "    是 Derived? " << (typeid(*bp) == typeid(Derived)) << "\n";
    delete bp;

    std::cout << "  注意：typeid 适合调试，生产代码更推荐用虚函数多态\n";
}

// ② sizeof...：统计变参模板参数数量
template <typename... Args>
void showCount(Args... args)
{
    std::cout << "  参数数量=" << sizeof...(args)
        << "  类型数量=" << sizeof...(Args) << "\n";
    (void)(args, ...); // 展开 args，避免"未使用参数"警告
}

void demo02_sizeof_pack()
{
    std::cout << "\n② sizeof...（变参包大小）\n";
    showCount();
    showCount(1);
    showCount(1, 2.0, "hi");
    showCount(1, 2, 3, 4, 5);
    std::cout << "  sizeof...(Args) 和 sizeof...(args) 结果相同\n";
}

// ③ 变参模板基础（C++11）
// 递归展开：把参数包逐一取出来处理
void printAll() { std::cout << "\n"; } // 递归终止条件

template <typename T, typename... Rest>
void printAll(T first, Rest... rest)
{
    std::cout << first;
    if constexpr (sizeof...(rest) > 0) std::cout << ", ";
    printAll(rest...); // 递归：去掉第一个，继续处理剩余
}

// C++17 折叠表达式：更简洁地处理参数包
template <typename... Args>
auto sumAll(Args... args)
{
    return (args + ...); // 折叠表达式：a1 + a2 + a3 + ...
}

template <typename... Args>
void printAllFold(Args... args)
{
    ((std::cout << args << " "), ...); // 逗号折叠：展开每个 args
    std::cout << "\n";
}

void demo03_variadic_template()
{
    std::cout << "\n③ 变参模板\n";

    std::cout << "  printAll(1,2.5,\"hi\"): ";
    printAll(1, 2.5, std::string("hi"));

    std::cout << "  sumAll(1,2,3,4,5)=" << sumAll(1, 2, 3, 4, 5) << "\n";
    std::cout << "  sumAll(0.5,1.5,2.0)=" << sumAll(0.5, 1.5, 2.0) << "\n";

    std::cout << "  printAllFold(10,20,30): ";
    printAllFold(10, 20, 30);
}

// ④ 编译期类型判断（type_traits）
void demo04_type_traits()
{
    std::cout << "\n④ type_traits（编译期类型查询）\n";

    // is_same_v：两个类型是否相同
    std::cout << "  is_same_v<int,int>    = " << std::is_same_v<int, int> << "\n";
    std::cout << "  is_same_v<int,double> = " << std::is_same_v<int, double> << "\n";

    // is_integral_v / is_floating_point_v
    std::cout << "  is_integral_v<int>    = " << std::is_integral_v<int> << "\n";
    std::cout << "  is_integral_v<double> = " << std::is_integral_v<double> << "\n";
    std::cout << "  is_floating_point_v<double> = " << std::is_floating_point_v<double> << "\n";

    // is_pointer_v / is_reference_v
    std::cout << "  is_pointer_v<int*>   = " << std::is_pointer_v<int*> << "\n";
    std::cout << "  is_reference_v<int&> = " << std::is_reference_v<int&> << "\n";

    // add/remove 系列：变换类型
    using NoConst = std::remove_const_t<const int>; // const int → int
    using AddPtr = std::add_pointer_t<int>; // int → int*
    std::cout << "  remove_const<const int> = int? "
        << std::is_same_v<NoConst, int> << "\n";
    std::cout << "  add_pointer<int> = int*? "
        << std::is_same_v<AddPtr, int*> << "\n";

    std::cout << "  （type_traits 全在 <type_traits> 头文件，配合模板使用）\n";
}

int main()
{
    std::cout << "=== 12_typeid_pack: typeid 与变参包 ===";
    demo01_typeid();
    demo02_sizeof_pack();
    demo03_variadic_template();
    demo04_type_traits();
    std::cout << "\n完成。\n";
    return 0;
}
