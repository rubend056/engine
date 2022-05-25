/**
 * @file inspector.cpp
 * @author RubenD (rubendariopm14@gmail.com)
 * @brief Defines inspector menu
 * @version 0.1
 * 
 */
#include <typeinfo>

// Ours
#include "engine_globals.h"
#include "engine_scene.h"
#include "helper.h"
#include "type_name.h"
#include "helper.h"
#include "_menus.h"

using namespace std;

namespace menus {
#define MAX_WIN_NAME 100
std::set<std::shared_ptr<IDraw>> inspector_o;
void inspector(bool* p_open) {
	//? INSPECTOR NAME AND COMPONENT LOOP
	if(!inspector_o.size())return;
	
	ImGui::SetNextWindowSize(ImVec2(250, 300), ImGuiCond_FirstUseEver);
	auto d = helper::string_format("Inspector");
	ImGui::Begin(d.c_str(), p_open);
	
	
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
	if(ImGui::BeginTabBar("Items")){
		int i=0;
		for(auto&item:inspector_o){
			if(ImGui::BeginTabItem(helper::string_format("%s###%d", item->imgui_name().c_str(), i++).c_str())){
				if(auto r = std::dynamic_pointer_cast<Referentiable>(item))
				if(r->parent){
					if(ImGui::Button("Delete")){
						Referentiable::orphan(r);
						ImGui::EndTabItem();
						break;
					}
					ImGui::SameLine();
				}
				// ImGui::SetNextItemWidth(-1);
				if(ImGui::Button("Close", ImVec2(0,0))){
					inspector_o.erase(std::find(inspector_o.begin(), inspector_o.end(), item));
					ImGui::EndTabItem();break;
				}
				
				item->imgui_draw();
				ImGui::EndTabItem();
			}
		}
		ImGui::EndTabBar();
	}
	ImGui::PopStyleVar();
	
	
	
	ImGui::End();
}

void scene(bool* p_open){
	if(!engine::scene)return;
	
	char d[engine::scene->filename().size()+1];
	sprintf(d,"Scene <%s>###scene", engine::scene->filename().c_str());
	ImGui::Begin(d, p_open);
	
	static_assert(std::is_base_of<IDraw, GameObject>::value, "GameObject not derived from IDraw");
	
	if (ImGui::Button("Add GameObject", ImVec2(-1,0))){
		auto go = std::make_shared<GameObject>();
		go->set_filename("GameObject");
		engine::scene->foster(go);
	}
		
	
	ImGui::Separator();
	
	if(engine::scene){
		auto objects = helper::dynamic_pointer_cast_array<GameObject>(engine::scene->children);
		for(auto&obj:objects){
			ImGui::Text(obj->filename().c_str());
			if(ImGui::IsItemClicked())
				if(auto draw = std::static_pointer_cast<IDraw>(obj))inspector_o.insert(draw);
		}
	}
	
	ImGui::End();
}

}  // namespace menus
