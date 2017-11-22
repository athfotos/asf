cmake_minimum_required(VERSION 3.8)
project(asf)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/Asf.cpp src/Asf.h src/MyFileReader.cpp src/MyFileReader.h)
add_executable(asf ${SOURCE_FILES})