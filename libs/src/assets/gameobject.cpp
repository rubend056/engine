#include "gameobject.h"

#include "components.h"
#include "mesh.h"

GameObject::GameObject(const fs::path& rpath):File(FILE_CONSTRUCT_VARS){}

void GameObject::add(const std::shared_ptr<Component>& comp){
	if(!comp)return;
	auto t_name = helper::demangle(typeid(*comp).name());
	if(comp->max_num() >0 && get_comps(t_name).size()>=comp->max_num()){
		printf(ANSI_COLOR_YELLOW "Max num %d for %s" ANSI_COLOR_RESET "\n", comp->max_num(), t_name.c_str());
		return;
	}
	components.push_back(comp);
	type_component_ht.insert(std::make_pair(t_name, comp));
}
void GameObject::del(const std::shared_ptr<Component>& comp){
	if(!comp)return;
	components.erase( std::find(components.begin(), components.end(), comp));
	type_component_ht.erase(std::find_if(type_component_ht.begin(), type_component_ht.end(), [&](const auto& j){
		return j.second == comp;
	}));
}

#include "menus.h"

void GameObject::imgui_draw(){
	//? COMPONENT DRAW LOOP
	File::imgui_draw_filename_edit();
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);	
	{
		if(ImGui::TreeNode("Transform")){
			trans->imgui_draw();
			ImGui::TreePop();
		}
		int i=0;
		for (auto&c:components) {++i;
			if(!c)break;
			char name[20];
			sprintf(name, "%s##%d", c->imgui_name().c_str(), i);
			if (ImGui::TreeNode(name)) {
				if(ImGui::SmallButton("Delete")){del(c);ImGui::TreePop();break;}
				c->imgui_draw();
				ImGui::TreePop();
			}
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
		// if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_FILE"))
		// {
		// 	IM_ASSERT(payload->DataSize == sizeof(int));
		// 	auto f_id = *(int*)payload->Data;
		// 	comp = assets::get_file<Component>([&](File*t){return t->get_id() == f_id;});
		// }
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_REF"))
		{
			unsigned int data[11];
			assert(payload->DataSize == sizeof(data));
			memcpy(data, payload->Data, sizeof(data));
			std::vector<unsigned int> refs;for(int i=0;i<data[0];++i)refs.push_back(data[i+1]);
			comp = assets::get_file<Component>(refs);
		}
		
		if(comp){add(comp->is_ref()?comp:comp->clone());}
		ImGui::EndDragDropTarget();
	}
	//? ############ Component DRAG N DROP

	if (ImGui::BeginPopup("add_component_popup")) {
		// Filter code
		// static ImGuiTextFilter filter;
		// filter.Draw("##filter0", 100);
		// std::shared_ptr<Component> c; const char* type_id_name;
		// auto component_button = [&]() -> void {add(c); ImGui::CloseCurrentPopup();};
		// if(ImGui::Button("Transform"))add(std::make_shared<Transform>());
		// if (c = component_button<Program>()){type_id_name = typeid(Program).name(); push();}

		ImGui::EndPopup();
	}
}