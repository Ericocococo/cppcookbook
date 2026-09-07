// C++20
// 演示：函数模板、类模板、全特化、偏特化、模板参数推断、非类型模板参数
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>     // std::runtime_error
#include <type_traits>   // is_integral_v 等

// ① 函数模板：一份代码适用多种类型
template <typename T>
T maxVal(T a, T b)
{
    return (a > b) ? a : b;
}

// 知识点 1.1.2：多个类型参数（printPair 定义）
template <typename T1, typename T2>
void printPair(const T1& a, const T2& b)
{
    std::cout << "  (" << a << ", " << b << ")\n";
}

void demo01_function_template()
{
    std::cout << "\n① 函数模板\n";

    // 知识点 1.1.1：编译器自动推断 T（调用时不写类型，编译器按实参推断）
    std::cout << "  maxVal(3, 5)       = " << maxVal(3, 5) << "（T=int）\n";
    std::cout << "  maxVal(3.14, 2.71) = " << maxVal(3.14, 2.71) << "（T=double）\n";
    std::cout << "  maxVal('a', 'z')   = " << maxVal('a', 'z') << "（T=char）\n";

    // 知识点 1.1.3：显式指定类型（尖括号写明 T，实参自动转换）
    std::cout << "  maxVal<double>(3, 5) = " << maxVal<double>(3, 5) << "\n";

    // 知识点 1.1.2：printPair 调用演示（T1、T2 各推各的，类型完全不同也行）
    std::cout << "  printPair(1, 3.14): ";
    printPair(1, 3.14);
    std::cout << "  printPair(\"hello\", 42): ";
    printPair(std::string("hello"), 42);

    // 知识点 1.1.4：类型推断失败——只能写注释，编译错误无法运行演示
    // maxVal(3, 5.0);   // 编译错误！单类型参数 T 无法同时推断为 int 和 double
    // 两个值该同类型 → 显式指定：maxVal<double>(3, 5.0)
    // 两个值本就不同类型 → 用双类型参数（见上 printPair 的 T1/T2）

    std::cout << "  原理：编译器根据实际类型生成独立的函数，零运行时开销\n";
}

// ② 类模板
template <typename T>
class Stack
{
    std::vector<T> data_;

public:
    void push(const T& val) { data_.push_back(val); }

    T pop()
    {
        if (data_.empty()) throw std::runtime_error("栈为空");
        T val = data_.back();
        data_.pop_back();
        return val;
    }

    const T& top() const
    {
        if (data_.empty()) throw std::runtime_error("栈为空");
        return data_.back();
    }

    bool empty() const { return data_.empty(); }
    size_t size() const { return data_.size(); }
};

void demo02_class_template()
{
    std::cout << "\n② 类模板\n";

    Stack<int> si;
    si.push(1);
    si.push(2);
    si.push(3);
    std::cout << "  Stack<int>: push 1 2 3\n";
    std::cout << "    top=" << si.top() << "  size=" << si.size() << "\n";
    std::cout << "    pop=" << si.pop() << "  剩余=" << si.size() << "\n";

    Stack<std::string> ss;
    ss.push("hello");
    ss.push("world");
    std::cout << "  Stack<string>: top=" << ss.top() << "\n";

    // 知识点 1.2.1：C++17 类模板参数推断（CTAD）
    // Stack s = Stack<int>{};   // 需要推断规则
}

// ③ 全特化：为特定类型提供完全不同的实现
template <typename T>
std::string describe(T) { return "未知类型"; }

template <> // 全特化：T = bool
std::string describe<bool>(bool val)
{
    return val ? "布尔值:真" : "布尔值:假";
}

template <> // 全特化：T = std::string
std::string describe<std::string>(std::string val)
{
    return "字符串:\"" + val + "\"  长度=" + std::to_string(val.size());
}

void demo03_specialization()
{
    std::cout << "\n③ 全特化\n";
    std::cout << "  describe(42)=" << describe(42) << "\n";
    std::cout << "  describe(true)=" << describe(true) << "\n";
    std::cout << "  describe(string)=" << describe(std::string("hi")) << "\n";
}

// ④ 非类型模板参数：用编译期常量作为模板参数
template <typename T, size_t N>
class FixedArray
{
    T data_[N];

public:
    FixedArray() : data_{}
    {
    }

    T& operator[](size_t i) { return data_[i]; }
    const T& operator[](size_t i) const { return data_[i]; }
    size_t size() const { return N; }
};

void demo04_nontype_param()
{
    std::cout << "\n④ 非类型模板参数\n";
    FixedArray<int, 5> arr;
    for (size_t i = 0; i < arr.size(); ++i) arr[i] = (int)i * 10;
    std::cout << "  FixedArray<int,5>: ";
    for (size_t i = 0; i < arr.size(); ++i) std::cout << arr[i] << " ";
    std::cout << "\n";
    std::cout << "  大小在编译期确定，比 vector 更高效（无动态内存）\n";
}

// ⑤ 模板与 concepts（C++20）：约束模板参数
// 知识点 1.5.1：requires 关键字
// 指定类型必须满足的条件
template <typename T>
    requires std::is_arithmetic_v<T> // T 必须是算术类型（int/float等）
T average(T a, T b) { return (a + b) / 2; }

void demo05_concepts()
{
    std::cout << "\n⑤ Concepts（C++20）\n";
    std::cout << "  average(3, 5)   = " << average(3, 5) << "\n";
    std::cout << "  average(1.5,2.5)= " << average(1.5, 2.5) << "\n";
    // average("hi","lo");  // 编译错误：string 不满足 is_arithmetic_v
    std::cout << "  average(\"hi\",\"lo\") 编译错误：string 不满足算术类型约束\n";
}

// ⑥ if constexpr（C++17）：编译期条件，不同类型走不同代码路径
template <typename T>
void process(T val)
{
    if constexpr (std::is_integral_v<T>)
    {
        std::cout << "  整数，二进制表示最低8位=0x"
            << std::hex << (val & 0xFF) << std::dec << "\n";
    }
    else if constexpr (std::is_floating_point_v<T>)
    {
        std::cout << "  浮点，取整=" << static_cast<long long>(val) << "\n";
    }
    else
    {
        std::cout << "  其他类型: " << val << "\n";
    }
}

void demo06_if_constexpr()
{
    std::cout << "\n⑥ if constexpr + 类型特性\n";
    process(255);
    process(3.14);
    process(std::string("hello"));
}

int main()
{
    std::cout << "=== 10_templates: 模板 ===";
    demo01_function_template();
    demo02_class_template();
    demo03_specialization();
    demo04_nontype_param();
    demo05_concepts();
    demo06_if_constexpr();
    std::cout << "\n完成。\n";
    return 0;
}
