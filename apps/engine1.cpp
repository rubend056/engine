
#include "engine.h"

namespace engine{
    std::shared_ptr<GameObject> instantiate(){
        auto gobj = std::shared_ptr<GameObject>(new GameObject);
        objects.push_back(gobj);
        return gobj;
    }
}
