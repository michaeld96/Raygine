#!/bin/bash

BUILD_DIR="../build"
CMAKE_FILES_PATH="../build/CMakeFiles"
MAKEFILE_PATH="../build/Makefile"

if [ ! -d "$BUILD_DIR" ]; then
    bash build
fi

if [ ! -d "$CMAKE_FILES_PATH" ]; then
    cd "$BUILD_DIR"
    cmake ..
fi
cd "$BUILD_DIR"
make
make run

