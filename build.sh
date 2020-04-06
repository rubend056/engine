#!/bin/bash

THREADS=${1:-$(nproc)}
echo "Make using $THREADS pallalel jobs"

set -e

cd extern/glad
make
cd ../..

cd build
make -j${THREADS}
cd ..