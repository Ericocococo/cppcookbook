// C++20 | 依赖：pybind11
// 第四步：多种参数类型 + STL 容器自动转换。
//
// 演示:
//   - 多种基本类型: int / double / string
//   - STL 容器: vector ↔ list, map ↔ dict（加 pybind11/stl.h 自动转换）
//   - py::arg 关键字参数（Python 侧可按名传参）
//
// Python 使用:
//   import types_demo
//   types_demo.avg([1.0, 2.0, 3.0])         # → 2.0
//   types_demo.greet(name="世界")            # → "你好, 世界!"
//   types_demo.count_letters("hello")        # → {'h': 1, 'e': 1, 'l': 2, 'o': 1}
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>     // ← 加这一行，vector/map 就能自动和 Python list/dict 互转

#include <map>
#include <string>
#include <vector>

// 命名空间别名：给长名字起短名字，py:: 就是 pybind11::
//   没有别名：pybind11::arg("v")、pybind11::init<>()
//   有了别名：py::arg("v")、py::init<>()
namespace py = pybind11;

// ---- 多种基本类型 ----

double avg(const std::vector<double>& v)
{
    if (v.empty()) return 0.0;

    double sum = 0.0;
    for (double x : v)
    {
        sum += x;
    }
    return sum / v.size();
}

// const std::string& name：大对象用 const & 避免拷贝
//   & = 不拷贝，直接用原数据（引用传递）
//   const = 函数里只读不改
std::string greet(const std::string& name)
{
    return "你好, " + name + "!";
}

// ---- STL 容器返回 ----

// map<K, V>：键值对容器，key → value 的映射
//   插入：m[key] = value          访问：m[key]
//   查找：m.find(key)             删除：m.erase(key)
//   遍历：for (auto& [k, v] : m)
//
//   用法示例：
//     map<string, int> m;       // K=string（键），V=int（值）
//     m["hello"] = 1;            // 插入：hello → 1
//     m["hello"]++;              // 访问并自增（不存在时自动创建并初始化为 0）
//     m.size();                  // 键值对个数
//
//   返回 map，pybind11 自动转为 Python dict
std::map<std::string, int> count_letters(const std::string& s)
{
    std::map<std::string, int> out;
    /* 遍历字符串的每个字符，统计频次：
       for (char c : s)          → 逐个取出字符，如 'h', 'e', 'l', 'l', 'o'
       等价于
       for (int i = 0; i < s.size(); i++) {
           char c = s[i];
           out[std::string(1, c)]++;
       }
       std::string(1, c)        → 构造长度1的字符串，内容为字符 c
                                   例：std::string(1, 'h') → "h"
                                       std::string(3, 'x') → "xxx"
                                   map 的 key 是 string 不是 char，所以 char 得转成 string
       out[key]++                → key 不存在时自动创建并初始化为 0，然后 +1 */
    for (char c : s)
    {
        out[std::string(1, c)]++;
    }
    return out;
}

// 返回 vector<string>，pybind11 自动转为 Python list
std::vector<std::string> split_symbol(const std::string& symbol)
{
    size_t dot = symbol.find('.');
    if (dot == std::string::npos)
    {
        return {symbol};
    }
    return {symbol.substr(0, dot), symbol.substr(dot + 1)};
}

PYBIND11_MODULE(types_demo, m)
{
    m.doc() = "第四步：多种参数类型 + STL 容器自动转换";

    /* py::arg("v")：给参数命名，Python 侧可以按关键字传参
       py::arg("name") = "世界"：带默认值的关键字参数

       没有 py::arg 时： avg([1,2,3])              ← 只能按位置传参
       有了 py::arg 后： avg(v=[1,2,3])            ← 可以按名传参
       带默认值时：       greet()                   ← 不传就用默认
                         greet(name="Python")     ← 传了就覆盖 */
    m.def("avg", &avg, py::arg("v"), "求 vector<double> 的均值");

    m.def("greet", &greet, py::arg("name") = "世界",
          "打招呼（name 有默认值）");

    m.def("count_letters", &count_letters, py::arg("s"),
          "统计每个字符出现次数，返回 dict");

    m.def("split_symbol", &split_symbol, py::arg("symbol"),
          "按 '.' 拆分股票代码，返回 list");
}
