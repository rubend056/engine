#ifndef texture_h
#define texture_h

#include "_rendering.h"
#include "SOIL.h"

class Texture : public File {
   public:
    unsigned int t_id;
	
	int type=SOIL_LOAD_RGB;
	int width, height;
	
	bool loaded = false;
	static bool supported(const std::string& ext);
	void load();
	
    Texture(const fs::path& path="") : File(path) {
		create_metadata();
		glGenTextures(1, &t_id);
		if(!path.empty())load();
	}
	~Texture(){glDeleteTextures(1, &t_id);}
	
	template<class Archive>
	void serialize(Archive& ar){
		ar(cereal::base_class<File>(this));
		ar(CEREAL_NVP(type));
		load();
	}
};
// CEREAL_REGISTER_TYPE(Texture)

#endif // texture_h