#ifndef Engine_h
#define Engine_h

#include <vector>
#include <type_traits>
#include "gameobject.h"

// class Engine; // Forward declaration

class Engine{
    static Engine* _instance;
    static Engine* instance(){if (!_instance)_instance = new Engine;return _instance;};
    Engine(){};

    public:
        std::vector<GameObject*> objects;
        
        // template <class T>
        // static T* instantiate(){
        //     static_assert(std::is_base_of<GameObject, T>::value, "The type you are trying to make must derive from GameObject");
        //     T* gobj = new T;
        //     Engine::instance()->objects.push_back(gobj);
        //     return gobj;
        // }
        
        static GameObject* instantiate(){
            // static_assert(std::is_base_of<GameObject, T>::value, "The type you are trying to make must derive from GameObject");
            auto gobj = new GameObject;
            Engine::instance()->objects.push_back(gobj);
            return gobj;
        }

};

#endif