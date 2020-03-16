#include "rendering/gl_helper.h"
#include "groups/gl.h"
#include <cstring>
#include <iostream>

using namespace std;

#include <algorithm>
// vector<Shader> shaders;
// vector<Program> programs;

Mesh::Mesh(const char* _f,int n):IFile(_f){
	n_vertices = n;
	vertices = new Vertex[n];
	glGenBuffers(1, &vbo);
}
void Mesh::bind(){glBindBuffer(GL_ARRAY_BUFFER, vbo);}
void Mesh::set_data(){
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * n_vertices, vertices, GL_STATIC_DRAW);
}

void Shader::update(const char* src){
	glShaderSource(s_id, 1, &src, NULL);
	glCompileShader(s_id);
	glGetShaderiv(s_id, GL_COMPILE_STATUS, &status);
	glGetShaderInfoLog(s_id, sizeof(log), NULL, log);
	
	printf("Shader %s compilation %s\n", filename, (status == GL_TRUE)?"success!":"ERROR");
	if(status == GL_TRUE){for(auto&p:_programs)p->link();}
}

Shader::Shader(SHADER_ENUM _type, const char* _f, const char* src):IFile(_f){
	GLenum t;
	type = _type;
	switch(type){
		case VERTEX:t=GL_VERTEX_SHADER; break;
		case GEOMETRY:t=GL_GEOMETRY_SHADER; break;
		case FRAGMENT:t=GL_FRAGMENT_SHADER; break;
		default: printf("Coudn't find shader in SHADER_ENUM %s\n", type);
	}
	s_id = glCreateShader(t);
	if(_f){filename=new char[sizeof(_f)];strcpy(filename, _f);}
	log = new char[200];
	if(src)update (src);
}
Shader::~Shader(){
	delete[] log,filename;
	glDeleteShader(s_id);
}


Program::Program(const char* _f):IFile(_f){
	glGenVertexArrays(1, &vao);
	p_id = glCreateProgram();
}
Program::~Program(){clear_shaders();glDeleteProgram(p_id);}

void Program::attach_shader(Shader* shader){
	if(!shader)return;
	glAttachShader(p_id, shader->s_id);
	
	// Adding shader to our list and updating the shader's program list
	_shaders.push_back(shader);
	for(auto&p:shader->_programs){if (this == p)return;}
	shader->_programs.push_back(this);
	
	link();
}
void Program::detach_shader(unsigned int index){
	if(index >= _shaders.size())return;
	auto shaderit = _shaders.begin() + index;
	auto shader=*shaderit;
	_shaders.erase(shaderit);
	
	// If you shader is still in our list after we erased it we skip deleting ourselves from it's list
	if(find(_shaders.begin(), _shaders.end(), shader) == _shaders.end())goto more_than_one;
	// If the shader is gone for good, remove ourselves from it's list
	shader->_programs.erase(find(shader->_programs.begin(), shader->_programs.end(), this));
more_than_one:
	glDetachShader(p_id, shader->s_id);
	// link();
}
void Program::clear_shaders(){
	for(auto &s:_shaders){
		s->_programs.erase(find(s->_programs.begin(), s->_programs.end(), this));
		glDetachShader(p_id, s->s_id);
	}
	_shaders.clear();
	// link();
}
void Program::link_vertex(Mesh* mesh){
	glBindVertexArray(vao);
	mesh->set_data();
	glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, (GLsizei)(sizeof(Vertex)), (void*)0);
	glEnableVertexAttribArray(pos_attrib);
}
void Program::link(){
	glBindFragDataLocation(p_id, 0, "outColor");
	glLinkProgram(p_id);
	pos_attrib = glGetAttribLocation(p_id, "pos");
}
void Program::use(){
	glUseProgram(p_id);
	glBindVertexArray(vao);
}


