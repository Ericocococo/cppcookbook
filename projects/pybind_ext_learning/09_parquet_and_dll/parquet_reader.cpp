// C++17 | 依赖：pybind11 + arrow + parquet（vcpkg）
// 第九步：引入第三方依赖 — 用 arrow/parquet 读真实 parquet 文件。
//
// 数据由 C++ 自己读 parquet 文件（依赖 arrow/parquet 动态库），而不是 Python 构造后传入。
//
// 编译出的 .pyd 依赖 arrow.dll 等第三方 dll，拷贝 .pyd 时必须把依赖 dll 一并拷贝。
//
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <arrow/api.h>
#include <arrow/compute/api.h>   // arrow::compute::Cast
#include <arrow/io/api.h>
#include <parquet/arrow/reader.h>

#include <memory>
#include <stdexcept>
#include <string>

namespace py = pybind11;

namespace {

// parquet datetime 列可能是 Int64 / Timestamp[ns/us/ms]，统一转 int64 纳秒
std::shared_ptr<arrow::Int64Array> CastToInt64NsArray(const std::shared_ptr<arrow::Array>& arr)
{
    if (arr->type_id() == arrow::Type::INT64)
        return std::static_pointer_cast<arrow::Int64Array>(arr);
    if (arr->type_id() == arrow::Type::TIMESTAMP) {
        auto ts_type = std::static_pointer_cast<arrow::TimestampType>(arr->type());
        if (ts_type->unit() == arrow::TimeUnit::NANO)
            return std::static_pointer_cast<arrow::Int64Array>(arr->View(arrow::int64()).ValueOrDie());
        auto cast_res = arrow::compute::Cast(arr, arrow::timestamp(arrow::TimeUnit::NANO));
        if (!cast_res.ok())
            throw std::runtime_error("CastToInt64NsArray: " + cast_res.status().ToString());
        auto ns_arr = arrow::MakeArray(cast_res->array());
        return std::static_pointer_cast<arrow::Int64Array>(ns_arr->View(arrow::int64()).ValueOrDie());
    }
    throw std::runtime_error("CastToInt64NsArray: 不支持的列类型 " + arr->type()->ToString());
}

// parquet 数值列可能是 Double / Float / Int64 / Int32 等，统一转 double
std::shared_ptr<arrow::DoubleArray> CastToDoubleArray(const std::shared_ptr<arrow::Array>& arr)
{
    if (arr->type_id() == arrow::Type::DOUBLE)
        return std::static_pointer_cast<arrow::DoubleArray>(arr);
    auto cast_res = arrow::compute::Cast(arr, arrow::float64());
    if (!cast_res.ok())
        throw std::runtime_error("CastToDoubleArray: " + cast_res.status().ToString());
    return std::static_pointer_cast<arrow::DoubleArray>(arrow::MakeArray(cast_res->array()));
}

// 字符串列（string / large_string）
std::string GetStringValue(const std::shared_ptr<arrow::Array>& arr, int64_t i)
{
    if (arr->type_id() == arrow::Type::STRING)
        return std::static_pointer_cast<arrow::StringArray>(arr)->GetString(i);
    if (arr->type_id() == arrow::Type::LARGE_STRING)
        return std::static_pointer_cast<arrow::LargeStringArray>(arr)->GetString(i);
    return "";
}

}  // namespace

// 读 parquet 文件 → dict of lists（列式），Python 侧 pd.DataFrame(result) 即可。
// 列类型按需转换: datetime → int64 纳秒, 数值 → double, 字符串 → string
py::dict read_parquet(const std::string& path)
{
    // 1. 打开文件
    auto input = arrow::io::ReadableFile::Open(path);
    if (!input.ok()) {
        throw std::runtime_error("打开文件失败: " + input.status().ToString());
    }
    // 2. 创建 parquet reader（OpenFile 2 参数版，返回 Result<unique_ptr>）
    auto reader_result = parquet::arrow::OpenFile(input.ValueOrDie(),
                                                  arrow::default_memory_pool());
    if (!reader_result.ok()) {
        throw std::runtime_error("parquet 打开失败: " + reader_result.status().ToString());
    }
    // 3. 读成 Table + 合并分块
    std::shared_ptr<arrow::Table> table;
    auto read_st = reader_result.ValueOrDie()->ReadTable(&table);
    if (!read_st.ok()) {
        throw std::runtime_error("读表失败: " + read_st.ToString());
    }
    auto combined = table->CombineChunks();
    if (!combined.ok()) {
        throw std::runtime_error("合并分块失败: " + combined.status().ToString());
    }
    table = *combined;

    // 4. 按列取出，转成 Python list
    py::dict result;
    auto schema = table->schema();
    const int64_t rows = table->num_rows();
    for (int i = 0; i < schema->num_fields(); ++i) {
        const std::string& name = schema->field(i)->name();
        auto col = table->GetColumnByName(name);
        if (!col || col->num_chunks() == 0) continue;
        auto arr = col->chunk(0);

        py::list values;
        switch (arr->type_id()) {
            case arrow::Type::TIMESTAMP:
            case arrow::Type::INT64: {
                auto ns_arr = CastToInt64NsArray(arr);
                for (int64_t r = 0; r < rows; ++r)
                    values.append(ns_arr->Value(r));
                break;
            }
            case arrow::Type::DOUBLE:
            case arrow::Type::FLOAT:
            case arrow::Type::INT32: {
                auto d_arr = CastToDoubleArray(arr);
                for (int64_t r = 0; r < rows; ++r)
                    values.append(d_arr->Value(r));
                break;
            }
            case arrow::Type::STRING:
            case arrow::Type::LARGE_STRING:
                for (int64_t r = 0; r < rows; ++r)
                    values.append(GetStringValue(arr, r));
                break;
            default:
                // 未处理的类型按字符串转
                for (int64_t r = 0; r < rows; ++r)
                    values.append(arr->ToString());
                break;
        }
        result[py::str(name)] = values;
    }
    return result;
}

PYBIND11_MODULE(parquet_reader, m)
{
    m.doc() = "第九步：arrow/parquet 读真实 parquet 文件";
    m.def("read_parquet", &read_parquet, py::arg("path"),
          "读 parquet 文件，返回 {列名: [值...]}（Python 侧可转 pd.DataFrame）");
}
