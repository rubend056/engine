#ifndef Assets_h
#define Assets_h

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace assets{
	extern std::vector<fs::directory_entry> entries;
	
	bool init();
	void update();
	void exit();
}

#endif