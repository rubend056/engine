#!/bin/bash

mkdir -p build
cmake -S . -B build && cmake --build build
cd build
./engine ../game0
cd ..