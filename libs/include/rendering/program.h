#ifndef program_h
#define program_h

// Ours
#include "gl.h"
#include "file.h"
#include "idraw.h"

#include "cereal/cereal.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/unordered_set.hpp"
#include "cereal_glm.h"

// #include "helper.h"

#define FRAGMENT_EXT ".frag"
#define VERTEX_EXT ".vert"
#define PROGRAM_EXT ".prgm"


class Shader : public File, public IDraw {
public:
    unsigned int s_id=0;
	// GLenum type_force=0;
    GLenum type=0;
    int status=0;
    char log[200];
	
	// bool loaded = false;
	static bool supported(const std::string& ext);
	/**
	 * Will load code and recompile
	 */
	void load() override;
    
	// Careful if shader isn't loaded correctly, shader will not be created
    Shader(FILE_CONSTRUCT_PARAM) : File(FILE_CONSTRUCT_VARS){
		if(!rpath.empty()){
			load();
			if(!status)std::printf("Shader %s loading failed\n", data_path().c_str());
		}
	}
	~Shader(){glDeleteShader(s_id);}
	
	IDRAW_IMGUI_NAME override {return filename();}
	IDRAW_IMGUI_DRAW override ;
	
	// FILE_SUPPOSED_EXT override {return ".meta";}
	
	template<class Archive>
	void serialize(Archive& ar){
		ar(FILE_SERIALIZE);
		// ar(CEREAL_NVP(type_force));
		// Only load if compile failed
		load();
	}
};
CEREAL_REGISTER_TYPE(Shader)




struct Attribute{
// static std::unique_ptr<Attribute> create_attrib_val(GLenum _type, void*_last_val=nullptr);

public:
	GLboolean uniform=false;
	GLint i;
	GLint location;
	
	static const GLsizei max_name=20;
	GLchar name[max_name];
	GLsizei name_length;
	
	void* val=nullptr;
	// The size of the attribute, in units of the type
	GLint size;
	// The type of the attribute, from OPENGL macros
	GLenum type=0;
	// The the number of bytes the type occupies
	// unsigned int type_bytes=0;
	
	virtual ~Attribute()=default;
	
	template<class Archive>
	void serialize(Archive& ar){
		// assert(val);
		std::string s_name(name);
		ar(CEREAL_NVP(s_name), CEREAL_NVP(uniform), CEREAL_NVP(size), CEREAL_NVP(type)); // The only thing needed for matching
		strcpy(name, s_name.c_str());
		
		// if(val)
		// switch (type)
		// {
		// case GL_FLOAT: 	ar(*(float*)val); break;
		// case GL_FLOAT_VEC2: ar(*(glm::vec2*)val); break;
		// case GL_FLOAT_VEC3: ar(*(glm::vec3*)val); break;
		// case GL_FLOAT_VEC4: ar(*(glm::vec4*)val); break;
		// case GL_FLOAT_MAT2: ar(*(glm::mat2*)val); break;
		// case GL_FLOAT_MAT3: ar(*(glm::mat3*)val); break;
		// case GL_FLOAT_MAT4: ar(*(glm::mat4*)val); break;
		// case GL_SAMPLER_2D: ar(*(GLuint*)val); break;
		// default:
		// 	throw std::range_error("Type not defined");
		// 	break;
		// }
	}
};

#define TYPE_EXPANSION(func)\
	func(GL_FLOAT		,float			,glUniform1fv)\
	func(GL_DOUBLE		,double			,glUniform1dv)\
	func(GL_FLOAT_VEC2	,glm::vec2		,glUniform2fv)\
	func(GL_FLOAT_VEC3	,glm::vec3		,glUniform3fv)\
	func(GL_FLOAT_VEC4	,glm::vec4		,glUniform4fv)\
	func(GL_FLOAT_MAT2	,glm::mat2		,glUniformMatrix2fv)\
	func(GL_FLOAT_MAT3	,glm::mat3		,glUniformMatrix2fv)\
	func(GL_FLOAT_MAT4	,glm::mat4		,glUniformMatrix2fv)\
	func(GL_SAMPLER_2D	,unsigned int	,)

template <typename T>
struct AttributeVar:public Attribute{
	std::unique_ptr<T> _val = std::unique_ptr<T>(new T(0));
	AttributeVar(void*_last_val=nullptr){
		if(_last_val)memcpy(_val.get(), _last_val, sizeof(T));
		// type_bytes = sizeof(T);
		val = _val.get();
	}
	template<class Archive>
	void serialize(Archive& ar){
		ar(cereal::make_nvp("attrib",cereal::base_class<Attribute>(this)));
		ar(cereal::make_nvp("val", _val));
		val = _val.get();
	}
};
#define CRT(gl_type, type, uniformfunc)\
	CEREAL_REGISTER_TYPE(AttributeVar<type>)
// #define CRT(gl_type, type, uniformfunc) CRT_(gl_type, type, uniformfunc)
TYPE_EXPANSION(CRT)

#define _BV32(x) ((uint32_t)1 << x)
#include "component.h"
#include "texture.h"

class Program : public File, public Component {
private:
	void add_shaders();
	void add_textures(const std::vector<std::string>& t_names);
	
	//! THIS WILL BE DELETED
	std::vector<std::vector<unsigned int>> shaders;
	
public:
    unsigned int p_id;
	// 1 means OK, 0 means Failed
    int link_status = 0;
    uint32_t attribs_enabled (){
		uint32_t t=0;
		for(auto&a:attributes)if(!a->uniform)t|= _BV32(a->location);
		return t;
	};
	
	std::vector<std::unique_ptr<Attribute>> attributes;
	std::vector<std::shared_ptr<Texture>> textures;

    /**
	 * Adds a shader to the program
	 * glAttachShader
	 */
    void attach_shader(GLuint s_id);
    void detach_shader(GLuint s_id);
	std::vector<unsigned int> get_shaders();
    void clear_shaders();
    void link();
	
    /**
	 * glUseProgram
	 */
    void use();
	void set_pmat(const glm::mat4& mat);
	
	bool loaded = false;
	static bool supported(const std::string& ext);
	
	IDRAW_IMGUI_DRAW override;
	IDRAW_IMGUI_NAME override{return filename();}
	
    Program(FILE_CONSTRUCT_PARAM) : File(FILE_CONSTRUCT_VARS){
		p_id = glCreateProgram();
	};
    ~Program(){glDeleteProgram(p_id);};
	
	FILE_SUPPOSED_EXT override {return ".prgm";}
	
	template<class Archive>
	void serialize(Archive& ar){
		ar(FILE_SERIALIZE);
		ar(COMPONENT_SERIALIZE);
		
		bool s_empty = shaders.empty();
		std::cout << "Saving Shaders:" << std::endl;
		for(auto&s:shaders)std::cout << "  " << s[0] << std::endl;
		ar(CEREAL_NVP(shaders));
		if(s_empty)add_shaders();
		
		ar(CEREAL_NVP(attributes));
		
		std::vector<std::string> t_names;for(auto&t:textures)t_names.push_back(t->hash_path());
		ar(cereal::make_nvp("textures", t_names));
		if(textures.empty() && t_names.size())add_textures(t_names);
		
		link();
	}
};
CEREAL_REGISTER_TYPE(Program)

#endif // program_h