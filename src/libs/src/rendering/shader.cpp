#include "shader.h"
#include "engine_globals.h"

using namespace std;

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