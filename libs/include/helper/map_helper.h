#ifndef map_helper_h
#define map_helper_h


// ? (Hash name HAS to be <string, shared_ptr<T>>)
#define MAP_GET_ELEMENT(type, func_name, hash_name)\
std::shared_ptr<type> get_##func_name (const std::string& t_name){\
	auto it = hash_name.find(t_name);\
	if(it == hash_name.end())return std::shared_ptr<type>();\
	else return it->second;\
}\
template<class T>\
std::shared_ptr<T> get_##func_name(){\
	assert(strcmp(#func_name,"file_path") != 0);\
	return std::dynamic_pointer_cast<T>(\
		get_##func_name(helper::demangle(typeid(T).name()))\
	);\
}

// ? (Hash name HAS to be <string, shared_ptr<T>>)
#define MAP_GET_ELEMENTS(type, func_name, hash_name)\
/* t_name is string to search in map */\
std::vector<std::shared_ptr<type>> get_##func_name (const std::string& t_name){\
	auto eq_range = hash_name.equal_range(t_name);\
	std::vector<std::shared_ptr<type>> v;\
	for(auto it = eq_range.first; it!=eq_range.second;++it)\
		v.push_back(it->second);\
	return v;\
}\
/* T is return type, D is type to search for */\
template<class T, class D=T>\
std::vector<std::shared_ptr<T>> get_##func_name (){\
	std::string t_name = helper::demangle(typeid(D).name());\
	auto o = get_##func_name(t_name);\
	std::vector<std::shared_ptr<T>> v;v.reserve(o.size());\
	for(auto&a:o)\
		v.push_back(std::dynamic_pointer_cast<T>(a));\
	return v;\
}

namespace helper{
	template<class T, class J>
	std::vector<std::shared_ptr<T>> dynamic_pointer_cast(
		const std::vector<std::shared_ptr<J>>& other){
		std::vector<std::shared_ptr<T>> v;v.reserve(other.size());
		for(auto&a:other)
			if(auto d = std::dynamic_pointer_cast<T>(a))v.push_back(d);
		return v;
	}
}

#endif