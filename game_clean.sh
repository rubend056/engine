#!/bin/bash

# FILES="game0"/*.meta
# echo $FILES

# rm game0/*.meta game0/*.prgm game0/*.scene
find game0 -type f -name "*.meta" -delete
find game0 -type f -name "*.scene" -delete
find game0 -type f -name "*.prgm" -delete
find game0 -type f -name "*.json" -delete