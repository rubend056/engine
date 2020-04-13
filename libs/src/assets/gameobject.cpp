#include "gameobject.h"

#include "components.h"

GameObject::GameObject(const fs::path& rpath):File(FILE_CONSTRUCT_VARS){
	// components.push_back();
	add(std::make_shared<Transform>(), typeid(Transform).name());
}

void GameObject::imgui_draw(){
	//? COMPONENT DRAW LOOP
	File::imgui_draw_filename_edit();
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);
	int i = 0;
	for (auto& c : components) {
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
		std::shared_ptr<Component> c; const char* type_id_name;
		auto push = [&]() -> void {add(c, type_id_name); ImGui::CloseCurrentPopup(); };
		if (c = component_button<Transform>()){type_id_name = typeid(Transform).name(); push();}
		if (c = component_button<Program>())push();

		ImGui::EndPopup();
	}
}