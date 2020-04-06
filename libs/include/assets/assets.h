#ifndef Assets_h
#define Assets_h

// Ours
// #include "rendering.h"
#include "file.h"
#include "my_filesystem.h"

// Ext
#include "type_name.h"
#include <vector>
#include <string>
#include <unordered_map>


namespace assets{
	
	
	extern std::vector<fs::directory_entry> entries;
	
	extern std::vector<std::shared_ptr<File>> files;
	extern std::unordered_map<std::string, std::shared_ptr<File>> rpath_asset_ht;
	extern std::unordered_multimap<std::string, std::shared_ptr<File>> type_asset_ht;
	// extern std::unordered_map<unsigned int, std::shared_ptr<File>> asset_asset_ht;
	
	void add(const std::shared_ptr<File>& file);
	void clear();
	
	template<class T>
	std::shared_ptr<T> get_file(const fs::path& rel_path){
		auto it = rpath_asset_ht.find(rel_path);
		if(it == rpath_asset_ht.end())return nullptr;
		else return std::dynamic_pointer_cast<T>(it->second);
	}
	
	std::vector<std::shared_ptr<File>> get_files(const std::string t_name);
	
	template<class T>
	std::vector<std::shared_ptr<T>> get_files(){
		std::string t_name = helper::demangle(typeid(T).name());
		auto o = get_files(t_name);
		std::vector<std::shared_ptr<T>> v;v.reserve(o.size());
		for(auto&a:o)
			v.push_back(std::dynamic_pointer_cast<T>(a));
		return std::move(v);
	}
	
	template<class T>
	std::shared_ptr<T> get_file(const std::function<bool(T*)>& pred){
		auto files = get_files<T>();
		for(auto&v:files)
			if(pred(v.get()))return v;
		return std::make_shared<T>();
	}
	
	void init();
	void update();
	void exit();
}

#endif