# utils库的CMakeLists.txt

# 创建utils静态库
add_library(${PROJECT_NAME}_utils STATIC
    # 这里放utils相关的源文件
    # 由于大部分是头文件模板，我们创建一个空的源文件来编译库
    utils.cpp
    CallbackQueue.cpp
)

# 设置包含目录
target_include_directories(${PROJECT_NAME}_utils PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# 链接线程库
target_link_libraries(${PROJECT_NAME}_utils Threads::Threads)
