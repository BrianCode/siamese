# CMake toolchain file for ARMv8 (aarch64) cross-compilation optimized for Cortex-A53

# Specify the target system
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Specify the cross compiler
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

# Specify where the target environment is
set(CMAKE_FIND_ROOT_PATH /usr/aarch64-linux-gnu)

# Search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Search for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Don't use host's stuff
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# ARMv8 Cortex-A53 specific compile flags with enhanced optimizations
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv8-a+crypto -mtune=cortex-a53 -O3 -ftree-vectorize -fPIC -funroll-loops -fno-strict-aliasing -flto")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8-a+crypto -mtune=cortex-a53 -O3 -ftree-vectorize -fPIC -funroll-loops  -fno-strict-aliasing -flto")

# Set linker flags for Link-Time Optimization
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -flto")

# Enable NEON optimizations
add_definitions(-DGF256_TRY_NEON -DGF256_TARGET_MOBILE)

# Disable automatic -march=native in the main CMakeLists.txt
set(COMPILER_SUPPORTS_MARCH_NATIVE OFF CACHE BOOL "Disable automatic -march=native")

# Set Cortex-A53 specific variable
set(CORTEX_A53 ON CACHE BOOL "Optimizing for Cortex-A53")
