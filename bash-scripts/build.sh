#!/bin/bash

BUILD_DIR="../build"

if [ -d "$BUILD_DIR" ]; then
    echo "Going into build directory and running cmake..."
    cd "$BUILD_DIR"
    cmake ..
else
    echo "Creating build directory..."
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR" 
    cmake ..
fi