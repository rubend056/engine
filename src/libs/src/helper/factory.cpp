#include "factory.h"

#include "components.h"
#include "engine_globals.h"
#include "mesh.h"
#include "scene.h"
#include "texture.h"

#define _BV32(x) ((uint32_t)1 << x)

Factory factory;

Factory::Factory() {
	size_t size = 0;
	for (size_t i = 0; i < sizeof(FACTORY_KEY_TYPE); ++i) {
		if (((Factory_All - 1) >> i) & 1)
			size = i;
		break;
	}
#define REGISTER(T, i) Register<T>(Factory_##T);
	FACTORY_TYPES(REGISTER)
}
