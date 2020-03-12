#include "assets.h"
#include "imgui.h"
#include "inotify_imp.h"
#include <thread>

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <filesystem>



namespace fs = std::filesystem;
using namespace std;



#include <limits>

namespace assets{
	vector<fs::directory_entry> entries;
	void list_dir(string d){
		for (const auto &entry : fs::directory_iterator(d)){
			assets::entries.push_back(entry);
			if (entry.is_directory()){list_dir(entry.path().string());}
		}
	}
	
	thread* inotify_thread; //! WE NEVER DECONSTRUCT THIS
	bool init(const char* assets_path){
		if(!assets_path)return false;
		
		printf("assets path is %s\n", assets_path);
		
		// List all files
		list_dir(string(assets_path));
		
		//Getting number of dirs
		
		// static vector<string> dirs;
		// dirs.clear();
		// dirs.push_back(assets_path);
		// for(auto&f:fl){if(f.is_directory())dirs.push_back(f.path().string());}
		
		
		
		
		// static int argc;argc=dirs.size();
		// static const char** argv;
		// argv = (const char**) calloc(argc, sizeof(char*));
		// for (int i = 0; i < argc; i++ ){
		// 	argv[i] = (char*) calloc(
		// 		// i==0 ? 1 : dirs[i-1].size()+1
		// 		100
		// 		, sizeof(char));
		// 	argv[i] = dirs[i].c_str();
		// 	cout << argv[i] << endl;
		// 	char t[100];
		// 	realpath(argv[i], t);
		// }
		
		int argc = 0;
		static char* argv[100];for(int i=0;i<100;i++)argv[i]=(char*)calloc(100,1);
		for(int i=0;i<entries.size()+1;++i){
			// path 0 is fullpath to our assets, others are real paths to folders inside our assets
			if(i){
				auto &e = entries[i-1];
				if(e.is_directory()){
					realpath(e.path().c_str(), argv[argc]);
					cout << argv[argc] << endl;
					argc++;
				}
			}else{
				realpath(assets_path, argv[argc]);
				cout << argv[argc] << endl;
				argc++;
			}
			
		}
		
		
		
		if(!inotify_thread)inotify_thread = new thread(inotify::init, argc, argv);
		
		
		// Populating inotify's allowed file list
		for(auto&entry:entries){if(!entry.is_directory())inotify::filesnames_allowed.push_back(entry.path().filename().string());}
		
		return true;
	}
	void update(){
		inotify::update();
		
		ImGui::Begin("Events");
		ImGui::Text("Event size is %d", inotify::events.size());
		for(auto&e:inotify::events){
			ImGui::Text ("File %s %d, in folder %s", e->filename, e->event, e->folder_path);
		}
		ImGui::End();
		
		
	}
	void exit(){
		inotify_thread->join();
	}
}