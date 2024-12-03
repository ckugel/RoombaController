cmake_minimum_required(VERSION 3.10)
project(RoombaController)

# Standard C++ settings
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Add source and test directories
add_subdirectory(src)
add_subdirectory(tests)
