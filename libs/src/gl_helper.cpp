#include "gl_helper.h"
#include <cstring>
#include <iostream>

using namespace std;

vector<Shader> shaders;
vector<Program> programs;

void Shader::update(const char* src){
	glShaderSource(s_id, 1, &src, NULL);
	glCompileShader(s_id);
	glGetShaderiv(s_id, GL_COMPILE_STATUS, &status);
	glGetShaderInfoLog(s_id, sizeof(log), NULL, log);
	
	printf("Shader %s compilation %s", name, (status == GL_TRUE)?"success!":"ERROR");
	if(status == GL_TRUE){for(auto&p:_programs)p->link();}
}
Shader::Shader(GLenum type, const char* _name=NULL, const char* src=NULL){
	s_id = glCreateShader(type);
	if(_name){name=new char[sizeof(_name)];strcpy(name, _name);}
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
Program::~Program(){delete[] name;}


