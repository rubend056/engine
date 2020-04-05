#!/bin/bash

if hash pacman ; then
    sudo pacman --needed -S glm assimp inotify-tools
elif hash apt ; then 
    sudo apt install libglm-dev libassimp-dev inotify-tools-dev libinotifytools0-dev 
fi