#include "engine_globals.h"
#include "menus/menus.h"
#include "engine.h"

void menus::stats(bool* p_open){
    ImGui::Begin("Stats", p_open);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Number of GameObjects: %d", engine::objects.size());
    
    ImGui::End();
}