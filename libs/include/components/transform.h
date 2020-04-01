#ifndef transform_h
#define transform_h

#include <glm/vec3.hpp>
#include "components/component.h"

class Transform: public Component{
public:
	glm::vec3 
	pos=glm::vec3(0),
	rot=glm::vec3(0),
	sca=glm::vec3(0);
	
	IDRAW_IMGUI_DRAW override;
	IDRAW_IMGUI_NAME override {return "Transform";}
	// COMPONENT_NAME {return "Transform";}
	
	template<class Archive>
	void serialize(Archive& ar){
		ar(pos.x, pos.y, pos.z, 
		   rot.x, rot.y, rot.z,
		   sca.x, sca.y, sca.z);
	}
};

#endif // transform_h