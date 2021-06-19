#ifndef Assets_h
#define Assets_h

// Ours
// #include "rendering.h"
#include "file.h"
#include "my_filesystem.h"
#include "map_helper.h"
#include "type_name.h"

// Ext
#include <string>
#include <vector>
#include <unordered_map>


namespace assets{
	extern std::vector<fs::directory_entry> entries;
	
	extern std::vector<std::shared_ptr<File>> files; // All the assets currently loaded
	extern std::unordered_map<std::string, std::shared_ptr<File>> rpath_asset_ht;
	extern std::unordered_map<unsigned int, std::string> id_rpath_ht;
	extern std::unordered_multimap<std::string, std::shared_ptr<File>> type_asset_ht;
	// extern std::unordered_map<unsigned int, std::shared_ptr<File>> asset_asset_ht;
	
	// Add file to all the hash tables, you can get the typeid_name with 
	void add(const std::shared_ptr<File>& file);
	void clear();
	
	// MAP_GET_ELEMENT(File, file_path, rpath_asset_ht)
	// Get file by path, returns casted File to T
	template<class T=File>
	std::shared_ptr<T> get_file_path (const std::string& t_name){
		return std::dynamic_pointer_cast<T>(get_file_path<File>(t_name));
	}
	// Gets the specified file, returns File
	template<>
	std::shared_ptr<File> get_file_path (const std::string& t_name);
	
	// MAP_GET_ELEMENT(File, file_type, type_asset_ht)
	// MAP_GET_ELEMENTS(File, files_type, type_asset_ht)
	
	// GET_ELEMENT
	std::shared_ptr<File> get_file_type (const std::string& t_name);
	template<class T>\
	std::shared_ptr<T> get_file_type(){\
		return std::dynamic_pointer_cast<T>(\
			get_file_type(helper::demangle(typeid(T).name()))\
		);\
	}
	
	// GET_ELEMENTS
	/* t_name is string to search in map */
	std::vector<std::shared_ptr<File>> get_files_type (const std::string& t_name);
	/* T is return type, D is type to search for */
	template<class T, class D=T>\
	std::vector<std::shared_ptr<T>> get_files_type (){\
		std::string t_name = helper::demangle(typeid(D).name());\
		auto o = get_files_type(t_name);\
		std::vector<std::shared_ptr<T>> v;v.reserve(o.size());\
		for(auto&a:o)\
			v.push_back(std::dynamic_pointer_cast<T>(a));\
		return v;\
	}
	
	
	template<class T=File>
	std::shared_ptr<T> get_load_file(const fs::path& rel_path){
		auto f = get_file_path(rel_path);
		if(!f){
			f = File::load_file(rel_path);
			if(f) assets::add(f);
		}
		return std::dynamic_pointer_cast<T>(f);
	}
	
	// Filter files for type D, then filter through pred, return casted to T
	template<class T=File, class D>
	std::shared_ptr<T> get_file(const std::function<bool(D*)>& pred){
		auto files = get_files_type<D>();
		for(auto&v:files)
			if(pred(v.get()))return std::dynamic_pointer_cast<T>(v);
		return std::shared_ptr<T>();
	}
	// Filter files through pred, return casted to T
	template<class T>
	std::shared_ptr<T> get_file(const std::function<bool(File*)>& pred){
		for(auto&v:assets::files)
			if(pred(v.get()))return std::dynamic_pointer_cast<T>(v);
		return std::shared_ptr<T>();
	}
		
	// Find file by id
	
	template<class T=Referentiable>
	std::shared_ptr<T> get_file(const std::vector<unsigned int>& refs){
		return std::dynamic_pointer_cast<T>(get_file<Referentiable>(refs));
	}
	template<>
	std::shared_ptr<Referentiable> get_file(const std::vector<unsigned int>& refs);
	
	void reload_project();
	void init();
	void update();
	void exit();
}

#endif