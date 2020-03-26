#ifndef Engine_Globals_h
#define Engine_Globals_h


#include "gameobject.h"

#include <string>
#include <vector>
#include <memory>

// Has initialization of everything as well as basic functions init, update, render, etc...
namespace engine{
    extern bool run;
    extern std::vector<std::shared_ptr<GameObject>> objects;
    extern std::vector<std::shared_ptr<GameObject>> selected;
    extern std::string project_path;
}


#endif