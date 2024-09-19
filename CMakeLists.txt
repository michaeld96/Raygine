cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME Raygine)

# Project name and version.
project(${PROJECT_NAME} VERSION 0.1)

# Set the C++ Standard.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set default build to debug.
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Making sure debug symbols are included.
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# Find SDL2
if (APPLE)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    # include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
    # link_directories(/usr/local/lib)
elseif (UNIX)
    # Find SDL2 using pkg-config
    find_package(PkgConfig REQUIRED)
    # Finding pkgConfig, use this to populate SDL2 variable.
    # When found, it populates *_INCLUDE_DIRS and *_LIBRARIES.
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
endif()

# Include directories.
include_directories(${PROJECT_SOURCE_DIR}/include
                    ${SDL2_INCLUDE_DIRS}
                    ${SDL2_IMAGE_INCLUDE_DIRS}
                    ${PROJECT_SOURCE_DIR}/test
                    ${PROJECT_SOURCE_DIR}/libs)

# Add source files.
file(GLOB SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Add executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Link SDL2 to the executable using the imported target
target_link_libraries(${PROJECT_NAME} 
                      ${SDL2_LIBRARIES}
                      ${SDL2_IMAGE_LIBRARIES})

# Adding custom target to run the executable, "make run".
add_custom_target(
    run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
    COMMENT "Running ${PROJECT_NAME}..."
)

# Adding testing src files.
set(TEST_VEC2_SRC_FILE ${PROJECT_SOURCE_DIR}/test/vec2_tests.cpp
                       )

add_executable(TestVec2 ${TEST_VEC2_SRC_FILE})

add_custom_target(
    test_vec2
    COMMAND TestVec2
    DEPENDS TestVec2
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
    COMMENT "Running Vec2 Tests..."
)