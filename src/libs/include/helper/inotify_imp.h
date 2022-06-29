#ifndef Inotify_imp_h
#define Inotify_imp_h

#include <atomic>
#include <functional>
#include <mutex>
#include <string>
#include <vector>

namespace inotify {
#define FILTER_EVENTS_DELAY 100
// extern std::atomic<bool> exit_thread;

enum FILE_EVENT { OPEN,
				  CLOSE,
				  MODIFY,
				  CREATE,
				  DELETE,
				  DELETE_SELF };
struct FileEvent {
	FILE_EVENT event;
	std::string folder_path;
	std::string filename;
	bool isdir;
};

extern std::vector<FileEvent> events;
extern std::vector<std::string> entries_allowed;
// extern

// std::function<void(FileEvent)> event_handler;

void inotify_init(int argc, char const* const* argv);
void inotify_update();
void inotify_exit();
}  // namespace inotify

#endif