cmake_minimum_required(VERSION 3.10) # 设置所需的最低CMake版本
project(Sylar VERSION 1.0) # 定义项目的名称和版本号

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Logger)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ptr)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Utility)

# 添加库
add_library(Logger STATIC ${CMAKE_CURRENT_SOURCE_DIR}/Logger/log.cc)
add_library(Utility STATIC ${CMAKE_CURRENT_SOURCE_DIR}/Utility/cmutex.cc ${CMAKE_CURRENT_SOURCE_DIR}/Utility/util.cpp)

# 添加测试可执行文件
add_executable(ptr_test ${CMAKE_CURRENT_SOURCE_DIR}/test/ptr_test.cpp)
target_link_libraries(ptr_test PRIVATE Logger Utility)

add_executable(shared_ptr_test ${CMAKE_CURRENT_SOURCE_DIR}/test/shared_ptr_test.cpp)
target_link_libraries(shared_ptr_test PRIVATE Logger Utility)

add_executable(test_logger ${CMAKE_CURRENT_SOURCE_DIR}/test/test_logger.cc)
target_link_libraries(test_logger PRIVATE Logger Utility)