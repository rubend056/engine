#ifndef transform_h
#define transform_h

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include "component.h"

#include "helper/cereal_glm.h"

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
	glm::mat4 get_matrix_trans();
	glm::quat get_rot_quat();
	glm::mat4 get_rot_mat();
	glm::mat4 get_pos_mat();
	glm::mat4 get_sca_mat();
	Transform(){sca = glm::vec3(1);}
	IDRAW_IMGUI_DRAW override;
	IDRAW_IMGUI_NAME override {return "Transform";if(pos == rot);}
	COMPONENT_MAX_NUM override {return 1;}
	
	COMPONENT_IS_REF override {return false;}
	std::shared_ptr<Component> clone() override{return std::make_shared<CLASSNAME_NORMAL>(*this);}
	
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