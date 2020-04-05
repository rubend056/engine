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
std::shared_ptr<IDraw> inspector_o;
void inspector(bool* p_open) {
	//? INSPECTOR NAME AND COMPONENT LOOP
	if(!inspector_o)return;
	
	char d[inspector_o->imgui_name().size()+1];sprintf(d,"Inspector <%s>###inspector", inspector_o->imgui_name().c_str());
	ImGui::Begin(d, p_open);
	
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
	inspector_o->imgui_draw();
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
			inspector_o = std::static_pointer_cast<IDraw>(obj);
	}
	
	ImGui::End();
}

}  // namespace menus
