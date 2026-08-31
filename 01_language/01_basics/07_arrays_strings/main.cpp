// C++20
// 演示：原始数组、std::array、std::vector（扩容机制）、std::string、string_view
// 构建：cmake -B build-mingw（详见 README.md）

#include <iostream>
#include <string>
#include <string_view>  // C++17
#include <array>
#include <vector>
#include <algorithm>    // std::sort / std::find

// ① 原始数组
void demo01_raw_array()
{
    std::cout << "\n① 原始数组\n";

    int arr[5] = {1, 2, 3, 4, 5};
    std::cout << "  arr[0]=" << arr[0] << "  arr[4]=" << arr[4] << "\n";

    // 自动推断长度
    int arr2[] = {10, 20, 30};
    std::cout << "  元素数=" << sizeof(arr2) / sizeof(arr2[0]) << "\n";

    // 零初始化
    int zeros[5]{};
    std::cout << "  zeros[5]{}: ";
    for (int x : zeros) std::cout << x << " ";
    std::cout << "\n";

    // 二维数组
    int mat[2][3] = {{1, 2, 3}, {4, 5, 6}};
    std::cout << "  mat[1][2]=" << mat[1][2] << "\n";

    // 数组的陷阱：传给函数时退化为指针，丢失长度
    // void f(int arr[]) {}  等价于 void f(int* arr)
    // sizeof(arr) 在函数内只有指针大小！
    std::cout << "  陷阱：数组传给函数退化为指针，sizeof 只有指针大小\n";
    std::cout << "  解决：用 std::array 或传 size 参数\n";
}

// ② std::array（C++11）：固定大小，安全
void demo02_std_array()
{
    std::cout << "\n② std::array（固定大小）\n";

    std::array < int, 5 > arr = {5, 3, 1, 4, 2};
    std::cout << "  大小=" << arr.size() << "\n";
    std::cout << "  arr[0]=" << arr[0] << "\n";
    std::cout << "  front=" << arr.front() << "  back=" << arr.back() << "\n";

    // at() 带越界检查（[] 不检查）
    std::cout << "  arr.at(2)=" << arr.at(2) << "（越界会抛 out_of_range）\n";

    // 传给函数不退化（保留大小信息）
    auto printArr = [](const std::array<int, 5>& a)
    {
        for (auto x : a) std::cout << x << " ";
    };
    std::cout << "  遍历: ";
    printArr(arr);
    std::cout << "\n";

    // 排序
    std::sort(arr.begin(), arr.end());
    std::cout << "  排序后: ";
    for (auto x : arr) std::cout << x << " ";
    std::cout << "\n";
}

// ③ std::vector：动态数组（最常用）
void demo03_vector()
{
    std::cout << "\n③ std::vector\n";

    std::vector<int> v;
    std::cout << "  初始: size=" << v.size() << " capacity=" << v.capacity() << "\n";

    // push_back：可能触发扩容（重新分配，通常容量翻倍）
    for (int i = 1; i <= 8; ++i)
    {
        size_t old_cap = v.capacity();
        v.push_back(i);
        if (v.capacity() != old_cap)
            std::cout << "  push_back(" << i << ") 触发扩容: capacity "
                << old_cap << " -> " << v.capacity() << "\n";
    }
    std::cout << "  最终: size=" << v.size() << " capacity=" << v.capacity() << "\n";

    // reserve：预留空间，避免反复扩容
    std::vector<int> v2;
    v2.reserve(100); // 预留100个元素的空间
    std::cout << "  reserve(100): capacity=" << v2.capacity() << "\n";

    // 常用操作
    std::cout << "  v[0]=" << v[0] << " back=" << v.back() << "\n";
    v.pop_back();
    std::cout << "  pop_back后 size=" << v.size() << "\n";

    // emplace_back：直接在末尾构造，比 push_back 少一次复制
    v.emplace_back(99);
    std::cout << "  emplace_back(99): back=" << v.back() << "\n";

    // insert / erase
    v.insert(v.begin(), 0); // 头部插入 0（O(n)，慢）
    std::cout << "  insert(begin,0) 后 front=" << v.front() << "\n";
    v.erase(v.begin()); // 删除第一个
    std::cout << "  erase(begin) 后 front=" << v.front() << "\n";

    // 初始化方式
    std::vector<int> v3(5, 42); // 5 个 42
    std::vector<int> v4 = {1, 2, 3, 4, 5};
    std::cout << "  vector(5,42): ";
    for (auto x : v3) std::cout << x << " ";
    std::cout << "\n";
}

// ④ std::string
void demo04_string()
{
    std::cout << "\n④ std::string\n";

    std::string s = "Hello";
    std::cout << "  s=\"" << s << "\"  length=" << s.length() << "\n";

    // 拼接
    s += ", World";
    s.append("!");
    std::cout << "  拼接后: \"" << s << "\"\n";

    // 访问
    std::cout << "  s[0]='" << s[0] << "'  s.back()='" << s.back() << "'\n";

    // 子串
    std::cout << "  substr(0,5)=\"" << s.substr(0, 5) << "\"\n";

    // 查找
    size_t pos = s.find("World");
    std::cout << "  find(\"World\")=" << pos << "\n";
    std::cout << "  find(\"xyz\")=" << s.find("xyz")
        << "（npos=" << std::string::npos << "表示未找到）\n";

    // 替换
    s.replace(7, 5, "C++"); // 从位置7，替换5个字符
    std::cout << "  replace(7,5,\"C++\"): \"" << s << "\"\n";

    // 数值互转
    std::string ns = "123";
    int n = std::stoi(ns);
    double d = std::stod("3.14");
    std::string back = std::to_string(42);
    std::cout << "  stoi(\"123\")=" << n << "  stod(\"3.14\")=" << d
        << "  to_string(42)=\"" << back << "\"\n";

    // 比较（字典序）
    std::cout << "  \"apple\"<\"banana\": " << ("apple" < std::string("banana")) << "\n";

    // C 字符串互转
    const char* cs = s.c_str(); // string → const char*
    std::string from_c(cs); // const char* → string
    std::cout << "  c_str()=\"" << cs << "\"\n";
    (void)from_c;
}

// ⑤ std::string_view（C++17）：只读视图，不复制
void printSV(std::string_view sv)
{
    // string_view：只读引用，不分配内存，比传 const string& 更灵活
    std::cout << "  string_view: \"" << sv << "\"  len=" << sv.length() << "\n";
}

void demo05_string_view()
{
    std::cout << "\n⑤ string_view（C++17，只读视图）\n";

    std::string s = "Hello, World";
    printSV(s); // 从 string 构造，不复制
    printSV("C++ literal"); // 从字符串字面量构造，不复制

    // string_view 的子串也不复制
    std::string_view sv = s;
    std::cout << "  sv.substr(0,5)=\"" << sv.substr(0, 5) << "\"（不分配内存）\n";

    // 注意：string_view 不拥有数据，原字符串销毁后 string_view 成悬空引用
    std::cout << "  注意：string_view 不延长字符串生命周期，不要存储临时字符串的 view\n";
}

int main()
{
    std::cout << "=== 07_arrays_strings: 数组与字符串 ===";
    demo01_raw_array();
    demo02_std_array();
    demo03_vector();
    demo04_string();
    demo05_string_view();
    std::cout << "\n完成。\n";
    return 0;
}
