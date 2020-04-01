#ifndef GameObject_h
#define GameObject_h

#include "components/components.h"
#include "serialize.h"

#include "cereal/archives/json.hpp"
#include <vector>
#include <map>
#include <glm/vec3.hpp>

// template<typename T>
// struct PrefabVar{
// private:
// 	T val;
// 	bool dirty =false;
// };

#define PREFAB_VAR_DIRTY_NAME(name) name##_dirty
#define PREFAB_VAR(type, name) type name;bool PREFAB_VAR_DIRTY_NAME(name) = false;

#define PREFAB_VAR_SERIALIZE(type, name) ar(CEREAL_NVP(name));
#define PREFAB_VAR_SERIALIZE_VARS(type, name) if(dirty[DIRTY_ENUM::name##_enum]) PREFAB_VAR_SERIALIZE(type, name)
#define PREFAB_VAR_SERIALIZE_DIRTY(type, name) _PREFAB_VAR_SERIALIZE_DIRTY(type, PREFAB_VAR_DIRTY_NAME(name))

#define PREFAB_VAR_SET(name, val) name = val; PREFAB_VAR_DIRTY_NAME(name) = ref->name == name;
// #define PREFAB_VAR_CHECK(name) PREFAB_VAR_DIRTY_NAME(name) = ref.name == name;
#define PREFAB_VAR_CHECK(type, name) PREFAB_VAR_DIRTY_NAME(name) = ref->name == name;

#define PREFAB_VAR_ENUM_DIRTY(type, name) name##_enum,

#define PREFAB_VAR_ADD_DIRTY(type, name) dirty.push_back(PREFAB_VAR_DIRTY_NAME(name));

#define GAMEOBJECT_EXPANSION(FUNC) FUNC(std::vector<std::shared_ptr<Component>>, comps)

// class File;
#include "groups/type_name.h"

template <class T>
std::shared_ptr<T> component_button() {
	static_assert(std::is_base_of<Component, T>::value, "T not derived from Component");
	if (ImGui::Button(helper::demangle(typeid(T).name()).c_str()))
		return std::shared_ptr<T>(new T);
	else
		return std::shared_ptr<T>();
}

extern bool ser_ref;

class GameObject : public File, public IDraw{
    
    public:
		enum DIRTY_ENUM{GAMEOBJECT_EXPANSION(PREFAB_VAR_ENUM_DIRTY) LAST};
		std::shared_ptr<GameObject> ref;
		// bool ref=false;// Is this the original file reference
							// Else you only save dirty parts in components
        bool enabled=true;
		
		
		GAMEOBJECT_EXPANSION(PREFAB_VAR)
		// std::vector<std::shared_ptr<Component>> comps;
        // std::map<std::string*, std::shared_ptr<Component>> _comps;
		
        // void add_component(std::shared_ptr<Component> c);
		
        GameObject(const fs::path& path=""):File(path){
			comps.push_back(std::make_shared<Transform>());
		}
		
		
		
		template<class Archive>
		void serialize(Archive & ar) {
			ar(CEREAL_NVP(ref));
			// ar(CEREAL_NVP(prefab));
			ar(CEREAL_NVP(enabled));
			ser_ref=!(bool)ref; // Serialize everything if no ref pointer
			
			std::vector<bool> dirty;GAMEOBJECT_EXPANSION(PREFAB_VAR_ADD_DIRTY)
			ar(CEREAL_NVP(dirty));
			GAMEOBJECT_EXPANSION(PREFAB_VAR_SERIALIZE_VARS)
		}
		
		
		IDRAW_IMGUI_NAME override{return filename().c_str();}
		IDRAW_IMGUI_DRAW override{
			//? COMPONENT DRAW LOOP
			File::imgui_draw_filename_edit();
			ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);
			int i = 0;
			for (auto& c : comps) {
				char name[20];
				sprintf(name, "%s##%d", c->imgui_name(), i++);
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



// CEREAL_CLASS_VERSION(GameObject, 0)
// CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES( GameObject, cereal::specialization::member_load_save )

#endif