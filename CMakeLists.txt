cmake_minimum_required(VERSION 3.12)

# Define the project name
project(CharCounter CXX)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

# Add the executable target
add_executable(CharCounter
    main.cpp
)

# Optional: If you want to enable Unicode character set
# target_compile_options(CharCounter PRIVATE /source-charset:utf-8)
# target_compile_options(CharCounter PRIVATE /execution-charset:utf-8)

# Optional: If you want to enable exceptions
# add_compile_options(/EHsc)

# Optional: If you want to enable multithreading
# find_package(Threads REQUIRED)
# target_link_libraries(CharCounter PRIVATE Threads::Threads)

# Optional: If you want to enable Windows-specific features
# if(WIN32)
#     target_compile_definitions(CharCounter PRIVATE _WIN32_WINNT=0x0601) # Windows 7 or later
#     target_compile_definitions(CharCounter PRIVATE _CRT_SECURE_NO_WARNINGS) # Suppress CRT warnings
# endif()
