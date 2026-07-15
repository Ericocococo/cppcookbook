// C++20
// 演示：函数模板、类模板、模板特化、异常处理（try/catch/throw）
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>
#include <stdexcept>  // runtime_error, out_of_range 等标准异常
#include <string>
#include <vector>

// ========== ① 函数模板 ==========
// 问题：想写一个函数，对 int/double/string 都能求最大值
// 不用模板：要写三个重载函数
// 用模板：写一个，编译器自动生成多个版本

// template<typename T>：T 是占位符，调用时由编译器推断
template<typename T>
T maxVal(T a, T b) {
    return (a > b) ? a : b;
}

// 多个类型参数
template<typename T1, typename T2>
void printPair(T1 a, T2 b) {
    std::cout << "  (" << a << ", " << b << ")\n";
}

void demo01_function_template() {
    std::cout << "\n① 函数模板\n";

    // 编译器自动推断 T 的类型
    std::cout << "  maxVal(3, 5)         = " << maxVal(3, 5)         << "  (int)\n";
    std::cout << "  maxVal(3.14, 2.71)   = " << maxVal(3.14, 2.71)   << "  (double)\n";
    std::cout << "  maxVal('a', 'z')     = " << maxVal('a', 'z')     << "  (char)\n";

    // 也可以显式指定类型
    std::cout << "  maxVal<double>(3, 5) = " << maxVal<double>(3, 5) << "  (强制double)\n";

    // 多类型参数
    std::cout << "  printPair(1, 3.14): ";
    printPair(1, 3.14);
    std::cout << "  printPair(\"hello\", 42): ";
    printPair(std::string("hello"), 42);
}

// ========== ② 类模板 ==========
// 一个可以存任意类型的简单栈
template<typename T>
class Stack {
private:
    std::vector<T> data;   // 用 vector 存储元素

public:
    // 压栈：把元素加到顶部
    void push(const T& val) {
        data.push_back(val);
    }

    // 弹栈：取出并移除顶部元素
    T pop() {
        if (data.empty()) {
            throw std::runtime_error("栈为空，无法 pop");
        }
        T val = data.back();
        data.pop_back();
        return val;
    }

    // 查看顶部元素（不移除）
    const T& top() const {
        if (data.empty()) {
            throw std::runtime_error("栈为空，无法 top");
        }
        return data.back();
    }

    bool empty() const { return data.empty(); }
    int  size()  const { return static_cast<int>(data.size()); }
};

void demo02_class_template() {
    std::cout << "\n② 类模板\n";

    // 用 <int> 实例化一个整数栈
    Stack<int> si;
    si.push(1);
    si.push(2);
    si.push(3);
    std::cout << "  Stack<int>: push 1 2 3\n";
    std::cout << "  top()=" << si.top() << "  size=" << si.size() << "\n";
    std::cout << "  pop(): " << si.pop() << "\n";
    std::cout << "  pop(): " << si.pop() << "\n";

    // 用 <string> 实例化一个字符串栈，同一套代码
    Stack<std::string> ss;
    ss.push("hello");
    ss.push("world");
    std::cout << "  Stack<string>: push \"hello\" \"world\"\n";
    std::cout << "  top()=" << ss.top() << "\n";
}

// ========== ③ 模板特化 ==========
// 对某个特定类型提供专门的实现（覆盖通用版本）

// 通用版本
template<typename T>
std::string describe(T val) {
    return "未知类型，值=" + std::to_string(val);  // to_string 对 T 不一定有效
}

// 针对 bool 的特化版本
template<>
std::string describe<bool>(bool val) {
    return val ? "布尔值：真" : "布尔值：假";
}

// 针对 std::string 的特化版本
template<>
std::string describe<std::string>(std::string val) {
    return "字符串：\"" + val + "\"  长度=" + std::to_string(val.size());
}

void demo03_specialization() {
    std::cout << "\n③ 模板特化\n";
    std::cout << "  describe(true)         = " << describe(true)             << "\n";
    std::cout << "  describe(false)        = " << describe(false)            << "\n";
    std::cout << "  describe(string(hi))   = " << describe(std::string("hi")) << "\n";
}

// ========== ④ 异常处理 ==========
// 当发生错误时，throw 抛出一个异常对象，try-catch 捕获并处理
double safeDivide(double a, double b) {
    if (b == 0.0) {
        // throw：抛出异常，函数立即终止，跳到最近的 catch
        throw std::runtime_error("除数不能为零");
    }
    return a / b;
}

int getElement(const std::vector<int>& v, int i) {
    if (i < 0 || i >= static_cast<int>(v.size())) {
        throw std::out_of_range("下标 " + std::to_string(i) + " 越界");
    }
    return v[i];
}

void demo04_exceptions() {
    std::cout << "\n④ 异常处理\n";

    // 正常情况
    try {
        double r = safeDivide(10.0, 3.0);
        std::cout << "  10 / 3 = " << r << "\n";
    } catch (const std::exception& e) {
        std::cout << "  捕获异常: " << e.what() << "\n";
    }

    // 触发异常
    try {
        double r = safeDivide(10.0, 0.0);  // 会 throw
        std::cout << "  这行不会执行\n";
        (void)r;
    } catch (const std::runtime_error& e) {
        // 捕获 runtime_error 类型的异常
        std::cout << "  捕获 runtime_error: " << e.what() << "\n";
    }

    // 多个 catch：从具体到通用排列
    std::vector<int> v = {10, 20, 30};
    for (int idx : {1, 5, -1}) {
        try {
            int val = getElement(v, idx);
            std::cout << "  v[" << idx << "] = " << val << "\n";
        } catch (const std::out_of_range& e) {
            std::cout << "  越界异常: " << e.what() << "\n";
        } catch (const std::exception& e) {
            // 捕获所有标准异常（更通用）
            std::cout << "  标准异常: " << e.what() << "\n";
        } catch (...) {
            // 捕获任何类型的异常（最后兜底）
            std::cout << "  未知异常\n";
        }
    }

    // noexcept：承诺函数不会抛异常（编译器可以优化）
    auto safe = []() noexcept { return 42; };
    std::cout << "  noexcept 函数: " << safe() << "\n";
}

int main() {
    std::cout << "=== 09_templates_exceptions: 模板与异常 ===";
    demo01_function_template();
    demo02_class_template();
    demo03_specialization();
    demo04_exceptions();
    std::cout << "\n完成。\n";
    return 0;
}
