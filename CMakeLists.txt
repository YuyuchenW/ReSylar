cmake_minimum_required(VERSION 3.10) # 指定最小版本
project(Sylar VERSION 1.0)          # 定义项目名称和版本

# 设置编译器标志
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加子目录
# add_subdirectory(Logger)
# add_subdirectory(Utility)
# add_subdirectory(ptr)
# add_subdirectory(test)
# 添加可执行文件
# add_executable(sylar_logger Logger/log.cc)
# target_include_directories(sylar_logger PRIVATE Logger)

# add_executable(sylar_utility Utility/cmutex.cc)
# target_include_directories(sylar_utility PRIVATE Utility)
set(SYLAR_SRC 
    cunique_ptr.hpp
)

add_executable(unique_ptr  test/ptr_test.cpp)
