#ifndef engine_globals_h
#define engine_globals_h

// #include "scene.h"

#include "my_filesystem.h"
// #include <memory>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define ARRAY_END(arr) ((arr) + ARRAY_SIZE(arr))

// Has initialization of everything as well as basic functions init, update, render, etc...
namespace engine {
// Controls whether the engine should stop
extern bool run;
extern double deltaTime;
extern double time;

// The absolute path for the project
extern fs::path project_path;
/**
 * Returns true if path is relative to project
 */
bool is_relative_to_project(const fs::path &asset_path);
/**
 * Returns absolute path from relative path
 */
fs::path get_absolute_from_project(const fs::path &asset_path);
/**
 * Returns relative path from absolute path
 */
fs::path get_relative_to_project(const fs::path &asset_path);
}  // namespace engine

#endif