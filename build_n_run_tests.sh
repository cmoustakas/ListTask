#!/bin/bash

echo "[+] Build and run Unit tests"
# Update and initialize git submodules
git submodule update --init --recursive

# Create the build directory if it doesn't exist
mkdir -p build
cd build || exit 1

cmake ..

# Leaves 1 core free to avoid system slowdown
CORES=$(nproc --ignore=1)  
make -j"$CORES"

./task_list_tests
