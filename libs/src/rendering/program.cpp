#include "program.h"

#include "glm/glm.hpp"

#include "debug.h"
#include "assets.h"
#include "menus.h"

#include <fstream>
using namespace std;

#include "engine_globals.h"

// ? SHADER ***************
bool Shader::supported(const std::string& ext){
	return ext.compare(".glsl") == 0 || ext.compare(".vert") == 0 || ext.compare(".frag") == 0;
}
void Shader::load(){
	auto d_path = engine::get_absolute_from_project(data_path());
	auto filename = d_path.filename().string();
	auto ext = d_path.extension().string();
	
	if (supported(ext)) {
        GLenum type = 0;
        if (ext.compare(FRAGMENT_EXT) == 0)
            type = GL_FRAGMENT_SHADER;
        else if (ext.compare(VERTEX_EXT) == 0)
            type = GL_VERTEX_SHADER;
        else if (filename.find("fragment") != string::npos || filename.find("frag") != string::npos)
            type = GL_FRAGMENT_SHADER;
        else if (filename.find("geometry") != string::npos)
#if (GLAD_OPENGL_CORE_VERSION >= 45)
            type = GL_GEOMETRY_SHADER;
#else
			type = GL_GEOMETRY_SHADER_ARB;
#endif
        else if (filename.find("vertex") != string::npos)
            type = GL_VERTEX_SHADER;

        if (type != 0) {
			
            printf("Importing %s as type %d shader\n", filename.c_str(), type);
            std::ifstream sfile(d_path);
            auto data = string((istreambuf_iterator<char>(sfile)), istreambuf_iterator<char>());
			
			auto src = data.c_str();
			if(s_id)glDeleteShader(s_id);
			s_id = glCreateShader(type);
            glShaderSource(s_id, 1, &src, NULL);
			glCompileShader(s_id);
			glGetShaderiv(s_id, GL_COMPILE_STATUS, &status);
			
			printf((status == GL_TRUE)?ANSI_COLOR_GREEN:ANSI_COLOR_RED);
			printf("Shader %s compilation %s\n" ANSI_COLOR_RESET, filename.c_str(), (status == GL_TRUE)?"success!":"ERROR");
			
			GLsizei log_length;
			// glGetShaderiv(s_id, GL_INFO_LOG_LENGTH, &log_length);
			glGetShaderInfoLog(s_id, sizeof(log), &log_length, log);
			if(log_length)cout << log << endl;
            
			loaded = true;
			return;
        }
        printf("Coudn't find type for shader %s, please update filename\n", filename.c_str());
    }
	loaded = false;
}


// ? PROGRAM ***************
void Program::attach_shader(GLuint s_id){
	glAttachShader(p_id, s_id);
}
void Program::detach_shader(GLuint s_id){
	glDetachShader(p_id, s_id);
}
void Program::clear_shaders(){
	for(auto&s:shaders){
		auto shader = assets::get_file<Shader>(s);
		if(shader)
			glDetachShader(p_id, shader->s_id);
	}
	shaders.clear();
}
bool Program::supported(const std::string& ext){
	return ext.compare(".prgm") == 0;
}

std::unique_ptr<Attribute> create_attrib_val(GLenum type, void*_last_val=nullptr){
	// Return if we're erasing and pointer is null, if there is no type defined, or if we're creating but pointer is not null
	switch (type)
	{
	case GL_FLOAT: 		return std::unique_ptr<Attribute>(new AttributeVar<float>    (_last_val)); break;
	case GL_FLOAT_VEC2: return std::unique_ptr<Attribute>(new AttributeVar<glm::vec2>(_last_val)); break;
	case GL_FLOAT_VEC3: return std::unique_ptr<Attribute>(new AttributeVar<glm::vec3>(_last_val)); break;
	case GL_FLOAT_VEC4: return std::unique_ptr<Attribute>(new AttributeVar<glm::vec4>(_last_val)); break;
	case GL_FLOAT_MAT2: return std::unique_ptr<Attribute>(new AttributeVar<glm::mat2>(_last_val)); break;
	case GL_FLOAT_MAT3: return std::unique_ptr<Attribute>(new AttributeVar<glm::mat3>(_last_val)); break;
	case GL_FLOAT_MAT4: return std::unique_ptr<Attribute>(new AttributeVar<glm::mat4>(_last_val)); break;
	case GL_SAMPLER_2D: return std::unique_ptr<Attribute>(new AttributeVar<GLuint>   (_last_val)); break;
	default:
		throw std::range_error("Type not defined");
		break;
	}
	return nullptr;
}

void print_link_status(int link_status, const char* filename){
	printf((link_status == GL_TRUE)? ANSI_COLOR_GREEN : ANSI_COLOR_RED);
	printf("Program %s link %s" ANSI_COLOR_RESET "\n", filename, (link_status)?"OK":"BAD");
}

