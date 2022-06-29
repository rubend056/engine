#include "texture.h"

#include "SOIL.h"
#include "engine_globals.h"
#include "gl.h"

// ? TEXTURE ******************************************************************

Texture::Texture(const fs::path& rpath) : File(FILE_CONSTRUCT_VARS) {
	glGenTextures(1, &t_id);
	if (!rpath.empty())
		load();
}

bool Texture::supported(std::string ext) {
	return (ext.compare(".bmp") == 0 ||
			ext.compare(".tga") == 0 ||
			ext.compare(".dds") == 0 ||
			ext.compare(".png") == 0 ||
			ext.compare(".jpg") == 0 ||
			ext.compare(".jpeg") == 0);
}
void Texture::load() {
	fs::path d_path = engine::get_absolute_from_project(data_path());
	auto ext = d_path.extension();

	if (!supported(ext)) {
		loaded = false;
		return;
	}

	int channels;
	auto image = SOIL_load_image(
		d_path.c_str(),
		&width,
		&height,
		&channels,
		tex_type);

	if (!image) {
		loaded = false;
		return;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t_id);
	// glTextureStorage2D(GL_TEXTURE_2D, 1, GL_RGB, width, height);
	// glTextureSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
	auto format = channels == SOIL_LOAD_RGB ? GL_RGB : GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
	printf("Loaded image %s, channels %d %s\n", filename().c_str(), channels, channels == SOIL_LOAD_RGB ? "GL_RGB" : "GL_RGBA");
	glGenerateMipmap(GL_TEXTURE_2D);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	loaded = true;
}

void Texture::imgui_draw() {
	
	ImGui::Text("Size %d x %d", width, height);
	// ImGui::SameLine();
	auto max = ImGui::GetWindowSize();
	max.x -= 10;
	ImGui::Text("Max rect %1.1f x %1.1f", max.x, max.y);
	ImGui::Image((void*)(intptr_t)t_id, ImVec2(max.x, ((float)height / width * max.x)));

	// I'll do this later
	// ImGui::Combo()
}