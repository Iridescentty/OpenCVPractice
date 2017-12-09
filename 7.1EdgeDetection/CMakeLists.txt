cmake_minimum_required(VERSION 2.8)
project(edgedetection)

find_package(OpenCV REQUIRED)
include_directories(OpenCV ${OpenCV_DIRS})

add_executable(edgedetection edgedetection.cpp)

target_link_libraries(edgedetection ${OpenCV_LIBS})