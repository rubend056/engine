#ifndef _menus_h
#define _menus_h

#include <memory>
#include <string>

#include "idraw.h"
#include "menus.h"

namespace menus {

void inspector(bool* p_open = NULL);
void files(bool* p_open = NULL);
void assets(bool* p_open = NULL);
void programs(bool* p_open = NULL);
void scene(bool* p_open = NULL);
void stats(bool* p_open = NULL);

void open_text_editor(fs::path path);
void text_editor();

// GL STUFF
void load_settings_glEnable();
void window_glEnable_config(bool* p_open);
}  // namespace menus

#endif