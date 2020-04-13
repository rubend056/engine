#ifndef Serialize_h
#define Serialize_h

#include <cstring>
#include <string>
#include <fstream>
#include <cassert>

#include <set>

#include "debug.h"
#include "my_filesystem.h"


#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"

// ? IF YOU ENCOUTER (CLASS DOESN"T HAVE ANY SERIALIZATION FUNCTION) 
// ? IT MIGHT NOT HAVE A SUPPORTED TYPE IN SERIALIZE FUNCTION
// ? Defining serialization functions for fs::path
namespace std {
#ifdef OLD_GNU
namespace experimental{
#endif
namespace filesystem {
	// cereal::traits::EnableIf<cereal::traits::is_text_archive<Archive>::value> = cereal::traits::sfinae
// ? SAVING
template <class Archive>
void save(Archive& ar, path const& path) {
	ar(cereal::make_nvp("path", path.string()));
}
// ? LOADING
template <class Archive>
inline void load(Archive& ar, path& val) {
	val.clear();
	std::string s;
	ar(cereal::make_nvp("path", s));
	val = s;
}
}  // namespace filesystem
#ifdef OLD_GNU
}
#endif
}  // namespace std

#define FILENAME_SIZE 30
#define METADATA_EXT ".meta"
#define FILE_CONSTRUCT_PARAM const fs::path& rpath=""
#define FILE_CONSTRUCT_VARS rpath
#define FILE_SUPPOSED_EXT const std::string supposed_ext()
#define FILE_SERIALIZE cereal::make_nvp("file", cereal::base_class<File>(this))

// ? FILE ********************
class File {
private:
	static std::unordered_set<unsigned int> file_ids;
	/**
	 * Gets next id and inserts to the set
	 * If given an id, will give next unused id starting from the one given
	 * 
	 * (Will start from (last id)/(1) always unless given an id)(static/non static)
	 */

	static unsigned int Next_id(unsigned int _f=1);
public:
	unsigned int file_id=0;
	fs::path _rel_path;
protected:
	
public:
	const std::string hash_path(){return data_path().string();}
   	
	const std::string filename(){return data_path().filename().string();}
	void filename_set(const std::string& _filename){_rel_path.replace_filename(_filename);}
	// const std::string stem();
	
	// ? Constructors
	File(FILE_CONSTRUCT_PARAM) {
		_rel_path = rpath;
		if(!_rel_path.empty()){
			// create_supposed_ext();
			// assert(_rel_path.is_relative());
			file_id=Next_id();
		}
	}
	virtual ~File(){}
	
	// Draw imgui filename edit text
	void imgui_draw_filename_edit();
	// Is this path a metadata (means is just a companion file to something else, texture, mesh, etc...)
	bool is_metadata();
	bool is_supposed_ext();
	void create_supposed_ext();
	// Returns the relative data path to the project path (checking if it's metadata)
	fs::path data_path();
	
	virtual FILE_SUPPOSED_EXT{return METADATA_EXT;}
	
	static void save_file(const std::shared_ptr<File>& f);
	static std::shared_ptr<File> load_file(const fs::path rel_path);
	
	template<class Archive>
	void serialize(Archive& ar){
		file_ids.erase(file_id);
		auto orel_path = _rel_path;
		
		ar(CEREAL_NVP(file_id), cereal::make_nvp("rel_path",_rel_path));
		
		if(!orel_path.empty() && orel_path != _rel_path) 
			printf(ANSI_COLOR_YELLOW "Warning _rel_path changed: old(%s) new(%s)" ANSI_COLOR_RESET "\n", 
			orel_path.c_str(), _rel_path.c_str());
		
		if(file_id)file_id = Next_id(file_id);
	}
};

#endif	// serialize_h