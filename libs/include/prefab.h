#ifndef prefab_h
#define prefab_h

#include <memory>

#define PREFAB_VAR_DIRTY_NAME(name) name##_dirty
#define PREFAB_VAR(type, name) type name;bool PREFAB_VAR_DIRTY_NAME(name) = false;

#define PREFAB_VAR_SERIALIZE(type, name) ar(CEREAL_NVP(name));
#define PREFAB_VAR_SERIALIZE_VARS(type, name) if(PREFAB_VAR_DIRTY_NAME(name)) PREFAB_VAR_SERIALIZE(type, name)
#define PREFAB_VAR_SERIALIZE_DIRTY(type, name) _PREFAB_VAR_SERIALIZE_DIRTY(type, PREFAB_VAR_DIRTY_NAME(name))

#define PREFAB_VAR_SET(name, val) name = val; PREFAB_VAR_DIRTY_NAME(name) = ref.name == name;
#define PREFAB_VAR_CHECK(name) PREFAB_VAR_DIRTY_NAME(name) = ref.name == name;
#define PREFAB_VAR_CHECK(name, type) PREFAB_VAR_DIRTY_NAME(name) = ref.name == name;

#ifndef CLASSNAME
#error "Please define a CLASSNAME before including prefab.h"
#endif

#define CLASSNAME_EXPANSION(func) CLASSNAME##_EXPANSION (func)

// This class will have the prefab functionality
template<class T>
class Prefab{
	std::shared_ptr<T> ref; // Link to our prefab inst
	
	void set(){
		
	}
};

#endif // prefab_h