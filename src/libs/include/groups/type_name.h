#ifndef type_h
#define type_h

#include <string>
#include <typeinfo>

namespace helper{
	std::string demangle(const char* name);

	// template <class T>
	// std::string type_name(const T& t=0) {
	// 	if(t) return demangle(typeid(t).name());
	// 	else return demangle(typeid(T).name());
	// }
}


#endif // type_h