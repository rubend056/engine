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
	
	/**
	 * @brief Adds a file to everything
	 * To assets, hash tables, files vector
	 * Also adds supposed extension
	 * @param file 
	 */
	void add(const std::shared_ptr<File>& file);
	/**
	 * @brief Clear all files, when reloading project
	 * 
	 */
	void clear();
	
	// MAP_GET_ELEMENT(File, file_path, rpath_asset_ht)
	
	/**
	 * @brief Get file by path, returns casted File to T
	 * 
	 * @tparam T 
	 * @param t_name 
	 * @return std::shared_ptr<T> 
	 */
	template<class T=File>
	std::shared_ptr<T> get_file_path (const std::string& t_name){
		return std::dynamic_pointer_cast<T>(get_file_path<File>(t_name));
	}
	// Gets the specified file, returns File
	template<>
	std::shared_ptr<File> get_file_path (const std::string& t_name);
	
	// MAP_GET_ELEMENT(File, file_type, type_asset_ht)
	// MAP_GET_ELEMENTS(File, files_type, type_asset_ht)
	
	
	
	std::shared_ptr<File> get_file_type (const std::string& t_name);
	/**
	 * @brief Get the file based on type name
	 * Type name is returned by helper::demangle(typeid(T).name())
	 * Will not work on g++ compilers, please don't use this too much
	 * @param t_name 
	 * @return std::shared_ptr<File> 
	 */
	template<class T>
	std::shared_ptr<T> get_file_type(){
		return std::dynamic_pointer_cast<T>(
			get_file_type(helper::demangle(typeid(T).name()))
		);
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
	
	/**
	 * @brief Gets or loads a file based on it's rel_path
	 * 
	 * @tparam T Type of object it will be casted to
	 * @param rel_path The rel_path of file
	 * @return std::shared_ptr<T> 
	 */
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
		
	/**
	 * @brief Gets a file based on a list/vector of refs 
	 * 
	 * @tparam T The type it will be casted to
	 * @param refs 
	 * @return std::shared_ptr<T> 
	 */
	template<class T=Referentiable>
	std::shared_ptr<T> get_file(const std::vector<unsigned int>& refs){
		return std::dynamic_pointer_cast<T>(get_file<Referentiable>(refs));
	}
	/**
	 * @brief Gets a file based on a single ref
	 * 
	 * @tparam T The type it will be casted to
	 * @param refs 
	 * @return std::shared_ptr<T> 
	 */
	template<class T=Referentiable>
	std::shared_ptr<T> get_file(const unsigned int ref){
		return get_file<T>(std::vector<unsigned int>{ref});
	}
	template<>
	std::shared_ptr<Referentiable> get_file(const std::vector<unsigned int>& refs);
	
	void reload_project();
	void init();
	void update();
	void exit();
}

#endif