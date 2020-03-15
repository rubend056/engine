#ifndef GL_helper_h
#define GL_helper_h

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include <string>
#include <vector>



enum SHADER_ENUM{NOTHING=0,VERTEX,GEOMETRY,FRAGMENT};

struct Program; //Forward declaration
struct Shader{
	char* name;
	unsigned int s_id;
	int status;
	char* log = new char[512];
	
	std::vector<Program*> _programs;
	
	/**
	 * Will udpate code and recompile, relinking any programs that use the shader in the process
	 */
	void update(const char* src);
	/**
	 * @param type Must be one of:
	 * GL_COMPUTE_SHADER, GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, or GL_FRAGMENT_SHADER
	 */
	Shader(SHADER_ENUM type, const char* _name=NULL, const char* src=NULL);
	~Shader();
};

struct Program{
	char * name;
	unsigned int p_id;
	
	std::vector<Shader*> _shaders;
	
	/**
	 * Adds a shader to the program
	 * glAttachShader
	 */
	void add_shader(Shader* shader);
	void link();
	/**
	 * glUseProgram
	 */
	void use();
	Program(const char* _name);
	~Program();
};

extern std::vector<Shader> shaders;
extern std::vector<Program> programs;



#endif