#ifndef GameObject_h
#define GameObject_h

// #include <SDL2/SDL.h>
// #include <SDL2/SDL2_gfxPrimitives.h>

#include <vector>
#include <glm/vec3.hpp>
#include "component.h"

class GameObject{
    public:
        bool enabled = true;
        glm::vec3 pos = glm::vec3(0);
        glm::vec3 rot = glm::vec3(0);
        glm::vec3 sca = glm::vec3(0);

        std::vector<Component*> comps;
        void add_component(Component* c);
        GameObject(){
        }
};

#endif