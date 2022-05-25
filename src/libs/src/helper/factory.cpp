#include "factory.h"

#include "components.h"
#include "engine_globals.h"
#include "mesh.h"
#include "scene.h"
#include "texture.h"

#define _BV32(x) ((uint32_t)1 << x)

namespace factory {

Factory factory;

Factory::Factory() {
#define REGISTER(type,i) Register<type>(FactoryType_##type,#type);

	FACTORY_TYPES(REGISTER)
}

std::vector<std::string> Factory::get_names(){
	std::vector<std::string> v;
	for(auto&mv:name_switchToKey){
		v.push_back(mv.first);
	}
	return v;
}
FACTORY_KEY_TYPE Factory::get_name_key(std::string name){
	auto itk = name_switchToKey.find(name);
	if (itk == name_switchToKey.end())
			return 0;
	return itk->second;
}


}  // namespace factory
