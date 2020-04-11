#ifndef transform_h
#define transform_h

#include <glm/vec3.hpp>
#include "component.h"

#include "helper/cereal_glmvec3.h"

// ? PREFAB ####################################################################
#define CLASSNAME TRANSFORM
#define CLASSNAME_NORMAL Transform
#define TRANSFORM_EXPANSION(FUNC) \
	FUNC(glm::vec3, pos)\
	FUNC(glm::vec3, rot)\
	FUNC(glm::vec3, sca)

class CLASSNAME_NORMAL;

#include "prefab.h"
// ? ###########################################################################

class CLASSNAME_NORMAL: public Component, public PREFAB_NAME{
public:
	
	IDRAW_IMGUI_DRAW override;
	IDRAW_IMGUI_NAME override {return "Transform";if(pos == rot);}
	COMPONENT_MAX_NUM override {return 1;}
	COMPONENT_IS_REF override {return false;}
	
	template<class Archive>
	void serialize(Archive& ar){
		ar(COMPONENT_SERIALIZE, PREFAB_SERIALIZE);
	}
};

// ? PREFAB ####################################################################
_CRT(CLASSNAME_NORMAL)
// CEREAL_REGISTER_POLYMORPHIC_RELATION(Prefab<GameObject>, GameObject)
// CEREAL_CLASS_VERSION(GameObject, 0)

#undef CLASSNAME
#undef CLASSNAME_NORMAL
// #undef TRANSFORM_EXPANSION
// ? ###########################################################################

#endif // transform_h