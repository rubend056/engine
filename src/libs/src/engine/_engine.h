#ifndef engine_h
#define engine_h

#include <SDL.h>

#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include "engine_globals.h"
#include "gl.h"

// Has initialization of everything as well as basic functions init, update, render, etc...
namespace engine {
void init();
void update();
void render();
void exit();
}  // namespace engine

#endif