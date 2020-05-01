#ifndef engine_globals_h
#define engine_globals_h

#include "gameobject.h"

#include <string>
#include <vector>
#include <memory>

#include "my_filesystem.h"
#include "scene.h"


// Has initialization of everything as well as basic functions init, update, render, etc...
namespace engine{
	// Controls whether the engine should stop
    extern bool run;
	extern double deltaTime;
	extern double time;
	
	// All the gameobjects in the scene
    extern std::shared_ptr<Scene> scene;
	// All the currently selected gameobjects
    // extern std::vector<std::shared_ptr<GameObject>> selected;
	
	
	
	// The absolute path for the project
    extern fs::path project_path;
	// Returns aboslute path to data file
	fs::path get_absolute_from_project(const fs::path &asset_path);
	// Should we serialize everything?
	fs::path get_relative_to_project(const fs::path &asset_path);
	
	void save_scene();
	void load_scene(const std::shared_ptr<Scene>& _scene);
}


#endif