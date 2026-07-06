add_executable(test_engine test_engine.cpp)
target_link_libraries(test_engine PRIVATE core project_options)
add_test(NAME engine COMMAND test_engine)

add_executable(test_utils test_utils.cpp)
target_link_libraries(test_utils PRIVATE utils project_options)
add_test(NAME utils COMMAND test_utils)
