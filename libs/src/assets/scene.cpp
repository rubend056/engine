#include "scene.h"

bool Scene::supported(const std::string& ext){
	return ext.compare(".scene") == 0;
}

std::shared_ptr<GameObject> Scene::instantiate(const std::string& _name){
	auto name = _name;
	if(name.empty())name += "GameObject";
	{
		auto oname = name;int i=0;
		while(rpath_object_ht.find(oname) != rpath_object_ht.end())oname = name + "_" + std::to_string(++i);
		name = oname;
	}
	auto gobj = std::make_shared<GameObject>(name);
	objects.push_back(gobj);
	rpath_object_ht.insert(std::make_pair(gobj->data_path(), gobj));
	return gobj;
}