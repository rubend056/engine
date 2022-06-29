/**
 * @file  Factory.h
 * @author RubenD (rubendariopm14@gmail.com)
 * @brief Defines  Factory
 * @version 0.1
 * 
 * We face two issues when dynamic creation and reference is concerned
 * 	1. Representing things to create on demand (without having to include the header file of the thing to create, this will 1. decrease compilation time hopefully, and 2. make the project modular), pass a key to factory and out comes a Referentiable of the specific class you want
 * 	2. Checking if instance is of certain type (IType will provide virtual function to get class type)
 * 
 * Factory
 * "Type" -> new ...
 * "Type" -> key
 *  key -> new ...
 * 	key -> "Type"
 * Our job is to convert "Type" -> key -> T
 * Checking if IType is of type is as simple as strcmp(factory.get_type(Type_Scene), obj->type())
 */
#ifndef Factory_h
#define Factory_h

#define FACTORY_KEY_TYPE uint32_t
#define FACTORY_BASE_CLASS Referentiable
#include <limits>
#include <map>
#include <string>
#include <vector>

#include "_factory.h"
#include "macros.h"

#define FACTORY_TYPES(f) f(Scene, 1) f(Mesh, 2) f(Texture, 3) f(Camera, 4) f(Transform, 5) f(Program, 6) f(Shader, 7) f(GameObject, 8)
#define FACTORY_COUNT(T, i) 1 +
#define FACTORY_SIZE (FACTORY_TYPES(FACTORY_COUNT) 0)

/**
 * This allows us to reference types even withouth having them included their headers, it's a compromise ok...
 */
enum Factory_ {
	Factory_None = 0,
#define ENUM_FUNC(T, i) Factory_##T = ((FACTORY_KEY_TYPE)1 << i),
	FACTORY_TYPES(ENUM_FUNC)
		Factory_All
};

/**
 *  A factory that implements the base _VariantFactory but also adds names to the keys
 */
class Factory : private _VariantFactory {
   private:
	/**
	 * T -> key map
	 */
	std::map<std::string, FACTORY_KEY_TYPE> tk;
	std::map<FACTORY_KEY_TYPE, std::string> kt;
	/**
	 * Gets random number not in Factory_ enum
	 */
	int get_rand() {
		auto r = rand();
		if (__builtin_popcount(r) == 1)	 // makes sure it's not in enum up there
			r += 1;
		return r;
	}

   public:
	Factory();
	/**
	 * -> ["Type",...]
	 */
	std::vector<std::string> get_types() {
		std::vector<std::string> v(tk.size());
		for (auto& l : tk) v.push_back(l.first);
		return v;
	};
	/**
	 * key -> "Type"
	 */
	std::string get_type(FACTORY_KEY_TYPE k) { return kt[k]; };

	/**
	 * "Type" -> key
	 */
	FACTORY_KEY_TYPE get_key(std::string type) { return tk[type]; };
	template <typename T>
	FACTORY_KEY_TYPE get_key() { return get_key(STRINGIFY(T)); };

	/** 
	 * key -> new ... 
	*/
	using _VariantFactory::create;
	/** 
	 * "Type" -> new ...
	*/
	std::shared_ptr<FACTORY_BASE_CLASS> create(std::string name) {
		for (auto& t : tk) {
			if (name == t.first)
				return create((FACTORY_KEY_TYPE)t.second);
		}
		return nullptr;
	}

	void Register(std::string type, FACTORY_KEY_TYPE k) {
		tk[type] = k;
		kt[k] = type;
	}
	/** 
	 * Registers everyone, called by constructor implementation
	 * Also allows extensions
	 * Ex: 
	 * 	Register<Test>(55)
	 * 	create(55) / create("Test")
	*/
	template <typename T>
	void Register(FACTORY_KEY_TYPE k) {
		Register(STRINGIFY(T), k);
		_VariantFactory::Register<T>(k);
	}
	/**
	 * Register that assigns a random number to class for you and returns it
	 */
	template <typename T>
	int Register() {
		auto n = get_rand();
		Register<T>(n);
		return n;
	}
};

extern Factory factory;

#endif	//  Factory_h