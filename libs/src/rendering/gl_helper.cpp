#include "rendering/gl_helper.h"
#include "groups/gl.h"
#include <cstring>
#include <iostream>

using namespace std;

#include <algorithm>
#include "groups/debug.h"
// vector<Shader> shaders;
// vector<Program> programs;

// ? MESH ***************
Mesh::Mesh(const char* _f,int n, bool _normals, bool _tex_cords):IFile(_f){
	n_vertices = n;
	// vertices = new Vertex[n];
	// Initialize vertices
	positions = new glm::vec3[n];
	if(_normals)normals = new glm::vec3[n];
	if(_tex_cords)tex_cords = new glm::vec2[n];
	
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
}
void Mesh::gl_draw(){glDrawArrays(GL_TRIANGLES, 0, n_vertices);}
void Mesh::vbo_bind(){glBindBuffer(GL_ARRAY_BUFFER, vbo);}
void Mesh::vao_bind(){glBindVertexArray(vao);}
void Mesh::vbo_set_data(){
	auto vbo_size = ((positions?sizeof(*positions):0) + 
					 (normals?sizeof(*normals):0) + 
					 (tex_cords?sizeof(*tex_cords):0)) * n_vertices;
	
	auto pos_s = sizeof(*positions)*n_vertices,
	norm_s = sizeof(*normals)*n_vertices,
	tex_s = sizeof(*tex_cords)*n_vertices;
	vbo_bind();
	// Setting the array buffer size
	glBufferData(GL_ARRAY_BUFFER, vbo_size, NULL , GL_STATIC_DRAW);
	// Filling the array buffer with our RAM data
	unsigned long i=0;
	auto f = [&i](unsigned long s, void*d) -> void{  glBufferSubData(GL_ARRAY_BUFFER, i, s, d);i+=s;  };
	f(pos_s, positions);
	if(normals)f(norm_s, normals);
	if(tex_cords)f(tex_s, tex_cords);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * n_vertices, vertices, GL_STATIC_DRAW);
	vao_set_vertex_attrib_pointer();
}
void Mesh::vao_set_vertex_attrib_pointer(){
	vao_bind();
	vbo_bind();
	auto pos_s = sizeof(*positions)*n_vertices,
	norm_s = sizeof(*normals)*n_vertices,
	tex_s = sizeof(*tex_cords)*n_vertices;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	if(normals)glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)pos_s);
	if(tex_cords)glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(pos_s+norm_s));
}


// ? SHADER ***************
void Shader::update(const char* src){
	glShaderSource(s_id, 1, &src, NULL);
	glCompileShader(s_id);
	glGetShaderiv(s_id, GL_COMPILE_STATUS, &status);
	
	printf((status == GL_TRUE)?ANSI_COLOR_GREEN:ANSI_COLOR_RED);
	printf("Shader %s compilation %s\n" ANSI_COLOR_RESET, filename, (status == GL_TRUE)?"success!":"ERROR");
	
	int log_length;
	glGetShaderiv(s_id, GL_INFO_LOG_LENGTH, &log_length);
	if(log_length){
		delete[] log;log = new char[log_length];
		glGetShaderInfoLog(s_id, log_length, NULL, log);
		cout << log << endl;
	}
	
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


// ? PROGRAM ***************
Program::Program(const char* _f):IFile(_f){
	// glGenVertexArrays(1, &vao);
	p_id = glCreateProgram();
}
Program::~Program(){clear_shaders();glDeleteProgram(p_id);}

void Program::attach_shader(Shader* shader){
	if(!shader){printf(ANSI_COLOR_RED "Shader %s is null" ANSI_COLOR_RESET "\n", shader->filename);return;}
	glAttachShader(p_id, shader->s_id);
	
	// Adding shader to our list and updating the shader's program list
	_shaders.push_back(shader);
	for(auto&p:shader->_programs){
		if (this == p)
		return;
	}
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
	link();
}
void Program::clear_shaders(){
	for(auto &s:_shaders){
		s->_programs.erase(find(s->_programs.begin(), s->_programs.end(), this));
		glDetachShader(p_id, s->s_id);
	}
	_shaders.clear();
	link();
}


bool attrib_bind(unsigned int p_id, int attrib_num, GLuint good_attrib_num, const char * name, const char* shader_filename, bool crucial=false){
	if(attrib_num != good_attrib_num){
		printf((crucial ? ANSI_COLOR_RED:ANSI_COLOR_YELLOW));
		printf((attrib_num < 0)?"No '%s' attrib":"'%s' attrib different than %d", name, good_attrib_num);
		printf( " in shader %s" ANSI_COLOR_RESET "\n", shader_filename);
		if(!crucial && attrib_num >= 0){glBindAttribLocation(p_id, good_attrib_num, name);return true;}
		else return false;
	}
	return true;
}


void print_link_status(int link_status, const char* filename){
	printf((link_status == GL_TRUE)?ANSI_COLOR_GREEN:ANSI_COLOR_RED);
	printf("Program %s link %s"ANSI_COLOR_RESET"\n", filename, (link_status)?"OK":"BAD");
}

void Program::link(){
	if(!_shaders.size())return;
	glBindFragDataLocation(p_id, 0, "outColor");
	glLinkProgram(p_id);
	glGetProgramiv(p_id,GL_LINK_STATUS, &link_status);
	
	print_link_status(link_status, filename);
	
	if(link_status == GL_TRUE){
		auto pos_attrib = glGetAttribLocation(p_id, "pos"),
		norm_atrrib = glGetAttribLocation(p_id, "norm"),
		tex_atrrib = glGetAttribLocation(p_id, "tex_cord");
		auto shader_f = _shaders[0]->filename;
		if(attrib_bind(p_id, pos_attrib, 0, "pos", shader_f, true))attribs_enabled |= _BV32(0);
		else{link_status=GL_FALSE;return;}
		if(attrib_bind(p_id, norm_atrrib, 1, "norm", shader_f))attribs_enabled |= _BV32(1);
		if(attrib_bind(p_id, tex_atrrib, 2, "tex_cord", shader_f))attribs_enabled |= _BV32(2);
		
		if(link_status){
			use();
			auto u = glGetUniformLocation(p_id, "color_uniform");
			if(u>=0)glUniform3f(u, 0, 0, 1);
		}
	}
}
void Program::use(){
	if(link_status)glUseProgram(p_id);
	else {printf("Can't use "); print_link_status(link_status, filename);}
}

