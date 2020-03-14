#ifndef Assets_h
#define Assets_h

#include "my_filesystem.h"
#include <vector>

namespace assets{
	extern std::vector<fs::directory_entry> entries;
	
	bool init();
	void update();
	void exit();
}

#endif