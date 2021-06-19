#ifndef Inotify_imp_h
#define Inotify_imp_h

#include <vector>
#include <mutex>
#include <functional>
#include <string>
#include <atomic>


namespace inotify{
	#define FILTER_EVENTS_DELAY 100
	// extern std::atomic<bool> exit_thread; 
	
	enum FILE_EVENT{OPEN, CLOSE, MODIFY, CREATE, DELETE, DELETE_SELF};
	struct FileEvent{
		FILE_EVENT event;
		const char* folder_path;
		char* filename;
		bool isdir;
	};

	extern std::vector<FileEvent*> events;
	extern std::vector<FileEvent> _events;
	extern std::vector<std::string> filenames_allowed;
	// extern 
	
	
	// std::function<void(FileEvent)> event_handler;

	void inotify_init(int argc, char const* const* argv);
	void inotify_update();
	void inotify_exit();
}


#endif