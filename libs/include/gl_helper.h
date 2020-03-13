#ifndef GL_helper_h
#define GL_helper_h

#include <vector>

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
	Shader(unsigned int type, const char* _name=NULL, const char* src=NULL);
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