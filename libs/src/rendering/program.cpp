#include "program.h"

#include <glm/glm.hpp>

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
// Only create a new shader when shader type changes
// 
void Shader::load(){
	auto d_path = engine::get_absolute_from_project(data_path());
	auto filename = d_path.filename().string();
	auto ext = d_path.extension().string();
	
	if (!supported(ext))return;
    
	int old_type = type;
	// Figure out shader type
	
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
	
	
	if (type == 0) {
		printf(ANSI_COLOR_RED "Coudn't find type for shader %s, please update filename" ANSI_COLOR_RESET "\n", filename.c_str());
		return;
	}
	
	printf("Importing %s as type %d shader\n", filename.c_str(), type);
	std::ifstream sfile(d_path);
	auto data = string((istreambuf_iterator<char>(sfile)), istreambuf_iterator<char>());
	
	auto src = data.c_str();
	if(!s_id || old_type != type)s_id = glCreateShader(type);
	glShaderSource(s_id, 1, &src, NULL);
	glCompileShader(s_id);
	glGetShaderiv(s_id, GL_COMPILE_STATUS, &status);
	
	printf((status == GL_TRUE)?ANSI_COLOR_GREEN:ANSI_COLOR_RED);
	printf("Shader %s compilation %s\n" ANSI_COLOR_RESET, filename.c_str(), (status == GL_TRUE)?"success!":"ERROR");
	
	GLsizei log_length;
	// glGetShaderiv(s_id, GL_INFO_LOG_LENGTH, &log_length);
	glGetShaderInfoLog(s_id, sizeof(log), &log_length, log);
	if(log_length)cout << log << endl;
	
	// loaded = true;
	return;
}
void Shader::imgui_draw(){
	ImGui::Text("Type: %d", type);
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


void Program::add_shaders(){
	for(auto&s:shaders){
		auto f = assets::get_file<Shader>(s);
		if(f){
			attach_shader(f->s_id);
		}
	}
}
void Program::add_textures(const std::vector<std::string>& t_names){
	for(auto&t_name:t_names)textures.push_back(assets::get_load_file<Texture>(t_name));
}
std::vector<unsigned int> Program::get_shaders(){
	std::vector<unsigned int> v;
	
	const GLsizei _SHADERS_SIZE=20;
	GLuint _shaders[_SHADERS_SIZE];
	GLsizei _shaders_count;
	glGetAttachedShaders(p_id, _SHADERS_SIZE, &_shaders_count, _shaders);
	v.reserve(_shaders_count);
	for(int i=0;i<_shaders_count;++i)v.push_back(_shaders[i]);
	
	return v;
}


std::unique_ptr<Attribute> attrib_create_val(GLenum _type, void*_last_val=nullptr){
	// Return if we're erasing and pointer is null, if there is no type defined, or if we're creating but pointer is not null
	switch (_type)
	{
	case GL_FLOAT: 		return std::unique_ptr<Attribute>(new AttributeVar<float>    (_last_val)); break;
	case GL_FLOAT_VEC2: return std::unique_ptr<Attribute>(new AttributeVar<glm::vec2>(_last_val)); break;
	case GL_FLOAT_VEC3: return std::unique_ptr<Attribute>(new AttributeVar<glm::vec3>(_last_val)); break;
	case GL_FLOAT_VEC4: return std::unique_ptr<Attribute>(new AttributeVar<glm::vec4>(_last_val)); break;
	case GL_FLOAT_MAT2: return std::unique_ptr<Attribute>(new AttributeVar<glm::mat2>(_last_val)); break;
	case GL_FLOAT_MAT3: return std::unique_ptr<Attribute>(new AttributeVar<glm::mat3>(_last_val)); break;
	case GL_FLOAT_MAT4: return std::unique_ptr<Attribute>(new AttributeVar<glm::mat4>(_last_val)); break;
	case GL_SAMPLER_2D: return std::unique_ptr<Attribute>(new AttributeVar<unsigned int>   (_last_val)); break;
	default:
		throw std::range_error("Type not defined");
		break;
	}
	return nullptr;
}
void attrib_set_uniform(const std::unique_ptr<Attribute>& attrib){
	if(!attrib->uniform)return; // Return if it's not a uniform
	#define TOM(type, offset) (*((type*)attrib->val+offset))
	switch (attrib->type)
	{
	case GL_FLOAT: glUniform1fv(attrib->location, 1, ((float*)attrib->val)); break;
	case GL_FLOAT_VEC2: glUniform2fv(attrib->location, 1, ((float*)attrib->val)); break;
	case GL_FLOAT_VEC3: glUniform3fv(attrib->location, 1, ((float*)attrib->val)); break;
	case GL_FLOAT_VEC4: glUniform4fv(attrib->location, 1, ((float*)attrib->val)); break;
	case GL_FLOAT_MAT2: glUniformMatrix2fv(attrib->location, 1, false, ((float*)attrib->val)); break;
	case GL_FLOAT_MAT3: glUniformMatrix2fv(attrib->location, 1, false, ((float*)attrib->val)); break;
	case GL_FLOAT_MAT4: glUniformMatrix2fv(attrib->location, 1, false, ((float*)attrib->val)); break;
	case GL_SAMPLER_2D:  break;
	default:
		throw std::range_error("Type not defined");
		break;
	}
	#undef TOM
}

void print_link_status(int link_status, const char* filename){
	printf((link_status == GL_TRUE)? ANSI_COLOR_GREEN : ANSI_COLOR_RED);
	printf("Program %s link %s" ANSI_COLOR_RESET "\n", filename, (link_status)?"OK":"BAD");
}

void Program::use(){
	if(link_status){
		// Use the program
		glUseProgram(p_id);
		// Set the uniforms
		for(auto&a:attributes){
			attrib_set_uniform(a);
		}
		// Binding all textures to their appropriate texture unit
		static uint16_t gl_texture[] = {GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2};
		for(
			auto it = textures.begin(); it < textures.end() && 
			it - textures.begin() < sizeof(gl_texture)/sizeof(gl_texture[0]); 
			++it){
			glActiveTexture(gl_texture[it-textures.begin()]);
			glBindTexture(GL_TEXTURE_2D, (*it)->t_id);
		}
	}else {printf("Can't use "); print_link_status(link_status, filename().c_str());}
}



void Program::link(){
	// if(!_shaders_count)return; 
	// glBindFragDataLocation(p_id, 0, "outColor"); 
	auto _shaders = get_shaders();
	shaders.clear();
	for(auto&s_id:_shaders){
		function<bool(Shader*)> f = [&](Shader* s){return s->s_id == s_id;};
		auto s = assets::get_file(f);
		if(s)shaders.push_back(s->my_ref());
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
		// The function tries to match the attribute with an old one and returns the old one's pointer
		auto merge_attrib = [&](Attribute &a) -> void*{
			auto b = find_if(attributes.begin(), attributes.end(), [&a](std::unique_ptr<Attribute>& b) -> bool{return strcmp(b->name,a.name) == 0 && a.uniform == b->uniform && b->size == a.size;});
			
			if(b == attributes.end() || (*b)->val == nullptr)return nullptr;
			else return (*b)->val;
		};
		
		// Getting attributes
		glGetProgramiv(p_id, GL_ACTIVE_ATTRIBUTES, &count);
		for(int i=0;i<count;++i){
			Attribute a;a.i=i;a.uniform=false;
			glGetActiveAttrib(p_id, i, a.max_name, &a.name_length, &a.size, &a.type, a.name);
			a.location = glGetAttribLocation(p_id, a.name);
			std::unique_ptr<Attribute> av = attrib_create_val(a.type,merge_attrib(a));
			av->i = a.i;
			av->uniform = a.uniform;
			av->name_length = a.name_length;
			av->size = a.size;
			av->type = a.type;
			av->location = a.location;
			strcpy(av->name, a.name);
			new_attributes.push_back(std::move(av));
		}
		
		// Getting uniforms
		glGetProgramiv(p_id, GL_ACTIVE_UNIFORMS, &count);
		for(int i=0;i<count;++i){
			Attribute a;a.i=i;a.uniform=true;
			glGetActiveUniform(p_id, i, a.max_name, &a.name_length, &a.size, &a.type, a.name);
			a.location = glGetUniformLocation(p_id, a.name);
			std::unique_ptr<Attribute> av = attrib_create_val(a.type,merge_attrib(a));
			av->i = a.i;
			av->uniform = a.uniform;
			av->name_length = a.name_length;
			av->size = a.size;
			av->type = a.type;
			av->location = a.location;
			strcpy(av->name, a.name);
			new_attributes.push_back(std::move(av));
		}
		
		attributes = std::move(new_attributes);
	}
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
			if(ImGui::SmallButton("X")){
				detach_shader(shader->s_id);
				link();
				ImGui::PopID();
				break;
			}
			ImGui::PopID();
		}
	}
	
	ImGui::PopStyleVar();
	ImGui::EndChild();
	
	ImGui::PushID("add_shader");
	if(ImGui::Button("Add Shader", ImVec2(-1,0))){
		ImGui::OpenPopup("add_popup");
	}
	
	auto shader_files = assets::get_files_type<File,Shader>();
	auto shader = std::dynamic_pointer_cast<Shader>(menus::add_popup(shader_files));
	if(shader)
		{
			attach_shader(shader->s_id);link();
		}
	ImGui::PopID();
	if(ImGui::Button("Relink", ImVec2(-1,0)))link();
	ImGui::TextColored(link_status?ImVec4(0.f,1.f,0.f,1.f):ImVec4(1.f,0.f,0.f,1.f), link_status?"Ok":"Failed");
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
	
	ImGui::TextDisabled("Textures");
	for(auto it=textures.begin();it<textures.end();++it){
		int index = it-textures.begin();
		ImGui::PushID(index);
		ImGui::Image((void*)(intptr_t)((*it)->t_id), ImVec2(50,50));
		ImGui::SameLine();
		ImGui::Text("GL_TEXTURE%d", index);
		ImGui::SameLine();
		if(index && ImGui::SmallButton("Up")){
			auto temp = *it; // Store this index
			*it = *(it-1); // Copy previous to this
			*(it-1) = temp; // Copy temp to previous
		}
		ImGui::SameLine();
		if(index < textures.size()-1 && ImGui::SmallButton("Down")){
			auto temp = *it; // Store this index
			*it = *(it+1); // Copy next to this
			*(it+1) = temp; // Copy temp to next
		}
		ImGui::SameLine();
		if(ImGui::SmallButton("X")){
			textures.erase(it);
			ImGui::PopID();
			break;
		}
		ImGui::PopID();
	}
	
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.1,.1,.1,.2));
	if(ImGui::Button("Drag Textures Here",ImVec2(-1,0)))ImGui::OpenPopup("add_texture_popup");
	//? IMAGE DRAG N DROP
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_REF"))
		{
			unsigned int data[11];
			assert(payload->DataSize == sizeof(data));
			memcpy(data, payload->Data, sizeof(data));
			std::vector<unsigned int> refs;for(int i=0;i<data[0];++i)refs.push_back(data[i+1]);
			auto texture = assets::get_file<Texture>(refs);
			if(texture)textures.push_back(texture);
		}
		ImGui::EndDragDropTarget();
	}
	//? ############ IMAGE DRAG N DROP
	menus::add_popup(assets::get_files_type<File,Texture>(), false, "add_texture_popup");
	
	ImGui::PopStyleColor();
	
	ImGui::Separator();
	
	ImGui::TextDisabled("Uniforms");
	for(auto&a:attributes){
		if(!a->uniform)continue;
		ImGui::PushID(a->i);
		ImGui::TextDisabled("%d",a->i);ImGui::SameLine(0,_SEPARATION);
		#define PICKER_NAME "##Picker"
		bool is_color = std::string(a->name).find("col") != std::string::npos && (a->type == GL_FLOAT_VEC3 || a->type == GL_FLOAT_VEC4);
		
		if(is_color){
			if(a->type == GL_FLOAT_VEC3)
				ImGui::ColorEdit3(PICKER_NAME,(float*)a->val,ImGuiColorEditFlags_NoInputs);
			else 
				ImGui::ColorEdit4(PICKER_NAME,(float*)a->val,ImGuiColorEditFlags_NoInputs);
			ImGui::SameLine(0,_SEPARATION);	
		}
		ImGui::TextDisabled(a->name);
		if(!is_color){
			if(a->type == GL_FLOAT_VEC3 || a->type == GL_FLOAT_VEC4){
				ImGui::SameLine();
				ImGui::SetNextItemWidth(-1);
				if(a->type == GL_FLOAT_VEC3)ImGui::DragFloat3(PICKER_NAME,(float*)a->val);
				else if(a->type == GL_FLOAT_VEC4)ImGui::DragFloat4(PICKER_NAME,(float*)a->val);
			}else if (a->type == GL_SAMPLER_2D){
				auto val = (int*)a->val;
				ImGui::SameLine();
				ImGui::SetNextItemWidth(-1);
				ImGui::InputInt(PICKER_NAME, val, 1, 1);
				if(*val < 0)*val=0;
			}
		}
		
		ImGui::PopID();
	}
	
	
	// ImGui::PopID();
}