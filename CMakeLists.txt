cmake_minimum_required(VERSION 2.8)

project( Stippling )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} include )

add_subdirectory( src )