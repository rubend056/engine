#ifndef Assets_h
#define Assets_h

// Ours
// #include "rendering/rendering.h"
#include "serialize.h"
#include "my_filesystem.h"

// Ext
#include "groups/type_name.h"
#include <vector>
#include <string>
#include <unordered_map>


namespace assets{
	
	
	extern std::vector<fs::directory_entry> entries;
	
	extern std::unordered_map<std::string, std::shared_ptr<File>> rpath_asset_ht;
	extern std::unordered_multimap<std::string, std::shared_ptr<File>> type_asset_ht;
	// extern std::unordered_map<unsigned int, std::shared_ptr<File>> asset_asset_ht;
	
	void asset_hash_table_add(const std::shared_ptr<File>& file);
	
	template<class T>
	std::shared_ptr<T> get_file(const fs::path& rel_path){
		auto it = rpath_asset_ht.find(rel_path);
		if(it == rpath_asset_ht.end())return nullptr;
		else return std::dynamic_pointer_cast<T>(it->second);
	}
	
	template<class T>
	std::vector<std::shared_ptr<T>> get_files(){
		std::string t_name = helper::demangle(typeid(T).name());
		auto eq_range = type_asset_ht.equal_range(t_name);
		std::vector<std::shared_ptr<T>> v;
		for(auto it = eq_range.first; it!=eq_range.second;++it)
			v.push_back(std::dynamic_pointer_cast<T>(it->second));
		return v;
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