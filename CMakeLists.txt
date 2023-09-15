# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.12)

# Define the project name
project(CharCounter)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

# Find the Windows SDK for Windows-specific functions
find_package(WindowsSDK REQUIRED)

# Create an executable target named "CharCounter" and specify the source file(s)
add_executable(CharCounter Main.cpp)

# Link the necessary Windows SDK libraries to the target
target_link_libraries(CharCounter WindowsSDK::WindowsSDK)
