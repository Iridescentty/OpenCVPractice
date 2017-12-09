cmake_minimum_required(VERSION 2.8.0)
project(roi)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_DIRS})

add_executable(roi roi.cpp)
target_link_libraries(roi ${OpenCV_LIBS})