#ifndef engine_scene_h
#define engine_scene_h

#include <memory>

#include "scene.h"

// Has initialization of everything as well as basic functions init, update, render, etc...
namespace engine {
// All the gameobjects in the scene
extern std::shared_ptr<Scene> scene;
// All the currently selected gameobjects
// extern std::vector<std::shared_ptr<GameObject>> selected;
void save_engine_state();
void load_scene(const std::shared_ptr<Scene>& _scene);
}  // namespace engine

#endif
