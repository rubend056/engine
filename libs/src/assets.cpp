
#include "assets.h"
#include "_assets.h"
#include "imgui.h"


#include <algorithm>
namespace assets{
	
	//? ENTRIES **************************
	vector<fs::directory_entry> entries;
	void list_dir(string d){
		for (const auto &entry : fs::directory_iterator(d)){
			assets::entries.push_back(entry);
			if (ENTRY_IS_DIR(entry)){list_dir(entry.path().string());}
		}
	}
	
	
	void init(){
		if(!engine::project_path.c_str())return;
		
		printf("Assets path is %s\n", engine::project_path.c_str());
		
		// List all files
		list_dir(engine::project_path);
		
		inotify_init();
		
		import_assets();
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
					auto p = fs::path(event->folder_path);
					p/= event->filename;
					
					auto shader = get_file<Shader> (p);
    				if(shader)shader->load();
				}
			}
			inotify::events.clear();
			inotify::_events.clear();
		}
		
	}
	
	
	void exit(){
		// INOTIFY EXIT
		inotify::exit_thread = true;
		inotify_thread->join();
		delete inotify_thread;
		
	}
}