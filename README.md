<h1 align="center">Glorious C++ Engine</h1>

<p align="center">
<img alt="Ubuntu18.04 Build Status" src="https://github.com/rubend056/engine/workflows/Ubuntu18.04/badge.svg">

I've been meaning to make my own game engine for years now. Ever since using Unity3D I've always felt like I needed more control. This project is the answer to that:

Summing up all my knowledge of C++ over the years. This engine will stitch up multiple libries like *SDL2, Assimp, Cereal, SOIL* as well as the OpenGL loader *Glad* to create a simple but powerful game creation platform that suits my purposes. It will be a colossal task and an equaly immense learning oportunity.

One of the things I aim to overcome is my utter lack to plan ahead and set goals. I shall be tackling that issue with text file `idea.txt` and the various planning tools Github has to offer, **Projects**,  **Issues**, **Pull Requests**, and **Branches**. I will, define a feature to implement in Projects, order by time estimated, then execute based on the most critical feature needed.

<br>

Engine Compiling
--------
- LINUX
  - Run `./config.sh` to install dependencies
  - Run `./build.sh` to compile the engine
  - Run (pptionally) `./copytestgame.sh` to duplicate the test game project folder
  - Run `./build/engine game0` (or `./build/engine game1` if you chose to copy the base game) to launch the engine
	
- WINDOWS
  - Not available yet

- MAC
  - Not available yet

Game Compiling
---------
- Linux (x86_64)
  - Not yet

- Android (armv7, etc...)
  - Download NDK and Android SDK 16 and modify `extern/sdl2-src/android-project/local.properties` to point to them. (This is 4Gb last time I downloaded it all)
  - Make sure you have the Java SDK installed for your machine. (optionally, if gradlew asks for it, point the enviroment vairable to the right folder).
  - Run Gradlew -> `extern/sdl2-src/android-project/gradlew` with the, (I belive) `installDebug` argument. This will install the game to your phone

- Windows
  - Not available yet
  
- Mac
  - Not available yet

Progress
-------

<p align="center"><b>Day 1</b></p>
<p align="center">
<img class="image" width="90%" style="width: 90%;" alt="A simple screen" src="images/day1.png">

<p align="center"><b>Day 3</b></p>
<p align="center">
<img class="image" width="90%" alt="File Editor, and file events with inotify" src="images/day3.png">

<p align="center"><b>Day 10</b></p>
<p align="center">
Trying to get the textures working:<br>
<p align="center">
<img class="image" width="90%" alt="Still trying to get the textures working" src="images/day10.png">

<p align="center"><b>Day 25</b></p>
<p align="center">
Now compiles for Ubuntu 18.04 as well as Archlinux and is configurable to use OpenGL Core 3.0 or 4.5:

|Main | New Scene|
:----:|:---------:
|![](images/day25_1.png) | ![](images/day25_0.png) |

<p align="center"><b>Day 30</b></p>
<p align="center">
<img class="image" width="90%" src="images/day30.gif">

<p align="center"><b>Day 33</b></p>
<p align="center">
<img class="image" width="90%" src="images/day33.png">

<p align="center"><b>Day 49</b></p>
<p align="center">
A huge range of improvement from serailizing to getting the textures finally working. As well as dynamic project reloading:

|Textures | Color Edit|
:----:|:---------:
|![](images/day49_0.png) | ![](images/day49_1.png) |

<br>