void Program::link(){
	// if(!_shaders_count)return; 
	// glBindFragDataLocation(p_id, 0, "outColor"); 
	const GLsizei _SHADERS_SIZE=20;
	GLuint _shaders[_SHADERS_SIZE];
	GLsizei _shaders_count;
	glGetAttachedShaders(p_id, _SHADERS_SIZE, &_shaders_count, _shaders);
	shaders.clear();
	for(GLsizei i=0;i<_shaders_count;++i){
		function<bool(Shader*)> f = [&](Shader* s){return s->s_id == _shaders[i];};
		auto s = assets::get_file(f);
		if(s)shaders.insert(s->data_path());
	}
	
	glLinkProgram(p_id);
	glGetProgramiv(p_id,GL_LINK_STATUS, &link_status);
	
	print_link_status(link_status, filename().c_str());
	
	if(link_status == GL_TRUE){
		// auto pos_attrib = glGetAttribLocation(p_id, "pos"),
		// norm_atrrib = glGetAttribLocation(p_id, "norm"),
		// tex_atrrib = glGetAttribLocation(p_id, "tex_cord");
		// auto shader_f = _shaders[0]->filename;
		// if(attrib_bind(p_id, pos_attrib, 0, "pos", shader_f, true))attribs_enabled |= _BV32(0);
		// else{link_status=GL_FALSE;return;}
		// if(attrib_bind(p_id, norm_atrrib, 1, "norm", shader_f))attribs_enabled |= _BV32(1);
		// if(attrib_bind(p_id, tex_atrrib, 2, "tex_cord", shader_f))attribs_enabled |= _BV32(2);
		
		
		GLint count;
		
		std::vector<std::unique_ptr<Attribute>> new_attributes;
		auto merge_attrib = [&](Attribute &a) -> void*{
			auto b = find_if(attributes.begin(), attributes.end(), [&a](std::unique_ptr<Attribute>& b) -> bool{return strcmp(b->name,a.name) == 0 && a.uniform == b->uniform && b->size == a.size;});
			
			if(b == attributes.end() || (*b)->val == nullptr)return nullptr;
			else return (*b)->val;
		};
		
		// Getting attributes
		glGetProgramiv(p_id, GL_ACTIVE_ATTRIBUTES, &count);
		for(int i=0;i<count;++i){
			Attribute a;a.i=i;a.uniform=false;
			glGetActiveAttrib(p_id, i, a.max_name, &a.length, &a.size, &a.type, a.name);
			a.location = glGetAttribLocation(p_id, a.name);
			std::unique_ptr<Attribute> av = create_attrib_val(a.type,merge_attrib(a));
			av->i = a.i;
			av->uniform = a.uniform;
			av->length = a.length;
			av->size = a.size;
			av->type = a.type;
			strcpy(av->name, a.name);
			new_attributes.push_back(std::move(av));
		}
		
		// Getting uniforms
		glGetProgramiv(p_id, GL_ACTIVE_UNIFORMS, &count);
		for(int i=0;i<count;++i){
			Attribute a;a.i=i;a.uniform=true;
			glGetActiveUniform(p_id, i, a.max_name, &a.length, &a.size, &a.type, a.name);
			a.location = glGetUniformLocation(p_id, a.name);
			std::unique_ptr<Attribute> av = create_attrib_val(a.type,merge_attrib(a));
			av->i = a.i;
			av->uniform = a.uniform;
			av->length = a.length;
			av->size = a.size;
			av->type = a.type;
			strcpy(av->name, a.name);
			new_attributes.push_back(std::move(av));
		}
		
		attributes = std::move(new_attributes);
	}
}
void Program::use(){
	if(link_status)glUseProgram(p_id);
	else {printf("Can't use "); print_link_status(link_status, filename().c_str());}
}

void Program::imgui_draw(){
	// ImGui::Text("Program %s",filename);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
	ImGui::Text("Shaders:");
	// ImGui::Nexti	
	ImGui::BeginChild("Shaders", ImVec2(0,100), true);
	
	{
		int _i=0;
		for(auto&v:shaders){
			++_i;auto shader = assets::get_file<Shader>(v);
			ImGui::PushID(_i);
			ImGui::Text("%s",shader->filename().c_str());
			ImGui::SameLine();
			if(ImGui::Button("X")){detach_shader(shader->s_id);link();}
			ImGui::PopID();
		}
	}
	
	ImGui::PopStyleVar();
	ImGui::EndChild();
	
	ImGui::PushID("add_shader");
	if(ImGui::Button("Add Shader", ImVec2(-1,0))){
		ImGui::OpenPopup("add_popup");}
		
	#warning "Shader class used here"
	auto shader_files = assets::get_files("Shader");
	auto shader = std::dynamic_pointer_cast<Shader>(menus::add_popup(shader_files));
	if(shader)
		{attach_shader(shader->s_id);link();}
	ImGui::PopID();
	
	ImGui::Separator();
	#define _SEPARATION 3
	
	// ImGui::PushID("Attributes");
	ImGui::TextDisabled("Attributes");
	for(auto&a:attributes){
		if(a->uniform)continue;
		ImGui::PushID(a->i);
		ImGui::TextDisabled("%d",a->i);ImGui::SameLine(0,_SEPARATION);
		ImGui::TextDisabled(a->name);
		ImGui::PopID();
	}
	
	ImGui::Separator();
	
	ImGui::TextDisabled("Uniforms");
	for(auto&a:attributes){
		if(!a->uniform)continue;
		ImGui::PushID(a->i);
		ImGui::TextDisabled("%d",a->i);ImGui::SameLine(0,_SEPARATION);
		if(a->type == GL_FLOAT_VEC4)
			{ImGui::ColorPicker4("##Picker",(float*)a->val);ImGui::SameLine(0,_SEPARATION);}
		ImGui::TextDisabled(a->name);
		if(a->type == GL_FLOAT_VEC3){
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100);
			ImGui::DragFloat3("##Picker",(float*)a->val);
		}
		ImGui::PopID();
	}
	// ImGui::PopID();
}