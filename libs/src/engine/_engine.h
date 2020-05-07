#ifndef engine_h
#define engine_h

#include "engine_globals.h"
#include "gl.h"

#include <vector>
#include <type_traits>

#include <iostream>
#include <string>

#include <SDL.h>


// Has initialization of everything as well as basic functions init, update, render, etc...
namespace engine{
    void init();
    void update();
    void render();
	void exit();
}



#endif