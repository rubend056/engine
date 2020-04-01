#ifndef Menus_h
#define Menus_h

#include "imgui.h"
#include "gameobject.h"

#include <iostream>
#include <string>
#include <vector>
#include "my_filesystem.h"
#include "misc/cpp/imgui_stdlib.h"

namespace menus
{
    // // bool inspector_open();
    // static bool 
    // inspector_open=0,
    // files_open=0
    // // text_editor_open=0
    // ;
	extern std::shared_ptr<IDraw> inspector_o;
    void inspector(bool* p_open=NULL);

    void files(bool* p_open=NULL);
	void programs(bool* p_open=NULL);
	void scene(bool* p_open=NULL);
    
    void open_text_editor(fs::path path);
    void text_editor();
    void stats(bool* p_open=NULL);
	
	template <class T>
	std::shared_ptr<T> add_popup(std::vector<std::shared_ptr<T>>& vector) {
		static_assert(std::is_base_of<File, T>::value, "T not derived from File");

		if (ImGui::BeginPopup("add_popup")) {
			for (auto& v : vector) {
				std::shared_ptr<File> draw_inst = std::dynamic_pointer_cast<File>(v);
				// if(!draw_inst) ImGui::Text("%s ")
				if (ImGui::Button(draw_inst->filename().c_str())) {
					ImGui::CloseCurrentPopup();
					ImGui::EndPopup();
					return v;
				}
			}
			ImGui::EndPopup();
		}
		return std::shared_ptr<T>();
	}
}; // namespace menus



#endif