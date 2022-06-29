/**
 * @file shader.h
 * @author RubenD (rubendariopm14@gmail.com)
 * @brief Defines a shader
 * @version 0.1
 *
 */
#ifndef shader_h
#define shader_h

#include "cereal/cereal.hpp"
#include "cereal/types/polymorphic.hpp"
#include "file.h"
#include "gl.h"
#include "idraw.h"

#define FRAGMENT_EXT ".frag"
#define VERTEX_EXT ".vert"
#define GEOMETRY_EXT ".geo"

// *********************** SHADER *********************
class Shader : public File, public IDraw {
   public:
	unsigned int s_id = 0;
	GLenum shader_type = 0;
	int status = 0;
	char log[200];

	// bool loaded = false;
	static bool supported(std::string ext);
	/**
	 * Will load code and recompile
	 */
	void load() override;

	// Careful if shader isn't loaded correctly, shader will not be created
	Shader(FILE_CONSTRUCT_PARAM) : File(FILE_CONSTRUCT_VARS) {
		if (!rpath.empty()) {
			load();
			if (!status)
				std::printf("Shader %s loading failed\n", data_path().c_str());
		}
	}
	~Shader() { glDeleteShader(s_id); }

	IDRAW_IMGUI_NAME override { return filename(); }
	ITYPE override { return "Shader"; };
	IDRAW_IMGUI_DRAW override;

	// FILE_SUPPOSED_EXT override {return ".meta";}

	template <class Archive>
	void serialize(Archive& ar) {
		ar(FILE_SERIALIZE);
		load();
	}
};
CEREAL_REGISTER_TYPE(Shader)

#endif	// shader_h