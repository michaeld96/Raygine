cmake_minimum_required(VERSION 3.27)
project(Raygine VERSION 0.1)

# Set the C++ Standard.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define build types.
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

# Set default build type to Debug if not specified.
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Ensure debug symbols are included.
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")

# Find pkg-config.
find_package(PkgConfig REQUIRED)

# Find SDL2.
pkg_check_modules(SDL2 REQUIRED sdl2)

# Find SDL2_image.
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

# Include directories.
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/test
    ${PROJECT_SOURCE_DIR}/libs
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
)

# Link directories.
link_directories(
    ${SDL2_LIBRARY_DIRS}
    ${SDL2_IMAGE_LIBRARY_DIRS}
)

# Add source files.
file(GLOB SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Add executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Link SDL2 and SDL2_image libraries.
target_link_libraries(${PROJECT_NAME} 
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
)

# Custom target to run the executable.
add_custom_target(
    run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
    COMMENT "Running ${PROJECT_NAME}..."
)

# Adding testing source files.
set(TEST_VEC2_SRC_FILE "${PROJECT_SOURCE_DIR}/test/vec2_tests.cpp")

add_executable(TestVec2 ${TEST_VEC2_SRC_FILE})

add_custom_target(
    test_vec2
    COMMAND TestVec2
    DEPENDS TestVec2
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
    COMMENT "Running Vec2 Tests..."
)
