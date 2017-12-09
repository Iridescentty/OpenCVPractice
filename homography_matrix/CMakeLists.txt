cmake_minimum_required(VERSION 2.8.0)
project(homography)

find_package(OpenCV 3.1.0 REQUIRED)
include_directories(${OpenCV_DIRS})

add_executable(homography homography.cpp)
target_link_libraries(homography ${OpenCV_LIBS})