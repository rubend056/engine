#include "gameobject.h"

#include "imgui_helper.h"
#include "components.h"
#include "mesh.h"

GameObject::GameObject(const fs::path& rpath):File(FILE_CONSTRUCT_VARS){}

std::shared_ptr<Referentiable> GameObject::foster(const std::shared_ptr<Referentiable>& child){
	// If child is a gameobject, fosted the child
	if(auto go = std::dynamic_pointer_cast<GameObject>(child))Parent::foster(child);
	else if(auto comp = std::dynamic_pointer_cast<Component>(child)){
		comp->parent = this;
	}else throw(std::string("Not go or comp"));
	return child;
}

void GameObject::add(const std::shared_ptr<Component>& _comp){
	if(!_comp)return;
	auto comp = _comp;
	if(!comp->is_ref()){
		// If component has a parent, clone it
		if(comp->parent)comp = comp->clone();
		// Foster component/ assume ownership
		foster(comp);
	}
	
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
			if(!c)continue;
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
	
	
	
	static std::vector<uint> refs;
	imgui_dnd_ref(refs, false);
	if(refs.size()){
		if(auto comp = assets::get_file<Component>(refs)){
			// if(!comp->is_ref())comp = std::dynamic_pointer_cast<Component>(foster(comp->clone()));
			add(comp);
		}
	}

	if (auto comp = menus::select_asset<Component>("add_component_popup", FactoryType_Camera, MenuSelectFlags_NewOnly)) {
		add(comp);
	}
}