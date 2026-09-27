#cmakelist文件是从书上偷来的；
cmake_minimum_required(VERSION 2.6)
#创建工程
project(testopencv)
#C++版本为 11
set(CMAKE_CXX_FLAGS "-std=c++11")
find_package(OpenCV 4.1 REQUIRED) #找 OpenCV 4.1 安装路径
include_directories(${OpenCV_INCLUDE_DIRS}) #加载 OpenCV 4.1 的头文件
add_executable(zuoye_one main.cpp)
#将程序生成可执行文件
target_link_libraries(zuoye_one ${OpenCV_LIBS}) #链接 lib 文件到可执行文件中
