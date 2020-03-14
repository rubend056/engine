#include "engine_globals.h"
#include "assets.h"
#include "imgui.h"
#include "inotify_imp.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
	
	thread* inotify_thread;
	void inotify_init(){
		int argc = 0;
		static char* argv[100];for(int i=0;i<100;i++)argv[i]=(char*)calloc(100,1);
		for(int i=0;i<entries.size()+1;++i){
			// path 0 is fullpath to our assets, others are real paths to folders inside our assets
			if(i){
				auto &e = entries[i-1];
				if(e.is_directory()){
					realpath(e.path().c_str(), argv[argc]);
					// cout << argv[argc] << endl;
					argc++;
				}
			}else{
				realpath(engine::project_path.c_str(), argv[argc]);
				// cout << argv[argc] << endl;
				argc++;
			}
		}
		
		if(!inotify_thread)inotify_thread = new thread(inotify::init, argc, argv);
		
		
		// Populating inotify's allowed file list
		for(auto&entry:entries){if(!entry.is_directory())inotify::filesnames_allowed.push_back(entry.path().filename().string());}
	}
	
	
	
	
	
	bool init(){
		if(!engine::project_path.c_str())return false;
		
		printf("Assets path is %s\n", engine::project_path.c_str());
		
		// List all files
		list_dir(string(engine::project_path));
		inotify_init();
		
		inotify_init();
		
		// ? ASSIMP *****************
		Assimp::Importer importer;
		
		const auto scene = importer.ReadFile("../game0/monkey.obj", aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
		if(!scene){printf ("Assimp couldn't read file \n");}
		else {}
		
		// ? &&&&&&&&&&&&&&&&&&&&&&&&&&
		
		return true;
	}
	
	
	
	
	
	
	void update(){
		inotify::update();
		
#ifdef DEBUG_GUI
		ImGui::Begin("Events");
		ImGui::Text("Event size is %d", inotify::events.size());
		for(auto&e:inotify::events){
			ImGui::Text ("File %s %d, in folder %s", e->filename, e->event, e->folder_path);
		}
		ImGui::End();
#endif
		
		if(inotify::events.size()){
			for(auto&event:inotify::events){
				if(event->event ==  inotify::MODIFY){
					printf("Compiling shader for %s\n", event->filename);
					
				}
			}
			inotify::events.clear();
			inotify::_events.clear();
		}
		
	}
	
	void exit(){
		inotify::exit_thread = true;
		inotify_thread->join();
		delete inotify_thread;
	}
}