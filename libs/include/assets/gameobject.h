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
        bool enabled=true;
		
		// If instantiating as prefab_ref, then use param
        CLASSNAME_NORMAL(PREFAB_CONSTRUCT_PARAM);
		
		IDRAW_IMGUI_NAME override{return filename().c_str();}
		IDRAW_IMGUI_DRAW override;
		
		template<class Archive>
		void serialize(Archive & ar) {
			ar(cereal::make_nvp("template", cereal::base_class<Prefab<CLASSNAME_NORMAL>>(this)));
			ar(CEREAL_NVP(enabled));
		}
};
_CRT(CLASSNAME_NORMAL)


// _CSFAA(CLASSNAME_NORMAL, cereal::specialization::member_serialize)

// CEREAL_CLASS_VERSION(GameObject, 0)


#endif