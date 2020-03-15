#include "rendering/gl_helper.h"
#include "groups/gl.h"
#include <cstring>
#include <iostream>

using namespace std;

// vector<Shader> shaders;
// vector<Program> programs;

void Shader::update(const char* src){
	glShaderSource(s_id, 1, &src, NULL);
	glCompileShader(s_id);
	glGetShaderiv(s_id, GL_COMPILE_STATUS, &status);
	glGetShaderInfoLog(s_id, sizeof(log), NULL, log);
	
	printf("Shader %s compilation %s\n", name, (status == GL_TRUE)?"success!":"ERROR");
	if(status == GL_TRUE){for(auto&p:_programs)p->link();}
}

Shader::Shader(SHADER_ENUM type, const char* _name, const char* src){
	GLenum t;
	switch(type){
		case VERTEX:t=GL_VERTEX_SHADER; break;
		case GEOMETRY:t=GL_GEOMETRY_SHADER; break;
		case FRAGMENT:t=GL_FRAGMENT_SHADER; break;
		default: printf("Coudn't find shader in SHADER_ENUM %s\n", type);
	}
	s_id = glCreateShader(t);
	if(_name){name=new char[sizeof(_name)];strcpy(name, _name);}
	log = new char[200];
	if(src)update (src);
}
Shader::~Shader(){
	delete[] log,name;
	glDeleteShader(s_id);
}

	
void Program::add_shader(Shader* shader){
	glAttachShader(p_id, shader->s_id);
	
	_shaders.push_back(shader);
	for(auto &p:shader->_programs){if (this == p)return;}
	shader->_programs.push_back(this);
}
void Program::link(){
	glLinkProgram(p_id);
}
void Program::use(){
	glUseProgram(p_id);
}
Program::Program(const char* _name){
	if(_name){name=new char[sizeof(_name)];strcpy(name, _name);}
}
Program::~Program(){
	delete[] name;
	glDeleteProgram(p_id);
}


