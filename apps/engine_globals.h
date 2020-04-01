#ifndef Engine_Globals_h
#define Engine_Globals_h

#include "gameobject.h"

#include <string>
#include <vector>
#include <memory>
#include "my_filesystem.h"


// Has initialization of everything as well as basic functions init, update, render, etc...
namespace engine{
	// Controls whether the engine should stop
    extern bool run;
	
	// All the gameobjects in the scene
    extern std::vector<std::shared_ptr<GameObject>> objects;
	// All the currently selected gameobjects
    extern std::vector<std::shared_ptr<GameObject>> selected;
	
	std::shared_ptr<GameObject> instantiate(GameObject*obj=nullptr);
	
	// The absolute path for the project
    extern fs::path project_path;
	// Returns aboslute path to data file
	fs::path get_absolute_from_project(const fs::path &asset_path);
	// Should we serialize everything?
	fs::path get_relative_to_project(const fs::path &asset_path);
}


#endif