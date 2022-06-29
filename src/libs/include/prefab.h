/**
 * A prefab handles dirty variables: Point is to allow instance to set a prefab and take it's values if the instance hasn't changed them
 * 		A reference to a base prefab.
 * 		A list of variables which the current instance has dirtied/changed, making it diferent from the base prefab.
 * 	
 */

#include <memory>

#include "macros.h"

//? Cereal Serialize
#define PREFAB_SERIALIZE cereal::make_nvp("Prefab", cereal::base_class<PREFAB_NAME>(this))

//? Names
#define PREFAB_VAR_NAME_ENUM(name) name##_enum
#define PREFAB_VAR_NAME_DIRTY(name) name##_dirty

#define PREFAB_VAR_NAME_ENUM_DIRTY(type, name) PREFAB_VAR_NAME_ENUM(name),

//? Functions
#define PREFAB_VAR_SERIALIZE(type, name) cereal::make_nvp(STRINGIFY(name), name);
#define PREFAB_VAR_SERIALIZE_DIRTY(type, name) PREFAB_VAR_SERIALIZE(type, PREFAB_VAR_NAME_DIRTY(name))
//? DEF
#define PREFAB_VAR_DEF(type, name) \
	type name;                     \
	bool PREFAB_VAR_NAME_DIRTY(name) = false;
//? CHECK
#define PREFAB_VAR_CHECK_DEF(type, name)                              \
	inline void check_##name() {                                      \
		PREFAB_VAR_NAME_DIRTY(name) = ref ? ref->name == name : true; \
	}
//? GET
#define PREFAB_VAR_GET_DEF(type, name) \
	const type& get_##name() { return ref ? ref->name : name; }
//? SET
#define PREFAB_VAR_SET_DEF(type, name) \
	void set_##name(const type& val) { \
		name = val;                    \
		check_##name();                \
	}

#ifndef CLASSNAME
#error "Please define a CLASSNAME before including prefab.h, it should be all caps"
#endif
#ifndef CLASSNAME_NORMAL
#error "Please define a CLASSNAME_NORMAL before including prefab.h, it should be the normal name of your class"
#endif

#define __CLASSNAME_EXPANSION(classname, func) classname##_EXPANSION(func)
#define _CLASSNAME_EXPANSION(classname, func) __CLASSNAME_EXPANSION(classname, func)
#define CLASSNAME_EXPANSION(func) _CLASSNAME_EXPANSION(CLASSNAME, func)

#define __PREFAB_NAME(classname) classname##_Prefab
#define _PREFAB_NAME(classname) __PREFAB_NAME(classname)
#define PREFAB_NAME _PREFAB_NAME(CLASSNAME_NORMAL)

#include "assets.h"
#include "cereal/types/vector.hpp"
#include "file.h"

class PREFAB_NAME {
   protected:
	CLASSNAME_EXPANSION(PREFAB_VAR_DEF)
	CLASSNAME_EXPANSION(PREFAB_VAR_CHECK_DEF)

   public:
	std::shared_ptr<PREFAB_NAME> ref;
	CLASSNAME_EXPANSION(PREFAB_VAR_GET_DEF)
	CLASSNAME_EXPANSION(PREFAB_VAR_SET_DEF)

	virtual ~PREFAB_NAME(){};

	template <class Archive>
	void serialize(Archive& ar) {
		CLASSNAME_EXPANSION(PREFAB_VAR_SERIALIZE)
		CLASSNAME_EXPANSION(PREFAB_VAR_SERIALIZE_DIRTY)
	}
};

#define _CRT(classname) CEREAL_REGISTER_TYPE(classname)
_CRT(PREFAB_NAME)

#undef PREFAB_VAR_SERIALIZE
// #endif // prefab_h