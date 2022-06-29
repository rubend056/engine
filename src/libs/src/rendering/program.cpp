#include "program.h"

#include <fstream>
#include <glm/glm.hpp>

#include "assets.h"
#include "debug.h"
#include "imgui_helper.h"
#include "menus.h"

using namespace std;
#include "engine_globals.h"

#define MAX_SHADERS_SIZE 20

void Program::attach_shader(GLuint s_id) {
	glAttachShader(p_id, s_id);
}
void Program::detach_shader(GLuint s_id) {
	glDetachShader(p_id, s_id);
}
void Program::clear_shaders() {
	for (auto& s : shaders) {
		auto shader = assets::get_file<Shader>(s);
		if (shader)
			glDetachShader(p_id, shader->s_id);
	}
	shaders.clear();
}

bool Program::supported(std::string ext) {
	return ext.compare(PROGRAM_EXT) == 0;
}

void Program::add_shaders() {
	for (auto& s : shaders) {
		auto f = assets::get_file<Shader>(s);
		if (f) {
			attach_shader(f->s_id);
		}
	}
}
void Program::add_textures(const std::vector<std::vector<uint>>& t_ids) {
	for (auto& t_id : t_ids) textures.push_back(assets::get_file<Texture>(t_id));
}
std::vector<unsigned int> Program::get_shaders() {
	std::vector<unsigned int> v;

	// Get shaders
	GLuint _shaders[MAX_SHADERS_SIZE];
	GLsizei _shaders_count;
	glGetAttachedShaders(p_id, MAX_SHADERS_SIZE, &_shaders_count, _shaders);

	v.reserve(_shaders_count);
	for (int i = 0; i < _shaders_count; ++i) v.push_back(_shaders[i]);
	return v;
}

static const char* default_uniforms[] = {
	"itime",
	"idtime",
	"ipmat"};

std::unique_ptr<Attribute> attrib_create_val(GLenum _type, void* _last_val = nullptr) {
// Return if we're erasing and pointer is null, if there is no type defined, or if we're creating but pointer is not null
#define ATTRIB_CREATE_FUNC(gl_type, type, gl_type_name, uniformfunc)          \
	case gl_type:                                                             \
		return std::unique_ptr<Attribute>(new AttributeVar<type>(_last_val)); \
		break;

	switch (_type) {
		TYPE_EXPANSION(ATTRIB_CREATE_FUNC)
		default:
			throw std::range_error("Type not defined");
			break;
	}
	return nullptr;
}
/**
 * Sets uniform if attibute is a uniform and 
 */
void attrib_set_uniform(const std::unique_ptr<Attribute>& attrib, const void* val = nullptr) {
	if (!attrib->uniform || !val)
		return;	 // Return if it's not a uniform, or val null

	switch (attrib->type) {
		// TYPE_EXPANSION(SET_UNIFORM_FUNC)
		case GL_FLOAT:
			glUniform1fv(attrib->location, 1, ((float*)val));
			break;
		case GL_DOUBLE:
			glUniform1dv(attrib->location, 1, ((double*)val));
			break;
		case GL_FLOAT_VEC2:
			glUniform2fv(attrib->location, 1, ((float*)val));
			break;
		case GL_FLOAT_VEC3:
			glUniform3fv(attrib->location, 1, ((float*)val));
			break;
		case GL_FLOAT_VEC4:
			glUniform4fv(attrib->location, 1, ((float*)val));
			break;
		case GL_FLOAT_MAT2:
			glUniformMatrix2fv(attrib->location, 1, false, ((float*)val));
			break;
		case GL_FLOAT_MAT3:
			glUniformMatrix3fv(attrib->location, 1, false, ((float*)val));
			break;
		case GL_FLOAT_MAT4:
			glUniformMatrix4fv(attrib->location, 1, false, ((float*)val));
			break;
		case GL_SAMPLER_2D:
			break;
		default:
			throw std::range_error("Type not defined");
			break;
	}
	// #undef TOM
}
/**
 * Sets uniform if name and type match
 */
void attrib_set_uniform(std::string name, int gl_type, const void* val) {
}

void print_link_status(int link_status, const char* filename, const char* debug_info = nullptr) {
	printf(link_status == GL_TRUE ? ANSI_COLOR_GREEN : ANSI_COLOR_RED);
	printf("Program %s link %s" ENDL, filename, link_status ? "OK" : "BAD");
	if (debug_info)
		printf(debug_info);
	printf(ANSI_COLOR_RESET);
}

