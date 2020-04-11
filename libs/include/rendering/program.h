#ifndef program_h
#define program_h

#include "rendering_common.h"
#include "component.h"

#define FRAGMENT_EXT ".frag"
#define VERTEX_EXT ".vert"
#define PROGRAM_EXT ".prgm"

class Shader : public File {
public:
    unsigned int s_id=0;
    GLenum type=0;
    int status;
    char log[200];
	
	bool loaded = false;
	static bool supported(const std::string& ext);
	/**
	 * Will load code and recompile
	 */
	void load();
    
	// Careful if shader isn't loaded correctly, shader will not be created
    Shader(FILE_CONSTRUCT_PARAM) : File(FILE_CONSTRUCT_VARS){
		if(!rpath.empty()){
			load();
			if(!loaded)std::printf("Shader %s loading failed\n", data_path().c_str());
		}
	}
	~Shader(){glDeleteShader(s_id);}
	
	template<class Archive>
	void serialize(Archive& ar){
		ar(FILE_SERIALIZE);
		ar(CEREAL_NVP(type));
		load();
	}
};
CEREAL_REGISTER_TYPE(Shader)


struct Attribute{
	GLboolean uniform=false;
	GLint i;
	GLint location;
	GLint size;
	GLenum type=0;
	static const GLsizei max_name=20;
	GLchar name[max_name];
	GLsizei length;
	void* val=nullptr;
};
template <typename T>
struct AttributeVar:public Attribute{
	std::unique_ptr<T> _val = std::unique_ptr<T>(new T(0));
	AttributeVar(void*_last_val=nullptr){
		if(_last_val)memcpy(_val.get(), _last_val, sizeof(T));
		val = _val.get();
	}
};



class Program : public File, public Component {
public:
    unsigned int p_id;
    int link_status = 1;
    uint32_t attribs_enabled (){
		uint32_t t=0;
		for(auto&a:attributes)if(!a->uniform)t|=_BV32(a->location);
		return t;
	};

    // std::vector<std::shared_ptr<Shader>> _shaders;
	std::unordered_set<std::string> shaders;
	std::vector<std::unique_ptr<Attribute>> attributes;

    /**
	 * Adds a shader to the program
	 * glAttachShader
	 */
    void attach_shader(GLuint s_id);
    void detach_shader(GLuint s_id);
    void clear_shaders();
    void link();
	
    /**
	 * glUseProgram
	 */
    void use();
	
	bool loaded = false;
	static bool supported(const std::string& ext);
	
	IDRAW_IMGUI_DRAW override;
	IDRAW_IMGUI_NAME override{
		return (std::string("Program ") + filename()).c_str();
	}
	
    Program(FILE_CONSTRUCT_PARAM) : File(FILE_CONSTRUCT_VARS){
		p_id = glCreateProgram();
	};
    ~Program(){glDeleteProgram(p_id);};
	
	template<class Archive>
	void serialize(Archive& ar){
		clear_shaders();
		ar(FILE_SERIALIZE);
		// ar(CEREAL_NVP(type));
		
		link();
	}
};
CEREAL_REGISTER_TYPE(Program)

#endif // program_h