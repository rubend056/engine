#ifndef texture_h
#define texture_h

#include "rendering_common.h"


class Texture : public File {
   public:
    unsigned int t_id;
	
	int type=3; // SOIL_LOAD_RGB
	int width, height;
	
	bool loaded = false;
	static bool supported(const std::string& ext);
	void load();
	
    Texture(FILE_CONSTRUCT_PARAM) : File(FILE_CONSTRUCT_VARS) {
		// create_supposed_ext();
		glGenTextures(1, &t_id);
		if(!rpath.empty())load();
	}
	~Texture(){glDeleteTextures(1, &t_id);}
	
	template<class Archive>
	void serialize(Archive& ar){
		ar(FILE_SERIALIZE);
		ar(CEREAL_NVP(type));
		load();
	}
};
CEREAL_REGISTER_TYPE(Texture)

#endif // texture_h