void Program::use() {
	if (link_status != GL_TRUE) {
		printf("Can't use ");
		print_link_status(link_status, filename().c_str());
		return;
	}
	// Use the program
	glUseProgram(p_id);
	// Set the uniforms
	for (auto& a : attributes) {
		if (strcmp(a->name, "itime") == 0)
			glUniform1d(a->location, engine::time);
		else if (strcmp(a->name, "idtime") == 0)
			glUniform1d(a->location, engine::deltaTime);
		else
			attrib_set_uniform(a);
	}
	// Binding all textures to their appropriate texture unit
	static uint16_t gl_texture[] = {GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2};
	for (
		auto it = textures.begin(); it < textures.end() &&
									(size_t)(it - textures.begin()) < sizeof(gl_texture) / sizeof(gl_texture[0]);
		++it) {
		glActiveTexture(gl_texture[it - textures.begin()]);
		glBindTexture(GL_TEXTURE_2D, (*it)->t_id);
	}
}

void Program::set_pmat(const glm::mat4& mat) {
	// Find an attribute called ipmat
	for (auto& a : attributes) {
		if (strcmp(a->name, "ipmat") != 0 && strcmp(a->name, "pmat") != 0)
			continue;
		attrib_set_uniform(a, &mat);
		return;
	}
}

void Program::link() {
	shaders.clear();
	// Repopulate shaders
	auto _shaders = get_shaders();
	for (auto& s_id : _shaders) {
		// NOLINT
		function<bool(Shader*)> f = [&](Shader* s) { return s->s_id == s_id; };
		auto s = assets::get_file(f);
		if (s)
			shaders.push_back(s->my_ref());
	}

	// Link
	glLinkProgram(p_id);

	// Get link status
	glGetProgramiv(p_id, GL_LINK_STATUS, &link_status);

	// Get Log
	GLint maxLength = 0;
	glGetProgramiv(p_id, GL_INFO_LOG_LENGTH, &maxLength);
	GLchar infoLog[maxLength];
	glGetProgramInfoLog(p_id, maxLength, &maxLength, infoLog);

	// Print status and log
	print_link_status(link_status, filename().c_str(), maxLength > 1 ? infoLog : nullptr);

	if (link_status == GL_TRUE) {
		std::vector<std::unique_ptr<Attribute>> new_attributes;
		// The function tries to match the attribute with an old one and returns the old one's pointer
		auto merge_attrib = [&](Attribute& a) -> void* {
			auto b = find_if(attributes.begin(), attributes.end(), [&a](std::unique_ptr<Attribute>& b) -> bool { return strcmp(b->name, a.name) == 0 && a.uniform == b->uniform && b->size == a.size; });

			if (b == attributes.end())
				return nullptr;
			else
				return (*b)->val;
		};

		GLint count;
		// Will loop through attribs first, then uniforms
		for (int is_attrib = 1; is_attrib >= 0; --is_attrib) {
			is_attrib ? glGetProgramiv(p_id, GL_ACTIVE_ATTRIBUTES, &count) : glGetProgramiv(p_id, GL_ACTIVE_UNIFORMS, &count);
			for (int i = 0; i < count; ++i) {
				Attribute a;
				a.i = i;
				a.uniform = !is_attrib;
				is_attrib ? glGetActiveAttrib(p_id, i, a.max_name, &a.name_length, &a.size, &a.type, a.name) : glGetActiveUniform(p_id, i, a.max_name, &a.name_length, &a.size, &a.type, a.name);
				is_attrib ? a.location = glGetAttribLocation(p_id, a.name) : a.location = glGetUniformLocation(p_id, a.name);

				// Check if attribute is one of the known ones
				bool is_imp = false;
				for (size_t e = 0; e < sizeof(default_uniforms) / sizeof(default_uniforms[0]); ++e) {
					if (strcmp(default_uniforms[e], a.name) == 0) {
						is_imp = true;
						break;
					}
				}

				std::unique_ptr<Attribute> av =
					(!a.uniform || is_imp) ? std::make_unique<Attribute>() : attrib_create_val(a.type, merge_attrib(a));
				av->i = a.i;
				av->uniform = a.uniform;
				av->name_length = a.name_length;
				av->size = a.size;
				av->type = a.type;
				av->location = a.location;
				strcpy(av->name, a.name);
				new_attributes.push_back(std::move(av));
			}
		}

		attributes = std::move(new_attributes);
	}
}

