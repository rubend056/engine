#include "engine_globals.h"
#include "engine.h"

namespace engine{
    GameObject* instantiate(){
        auto gobj = new GameObject;
        objects.push_back(gobj);
        return gobj;
    }
    void destroy(GameObject* gobj){
        delete gobj;
    }
}
