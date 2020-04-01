
#include "engine.h"

namespace engine{
    std::shared_ptr<GameObject> instantiate(GameObject*obj){
        auto gobj = std::shared_ptr<GameObject>(obj?obj:new GameObject);
        objects.push_back(gobj);
        return gobj;
    }
}
