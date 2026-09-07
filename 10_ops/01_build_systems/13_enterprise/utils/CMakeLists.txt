add_library(utils STATIC string_utils.cpp)
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(utils PRIVATE project_options)
