#ifndef scene_h
#define scene_h

#include "gameobject.h"
#include "file.h"

#include "cereal/types/vector.hpp"
#include "cereal/types/unordered_map.hpp"

class Scene : public File{
public:
	std::vector<std::shared_ptr<GameObject>> objects;
	std::unordered_map<std::string, std::shared_ptr<GameObject>> rpath_object_ht;
	
	Scene(FILE_CONSTRUCT_PARAM):File(FILE_CONSTRUCT_VARS){}
	
	std::shared_ptr<GameObject> instantiate(const std::string& _name="");
	
	FILE_SUPPOSED_EXT override {return ".scene";}
	
	template<class Archive>
	void serialize(Archive& ar){
		ar(FILE_SERIALIZE);
		ar(CEREAL_NVP(objects),CEREAL_NVP(rpath_object_ht));
	}
};
CEREAL_REGISTER_TYPE(Scene)

#endif // scene_h