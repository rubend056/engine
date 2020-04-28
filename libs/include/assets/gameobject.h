#ifndef GameObject_h
#define GameObject_h

#include "component.h"
#include "file.h"

#include "cereal/archives/json.hpp"
#include <vector>
#include <map>
#include "transform.h"

#include "type_name.h"

// ? PREFAB ####################################################################
#define CLASSNAME GAMEOBJECT
#define CLASSNAME_NORMAL GameObject
#define GAMEOBJECT_EXPANSION(FUNC) \
	FUNC(std::vector<std::shared_ptr<Component>>, components)

class CLASSNAME_NORMAL;

#define PREFAB_VAR_SERIALIZE(type, name)\
	if(!strcmp(#name,"components")){\
		int csize,oldsize;csize=oldsize=components.size();\
		ar(csize);\
		for(int i=0;i<csize-oldsize;++i)components.push_back(std::shared_ptr<Component>());\
		for(int i=0;i<csize;++i){\
			auto is_ref = components[i]?components[i]->parent != (Referentiable*)this:true;\
			ar(is_ref);\
			if(is_ref){\
				std::vector<unsigned int> ref;\
				if(components[i]) ref = components[i]->my_ref();\
				ar(ref);\
				components[i] = assets::get_file<Component>(ref);\
			}else ar(components[i]);\
		}\
	}else ar(name);

#include "prefab.h"
// ? ###########################################################################

#include "map_helper.h"

#include "cereal/types/unordered_map.hpp"
class CLASSNAME_NORMAL : public PREFAB_NAME, public File, public IDraw, public Parent{
    
    public:
        bool enabled=true;
		
		// If instantiating as prefab_ref, then use param
        // CLASSNAME_NORMAL(PREFAB_CONSTRUCT_PARAM);
		CLASSNAME_NORMAL(FILE_CONSTRUCT_PARAM);
		
		IDRAW_IMGUI_NAME override{return filename().c_str();}
		IDRAW_IMGUI_DRAW override;
		
		std::unique_ptr<Transform> trans=std::make_unique<Transform>();
		
		std::unordered_multimap<std::string, std::shared_ptr<Component>> type_component_ht;
		
		// ? add ************
		void add(const std::shared_ptr<Component>& comp);
		void del(const std::shared_ptr<Component>& comp);
		
		MAP_GET_ELEMENT(Component, comp, type_component_ht)
		MAP_GET_ELEMENTS(Component, comps, type_component_ht)

		// ? ##########################
		
		template<class Archive>
		void serialize(Archive & ar) {
			ar(CEREAL_NVP(enabled));
			ar(CEREAL_NVP(trans));
			ar(cereal::virtual_base_class<Parent>(this));
			// ? Base Classes
			ar(FILE_SERIALIZE);
			
			// ? Serializing ref
			// std::string d_name = ref?std::static_pointer_cast<GameObject>(ref)->data_path():"";
			// ar(cereal::make_nvp("ref",d_name));
			// // If ref null and d_name not empty, means we loading, then fill ref
			// if(!ref && !d_name.empty())ref = assets::get_file<GameObject>(d_name);
			
			// // Print error if we have a ref_name serialized 
			// // but can't find the actual reference
			// if(!ref && !d_name.empty()) 
			// 	printf(ANSI_COLOR_RED "%s ref not found in %s" ANSI_COLOR_RESET "\n", d_name.c_str(), filename().c_str()); 
			
			ar(PREFAB_SERIALIZE);
			
			if(!type_component_ht.size())
			for(auto&c:components)
				if(c)type_component_ht.insert(std::make_pair(helper::demangle(typeid(*c).name()), c));
			
			// Setting refs for loading
			// if(ref){
			// 	auto other_go = std::dynamic_pointer_cast<GameObject>(ref);
			// 	if(other_go)
			// 	for(auto&tc:type_component_ht){
			// 		auto other_comp = other_go->get_comp(tc.first);
			// 		if(other_comp)tc.second->set_ref(other_comp);
			// 	}
			// 	else printf(ANSI_COLOR_RED "OtherGO not set error" ANSI_COLOR_RESET "\n");
			// }
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