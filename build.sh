#!/bin/bash

THREADS=${1:-$(nproc)}
echo "Make using $THREADS parallel jobs"

set -e

# Build Glad
cd extern/glad
./build.sh
cd ../..


# Build 
cd build
make -j${THREADS}
cd ..