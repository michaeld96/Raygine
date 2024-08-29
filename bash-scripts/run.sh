#!/bin/bash

BUILD_DIR="../build"
CMAKE_FILES_PATH="../build/CMakeFiles"
MAKEFILE_PATH="../build/Makefile"

# run the build script.

sh build.sh

cd "$BUILD_DIR"
make
make run

