#include <typeinfo>

#include "assets.h"
#include "engine_globals.h"
#include "helper.h"
#include "type_name.h"
#include "_menus.h"
#include "rendering.h"

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
	
	char d[engine::scene->filename().size()+1];sprintf(d,"Scene <%s>###scene", engine::scene->filename().c_str());
	ImGui::Begin(d, p_open);
	
	static_assert(std::is_base_of<IDraw, GameObject>::value, "GameObject not derived from IDraw");
	
	if (ImGui::Button("Add GObject", ImVec2(-1,0)))
		engine::scene->instantiate();
	
	ImGui::Separator();
	
	if(engine::scene)
	for(auto&obj:engine::scene->objects){
		ImGui::Text(obj->filename().c_str());
		if(ImGui::IsItemClicked())
			if(auto draw = std::static_pointer_cast<IDraw>(obj))inspector_o.insert(draw);
	}
	
	ImGui::End();
}

}  // namespace menus
