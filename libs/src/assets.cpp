
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
		list_dir(string(engine::project_path));
		
		inotify_init();
		
		import_assets();
		
		auto p = new Program("test");
		programs.push_back(p);
		p->attach_shader(
			*find_if(shaders.begin(), shaders.end(), [](Shader* &s) -> bool{return s->type == VERTEX;})
		);
		p->attach_shader(
			*find_if(shaders.begin(), shaders.end(), [](Shader* &s) -> bool{return s->type == FRAGMENT;})
		);
		p->use();
		
		auto u = glGetUniformLocation(p->p_id, "color");
		glUniform3f(u, 0, 0, 1);
		
		// auto pred = [](Mesh*&m) -> bool{return string(m->filename).compare("testmesh") == 0;};
        // auto mesh = *find_if(assets::meshes.begin(), assets::meshes.end(), pred);
		// p->link();
		
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
					auto g = update_shader(string(event->filename));
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