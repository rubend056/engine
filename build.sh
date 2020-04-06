#!/bin/bash

set -e

./deps.sh
./config.sh

cd extern/glad
make
cd ../..

cd build
make -j4
cd ..