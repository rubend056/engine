#!/bin/bash

mkdir -p build
cd build
cmake .. && make -j1 && ./engine ../game0
cd ..