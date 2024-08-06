#!/bin/bash

BUILD_DIR="../build"

if [ -d "$BUILD_DIR" ]; then
    cd "$BUILD_DIR"
    cmake ..
else
    mkdir "$BUILD_DIR"
    cd "$BUILD_DIR" 
    cmake ..
fi