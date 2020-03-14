#ifndef Menus_h
#define Menus_h

#include "imgui.h"
#include "gameobject.h"
#include <iostream>
#include <string>
#include <vector>

#include "my_filesystem.h"

namespace menus
{
    // bool inspector_open();
    static bool 
    inspector_open=0,
    files_open=0
    // text_editor_open=0
    ;
    void inspector(GameObject*o);

    void files(std::string root);
    
    void open_text_editor(fs::path path);
    void text_editor();
    void stats();
}; // namespace menus



#endif