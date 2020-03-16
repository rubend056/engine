#ifndef Assets_h
#define Assets_h

#include "my_filesystem.h"
#include <vector>

#include "rendering/gl_helper.h"

namespace assets{
	extern std::vector<fs::directory_entry> entries;
	
	extern std::vector<Mesh*> meshes;
	extern std::vector<Shader*> shaders;
	extern std::vector<Texture*> textures;
	extern std::vector<Program*> programs;
	
	void init();
	void update();
	void exit();
}

#endif