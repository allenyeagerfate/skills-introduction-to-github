# DataTree库的CMakeLists.txt

# 创建DataTree静态库
add_library(${PROJECT_NAME}_datatree STATIC
    # DeviceData相关实现
    DeviceData.cpp
    # ModbusTcp相关实现  
    ModbusTcpTree.cpp
    modbus.cpp
)

# 设置包含目录
target_include_directories(${PROJECT_NAME}_datatree PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# 依赖utils库
target_link_libraries(${PROJECT_NAME}_datatree 
    ${PROJECT_NAME}_utils
    Threads::Threads
)
