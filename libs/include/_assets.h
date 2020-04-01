#ifndef _assets_h
#define _assets_h

#include "engine_globals.h"
#include "assets.h"

#include "rendering/rendering.h"
#include "groups/debug.h"

#include "inotify_imp.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
// #include <limits>

#include "my_filesystem.h"
using namespace std;

#include <boost/algorithm/string/case_conv.hpp>

namespace assets{
    extern thread* inotify_thread;
    void inotify_init();
    
    void import_assets();
}

#endif // _assets_h