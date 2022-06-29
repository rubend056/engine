#include "scene.h"

#include "menus.h"

bool Scene::supported(std::string ext) {
	return ext.compare(".scene") == 0;
}
void Scene::serialize_inspector() {
	cereal::make_nvp("inspector_o", menus::inspector_o);
}

// bool Scene::exists(const std::string& name_or_rpath){
// 	return rpath_object_ht.find(name_or_rpath) != rpath_object_ht.end();
// }
// std::string Scene::get_gameobject_path(const std::shared_ptr<GameObject>& go){
// 	if(name_or_rpath.empty())return "";
// 	auto go = get_obj(name_or_rpath);
// 	if(go){
// 		return data_path().string() + ":" + go->filename();
// 	}else return "";
// }
// std::shared_ptr<GameObject> Scene::find_gameobject_path(const std::vector<std::shared_ptr<Scene>>& scenes, const std::string& gameobject_path){
// 	if(!gameobject_path.empty()){
// 		auto pos = gameobject_path.find(':');
// 		if(pos!=std::string::npos){
// 			auto scene_path = gameobject_path.substr(0, pos);
// 			auto go_path = gameobject_path.substr(pos+1);
// 			std::shared_ptr<Scene>s;
// 			for(auto&s:scenes)
// 				if(s->data_path() == scene_path)
// 					return s->get_obj(go_path);
// 		}
// 	}
// 	return std::shared_ptr<GameObject>();
// }

// std::shared_ptr<GameObject> Scene::instantiate(const std::shared_ptr<Scene>& scene, const std::string& _name){
// 	auto name = _name;
// 	if(name.empty())name += "GameObject";
// 	{
// 		auto oname = name;int i=0;
// 		while(scene->rpath_object_ht.find(oname) != scene->rpath_object_ht.end())oname = name + "_" + std::to_string(++i);
// 		name = oname;
// 	}
// 	auto gobj = std::make_shared<GameObject>(name);
// 	Parent::foster(scene, gobj);
// 	// scene->objects.push_back(gobj);
// 	scene->rpath_object_ht.insert(std::make_pair(gobj->data_path(), gobj));
// 	return gobj;
// }
