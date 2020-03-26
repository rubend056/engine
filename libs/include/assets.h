#ifndef Assets_h
#define Assets_h

#include "my_filesystem.h"
#include <vector>

#include "rendering/gl_helper.h"

namespace assets{
	
	
	extern std::vector<fs::directory_entry> entries;
	
	extern std::vector<std::shared_ptr<Mesh>> meshes;
	extern std::vector<std::shared_ptr<Shader>> shaders;
	extern std::vector<std::shared_ptr<Texture>> textures;
	extern std::vector<std::shared_ptr<Program>> programs;
	
	void init();
	void update();
	void exit();
}

#endif