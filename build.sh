#!/bin/bash

set -e

cd extern/glad
make
cd ../..


mkdir -p build
cd build
cmake .. && make -j4
cd ..