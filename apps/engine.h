#ifndef Engine_h
#define Engine_h

#include <vector>
#include <type_traits>
#include "gameobject.h"

#include <iostream>
#include <string>
#include <SDL.h>



#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#endif




// Has initialization of everything as well as basic functions init, update, render, etc...
namespace engine{
    extern bool run;
    extern std::vector<GameObject*> objects;
    extern std::vector<GameObject*> selected;
    extern std::string project_path;
    
    void init();
    void update();
    void render(SDL_Window* window, ImGuiIO& io);
    
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