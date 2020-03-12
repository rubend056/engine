#ifndef Inotify_imp_h
#define Inotify_imp_h

#include <vector>
#include <mutex>

namespace inotify{
	#define FILTER_EVENTS_DELAY 100
	extern bool exit_thread; 
	
	enum FILE_EVENT{OPEN, CLOSE, MODIFY, CREATE, DELETE, DELETE_SELF};
	struct FileEvent{
		FILE_EVENT event;
		const char* folder_path;
		const char* filename;
		bool isdir;
	};

	extern std::vector<FileEvent> events;
	extern std::mutex events_mutex;


	int init(int argc, char* argv[100]);
	void update();
}


#endif