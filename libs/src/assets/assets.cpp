#include "_assets.h"
// #include "my_imgui.h"


#include <algorithm>
namespace assets{
	
	std::vector<std::shared_ptr<File>> get_files(const std::string t_name){
		// #warning "t_name calls for erroneous/nonexistent types"
		auto eq_range = type_asset_ht.equal_range(t_name);
		std::vector<std::shared_ptr<File>> v;
		for(auto it = eq_range.first; it!=eq_range.second;++it)
			v.push_back(it->second);
		return v;
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
	void load_project_entries(){
		entries.clear();
		entries.push_back(fs::directory_entry(engine::project_path));
		list_dir(engine::project_path);
		
		// Initialize inotify
		{
			int argc=0;
			for(auto&e:entries)if(ENTRY_IS_DIR(e))++argc;
			char* argv[argc];
			
			int c=0;
			for(auto&e:entries){
				// Populate filenames and argv
				if(!ENTRY_IS_DIR(e))inotify::filesnames_allowed.push_back(e.path().filename().string());
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
	
	void init(){
		if(!engine::project_path.c_str())return;
		
		printf("Assets path is %s\n", engine::project_path.c_str());
		
		// List all files
		load_project_entries();
		
		import_assets();
	}
	
	
	
	void update(){
		
		{// INOTIFY EVENT HANDLING
			inotify::inotify_update();
			bool reload=false;
			if(inotify::events.size()){
				for(auto&event:inotify::events){
					if(event->event == inotify::MODIFY){
						auto p = fs::path(event->folder_path);
						p/= event->filename;
						
						auto shader = get_file<Shader> (p);
						if(shader)shader->load();
					}else if(event->event == inotify::CREATE || event->event == inotify::DELETE){
						reload = true;
					}
				}
				inotify::events.clear();
				inotify::_events.clear();
			}
			if(reload)load_project_entries();
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