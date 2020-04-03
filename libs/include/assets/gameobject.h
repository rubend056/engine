#ifndef GameObject_h
#define GameObject_h

#include "components.h"
#include "file.h"

#include "cereal/archives/json.hpp"
#include <vector>
#include <map>
#include <glm/vec3.hpp>





#include "type_name.h"

template <class T>
std::shared_ptr<T> component_button() {
	static_assert(std::is_base_of<Component, T>::value, "T not derived from Component");
	if (ImGui::Button(helper::demangle(typeid(T).name()).c_str()))
		return std::shared_ptr<T>(new T);
	else
		return std::shared_ptr<T>();
}

class GameObject;

#define CLASSNAME GAMEOBJECT
#define CLASSNAME_NORMAL GameObject
#define GAMEOBJECT_EXPANSION(FUNC) \
	FUNC(std::vector<std::shared_ptr<Component>>, comps)

// ? PREFAB ######################################################################

#include "prefab.h"

// ? ###########################################################################

class CLASSNAME_NORMAL : public Prefab<CLASSNAME_NORMAL>, public IDraw{
    
    public:
		
		// std::shared_ptr<GameObject> ref;
		// bool ref=false;// Is this the original file reference
							// Else you only save dirty parts in components
        bool enabled=true;
		
		
		// std::vector<std::shared_ptr<Component>> comps;
        // std::map<std::string*, std::shared_ptr<Component>> _comps;
		
        // void add_component(std::shared_ptr<Component> c);
		
		// If instantiating as prefab_ref, then use param
        CLASSNAME_NORMAL(PREFAB_CONSTRUCT_PARAM):Prefab<CLASSNAME_NORMAL>(PREFAB_CONSTRUCT_VARS){
			comps.push_back(std::make_shared<Transform>());
		}
		
		
		
		template<class Archive>
		void serialize(Archive & ar) {
			ar(cereal::make_nvp("template", cereal::base_class<Prefab<CLASSNAME_NORMAL>>(this)));
			ar(CEREAL_NVP(enabled));
		}
		
		
		IDRAW_IMGUI_NAME override{return filename().c_str();}
		IDRAW_IMGUI_DRAW override{
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
		// template<class Archive>
		// void load(Archive & archive) {
		// 	ar(CEREAL_NVP(prefab));
		// 	comps.clear();
		// 	ar(CEREAL_NVP(comps))
		// }
};
_CRT(CLASSNAME_NORMAL)

#undef CLASSNAME
#undef CLASSNAME_NORMAL

// _CSFAA(CLASSNAME_NORMAL, cereal::specialization::member_serialize)

// CEREAL_CLASS_VERSION(GameObject, 0)


#endif