void Program::imgui_draw() {
	// ImGui::Text("Program %s",filename);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
	ImGui::Text("Shaders:");
	// ImGui::Nexti
	ImGui::BeginChild("Shaders", ImVec2(0, 100), true);

	{
		int _i = 0;
		bool x_pressed = false;
		for (auto& v : shaders) {
			++_i;
			auto shader = assets::get_file<Shader>(v);

			ImGui::PushID(_i);
			{
				ImGui::Text("%s", shader->filename().c_str());
				ImGui::SameLine();
				x_pressed = ImGui::SmallButton("X");
			}
			ImGui::PopID();

			if (x_pressed) {
				detach_shader(shader->s_id);
				link();
				break;
			}
		}
	}

	ImGui::PopStyleVar();
	ImGui::EndChild();

	ImGui::PushID("add_shader");  // ID add_shader
	{
		if (ImGui::Button("Add Shader", ImVec2(-1, 0))) {
			ImGui::OpenPopup("select_shader");
		}

		// auto shader_files = assets::get_files_type<File, Shader>();
		if (auto shader = menus::select_asset<Shader>("select_shader")) {
			attach_shader(shader->s_id);
			link();
		}
	}
	ImGui::PopID();	 // ID add_shader

	if (ImGui::Button("Relink", ImVec2(-1, 0)))
		link();
	ImGui::TextColored(link_status ? ImVec4(0.f, 1.f, 0.f, 1.f) : ImVec4(1.f, 0.f, 0.f, 1.f), link_status ? "Ok" : "Failed");
	ImGui::Separator();
#define _SEPARATION 3

	// ImGui::PushID("Attributes");
	ImGui::TextDisabled("Attributes");
	for (auto& a : attributes) {
		if (a->uniform)
			continue;
		ImGui::PushID(a->i);  // ID attr index
		ImGui::TextDisabled("%d", a->i);
		ImGui::SameLine(0, _SEPARATION);
		ImGui::TextDisabled(a->name);
		ImGui::PopID();	 // ID attr index
	}

	ImGui::Separator();

	ImGui::TextDisabled("Textures");
	for (auto it = textures.begin(); it < textures.end(); ++it) {
		size_t index = it - textures.begin();
		ImGui::PushID(index);
		ImGui::Image((void*)(intptr_t)((*it)->t_id), ImVec2(50, 50));
		ImGui::SameLine();
		ImGui::Text("GL_TEXTURE%zu", index);
		ImGui::SameLine();
		if (index && ImGui::SmallButton("Up")) {
			auto temp = *it;   // Store this index
			*it = *(it - 1);   // Copy previous to this
			*(it - 1) = temp;  // Copy temp to previous
		}
		ImGui::SameLine();
		if (index < textures.size() - 1 && ImGui::SmallButton("Down")) {
			auto temp = *it;   // Store this index
			*it = *(it + 1);   // Copy next to this
			*(it + 1) = temp;  // Copy temp to next
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("X")) {
			textures.erase(it);
			ImGui::PopID();
			break;
		}
		ImGui::PopID();
	}

	// ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.1, .1, .1, .2));
	if (ImGui::Button("Add Texture", ImVec2(-1, 0)))
		ImGui::OpenPopup("add_texture_popup");
	//? IMAGE DRAG N DROP

	static std::vector<uint> refs;
	imgui_dnd_ref(refs, false);
	if (refs.size()) {
		if (auto texture = assets::get_file<Texture>(refs)) {
			// Only add if it doesn't exist
			if (std::find(textures.begin(), textures.end(), texture) == textures.end())
				textures.push_back(texture);
		}
	}

	//? ############ IMAGE DRAG N DROP
	if (auto texture = menus::select_asset<Texture>("add_texture_popup")) {
		// Only add if it doesn't exist
		if (std::find(textures.begin(), textures.end(), texture) == textures.end())
			textures.push_back(texture);
	}

	// ImGui::PopStyleColor();

	ImGui::Separator();

	ImGui::TextDisabled("Uniforms");
	for (auto& a : attributes) {
		if (!a->uniform)
			continue;
		ImGui::PushID(a->i);
		ImGui::TextDisabled("%d", a->i);
		ImGui::SameLine(0, _SEPARATION);
#define PICKER_NAME "##Picker"
		bool is_color = std::string(a->name).find("col") != std::string::npos && (a->type == GL_FLOAT_VEC3 || a->type == GL_FLOAT_VEC4);

		if (is_color && a->val) {
			if (a->type == GL_FLOAT_VEC3)
				ImGui::ColorEdit3(PICKER_NAME, (float*)a->val, ImGuiColorEditFlags_NoInputs);
			else
				ImGui::ColorEdit4(PICKER_NAME, (float*)a->val, ImGuiColorEditFlags_NoInputs);
			ImGui::SameLine(0, _SEPARATION);
		}
		ImGui::TextDisabled(a->name);
		if (!is_color && a->val) {
			if (a->type == GL_FLOAT_VEC3 || a->type == GL_FLOAT_VEC4) {
				ImGui::SameLine();
				ImGui::SetNextItemWidth(-1);
				if (a->type == GL_FLOAT_VEC3)
					ImGui::DragFloat3(PICKER_NAME, (float*)a->val);
				else if (a->type == GL_FLOAT_VEC4)
					ImGui::DragFloat4(PICKER_NAME, (float*)a->val);
			} else if (a->type == GL_SAMPLER_2D) {
				auto val = (int*)a->val;
				ImGui::SameLine();
				ImGui::SetNextItemWidth(-1);
				ImGui::InputInt(PICKER_NAME, val, 1, 1);
				if (*val < 0)
					*val = 0;
			}
		}

		ImGui::PopID();
	}

	// ImGui::PopID();
}