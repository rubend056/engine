#ifndef Engine_h
#define Engine_h

#include "engine_globals.h"
#include "gameobject.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "groups/gl.h"

#include <vector>
#include <type_traits>

#include <iostream>
#include <string>

#include <SDL.h>


// Has initialization of everything as well as basic functions init, update, render, etc...
namespace engine{
    
    void init();
    void update();
    void render(SDL_Window* window, ImGuiIO& io);
	void exit();
    
    
    // void destroy(GameObject* gobj);
}

// struct A{
	
// };
// struct B{
// 	int j=4;
// 	virtual void t()=0;
// 	template<class Archive>
// 	void serialize(Archive & ar){
// 		ar(CEREAL_NVP(j));
// 	}
// };
// class J:public B{
// public:
// 	void t(){};
// 	int l=2;
// 	template<class Archive>
// 	void serialize(Archive & ar){
// 		ar(cereal::base_class<B>(this), CEREAL_NVP(l));
// 	}
// };
// #include "cereal/types/polymorphic.hpp"
// #include "cereal/archives/json.hpp"

// CEREAL_REGISTER_TYPE(J);

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