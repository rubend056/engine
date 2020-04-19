#include "_assets.h"
// #include "my_imgui.h"

#include "mesh.h"

#include <algorithm>
namespace assets{
	
	std::vector<std::shared_ptr<File>> get_files(const std::string& t_name){
		// #warning "t_name calls for erroneous/nonexistent types"
		auto eq_range = type_asset_ht.equal_range(t_name);
		std::vector<std::shared_ptr<File>> v;
		for(auto it = eq_range.first; it!=eq_range.second;++it)
			v.push_back(it->second);
		return v;
	}
	template<>
	std::shared_ptr<File> get_file<File>(const std::function<bool(File*)>& pred){
		for(auto&v:assets::files)
			if(pred(v.get()))return v;
		return std::shared_ptr<File>();
	}
	
	//? ENTRIES **************************
	// All entries, including project_path to the project
	vector<fs::directory_entry> entries;
	void list_dir(string d){
		for (const auto &entry : fs::directory_iterator(d)){
			assets::entries.push_back(entry);
			if (ENTRY_IS_DIR(entry)){list_dir(entry.path().string());}
		}
	}
	void reload_project(){
		entries.clear();
		entries.push_back(fs::directory_entry(engine::project_path));
		list_dir(engine::project_path);
		
		// Initialize inotify
		{
			inotify::filenames_allowed.clear();
			
			int argc=0;
			for(auto&e:entries)if(ENTRY_IS_DIR(e))++argc;
			char* argv[argc];
			
			int c=0;
			for(auto&e:entries){
				// Populate filenames and argv
				if(!ENTRY_IS_DIR(e))inotify::filenames_allowed.push_back(e.path().filename().string());
				else {
					// String and length defined
					auto path = e.path().string();
					auto p = path.c_str();auto l = strlen(p)+1;
					argv[c]=(char*)malloc(l); // Allocating
					memcpy(argv[c], p, l);
					cout << argv[c] << endl;
					++c;
				}
			}
			
			inotify::inotify_init(argc, argv);
			// Free argv's memory
			for(auto&av:argv)free(av);
		}
		
	}
	
	std::vector<fs::path> data_path(const std::shared_ptr<File>& file){
		std::vector<fs::path> v;
		auto pb = [&](const std::shared_ptr<File>& f){v.push_back(f->data_path());};
		
		if(!file)goto found;
		
		// If file is within root files, send it back
		for(auto&f:files){if(f == file) {pb(f);goto found;}}
		
		// Keep searching in root
		for(auto&f:files){
			// If f in root file is scene, then search inside scene
			if(auto s = std::dynamic_pointer_cast<Scene>(f)){
				for(auto&o:s->objects){
					// If file is an object, send it back
					if(o == file){pb(s); pb(o); goto found;}
					// Else search through components in the object
					else for(auto&c:o->components){
						// If file is a component, send it back
						if(std::dynamic_pointer_cast<File>(c) == file){pb(s); pb(o); pb(file); goto found;}
					}
				}
			}
		}
	
	found:
		return std::move(v);
	}
	std::shared_ptr<File> data_path_find(const std::vector<fs::path>& paths){
		if(!paths.size())return std::shared_ptr<File>();
		int count=0;
		
		auto print = [](const char* mes){printf(ANSI_COLOR_RED "%s" ANSI_COLOR_RESET "\n", mes); throw("Check this");};
		
		auto f = get_file<File>(paths[count++]);
		// Make sure this is a scene
		if(paths.size() > count){
			if(auto s = std::dynamic_pointer_cast<Scene>(f)){
				// Get gameobject by name/path
				if(auto o = s->get_obj(paths[count++])){
					if(paths.size() > count){
						if(auto c = o->get_comp(paths[count++])){
							if(paths.size()>count)print("paths size too big, what else are u putting there my friend");
							else f=std::dynamic_pointer_cast<File>(c);
						}else print("no component with such name");
					}else f=o;
				}else print("no gameobject with such name");
			}else print ("path > 1 but not a scene?");
		}
		return f;
	}
	
	void init(){
		if(!engine::project_path.c_str())return;
		
		printf("Assets path is %s\n", engine::project_path.c_str());
		
		// List all files
		reload_project();
		
		import_assets();
	}
	
	void update(){
		
		{// INOTIFY EVENT HANDLING
			inotify::inotify_update();
			bool reload=false;
			if(inotify::events.size()){
				for(auto&event:inotify::events){
					std::printf("Event %d on %s\n", (int)event->event, std::string(event->filename).c_str());
					if(event->event == inotify::MODIFY){
						auto p = fs::path(event->folder_path);
						p/= event->filename;
						p = engine::get_relative_to_project(p);
						
						auto shader = get_file<Shader> (p);
						if(shader)shader->load();
					}else if(event->event == inotify::CREATE || event->event == inotify::DELETE){
						reload = true;
					}
				}
				inotify::events.clear();
				inotify::_events.clear();
			}
			if(reload)reload_project();
		}
	}
	
	void exit(){
		// INOTIFY EXIT
		inotify::inotify_exit();
		
		//! Enable saving all files
		for(auto&f:assets::files){
			File::save_file(f);
		}
	}
}