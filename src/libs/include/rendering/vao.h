/**
 * @file vao.h
 * @author RubenD (rubendariopm14@gmail.com)
 * @brief Defines vao
 * @version 0.1
 * 
 * VAO holds all data of mesh in the GPU
 */
#ifndef vao_h
#define vao_h


#include "gl.h"
#include "idraw.h"
#include "component.h"
#include "cereal/types/polymorphic.hpp"

#define _BV32(x) ((uint32_t)1 << x)

struct VAO : public Component {
	// VARIABLES
	std::string name;
	unsigned int vao_id;
	unsigned int n_vertices;
	unsigned int draw_function = GL_TRIANGLES;
	bool positions = false, normals = false, tex_cords = false;
	
	// CONSTRUCTORS
	VAO();
	virtual ~VAO();
	
	// FUNCTIONS
	void vao_bind();
	void vao_attrib_enable(uint32_t attribs);
	void vao_attrib_disable();
	void gl_draw();
	
	// COMPONENT
	COMPONENT_MAX_NUM override { return 0; }
	
	// DRAW
	IDRAW_IMGUI_NAME override { return std::string("VAO ") + name; }
	IDRAW_IMGUI_DRAW override;
	
	// SERIALIZATION
	template <class Archive>
	void serialize(Archive& ar) {
		ar(COMPONENT_SERIALIZE);
		ar(cereal::make_nvp("name", name));
	}
};
CEREAL_REGISTER_TYPE_WITH_NAME(VAO, "VAO")

#endif // vao_h