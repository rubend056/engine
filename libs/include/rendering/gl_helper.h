#ifndef GL_helper_h
#define GL_helper_h

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include <string>
#include <vector>
#include <cstring>

class IFile{
	public:
		char* filename;
	IFile(const char* _f){filename=new char[strlen(_f)];strcpy(filename, _f);}
	~IFile(){delete[] filename;}
};

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh : public IFile{
	public:
		unsigned int v_id;
		Vertex* vertices;
		Mesh(const char* _f,int n):IFile(_f){vertices = new Vertex[n];}
		~Mesh(){delete[] vertices;}
};

class Texture : public IFile{
	public:
		unsigned int t_id;
		Texture(const char* _f,unsigned int _id):IFile(_f),t_id(_id){}
};


enum SHADER_ENUM{NOTHING=0,VERTEX,GEOMETRY,FRAGMENT};

struct Program; //Forward declaration
struct Shader : public IFile{
	// char* name;
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
	Shader(SHADER_ENUM type, const char* _f, const char* src=NULL);
	~Shader();
};

struct Program : public IFile{
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
	Program(const char* _f);
	~Program();
};



#endif