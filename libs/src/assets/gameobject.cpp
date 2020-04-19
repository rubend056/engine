#include "gameobject.h"

#include "components.h"
#include "mesh.h"

GameObject::GameObject(const fs::path& rpath):File(FILE_CONSTRUCT_VARS){
	// components.push_back();
	add(std::make_shared<Transform>(), typeid(Transform).name());
}

void GameObject::imgui_draw(){
	//? COMPONENT DRAW LOOP
	File::imgui_draw_filename_edit();
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);	
	for (auto it=components.begin();it<components.end();++it) {
		int i = it-components.begin();
		char name[20];
		sprintf(name, "%s##%d", (*it)->imgui_name().c_str(), i);
		if (ImGui::TreeNode(name)) {
			if(i && ImGui::SmallButton("Delete")){components.erase(it);ImGui::TreePop();break;}
			(*it)->imgui_draw();
			ImGui::TreePop();
		}
	}
	ImGui::PopStyleVar();

	//? ADD COMPONENT POPUP
	if (ImGui::Button("Add Component", ImVec2(-1,0)))
		ImGui::OpenPopup("add_component_popup");
	//? Component DRAG N DROP
	if (ImGui::BeginDragDropTarget())
	{
		std::shared_ptr<Component> comp;
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_FILE"))
		{
			IM_ASSERT(payload->DataSize == sizeof(int));
			auto f_id = *(int*)payload->Data;
			auto file = assets::get_file<File>([&](File*t){return t->file_id == f_id;});
			comp = std::dynamic_pointer_cast<Component>(file);
		}
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_VAO"))
		{
			struct FILEId_VAOIndex{int f_id; int vao_i;};
			IM_ASSERT(payload->DataSize == sizeof(FILEId_VAOIndex));
			auto f = *(FILEId_VAOIndex*)payload->Data;
			auto mesh = assets::get_file<Mesh>([&](Mesh*t){return t->file_id == f.f_id;});
			if(mesh){
				auto vao = mesh->vaos[f.vao_i];
				if(vao)components.push_back(vao);
			}
		}
		
		if(comp){
			// If component is not to be referenced, we create a copy
			if(!comp->is_ref()){components.push_back(std::shared_ptr<Component>(comp->clone()));}
			// Else we just use it
			else components.push_back(comp);
		}
		ImGui::EndDragDropTarget();
	}
	//? ############ Component DRAG N DROP

	if (ImGui::BeginPopup("add_component_popup")) {
		// Filter code
		// static ImGuiTextFilter filter;
		// filter.Draw("##filter0", 100);
		// std::shared_ptr<Component> c; const char* type_id_name;
		// auto push = [&]() -> void {add(c, type_id_name); ImGui::CloseCurrentPopup(); };
		// if (c = component_button<Transform>()){type_id_name = typeid(Transform).name(); push();}
		// if (c = component_button<Program>()){type_id_name = typeid(Program).name(); push();}

		ImGui::EndPopup();
	}
}