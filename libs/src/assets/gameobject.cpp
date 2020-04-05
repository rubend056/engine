#include "gameobject.h"

#include "components.h"

CLASSNAME_NORMAL::CLASSNAME_NORMAL(const fs::path rpath):Prefab<CLASSNAME_NORMAL>(PREFAB_CONSTRUCT_VARS){
	comps.push_back(std::make_shared<Transform>());
}

void CLASSNAME_NORMAL::imgui_draw(){
	//? COMPONENT DRAW LOOP
	File::imgui_draw_filename_edit();
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);
	int i = 0;
	for (auto& c : comps) {
		char name[20];
		sprintf(name, "%s##%d", c->imgui_name().c_str(), i++);
		if (ImGui::TreeNode(name)) {
			c->imgui_draw();
			ImGui::TreePop();
		}
	}
	ImGui::PopStyleVar();

	//? ADD COMPONENT POPUP
	if (ImGui::Button("Add Component", ImVec2(-1,0)))
		ImGui::OpenPopup("add_component_popup");

	if (ImGui::BeginPopup("add_component_popup")) {
		// Filter code
		static ImGuiTextFilter filter;
		filter.Draw("##filter0", 100);
		std::shared_ptr<Component> c;
		auto push = [&]() -> void {comps.push_back(c); ImGui::CloseCurrentPopup(); };
		if (c = component_button<Transform>())
			push();
		if (c = component_button<Program>())
			push();

		ImGui::EndPopup();
	}
}