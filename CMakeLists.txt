cmake_minimum_required(VERSION 3.0.0)
project(Video VERSION 0.1.0 LANGUAGES CXX)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Video main.cpp video.cpp video.h)
target_link_libraries(Video ${OpenCV_LIBS})
