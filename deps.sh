if hash pacman ; then
    sudo pacman --needed -S assimp inotify-tools
elif hash apt ; then 
    sudo apt install libglm-dev inotify-tools-dev libinotifytools0-dev 
fi