#include <memory>

#define PREFAB_SERIALIZE cereal::make_nvp("template", cereal::base_class<PREFAB_NAME>(this))

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

//? CHECK
#define PREFAB_VAR_CHECK_DEF(type, name) \
	void check_##name () { \
		if(ref)PREFAB_VAR_DIRTY_NAME(name) = ref->name == name;\
		else {PREFAB_VAR_DIRTY_NAME(name) = true;}\
	}
#define PREFAB_VAR_CHECK(type, name) check_##name();

//? GET AND SET
#define PREFAB_VAR_GET_DEF(type, name) \
	inline const type& get_##name () {return name;}
#define PREFAB_VAR_SET_DEF(type, name) \
	inline void set_##name (const type& val) {name = val; check_##name ();}

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

#define __PREFAB_NAME(classname) classname##_Prefab
#define _PREFAB_NAME(classname) __PREFAB_NAME(classname)
// KEEP IN MIND, you need to override set_ref
#define PREFAB_NAME _PREFAB_NAME(CLASSNAME_NORMAL)

// #define PREFAB_CONSTRUCT_PARAM FILE_CONSTRUCT_PARAM
// #define PREFAB_CONSTRUCT_VARS FILE_CONSTRUCT_VARS

#include "file.h"
#include "assets.h"
#include "cereal/types/vector.hpp"

// This class will have the prefab functionality
// template<class T>

class PREFAB_NAME{
private:
	
protected:
	// Prefab vars
	std::shared_ptr<PREFAB_NAME> ref; // Link to our prefab inst 
	enum class DIRTY_ENUM{First=-1, CLASSNAME_EXPANSION(PREFAB_VAR_ENUM_DIRTY) Last};
public:
	CLASSNAME_EXPANSION(PREFAB_VAR_DEF)
	CLASSNAME_EXPANSION(PREFAB_VAR_CHECK_DEF)
	CLASSNAME_EXPANSION(PREFAB_VAR_GET_DEF)
	CLASSNAME_EXPANSION(PREFAB_VAR_SET_DEF)
	
	// Prefab(FILE_CONSTRUCT_PARAM):File(FILE_CONSTRUCT_VARS){}
	// Prefab(FILE_CONSTRUCT_PARAM){}
	virtual ~PREFAB_NAME(){};
	
	template<class Archive>
	void serialize(Archive& ar){
		// ? Serailize ref
		// std::string d_name = ref?ref->data_path():"";
		// ar(cereal::make_nvp("ref",d_name));
		// if(!ref && !d_name.empty())ref = assets::get_file<T>(d_name);
		
		// bool good_ref = ref;
		// Print error if we have a ref_name serialized 
		// but can't find the actual reference
		// if(!ref && !d_name.empty()) 
		// 	printf("%s ref not found\n", d_name.c_str()); 
		
		// ? Serailize dirty
		std::vector<bool> dirty;
		if(ref){CLASSNAME_EXPANSION(PREFAB_VAR_ADD_DIRTY)}
		else {for(int i=0;i<(int)DIRTY_ENUM::Last;++i)dirty.push_back(true);}
		
		int count = dirty.size();
		ar(cereal::make_nvp("var_count", count));
		if(count != dirty.size()){
			printf(
				ANSI_COLOR_RED "Warning, count is different: %d in file, %d in class" 
				ANSI_COLOR_RESET "\n",count, (int)dirty.size()
			);
			// return; //! STOP LOADING, WE HAVE NO IDEA WHAT'S WHAT ANYMORE
		}
		ar(CEREAL_NVP(dirty));
		
		// ? Serialize vars
		int i=0;
#define PREFAB_VAR_SERIALIZE_VARS(type, name) \
		if(i++<count && dirty[(int)DIRTY_ENUM::PREFAB_VAR_ENUM_NAME(name)]) PREFAB_VAR_SERIALIZE(type, name)
		CLASSNAME_EXPANSION(PREFAB_VAR_SERIALIZE_VARS)
	}
};

#define _CRT(classname) CEREAL_REGISTER_TYPE(classname)
_CRT(PREFAB_NAME)


// #endif // prefab_h