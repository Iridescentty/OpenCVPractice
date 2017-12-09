cmake_minimum_required(VERSION 2.8)
project(remap)

find_package(OpenCV 3.1 REQUIRED)
include_directories(${OpenCV_DIRS})

add_executable(remap remap.cpp)
target_link_libraries(remap ${OpenCV_LIBS})