# src目录的CMakeLists.txt

# 添加子目录
add_subdirectory(utils)
add_subdirectory(DataTree)

# 主可执行文件
add_executable(${PROJECT_NAME}_exe
    main.cpp
)

# 链接库
target_link_libraries(${PROJECT_NAME}_exe
    ${PROJECT_NAME}_utils
    ${PROJECT_NAME}_datatree
    Threads::Threads
)
