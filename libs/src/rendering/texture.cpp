#include "rendering/texture.h"

#include "engine_globals.h"

// ? TEXTURE ******************************************************************
bool Texture::supported(const std::string& ext){
	return (ext.compare(".bmp") == 0 ||
			ext.compare(".tga") == 0 ||
			ext.compare(".dds") == 0 ||
			ext.compare(".png") == 0 ||
			ext.compare(".jpg") == 0 ||
			ext.compare(".jpeg") == 0);
}
void Texture::load(){
	
	fs::path d_path = engine::get_absolute_from_project(data_path());
	auto ext = d_path.extension();
	
	if (!supported(ext)) {loaded = false;return;}
	
	auto image = SOIL_load_image(
		d_path.c_str(),
		&width,
		&height,
		0,
		type);
	
	if (!image) {loaded = false;return;}
	
	glBindTexture(GL_TEXTURE_2D, t_id);
	// glTextureStorage2D(GL_TEXTURE_2D, 1, GL_RGB, width, height);
	// glTextureSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexImage2D(GL_TEXTURE_2D, 0, type==SOIL_LOAD_RGB?GL_RGB:GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	loaded = true;
}