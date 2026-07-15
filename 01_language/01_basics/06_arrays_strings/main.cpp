// C++20
// 演示：原始数组、std::array、std::vector、std::string 的基本用法
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>
#include <string>
#include <array>    // std::array
#include <vector>   // std::vector

// ① 原始数组：固定大小，声明时就要知道长度
void demo01_raw_array() {
    std::cout << "\n① 原始数组\n";

    // 声明并初始化
    int arr[5] = {10, 20, 30, 40, 50};
    std::cout << "  int arr[5] = {10,20,30,40,50}\n";

    // 访问元素：下标从 0 开始
    std::cout << "  arr[0]=" << arr[0] << "  arr[4]=" << arr[4] << "\n";

    // 修改元素
    arr[2] = 999;
    std::cout << "  arr[2]=999 后: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    // 初始化为全0：int arr2[5] = {}; 或 int arr2[5]{};
    int arr2[5] = {};
    std::cout << "  int arr2[5]={} 全部初始化为0: ";
    for (int x : arr2) std::cout << x << " ";
    std::cout << "\n";

    // 自动推断长度
    int arr3[] = {1, 2, 3};   // 长度自动推断为 3
    std::cout << "  int arr3[]={1,2,3} 长度="
              << sizeof(arr3)/sizeof(arr3[0]) << "\n";

    // 二维数组：3行4列
    int mat[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    std::cout << "  二维数组 mat[1][2]=" << mat[1][2] << "\n";  // 第2行第3列 = 7
}

// ② std::array（C++11）：固定大小，比原始数组更安全，有 size() 等方法
void demo02_std_array() {
    std::cout << "\n② std::array（固定大小，推荐替代原始数组）\n";

    std::array<int, 5> arr = {10, 20, 30, 40, 50};

    std::cout << "  大小: " << arr.size() << "\n";
    std::cout << "  arr[0]=" << arr[0] << "\n";

    // at() 带越界检查（越界会抛异常，比 [] 安全）
    std::cout << "  arr.at(2)=" << arr.at(2) << "\n";

    std::cout << "  遍历: ";
    for (const auto& x : arr) std::cout << x << " ";
    std::cout << "\n";

    // 排序等算法都支持
    arr.fill(0);   // 全部填充为 0
    std::cout << "  fill(0) 后: ";
    for (const auto& x : arr) std::cout << x << " ";
    std::cout << "\n";
}

// ③ std::vector：动态数组，大小可变（最常用）
void demo03_vector() {
    std::cout << "\n③ std::vector（大小可变，最常用）\n";

    std::vector<int> v = {1, 2, 3};
    std::cout << "  初始: 大小=" << v.size() << "  容量=" << v.capacity() << "\n";

    // push_back：在末尾添加元素
    v.push_back(4);
    v.push_back(5);
    std::cout << "  push_back(4)(5) 后: 大小=" << v.size() << "\n";

    std::cout << "  内容: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    // pop_back：移除末尾元素
    v.pop_back();
    std::cout << "  pop_back() 后: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";

    // 访问
    std::cout << "  v[0]=" << v[0] << "  v.front()=" << v.front()
              << "  v.back()=" << v.back() << "\n";

    // 是否为空
    std::cout << "  empty()=" << v.empty() << "\n";

    // 创建 n 个相同元素的 vector
    std::vector<int> zeros(5, 0);   // 5个0
    std::cout << "  vector<int>(5, 0): ";
    for (int x : zeros) std::cout << x << " ";
    std::cout << "\n";
}

// ④ std::string：C++ 字符串，比 C 风格 char[] 方便得多
void demo04_string() {
    std::cout << "\n④ std::string\n";

    std::string s = "Hello";
    std::cout << "  s = \"" << s << "\"\n";
    std::cout << "  长度 s.length() = " << s.length() << "\n";

    // 拼接
    s += ", World";      // 方法1：+=
    s.append("!");       // 方法2：append
    std::cout << "  拼接后: \"" << s << "\"\n";

    // 访问字符
    std::cout << "  s[0]='" << s[0] << "'  s.back()='" << s.back() << "'\n";

    // 子串：substr(起始位置, 长度)
    std::string sub = s.substr(0, 5);   // 从0开始，取5个字符
    std::cout << "  s.substr(0,5) = \"" << sub << "\"\n";

    // 查找：find() 返回位置，找不到返回 string::npos
    size_t pos = s.find("World");
    if (pos != std::string::npos) {
        std::cout << "  找到 \"World\" 在位置 " << pos << "\n";
    }

    // 字符串与数值互转
    std::string numStr = "123";
    int n = std::stoi(numStr);      // string -> int
    double d = std::stod("3.14");   // string -> double
    std::string back = std::to_string(42);  // int -> string
    std::cout << "  stoi(\"123\")=" << n << "  stod(\"3.14\")=" << d
              << "  to_string(42)=\"" << back << "\"\n";

    // 比较：直接用 == != < > 运算符
    std::string a = "apple", b = "banana";
    std::cout << "  \"apple\" < \"banana\" : " << (a < b) << "（按字典序）\n";

    // 遍历字符
    std::cout << "  逐字符: ";
    for (char c : s) {
        if (c != ' ') std::cout << c;  // 跳过空格
    }
    std::cout << "\n";
}

int main() {
    std::cout << "=== 06_arrays_strings: 数组与字符串 ===";
    demo01_raw_array();
    demo02_std_array();
    demo03_vector();
    demo04_string();
    std::cout << "\n完成。\n";
    return 0;
}
