<h1 align="center">Glorious C++ Engine</h1>

<p align="center">
<img alt="Ubuntu18.04 Build Status" src="https://github.com/rubend056/engine/workflows/Ubuntu18.04/badge.svg">

 I've been meaning to make my own game engine for years now. Ever since using Unity3D I've always felt like I needed more control. This project is the answer to that:

Summing up all my knowledge of C++ over the years. This engine will stitch up multiple libries like *SDL2, Assimp, Cereal, SOIL* as well as the OpenGL loader *Glad* to create a simple but powerful game creation platform that suits my purposes. It will be a colossal task and an equaly immense learning oportunity.

One of the things I aim to overcome is my utter lack to plan ahead and set goals. I shall be tackling that issue with text file `idea.txt` and the various planning tools Github has to offer, **Projects**,  **Issues**, **Pull Requests**, and **Branches**. I will, define a feature to implement in Projects, order by time estimated, then execute based on the most critical feature needed.

<br>

<p align="center"><b>Day 1<b></p>
<p align="center">
<img class="image" width="90%" style="width: 90%;" alt="A simple screen" src="images/day1.png">

-----
<p align="center"><b>Day 3<b></p>
<p align="center">
<img class="image" width="90%" alt="File Editor, and file events with inotify" src="images/day3.png">

-----

<p align="center"><b>Day 10<b></p>
<p align="center">
Trying to get the textures working:

<p align="center">
<img class="image" alt="Still trying to get the textures working" src="images/day10.png">

-----
<p align="center"><b>Day 25<b></p>
<p align="center">
Now compiles for Ubuntu 18.04 as well as Archlinux and is configurable to use OpenGL Core 3.0 or 4.5:

|Main | New Scene|
:----:|:---------:
|![](images/day25_1.png) | ![](images/day25_0.png) |

<br>

To Build:
--------

- LINUX
  - Run `./config.sh` to install dependencies
  - Run `./build.sh` to compile the engine
  - Run `./copytestgame.sh` to duplicate the test project folder
  - Run `./launch.sh` to launch the engine
	
- WINDOWS
  - Not available yet

- MAC
  - Not available yet
