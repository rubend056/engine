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

#include "cereal/archives/json.hpp"
#include "engine_globals.h"
#include "my_imgui.h"

void File::imgui_draw_filename_edit() {
	std::string t = filename();
	if (ImGui::InputText("FName", &t))
		set_filename(t);
}

// Is this path a metadata (means is just a companion file to something else, texture, mesh, etc...)
bool File::is_metadata() {
	return _rel_path.extension().compare(METADATA_EXT) == 0;
}
bool File::is_supposed_ext() {
	return _rel_path.extension().compare(supposed_ext()) == 0;
}
void File::create_supposed_ext() {
	if (_rel_path.empty())
		return;
	if (!is_supposed_ext()) {
		std::cout << "Adding supposed ext " << supposed_ext() << " to " << _rel_path << std::endl;
		_rel_path += supposed_ext();
	}
}

fs::path File::data_path() {
	if (_rel_path.empty())
		return "";

	// assert(is_metadata());
	auto p = _rel_path.parent_path();
	if (is_metadata()) {
		assert(_rel_path.has_stem());
		p /= _rel_path.stem();
	} else
		p /= _rel_path.filename();
	return p;
}

void File::save_file(const std::shared_ptr<File>& f, fs::path r_path) {
	if (!f || (f->filename().empty() && r_path.empty()))
		return;
	auto full_path = engine::get_absolute_from_project(r_path.empty() ? f->_rel_path : r_path);
	if (full_path.empty())
		return;
	std::cout << "Saving to: " << full_path << std::endl;

	// We output to a string just in case program crashes in middle of serialize
	std::ostringstream o;
	{  // Scope makes sure archive flushes on destructor before we do anything with the stringstream
		cereal::JSONOutputArchive ar(o);
		ar(f);
	}

	// Then we put it in a file
	std::ofstream file(full_path);
	if (!file.is_open()) {
		printf(ANSI(RED, "Can't open file '%s'") ENDL, full_path.c_str());
		return;
	}
	file << o.str();
}
std::shared_ptr<File> File::load_file(const fs::path rel_path) {
	std::shared_ptr<File> f;
	if (rel_path.empty())
		return f;
	auto full_path = engine::get_absolute_from_project(rel_path);
	std::ifstream file(full_path);
	if (file.is_open()) {
		cereal::JSONInputArchive ar(file);
		ar(f);
	}
	return f;
}
