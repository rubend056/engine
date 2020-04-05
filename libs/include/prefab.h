#ifndef prefab_h
#define prefab_h

#include <memory>

#define PREFAB_VAR_ENUM_NAME(name) name##_enum
#define PREFAB_VAR_ENUM_DIRTY(type, name) PREFAB_VAR_ENUM_NAME(name),

#define PREFAB_VAR_DIRTY_NAME(name) name##_dirty
#define PREFAB_VAR_DEF(type, name) type name;bool PREFAB_VAR_DIRTY_NAME(name) = false;

#define PREFAB_VAR_SERIALIZE(type, name) ar(CEREAL_NVP(name));
#define PREFAB_VAR_ADD_DIRTY(type, name) dirty.push_back(PREFAB_VAR_DIRTY_NAME(name));


// #define _PREFAB_VAR_SERIALIZE_DIRTY(type, name) _PREFAB_VAR_SERIALIZE_DIRTY(type, PREFAB_VAR_DIRTY_NAME(name))
// #define PREFAB_VAR_SERIALIZE_DIRTY(type, name) _PREFAB_VAR_SERIALIZE_DIRTY(type, PREFAB_VAR_DIRTY_NAME(name))

// #define PREFAB_VAR_SET(name, val) name = val; PREFAB_VAR_DIRTY_NAME(name) = ref.name == name;
// #define PREFAB_VAR_CHECK(name) PREFAB_VAR_DIRTY_NAME(name) = ref.name == name;
// #define PREFAB_VAR_CHECK(name, type) PREFAB_VAR_DIRTY_NAME(name) = ref.name == name;

#define PREFAB_VAR_CHECK_DEF(type, name) void check_##name () {\
		if(ref)PREFAB_VAR_DIRTY_NAME(name) = ref.name == name;\
		else {PREFAB_VAR_DIRTY_NAME(name) = true;}\
	}
#define PREFAB_VAR_CHECK(type, name) check_##name();
#define PREFAB_VAR_GET_DEF(type, name) const type& get_##name () inline {return name;}
#define PREFAB_VAR_SET_DEF(type, name) void set_##name () inline {name = val; check_##name ();}



#ifndef CLASSNAME
// #define CLASSNAME GAMEOBJECT
#error "Please define a CLASSNAME before including prefab.h, it should be all caps"
#endif
#ifndef CLASSNAME_NORMAL
// #define CLASSNAME_NORMAL GameObject
#error "Please define a CLASSNAME_NORMAL before including prefab.h, it should be the normal name of your class"
#endif

#define __CLASSNAME_EXPANSION(classname, func) classname ## _EXPANSION(func)
#define _CLASSNAME_EXPANSION(classname, func) __CLASSNAME_EXPANSION(classname,func)
#define CLASSNAME_EXPANSION(func) _CLASSNAME_EXPANSION(CLASSNAME,func)

#define PREFAB_CONSTRUCT_PARAM FILE_CONSTRUCT_PARAM
#define PREFAB_CONSTRUCT_VARS FILE_CONSTRUCT_VARS

#include "file.h"
#include "assets.h"
#include "cereal/types/vector.hpp"

// This class will have the prefab functionality
template<class T>
class Prefab:public File{
private:
	std::shared_ptr<T> ref; // Link to our prefab inst
	
protected:
	// Prefab vars
	enum DIRTY_ENUM{First, CLASSNAME_EXPANSION(PREFAB_VAR_ENUM_DIRTY) Last};
public:
	CLASSNAME_EXPANSION(PREFAB_VAR_DEF)
	
	Prefab(FILE_CONSTRUCT_PARAM):File(FILE_CONSTRUCT_VARS){}
	
	// Is this instance a prefab instantiation (are u the sole owner of a file?)
	// 		Should have ref null and a name that's not a supposed ext
	// 
	// bool is_prefab()
	
	template<class Archive>
	void serialize(Archive& ar){
		// ? Base Class
		ar(FILE_SERIALIZE);
		// ? Serailize ref
		std::string d_name = ref?ref->data_path():"";
		ar(cereal::make_nvp("ref",d_name));
		if(!ref && !d_name.empty())ref = assets::get_file<T>(d_name);
		
		// ? Serailize dirty and vars
		std::vector<bool> dirty;CLASSNAME_EXPANSION(PREFAB_VAR_ADD_DIRTY)
		
		int count = dirty.size();
		ar(cereal::make_nvp("var_count", count));
		if(count != dirty.size()){
			printf(
				ANSI_COLOR_RED "Warning, count is different %d in file, %d in class, file %s" 
				ANSI_COLOR_RESET "\n",count, dirty.size(), filename().c_str()
			);
			// return; //! STOP LOADING, WE HAVE NO IDEA WHAT'S WHAT ANYMORE
		}
		
		ar(CEREAL_NVP(dirty));
		int i=0;
#define PREFAB_VAR_SERIALIZE_VARS(type, name) \
		if(i++<count && dirty[PREFAB_VAR_ENUM_NAME(name)]) PREFAB_VAR_SERIALIZE(type, name)
		CLASSNAME_EXPANSION(PREFAB_VAR_SERIALIZE_VARS)
	}
};

#define _CRT(classname) CEREAL_REGISTER_TYPE(classname)
_CRT(Prefab<CLASSNAME_NORMAL>)
// #define _CSFAA(classname, member) CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES(classname, member)
// _CSFAA(Prefab<CLASSNAME_NORMAL>,cereal::specialization::member_load_save)


#endif // prefab_h