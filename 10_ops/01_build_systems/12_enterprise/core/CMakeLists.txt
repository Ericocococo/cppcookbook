add_library(core STATIC engine.cpp)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(core
    PUBLIC  utils          # 消费者也能用 utils 头文件
    PRIVATE project_options
)
