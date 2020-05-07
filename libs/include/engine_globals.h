#ifndef engine_globals_h
#define engine_globals_h

// #include "scene.h"

#include "my_filesystem.h"
// #include <memory>


// Has initialization of everything as well as basic functions init, update, render, etc...
namespace engine{
	// Controls whether the engine should stop
    extern bool run;
	extern double deltaTime;
	extern double time;
	
	// The absolute path for the project
    extern fs::path project_path;
	// Returns aboslute path to data file
	fs::path get_absolute_from_project(const fs::path &asset_path);
	// Should we serialize everything?
	fs::path get_relative_to_project(const fs::path &asset_path);
}


#endif