#ifndef GameObject_h
#define GameObject_h

#include <vector>
#include <glm/vec3.hpp>
#include "components/components.h"
#include "serialize.h"

class GameObject:public File{
    
    public:
        bool enabled = true;
		
        std::vector<std::shared_ptr<Component>> comps;
        void add_component(std::shared_ptr<Component> c);
		
        GameObject(const char* _f=nullptr):File(_f){
			comps.push_back(std::shared_ptr<Component>(new Transform()));
		}
};

#endif