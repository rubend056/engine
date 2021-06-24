#ifndef Serialize_h
#define Serialize_h

#include <cassert>
#include <cstring>
#include <fstream>
#include <set>
#include <string>

#include "cereal/types/polymorphic.hpp"
#include "debug.h"
#include "my_filesystem.h"

// ? IF YOU ENCOUTER (CLASS DOESN"T HAVE ANY SERIALIZATION FUNCTION)
// ? IT MIGHT NOT HAVE A SUPPORTED TYPE IN SERIALIZE FUNCTION
// ? Defining serialization functions for fs::path
namespace std {
#ifdef OLD_GNU
namespace experimental {
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
}  // namespace experimental
#endif
}  // namespace std

#include "referentiable.h"

#define FILENAME_SIZE 30
#define METADATA_EXT ".meta"
#define FILE_CONSTRUCT_PARAM const fs::path& rpath = ""
#define FILE_CONSTRUCT_VARS rpath
// #define FILE_CONSTRUCT_PARAM0 const fs::path& rpath=""
// #define FILE_CONSTRUCT_VARS0 rpath
#define FILE_SUPPOSED_EXT const std::string supposed_ext()
#define FILE_SERIALIZE cereal::make_nvp("file", cereal::base_class<File>(this))

// ? FILE ********************
/**
 * @brief Representation of data in a file.
 * Any class that implements this should expect to be saved/loaded into/from a file.
 */
class File : public virtual Referentiable {
   private:
	// static std::unordered_set<unsigned int> file_ids;
	/**
	 * Gets next id and inserts to the set
	 * If given an id, will give next unused id starting from the one given
	 * 
	 * (Will start from (last id)/(1) always unless given an id)(static/non static)
	 */

	// static unsigned int Next_id(unsigned int _f=1);
   public:
	fs::path _rel_path;

   protected:
   public:
	// Returns path used for hash tables
	const std::string hash_path() { return data_path().string(); }
	// Returns filename
	const std::string filename() { return data_path().filename().string(); }
	/**
	 * @brief Replaces the filename to the one specified
	 * @param _filename The new filename
	 */
	void set_filename(const std::string& _filename) { _rel_path.replace_filename(_filename); }
	/**
	 * @brief Replaces the path with the one specified
	 * @param _filename The new filename
	 */
	void set_rel_path(const std::string& _path) { _rel_path = fs::path(_path); }
	// const std::string stem();

	// ? Constructors
	File(FILE_CONSTRUCT_PARAM) {
		_rel_path = rpath;
		if (!_rel_path.empty()) {
		}
	}
	virtual ~File() {}
	virtual void load(){};

	// Draw imgui filename edit text
	void imgui_draw_filename_edit();

	// Is this rel_path a metadata (means is just a companion file to something else, texture, mesh, etc...)
	bool is_metadata();
	/** 
	 * Checks if rel_path contains the file's supposed extension.
	 * The file's supposed extension can be overriten by other classes.
	 *   Example: A Scene class might have a .scene supposed extension
	*/
	bool is_supposed_ext();
	/** 
	 * Creates the supposed extension if rel_path doesn't currently have it.
	 * The file's supposed extension can be overriten by other classes.
	 *   Example: If a Scene class's rel_path doesn't have .scene as an extension it will get added with this function
	*/
	void create_supposed_ext();
	/** 
	 * Returns the relative data path to the project path (checking if it's metadata)
	 * Effectively stripping .meta from rel_path
	*/
	fs::path data_path();

	// A virtual function def of supposed_ext() that returns the metadata extension (.meta)
	virtual FILE_SUPPOSED_EXT { return METADATA_EXT; }

	/**
	 * @brief Static function to save desired file
	 * @param f The file to be saved
	 */
	static void save_file(const std::shared_ptr<File>& f);
	static std::shared_ptr<File> load_file(const fs::path rel_path);

	// Serializes the file
	template <class Archive>
	void serialize(Archive& ar) {
		ar(cereal::virtual_base_class<Referentiable>(this));

		auto orel_path = _rel_path;
		ar(cereal::make_nvp("rel_path", _rel_path));

		if (!orel_path.empty() && orel_path != _rel_path)
			printf(ANSI_COLOR_YELLOW "Warning _rel_path changed: old(%s) new(%s)" ANSI_COLOR_RESET "\n",
				   orel_path.c_str(), _rel_path.c_str());
	}
};

#endif	// serialize_h
