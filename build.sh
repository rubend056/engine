#!/bin/bash

THREADS=${1:-1}

set -e

cd extern/glad
make
cd ../..

cd build
make -j${THREADS}
cd ..