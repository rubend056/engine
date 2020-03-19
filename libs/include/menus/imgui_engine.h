#ifndef imgui_engine_h
#define imgui_engine_h

#include "cereal/archives/json.hpp"
#include "engine.h"

namespace menus{
	void imgui_engine_update();
}

struct Gl_Enable_Setting
{
    const int index;
    const std::string name;
    const std::string info;
    bool value = false;
    template <class Archive>
    void serialize(Archive &ar) {  
        ar(cereal::make_nvp(name.c_str(), value));
    }
};


extern Gl_Enable_Setting gl_enable_settings[];

void load_settings_glEnable();
void window_glEnable_config(bool*p_open);
void window_open_project(bool*p_open);
void window_save_as(bool*p_open);



#endif // imgui_engine_h