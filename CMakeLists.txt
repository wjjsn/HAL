cmake_minimum_required(VERSION 3.22)
enable_language(CXX)

# 纯头文件库（header-only），导出头文件搜索路径
add_library(HAL INTERFACE)
target_include_directories(HAL INTERFACE "${CMAKE_CURRENT_LIST_DIR}")