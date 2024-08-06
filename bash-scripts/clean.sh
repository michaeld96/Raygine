#!/bin/bash

BUILD_DIR="../build"
if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR"
    echo "Removed build folder."
else
    echo "No build folder present!"
fi