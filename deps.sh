#!/bin/bash

set -e

if hash pacman ; then
    sudo pacman --needed -S glm assimp inotify-tools
elif hash apt ; then 
    sudo apt-get install libglm-dev libassimp-dev libinotifytools0-dev 
fi