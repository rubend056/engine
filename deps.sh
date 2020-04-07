#!/bin/bash

set -e

if hash pacman ; then
    sudo pacman --noconfirm --needed -S glm assimp inotify-tools
elif hash apt ; then 
    sudo apt-get -y install cmake build-essential libsdl2-dev libglm-dev libassimp-dev libinotifytools0-dev      libx11-dev 
fi