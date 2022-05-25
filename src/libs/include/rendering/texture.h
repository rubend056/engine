#ifndef texture_h
#define texture_h

#define _BV32(x) ((uint32_t)1 << x)
#include "cereal/types/polymorphic.hpp"
#include "file.h"
#include "gl.h"
#include "idraw.h"

class Texture : public File, public IDraw {
   public:
	unsigned int t_id;

	int type = 0;  // SOIL_LOAD_AUTO
	int width, height;

	bool loaded = false;
	static bool supported(const std::string& ext);
	void load() override;

	Texture(FILE_CONSTRUCT_PARAM);
	~Texture() { glDeleteTextures(1, &t_id); }

	IDRAW_IMGUI_NAME override { return filename(); }
	IDRAW_IMGUI_TYPE_NAME override { return "Texture"; };
	IDRAW_IMGUI_DRAW override;

	template <class Archive>
	void serialize(Archive& ar) {
		ar(
			FILE_SERIALIZE,
			cereal::make_nvp("type", type));
		load();
	}
};
CEREAL_REGISTER_TYPE(Texture)

#endif	// texture_h