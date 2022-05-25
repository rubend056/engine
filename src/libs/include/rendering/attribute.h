/**
 * @file attribute.h
 * @author RubenD (rubendariopm14@gmail.com)
 * @brief Defines an attribute in a program
 * @version 0.1
 * 
 * This class was made to generalize variables in an OpenGL 'program'
 * Deals with serializing/deserializing variables
 */
#ifndef attribute_h
#define attribute_h

#include "cereal/types/polymorphic.hpp"
#include "file.h"
#include "gl.h"
#include "idraw.h"

#include <glm/glm.hpp>

// GL_ATTRIBUTE_TYPE,  AtributeVar type, glUniform value type, glUniformFunction
#define TYPE_EXPANSION(func)\
	func(GL_FLOAT				,float				,float			,glUniform1fv)\
	func(GL_DOUBLE			,double				,double			,glUniform1dv)\
	func(GL_FLOAT_VEC2	,glm::vec2		,float			,glUniform2fv)\
	func(GL_FLOAT_VEC3	,glm::vec3		,float			,glUniform3fv)\
	func(GL_FLOAT_VEC4	,glm::vec4		,float			,glUniform4fv)\
	func(GL_FLOAT_MAT2	,glm::mat2		,float			,glUniformMatrix2fv)\
	func(GL_FLOAT_MAT3	,glm::mat3		,float			,glUniformMatrix2fv)\
	func(GL_FLOAT_MAT4	,glm::mat4		,float			,glUniformMatrix2fv)\
	func(GL_SAMPLER_2D	,unsigned int	,			,)

struct Attribute {
	// static std::unique_ptr<Attribute> create_attrib_val(GLenum _type, void*_last_val=nullptr);

   public:
	// Is this a uniform in the program
	GLboolean uniform = false;
	// THe index
	GLint i;
	// The location in the program
	GLint location;

	// Max name length in character length
	static const GLsizei max_name = 20;
	// Name variable
	GLchar name[max_name];
	// Name Length
	GLsizei name_length;

	// The value pointer
	void* val = nullptr;
	// The size of the attribute, in units of the type
	GLint size;
	// The type of the attribute, from OPENGL macros
	GLenum type = 0;

	virtual ~Attribute() = default;

	template <class Archive>
	void serialize(Archive& ar) {
		std::string s_name(name);
		ar(CEREAL_NVP(s_name), CEREAL_NVP(uniform), CEREAL_NVP(size), CEREAL_NVP(type));  // The only thing needed for matching
		strcpy(name, s_name.c_str());
	}
};

/**
 * @brief An attribute class variable for serialization
 * @tparam T the type of variable
 */
template <typename T>
struct AttributeVar : public Attribute {
	std::unique_ptr<T> _val = std::unique_ptr<T>(new T(0));
	AttributeVar(void* _last_val = nullptr) {
		if (_last_val)
			memcpy(_val.get(), _last_val, sizeof(T));
		// type_bytes = sizeof(T);
		val = _val.get();
	}
	template <class Archive>
	void serialize(Archive& ar) {
		ar(cereal::make_nvp("attrib", cereal::base_class<Attribute>(this)));
		ar(cereal::make_nvp("val", _val));
		val = _val.get();
	}
};
#define CRT(gl_type, type, gl_type_name, uniformfunc) \
	CEREAL_REGISTER_TYPE(AttributeVar<type>)
// #define CRT(gl_type, type, uniformfunc) CRT_(gl_type, type, uniformfunc)
TYPE_EXPANSION(CRT)

#endif	// attribute_h