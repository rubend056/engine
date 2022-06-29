#ifndef Helper_h
#define Helper_h

#include <algorithm>
#include <memory>
#include <string>
#include <typeinfo>
#include <vector>

#include "component.h"
#include "type_name.h"

#define FOR_IT(it_name, v_name)    \
	auto it_name = v_name.begin(); \
	it_name != v_name.end();       \
	++it_name

namespace helper {

template <typename T, typename B>
std::shared_ptr<T> find_type_in_vector(std::vector<std::shared_ptr<B>>& vector) {
	static_assert(std::is_base_of<B, T>::value, "T not derived from B");

	auto it = std::find_if(vector.begin(), vector.end(), [](std::shared_ptr<B>& v) -> bool {
		return typeid(v) == typeid(T);
	});

	if (it < vector.end())
		return std::dynamic_pointer_cast<T>();
	return std::shared_ptr<T>();
}

// The string_format template
template <typename... Args>
std::string string_format(const std::string& format, Args... args) {
	size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1;  // Extra space for '\0'
	if (size <= 0) {
		throw std::runtime_error("Error during formatting.");
	}
	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args...);
	return std::string(buf.get(), buf.get() + size - 1);  // We don't want the '\0' inside
}

// Buttons to return new instance of T, a component or file
template <class T>
std::shared_ptr<T> component_add_button() {
	static_assert(std::is_base_of<Component, T>::value, "T not derived from Component");
	if (ImGui::Button(helper::demangle(typeid(T).name()).c_str()))
		return std::shared_ptr<T>(new T);
	else
		return std::shared_ptr<T>();
}
template <class T>
std::shared_ptr<T> file_add_button() {
	static_assert(std::is_base_of<File, T>::value, "T not derived from File");
	if (ImGui::Button(helper::demangle(typeid(T).name()).c_str()))
		return std::shared_ptr<T>(new T);
	else
		return std::shared_ptr<T>();
}

template <class T, class J>
std::vector<std::shared_ptr<T>> dynamic_pointer_cast_array(
	const std::vector<std::shared_ptr<J>>& other) {
	std::vector<std::shared_ptr<T>> v;
	v.reserve(other.size());
	for (auto& a : other)
		if (auto d = std::dynamic_pointer_cast<T>(a))
			v.push_back(d);
	return v;
}

}  // namespace helper

#define FIND_IF_IN_VECTOR(vector, pred) std::find_if(vector.begin(), vector.end(), pred)

#endif	// Helper_h