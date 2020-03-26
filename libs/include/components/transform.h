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
	
	COMPONENT_IMGUI_DRAW override;
	COMPONENT_IMGUI_NAME override {return "Transform";}
	COMPONENT_NAME {return "Transform";}
};

#endif // transform_h