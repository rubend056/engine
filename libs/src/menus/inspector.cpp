#include "menus/menus.h"

// #include <type
// void add_component_popup() {

// }
#include "rendering/gl_helper.h"
using namespace std;

template<class T>
shared_ptr<T> component_button(){
	static_assert(std::is_base_of<Component, T>::value, "T not derived from Component");
	if(ImGui::Button(T::name()))return shared_ptr<T>(new T);
	else return nullptr;
}

namespace menus {
// bool _inspector_open=true;
// bool inspector_open(){return _inspector_open;}
void inspector(shared_ptr<GameObject> o, bool* p_open) {
    //? INSPECTOR NAME AND COMPONENT LOOP
    ImGui::Begin("Inspector", p_open);
	// ImGui::SetNextItemWidth(0);
    ImGui::InputText("Name", o->filename, sizeof(o->filename));
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
    int i = 0;
    for (auto& c : o->comps) {
        char name[20];
        sprintf(name, "%s##%d",c->imgui_name(), i++);
        if (ImGui::TreeNode(name)) {
            c->imgui_draw();
            ImGui::TreePop();
        }
    }
    ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	
    //? ADD COMPONENT POPUP
    if (ImGui::Button("Add Component"))
        ImGui::OpenPopup("add_component_popup");

    if(ImGui::BeginPopup("add_component_popup")){
		// Filter code
		static ImGuiTextFilter filter;
		filter.Draw("Filter0", -1);
		shared_ptr<Component> c;
		if(c=component_button<Transform>()){o->comps.push_back(c); ImGui::CloseCurrentPopup();};
		if(c=component_button<Program>()){o->comps.push_back(c); ImGui::CloseCurrentPopup();};
		
		ImGui::EndPopup();
	}

    ImGui::End();
	
	
}
}  // namespace menus
