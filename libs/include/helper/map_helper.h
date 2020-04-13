#ifndef map_helper_h
#define map_helper_h

// namespace helper{
	// ? get_comp
	#define MAP_GET_ELEMENT(type, s_name, hash_name) \
	std::shared_ptr<type> get_##s_name (const std::string& t_name){ \
		auto it = hash_name.find(t_name); \
		if(it == hash_name.end())return std::shared_ptr<type>(); \
		else return it->second; \
	} \
	template<class T> \
	std::shared_ptr<T> get_##s_name(){ \
		return std::dynamic_pointer_cast<T>(get_##s_name(helper::demangle(typeid(T).name()))); \
	}
	
	// ? get_comps 
	#define MAP_GET_ELEMENTS(type, s_name, hash_name) \
	std::vector<std::shared_ptr<type>> get_##s_name (const std::string& t_name){ \
		auto eq_range = hash_name.equal_range(t_name); \
		std::vector<std::shared_ptr<type>> v; \
		for(auto it = eq_range.first; it!=eq_range.second;++it) \
			v.push_back(it->second); \
		return v; \
	} \
	template<class T> \
	std::vector<std::shared_ptr<T>> get_##s_name (){ \
		std::string t_name = helper::demangle(typeid(T).name()); \
		auto o = get_##s_name(t_name); \
		std::vector<std::shared_ptr<T>> v;v.reserve(o.size()); \
		for(auto&a:o) \
			v.push_back(std::dynamic_pointer_cast<T>(a)); \
		return std::move(v); \
	}
// }

#endif