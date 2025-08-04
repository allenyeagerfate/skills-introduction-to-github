# test目录的CMakeLists.txt

# 简单的测试可执行文件
add_executable(${PROJECT_NAME}_test
    main_test.cpp
    device_data_test.cpp
    utils_test.cpp
)

# 链接测试需要的库
target_link_libraries(${PROJECT_NAME}_test
    ${PROJECT_NAME}_utils
    ${PROJECT_NAME}_datatree
    Threads::Threads
)

# 添加测试
add_test(NAME unit_tests COMMAND ${PROJECT_NAME}_test)
