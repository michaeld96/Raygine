cmake_minimum_required(VERSION 3.10) 
set(PROJECT_NAME Raygine)
# Project name and version.
project(${PROJECT_NAME} VERSION 0.1)

# Set the C++ Standard.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# Set default build to debug.
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Add source files.
set(SRC_FILES ${PROJECT_SOURCE_DIR}/src/raymaster_driver.cpp)

# Add executable
add_executable(${PROJECT_NAME}  ${SRC_FILES})