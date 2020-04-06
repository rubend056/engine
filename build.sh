#!/bin/bash

set -e

cd extern/glad
make
cd ../..

cd build
make -j4
cd ..