#ifndef scene_h
#define scene_h

#include "gameobject.h"
#include "file.h"

#include "cereal/types/polymorphic.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/unordered_map.hpp"
#include "map_helper.h"

class Scene : public File, public Parent{
protected:
	// std::unordered_map<std::string, std::shared_ptr<GameObject>> rpath_object_ht;
public:
	// std::vector<std::shared_ptr<GameObject>> objects;
	
	
	Scene(FILE_CONSTRUCT_PARAM):File(FILE_CONSTRUCT_VARS){}
	
	// Instantiate
	// static std::shared_ptr<GameObject> instantiate(const std::shared_ptr<Scene>& scene, const std::string& _name="");
		
	// MAP_GET_ELEMENT(GameObject, obj, rpath_object_ht)
	// MAP_GET_ELEMENTS(GameObject, objs, rpath_object_ht)
	
	FILE_SUPPOSED_EXT override {return ".scene";}
	static bool supported(const std::string& ext);
	// bool exists(const std::string& name_or_rpath);
	
	// static bool is_gameobject_path(const std::string& gameobject_path)
	// 	{return gameobject_path.find(':') != std::string::npos;};
	
	// static std::shared_ptr<GameObject> find_gameobject_path(const std::vector<std::shared_ptr<Scene>>& scenes, const std::string& gameobject_path);
	
	template<class Archive>
	void serialize(Archive& ar){
		ar(cereal::virtual_base_class<Parent>(this));
		ar(FILE_SERIALIZE);
	}
};
CEREAL_REGISTER_TYPE(Scene)

#endif // scene_h