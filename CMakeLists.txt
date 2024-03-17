cmake_minimum_required(VERSION 3.0)
project(Webserver)

# 设置编译器
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TARGET_NAME webserver)
set(TARGET_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)

# 添加源文件
file(GLOB SOURCES ${TARGET_PATH}/*.cpp)
# 添加头文件路径
include_directories(${TARGET_PATH}/include)


add_executable(${TARGET_NAME} ${SOURCES})