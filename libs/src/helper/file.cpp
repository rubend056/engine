// #include "cereal/cereal.hpp"
// #include "cereal/types/polymorphic.hpp"
// #include "cereal/archives/json.hpp"

// class File2 {
//    public:

// 	File2(const int j=0) {}
// 	virtual ~File2(){}
// 	int file_id =0;

// 	template<class Archive>
// 	void serialize(Archive& ar){
// 		ar(CEREAL_NVP(file_id));
// 		// file_ids.insert(file_id);
// 	}
// };
// class Test
// :public File2
// {
// 	int j;
// public:
// 	virtual void testing(){};
// 	template<class Archive>
// 	void serialize(Archive& ar){
// 		ar(cereal::base_class<File2>(this));
// 		ar(j);
// 	}
// };
// CEREAL_REGISTER_TYPE(Test)

#include "file.h"
#include "my_imgui.h"

#include "engine_globals.h"


std::unordered_set<unsigned int> File::file_ids;

unsigned int File::Next_id(unsigned int _f){
	// static 
	unsigned int file_id_next=1;if(_f)file_id_next=_f; 
	// If file_id exists, move forward
	while(file_ids.find(file_id_next) != file_ids.end()){++file_id_next;}
	file_ids.insert(file_id_next);
	return file_id_next;
}

void File::imgui_draw_filename_edit() {
	std::string t = filename();
	if (ImGui::InputText("FName", &t))
		filename_set(t);
	
		
}

// Is this path a metadata (means is just a companion file to something else, texture, mesh, etc...)
bool File::is_metadata() {
	return _rel_path.extension().compare(std::string(METADATA_EXT)) == 0;
}
bool File::is_supposed_ext(){
	return _rel_path.extension().compare(std::string(supposed_ext())) == 0;
}
void File::create_supposed_ext() {
	if (_rel_path.empty())return;
	if (!is_supposed_ext()) {
		std::cout << "Adding supposed ext " << supposed_ext() << " to " << _rel_path << std::endl;
		_rel_path += supposed_ext();
	}
}
// Returns the relative data path to the project path (checking if it's metadata), should concatenate engine::project_path with it
fs::path File::data_path() {
	if(_rel_path.empty())return "";
	
	// assert(is_metadata());
	auto p = _rel_path.parent_path();
	if (is_metadata()){
		assert(_rel_path.has_stem());
		p /= _rel_path.stem();
	}else
		p /= _rel_path.filename();
	return p;
}

void File::save_file(const std::shared_ptr<File>& f) {
	if(!f || f->filename().empty())return;
	auto full_path = engine::get_absolute_from_project(f->_rel_path);
	if (full_path.empty())
		return;
	std::cout << "Saving to: " << full_path << std::endl;
	std::ofstream file(full_path);
	cereal::JSONOutputArchive ar(file);
	ar(f);
}
std::shared_ptr<File> File::load_file(const fs::path rel_path) {
	std::shared_ptr<File> f;
	if (rel_path.empty())
		return f;
	auto full_path = engine::get_absolute_from_project(rel_path);
	std::ifstream file(full_path);
	cereal::JSONInputArchive ar(file);
	ar(f);
	return f;
}
