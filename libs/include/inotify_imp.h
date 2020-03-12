#ifndef Inotify_imp_h
#define Inotify_imp_h

#include <vector>
#include <mutex>
#include <functional>
#include <string>



namespace inotify{
	#define FILTER_EVENTS_DELAY 100
	extern bool exit_thread; 
	
	enum FILE_EVENT{OPEN, CLOSE, MODIFY, CREATE, DELETE, DELETE_SELF};
	struct FileEvent{
		FILE_EVENT event;
		char* folder_path;
		char* filename;
		bool isdir;
	};

	extern std::vector<FileEvent*> events;
	extern std::vector<std::string> filesnames_allowed;
	// extern 
	
	
	// std::function<void(FileEvent)> event_handler;

	int init(int argc, char* argv[100]);
	void update();
}


#endif