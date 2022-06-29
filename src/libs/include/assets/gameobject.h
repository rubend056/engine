/**
 * @file gameobject.h
 * @author RubenD (rubendariopm14@gmail.com)
 * @brief Defines a GameObject in the engine
 * @version 0.1
 * 
 * By far the most complex class in the engine, preceded by Program
 * 
 * - Controls serialization of GameObject as a prefab/instance
 * - 
 */
#ifndef GameObject_h
#define GameObject_h

// My libs
#include <map>
#include <vector>

#include "cereal/cereal.hpp"
#include "component.h"
#include "file.h"
#include "transform.h"
#include "type_name.h"

// ? PREFAB ####################################################################
#define CLASSNAME GAMEOBJECT
#define CLASSNAME_NORMAL GameObject
#define GAMEOBJECT_EXPANSION(FUNC)

class CLASSNAME_NORMAL;

#include "prefab.h"
// ? ###########################################################################

#include "cereal/types/unordered_map.hpp"
#include "map_helper.h"
class CLASSNAME_NORMAL : public PREFAB_NAME, public File, public IDraw, public Parent {
   public:
	bool enabled = true;

	// If instantiating as prefab_ref, then use param
	// CLASSNAME_NORMAL(PREFAB_CONSTRUCT_PARAM);
	CLASSNAME_NORMAL(FILE_CONSTRUCT_PARAM);
	std::vector<std::shared_ptr<Component>> components;

	std::shared_ptr<Referentiable> foster(const std::shared_ptr<Referentiable>& child) override;

	IDRAW_IMGUI_NAME override { return filename().c_str(); }
	ITYPE override { return STRINGIFY(CLASSNAME_NORMAL); };
	IDRAW_IMGUI_DRAW override;

	std::unique_ptr<Transform> trans = std::make_unique<Transform>();

	std::unordered_multimap<std::string, std::shared_ptr<Component>> type_component_ht;

	// ? add ************
	void add(const std::shared_ptr<Component>& comp);
	void del(const std::shared_ptr<Component>& comp);

	MAP_GET_ELEMENT(Component, comp, type_component_ht)
	MAP_GET_ELEMENTS(Component, comps, type_component_ht)

	// ? ##########################

	template <class Archive>
	void serialize(Archive& ar) {
		ar(CEREAL_NVP(enabled));
		ar(CEREAL_NVP(trans));
		ar(PARENT_SERIALIZE, FILE_SERIALIZE, PREFAB_SERIALIZE);

		/*Serailizing the size of components*/
		int csize, oldsize;
		csize = oldsize = components.size();
		ar(csize);
		/*Equaling the size of components to the size of serialized*/
		for (int i = 0; i < csize - oldsize; ++i) components.push_back(std::shared_ptr<Component>());
		for (int i = 0; i < csize; ++i) {
			/*Set is_ref to -> if component's parent doesn't equal 'this'*/
			auto is_ref = components[i] ? components[i]->parent != (Referentiable*)this : true;
			ar(is_ref);
			if (is_ref) {
				std::vector<unsigned int> ref;
				if (components[i])
					ref = components[i]->my_ref();
				ar(ref);
				components[i] = assets::get_file<Component>(ref);
			} else {
				ar(components[i]);
				foster(components[i]);
			}
		}

		// Populate type_component_ht
		if (!type_component_ht.size())
			for (auto& c : components)
				if (c)
					type_component_ht.insert(std::make_pair(helper::demangle(typeid(*c).name()), c));
	}
};

// ? PREFAB ####################################################################
_CRT(CLASSNAME_NORMAL)

#undef CLASSNAME
#undef CLASSNAME_NORMAL
// ? ###########################################################################

#endif