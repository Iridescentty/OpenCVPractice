cmake_minimum_required(VERSION 2.8.0)
project(redeyes)

find_package(OpenCV 3.1.0 REQUIRED)
include_directories(${OpenCV_DIRS})

add_executable(redeyes redeyes.cpp)
target_link_libraries(redeyes ${OpenCV_LIBS})