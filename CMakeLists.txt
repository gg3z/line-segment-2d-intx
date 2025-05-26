# Minimum version of CMake
cmake_minimum_required(VERSION 3.12)

# Project name and language
project(lineseg LANGUAGES CXX)

# Set the C++ standard (C++17 or C++20)
set(CMAKE_CXX_STANDARD 20)  # Change to 17 if using C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_BUILD_TYPE Release)

set(SOURCES
    interval.cpp
    lseg.cpp
    lseg_intersector.cpp
    test_intersector.cpp
    main.cpp
    )
# Add the executable
add_executable(lineseg ${SOURCES})

# Specify the compiler if necessary (for clang)
set(CMAKE_CXX_COMPILER C:/Program\ Files/LLVM/bin/clang++.exe)
