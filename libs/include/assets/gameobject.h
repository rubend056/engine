#ifndef GameObject_h
#define GameObject_h

#include "component.h"
#include "file.h"

#include "cereal/archives/json.hpp"
#include <vector>
#include <map>
// #include <glm/vec3.hpp>

#include "type_name.h"

template <class T>
std::shared_ptr<T> component_button() {
	static_assert(std::is_base_of<Component, T>::value, "T not derived from Component");
	if (ImGui::Button(helper::demangle(typeid(T).name()).c_str()))
		return std::shared_ptr<T>(new T);
	else
		return std::shared_ptr<T>();
}



// ? PREFAB ####################################################################
#define CLASSNAME GAMEOBJECT
#define CLASSNAME_NORMAL GameObject
#define GAMEOBJECT_EXPANSION(FUNC) \
	FUNC(std::vector<std::shared_ptr<Component>>, components)

class CLASSNAME_NORMAL;

#include "prefab.h"
// ? ###########################################################################

#include "map_helper.h"

class CLASSNAME_NORMAL : public PREFAB_NAME, public File, public IDraw{
    
    public:
        bool enabled=true;
		
		// If instantiating as prefab_ref, then use param
        // CLASSNAME_NORMAL(PREFAB_CONSTRUCT_PARAM);
		CLASSNAME_NORMAL(FILE_CONSTRUCT_PARAM);
		
		IDRAW_IMGUI_NAME override{return filename().c_str();}
		IDRAW_IMGUI_DRAW override;
		
		std::unordered_multimap<std::string, std::shared_ptr<Component>> type_component_ht;
		
		// ? add ************
		void add(const std::shared_ptr<Component>& comp, const char* type_info_name){
			auto t_name = helper::demangle(type_info_name);
			if(get_comps(t_name).size()>=comp->max_num()){
				printf(ANSI_COLOR_YELLOW "Max num %d for %s" ANSI_COLOR_RESET "\n", comp->max_num(), t_name.c_str());
				return;
			}
			components.push_back(comp);
			type_component_ht.insert(std::make_pair(t_name, comp));
		}
		
		MAP_GET_ELEMENT(Component, comp, type_component_ht)
		
		MAP_GET_ELEMENTS(Component, comps, type_component_ht)
		// // ? get_comp
		// std::shared_ptr<Component> get_comp(const std::string& t_name){
		// 	auto it = type_component_ht.find(t_name);
		// 	if(it == type_component_ht.end())return std::shared_ptr<Component>();
		// 	else return it->second;
		// }
		// template<class T>
		// std::shared_ptr<T> get_comp(){
		// 	return std::dynamic_pointer_cast<T>(get_comp(helper::demangle(typeid(T).name())));
		// }
		// // ? get_comps
		// std::vector<std::shared_ptr<Component>> get_comps(const std::string t_name){
		// 	// #warning "t_name calls for erroneous/nonexistent types"
		// 	auto eq_range = type_component_ht.equal_range(t_name);
		// 	std::vector<std::shared_ptr<Component>> v;
		// 	for(auto it = eq_range.first; it!=eq_range.second;++it)
		// 		v.push_back(it->second);
		// 	return v;
		// }
		// template<class T>
		// std::vector<std::shared_ptr<T>> get_comps(){
		// 	std::string t_name = helper::demangle(typeid(T).name());
		// 	auto o = get_comps(t_name);
		// 	std::vector<std::shared_ptr<T>> v;v.reserve(o.size());
		// 	for(auto&a:o)
		// 		v.push_back(std::dynamic_pointer_cast<T>(a));
		// 	return std::move(v);
		// }
		// ? ##########################
		
		template<class Archive>
		void serialize(Archive & ar) {
			ar(CEREAL_NVP(enabled));
			// ? Base Classes
			ar(FILE_SERIALIZE);
			
			// ? Serializing ref
			std::string d_name = ref?std::static_pointer_cast<GameObject>(ref)->data_path():"";
			ar(cereal::make_nvp("ref",d_name));
			// If ref null and d_name not empty, means we loading, then fill ref
			if(!ref && !d_name.empty())ref = assets::get_file<GameObject>(d_name);
			
			// Print error if we have a ref_name serialized 
			// but can't find the actual reference
			if(!ref && !d_name.empty()) 
				printf(ANSI_COLOR_RED "%s ref not found in %s" ANSI_COLOR_RESET "\n", d_name.c_str(), filename().c_str()); 
			
			ar(PREFAB_SERIALIZE);
			
			// Setting refs for loading
			if(ref){
				auto other_go = std::dynamic_pointer_cast<GameObject>(ref);
				if(other_go)
				for(auto&tc:type_component_ht){
					auto other_comp = other_go->get_comp(tc.first);
					if(other_comp)tc.second->set_ref(other_comp);
				}
				else printf(ANSI_COLOR_RED "OtherGO not set error" ANSI_COLOR_RESET "\n");
			}
		}
};

// ? PREFAB ####################################################################
_CRT(CLASSNAME_NORMAL)
// CEREAL_REGISTER_POLYMORPHIC_RELATION(Prefab<GameObject>, GameObject)
// CEREAL_CLASS_VERSION(GameObject, 0)

#undef CLASSNAME
#undef CLASSNAME_NORMAL
// #undef GAMEOBJECT_EXPANSION
// ? ###########################################################################

#endif