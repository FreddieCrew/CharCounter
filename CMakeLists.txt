cmake_minimum_required(VERSION 3.10)

project(CharCounter)  # Project name

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add the executable
add_executable(CharCounter main.cpp)
