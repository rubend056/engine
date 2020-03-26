#ifndef serialize_h
#define serialize_h

#include <cstring>
#include <string>

#include "my_filesystem.h"

#define FILENAME_SIZE 30

class File {
   public:
	fs::path path;
	char filename[FILENAME_SIZE];
	File(const char *_f = nullptr) {
		if (_f)
			strcpy(filename, _f);
		else
			filename[0] = '\0';
	}
	File(fs::path& _path) {
		path = _path;
		strcpy(filename, path.stem().c_str());
	}
};

#endif	// serialize_h