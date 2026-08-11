// C++20 | 依赖：pybind11
// 第一步：最小 pybind11 模块 — 只绑定一个函数。
//
// 从这里开始，你将第一次把 C++ 函数暴露给 Python。
// 整个文件只有 3 行有效代码（不算 include），但跑通了完整的 C++ → Python 流程。
//
// Python 使用:
//   import hello
//   hello.add(3, 5)  # → 8
//
// pybind11 安装后的目录结构：
//   pybind11/
//     include/
//       pybind11/
//         pybind11.h      ← 所以写 #include <pybind11/pybind11.h>
//         stl.h           ← 用 STL 容器自动转换时写 #include <pybind11/stl.h>
// CMake 把 include/ 加入头文件搜索路径，加 pybind11/ 前缀是为了防重名
#include <pybind11/pybind11.h>   // pybind11 核心头文件

namespace py = pybind11;         // 本步没用上，但后续会大量写 py::arg / py::class_ 等，先认识这个缩写

// 这是一个普通的 C++ 函数，和 Python 没有任何关系
int add(int a, int b) {
    return a + b;
}

// PYBIND11_MODULE：pybind11 入口宏，Python import 时自动调用。
//   hello = Python 侧的模块名（import hello）
//   m     = 模块对象，往上面挂函数/类
//
// 整个宏展开后大致是这样的（不用深究，感受一下原理）：
//   extern "C" PyObject* PyInit_hello() {          // Python 加载 .pyd 时调用这个函数
//       auto m = pybind11::module_("hello", ...);  // 创建模块对象
//       m.doc() = "...";                            // 设 __doc__
//       m.def("add", &add, "...");                 // 注册函数
//       return m.ptr();                             // 把模块交给 Python
//   }
//
// m.doc()：设置模块的 __doc__，对应 Python 侧的 help(hello) 内容
//
// m.def：把 C++ 函数注册为 Python 函数
//   第1个参数 = Python 侧的函数名  ← 两边名字可以不一样，但习惯保持一致
//   第2个参数 = C++ 函数的指针      ← &add 就是取 add 函数的地址
//   第3个参数 = 函数文档字符串       ← 需要配合 py::arg 使用才生效
//
// 做完后 Python 侧等于有了：
//   hello.add = add           # C++ 函数挂到模块上
//   hello.__doc__ = "..."     # m.doc() 设置的模块文档
PYBIND11_MODULE(hello, m) {
    m.doc() = "第一步：最小 pybind11 模块";
    m.def("add", &add, "两个整数相加");
}
