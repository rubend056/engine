#include <typeinfo>

#include "assets.h"
#include "engine_globals.h"
#include "groups/helper.h"
#include "groups/type_name.h"
#include "menus/menus.h"
#include "rendering/rendering.h"

using namespace std;



namespace menus {
#define MAX_WIN_NAME 100
std::shared_ptr<IDraw> inspector_o;
void inspector(bool* p_open) {
	//? INSPECTOR NAME AND COMPONENT LOOP
	if(!inspector_o)return;
	
	// char d[MAX_WIN_NAME];snprintf(d, MAX_WIN_NAME,"Inspector <%s>##inspector", inspector_o->imgui_name());
	ImGui::Begin("Inspector", p_open);
	
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
	inspector_o->imgui_draw();
	ImGui::PopStyleVar();
	
	ImGui::End();
}

void scene(bool* p_open){
	// char d[MAX_WIN_NAME];snprintf(d, MAX_WIN_NAME,"Scene <%s>##scene", "name");
	ImGui::Begin("Scene", p_open);
	
	static_assert(std::is_base_of<IDraw, GameObject>::value, "IDraw not derived from GameObject");
	
	if (ImGui::Button("Add GObject", ImVec2(-1,0)))
		engine::instantiate(new GameObject("Test GO"));
	
	ImGui::Separator();
	
	for(auto&obj:engine::objects){
		ImGui::Text(obj->filename().c_str());
		if(ImGui::IsItemClicked())
			inspector_o = std::static_pointer_cast<IDraw>(obj);
	}
	
	ImGui::End();
}

}  // namespace menus
