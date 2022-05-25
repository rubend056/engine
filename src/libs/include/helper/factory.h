/**
 * @file  Factory.h
 * @author RubenD (rubendariopm14@gmail.com)
 * @brief Defines  Factory
 * @version 0.1
 * 
 */
#ifndef Factory_h
#define Factory_h

#define FACTORY_KEY_TYPE uint32_t
#define FACTORY_BASE_CLASS Referentiable
#include <string>
#include <vector>
#include <limits>
#include "_factory.h"

#define FACTORY_TYPES(func)              \
	func(Scene,1)                          \
		func(Mesh,2)                       \
			func(Texture,3)                \
				func(Camera,4)             \
					func(Transform,5)      \
						func(Program,6)    \
							func(Shader,7) \
								func(GameObject,8)

enum FactoryType_ {
	FactoryType_None = 0,
#define ENUM_FUNC(type,i) FactoryType_##type = ((FACTORY_KEY_TYPE)1 << i),
	FACTORY_TYPES(ENUM_FUNC)
	FactoryType_All = std::numeric_limits<FACTORY_KEY_TYPE>::max()
};

namespace factory {
// A factory that implements the base _VariantFactory but also adds names to the keys
class Factory : private _VariantFactory {
   public:
	Factory();

	std::vector<std::string> get_names();
	FACTORY_KEY_TYPE get_name_key(std::string name);

	using _VariantFactory::create;
	std::shared_ptr<FACTORY_BASE_CLASS> create(std::string name) {
		auto itk = name_switchToKey.find(name);
		if (itk == name_switchToKey.end())
			return nullptr;
		auto it = int_switchToVariant.find(itk->second);
		if (it == int_switchToVariant.end())
			return nullptr;
		return std::move(it->second->Create());
	}

   private:
	
	void Register(std::string name, FACTORY_KEY_TYPE key) {
		name_switchToKey[name] = key;
	}
	template <typename T>
	void Register(FACTORY_KEY_TYPE type, std::string name) {
		Register(name, type);
		_VariantFactory::Register(type, std::make_unique<_Variantinator<T>>());
	}
	std::map<std::string, FACTORY_KEY_TYPE> name_switchToKey;
};

extern Factory factory;
}  // namespace factory

#endif	//  Factory_h