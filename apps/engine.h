#ifndef Engine_h
#define Engine_h

#include "engine_globals.h"

#include <vector>
#include <type_traits>
#include "gameobject.h"

#include <iostream>
#include <string>

#include <SDL.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "groups/gl_group.h"


// Has initialization of everything as well as basic functions init, update, render, etc...
namespace engine{
    
    void init();
    void update();
    void render(SDL_Window* window, ImGuiIO& io);
	void exit();
    
    GameObject* instantiate();
    void destroy(GameObject* gobj);
}



// class Engine{
//     static Engine* _instance;
//     static Engine* instance(){if (!_instance)_instance = new Engine;return _instance;};
//     Engine(){};

//     public:
//         std::vector<GameObject*> objects;
        
//         // template <class T>
//         // static T* instantiate(){
//         //     static_assert(std::is_base_of<GameObject, T>::value, "The type you are trying to make must derive from GameObject");
//         //     T* gobj = new T;
//         //     Engine::instance()->objects.push_back(gobj);
//         //     return gobj;
//         // }
        
//         static GameObject* instantiate(){
//             auto gobj = new GameObject;
//             objects.push_back(gobj);
//             return gobj;
//         }

// };



#endif