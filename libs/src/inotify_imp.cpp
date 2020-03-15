#include "inotify_imp.h"
#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>


/* Read all available inotify events from the file descriptor 'fd'.
	wd is the table of watch descriptors for the directories in argv.
	argc is the length of wd and argv.
	argv is the list of watched directories.
	Entry 0 of wd and argv is unused. */

using namespace std;

namespace inotify{
	bool exit_thread = false;
	vector<FileEvent*> events;
	vector<FileEvent> _events;
	std::mutex _events_mutex;

	static void handle_events(int fd, int *wd, int argc, char** argv)
	{
		/* Some systems cannot read integer variables if they are not
			properly aligned. On other systems, incorrect alignment may
			decrease performance. Hence, the buffer used for reading from
			the inotify file descriptor should have the same alignment as
			struct inotify_event. */

		char buf[4096]
			__attribute__ ((aligned(__alignof__(struct inotify_event))));
		const struct inotify_event *event;
		int i;
		ssize_t len;
		char *ptr;

		/* Loop while events can be read from inotify file descriptor. */

		for (;;) {

			/* Read some events. */

			len = read(fd, buf, sizeof buf);
			if (len == -1 && errno != EAGAIN) {
				perror("read");
				exit(EXIT_FAILURE);
			}

			/* If the nonblocking read() found no events to read, then
				it returns -1 with errno set to EAGAIN. In that case,
				we exit the loop. */

			if (len <= 0)
				break;

			/* Loop over all events in the buffer */

			for (ptr = buf; ptr < buf + len;
					ptr += sizeof(struct inotify_event) + event->len) {

				event = (const struct inotify_event *) ptr;

				/* Print event type */
				FileEvent myevent;
				
				// if (event->mask & IN_OPEN){
				// 	// printf("Opened\n");
				// 	myevent.event = FILE_EVENT::OPEN;
				// }
				// if (event->mask & (IN_CLOSE_NOWRITE | IN_CLOSE_WRITE)){
				// 	// printf("Closed\n");
				// 	myevent.event = FILE_EVENT::CLOSE;
				// }
				if (event->mask & IN_MODIFY){
					printf("Modified %s\n", event->name);
					myevent.event = FILE_EVENT::MODIFY;
				}
					
				if (event->mask & IN_CREATE)
					myevent.event = FILE_EVENT::CREATE;
				if (event->mask & IN_DELETE)
					myevent.event = FILE_EVENT::DELETE;
				if (event->mask & IN_DELETE_SELF)
					myevent.event = FILE_EVENT::DELETE_SELF;
				
				

				/* Print the name of the watched directory */
				for (i = 0; i < argc; ++i) {
					if (wd[i] == event->wd) {
						myevent.folder_path = argv[i];
						break;
					}
				}

				/* Print the name of the file */
				if (event->len>1){
					myevent.filename = (char*)calloc(event->len,sizeof(char));
					strcpy(myevent.filename,event->name);
				}

				/* Print type of filesystem object */
				myevent.isdir = (event->mask & IN_ISDIR);
				
				{	
					lock_guard<mutex> lock(_events_mutex);
					_events.push_back(myevent);
				}
			}
		}
	}

	int init(int argc, char** argv)
	{
		
		
		// cout << "Hello there from another thread" << endl;
		// cout << "Initializing inotify" << endl;
		// if (argc < 2) {
		// 	printf("Usage: %s PATH [PATH ...]\n", argv[0]);
		// 	return -1;
		// }

		/* Create the file descriptor for accessing the inotify API */
		
		int fd = inotify_init1(IN_NONBLOCK);
		if (fd == -1) {printf("Inotify init failed\n");exit(EXIT_FAILURE);}

		/* Allocate memory for watch descriptors */

		auto wd = (int*)calloc(argc, sizeof(int));
		
		for (int i = 0; i < argc; i++) {
			cout << "Trying to watch " << argv[i] << endl;
			
			wd[i] = inotify_add_watch(fd, argv[i],
												// IN_OPEN | IN_CLOSE | 
															IN_MODIFY | IN_CREATE | IN_DELETE | IN_DELETE_SELF);
			if (wd[i] == -1) {
				fprintf(stderr, "Cannot watch %s %s", argv[i], strerror(errno));
				exit(EXIT_FAILURE);
				// return -1;
			}
		}
		
		nfds_t nfds = 1;
		struct pollfd fds;
		fds.fd = fd;
		fds.events = POLLIN;

		/* Wait for events and/or terminal input */
		printf("Listening for events.\n");
		while (1) {
			if (exit_thread){exit_thread = false; break;}
			
			int poll_num = poll(&fds, nfds, 1);
			if (poll_num == -1) {
				if (errno == EINTR)
					continue;
				perror("poll");
				exit(EXIT_FAILURE);
			}

			if (poll_num > 0) {
				if (fds.revents & POLLIN) {
					/* Inotify events are available */
					handle_events(fd, wd, argc, argv);
				}
			}
		}

		printf("Listening for events stopped.\n");
		
		close(fd);
		free(wd);
		exit(EXIT_SUCCESS);
	}

	#include <chrono>
	// #include <algorithm>
	vector<string> filesnames_allowed;
	function<void(FileEvent)> event_handler;
	void update(){
		
		static auto start = chrono::steady_clock::now();
		if (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count() > 500){
			
			
			
			lock_guard<mutex> lock(_events_mutex);
			// cout << "Filtering " << endl;
			
			if(_events.size()>0)
			for(auto it=_events.end()-1;
			it>=_events.begin() && 
			it<_events.end();--it){
				
				if( !(*it).filename)continue;
				bool _events_found = false, filelist_found = false;
				for(auto&e:events){
					if(e == &(*it) || (e->event == (*it).event && strcmp(e->filename,(*it).filename) == 0))
					{_events_found=true;break;}}
				if(_events_found) continue;
				
				for(auto&f:filesnames_allowed){int d = f.compare((*it).filename);if(!d){filelist_found=true;break;}}
				
					
				if(!_events_found){
					if(filelist_found)events.push_back(&(*it));
					else _events.erase(it);
				}
				
				// cout << "Did somethin" << endl;
			}
			
			
			
			start = chrono::steady_clock::now();
		}
	}

}