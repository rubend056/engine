#ifndef Helper_h
#define Helper_h

#include <algorithm>
#include <memory>
#include <typeinfo>
#include <vector>

namespace helper {

template <typename T, typename B>
std::shared_ptr<T> find_type_in_vector(std::vector<std::shared_ptr<B>>& vector) {
	static_assert(std::is_base_of<B, T>::value, "T not derived from B");
	
	auto it = std::find_if(vector.begin(), vector.end(), [](std::shared_ptr<B>& v) -> bool { 
		return typeid(v) == typeid(T); 
	});
	
	if (it < vector.end()) return std::dynamic_pointer_cast<T>();
	return std::shared_ptr<T>();
}



}  // namespace helper

#define FIND_IF_IN_VECTOR(vector, pred) std::find_if(vector.begin(), vector.end(), pred)

#endif	// Helper_h