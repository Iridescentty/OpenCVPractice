cmake_minimum_required( VERSION 2.8 )
project( main )

#OpenCV
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_DIRS} )

add_executable( main main.cpp )

#链接OpenCV库
target_link_libraries( main ${OpenCV_LIBS} )