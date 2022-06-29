#ifndef _assets_h
#define _assets_h

// #include "engine_globals.h"
#include "assets.h"
#include "debug.h"
#include "inotify_imp.h"
#include "rendering.h"

// #include <assimp/Importer.hpp>
// #include <assimp/scene.h>
// #include <assimp/postprocess.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
// #include <limits>

#include "my_filesystem.h"
using namespace std;

namespace assets {
// extern thread* inotify_thread;
// void inotify_init();

void import_assets();
}  // namespace assets

#endif	// _assets_h