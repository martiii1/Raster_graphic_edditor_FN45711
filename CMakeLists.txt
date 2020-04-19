cmake_minimum_required(VERSION 3.16)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled main.cpp ImageEditor.cpp ImageEditor.hpp Session.cpp Session.hpp ImageData.cpp ImageData.hpp)