#!/bin/bash

# Script to cross-compile Siamese library for ARMv8 in static mode with Cortex-A53 optimizations

# Create build directory
mkdir -p build_armv8
cd build_armv8

# Check if the cross-compiler is installed
if ! command -v aarch64-linux-gnu-gcc &> /dev/null
then
    echo "Error: aarch64-linux-gnu-gcc is not installed"
    echo "Please install the cross-compiler with:"
    echo "sudo apt-get install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu"
    exit 1
fi

# Configure with CMake using the ARMv8 toolchain
cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=../armv8-toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_TEST=OFF \
    -DCORTEX_A53=ON \
    -DCMAKE_INSTALL_PREFIX=../install_armv8 \
    -DCMAKE_VERBOSE_MAKEFILE=ON

# Build
cmake --build . -- -j$(nproc)

# Install to the local install directory
cmake --install .

echo "Cross-compilation for ARMv8 Cortex-A53 completed successfully."
echo "The static library is available at: $(pwd)/../install_armv8/lib/libsiamese.a"