#!/bin/bash

set -e

./deps.sh

mkdir -p build
cd build
cmake ..
cd ..