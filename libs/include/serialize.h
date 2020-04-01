#ifndef Serialize_h
#define Serialize_h


// #include "engine_globals.h"


#include <cstring>
#include <string>
#include <fstream>
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "my_filesystem.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#define FILENAME_SIZE 30
#define METADATA_EXT ".meta"

#include <set>

class File {
	private:
	
	
	static std::unordered_set<unsigned int> file_ids;
	// static std::vector<unsigned int> file_ids;	
	// static void Set_id(unsigned int id){
	// 	// Make sure only one id is in the vector
	// 	file_ids.push_back(id);
	// 	std::sort(file_ids.begin(), file_ids.end());
	// 	auto it = std::unique(file_ids.begin(), file_ids.end());
	// 	file_ids.resize(std::distance(file_ids.begin(), it));
	// }
	static unsigned int Next_id(){
		static unsigned int file_id_next=0;
		file_ids.insert(++file_id_next);
		return file_id_next;
	}
   public:
   	unsigned int file_id=0;
	fs::path rel_path="";
	const std::string filename(){return rel_path.filename().string();}
	void filename_set(const std::string& _filename){rel_path.replace_filename(_filename);}
	// File(const char *_f) {
	// 	if (_f)
	// 		rel_path = _f;
	// }
	File(const fs::path _rel_path="") {
		if(!_rel_path.empty()){
			assert(_rel_path.is_relative());
			file_id=Next_id();
		}
		rel_path = _rel_path;
	}
	virtual ~File(){}
	
	void next_id(){file_id = Next_id();}
	
	void imgui_draw_filename_edit(){
		std::string t = filename();
		if(ImGui::InputText("FName", &t))filename_set(t);
	}
	
	
	// Is this path a metadata (means is just a companion file to something else, texture, mesh, etc...)
	bool is_metadata(){
		return rel_path.extension().compare(std::string(METADATA_EXT)) == 0;
	}
	void create_metadata(){
		if(rel_path.empty())return;
		if(!is_metadata()){rel_path+=METADATA_EXT;}
	}
	// Returns the relative data path to the project path (checking if it's metadata), should concatenate engine::project_path with it
	fs::path data_path(){
		assert(!rel_path.empty());
		assert(rel_path.has_stem());
		// assert(is_metadata());
		auto p = rel_path.parent_path();
		if(is_metadata()) p/=rel_path.stem();
		else p/=rel_path.filename();
		return p;
	}
	
	// static void save_file(std::shared_ptr<File>& inst){
	// 	std::ofstream f(engine::get_absolute_from_project(inst->rel_path));
	// 	cereal::JSONOutputArchive ar(f);
	// 	ar(inst);
	// }
	
	template<class Archive>
	void serialize(Archive& ar){
		ar(CEREAL_NVP(file_id), CEREAL_NVP(rel_path));
		file_ids.insert(file_id);
	}
};

#endif	// serialize_h