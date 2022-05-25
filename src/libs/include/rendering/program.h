/**
 * @file program.h
 * @author RubenD (rubendariopm14@gmail.com)
 * @brief Defines a program
 * @version 0.1
 * 
 * Has opengl functions for creating/using OpenGL 'Shader Programs'
 * 
 */
#ifndef program_h
#define program_h

#include "attribute.h"
#include "cereal/cereal.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/types/unordered_set.hpp"
#include "cereal/types/vector.hpp"
#include "cereal_glm.h"
#include "file.h"
#include "gl.h"
#include "idraw.h"
#include "shader.h"

#define PROGRAM_EXT ".prgm"

// *********************** PROGRAM *********************

#define _BV32(x) ((uint32_t)1 << x)
#include "component.h"
#include "texture.h"

class Program : public File, public Component {
   private:
	

	// Shader id's from File.id (this is the id generated for each file) in current program
	// Not to be confused with OpenGL shader id
	// Gets updated with 'link' function
	std::vector<std::vector<unsigned int>> shaders;
	/**
	 * @brief Add shaders from 'shaders' into current OpenGL program
	 * Used when loading the program file
	 */
	void add_shaders();
	/**
	 * @brief 
	 * @param t_ids 
	 */
	void add_textures(const std::vector<std::vector<uint>>&);

	

   public:
	// The program id
	unsigned int p_id;
	// Has the program loaded
	bool loaded = false;
	// 1 (GL_TRUE) means OK, 0 means Failed
	int link_status = 0;

	std::vector<std::unique_ptr<Attribute>> attributes;
	std::vector<std::shared_ptr<Texture>> textures;

	// CONSTRUCTORS
	Program(FILE_CONSTRUCT_PARAM) : File(FILE_CONSTRUCT_VARS) {
		p_id = glCreateProgram();
	};
	~Program() { glDeleteProgram(p_id); };

	/**
	 * @brief Get enabled attributes
	 * @return uint32_t 
	 */
	uint32_t attribs_enabled() {
		uint32_t t = 0;
		for (auto& a : attributes)
			if (!a->uniform)
				t |= _BV32(a->location);
		return t;
	};

	/**
	 * Adds a shader to the program
	 * glAttachShader
	 */
	void attach_shader(GLuint s_id);
	/**
	 * Detaches a shader to the program
	 * glDetachShader
	 */
	void detach_shader(GLuint s_id);

	/**
	 * @brief Gets shader id's from the program in GPU
	 * @return std::vector<unsigned int> 
	 */
	std::vector<unsigned int> get_shaders();

	/**
	 * @brief Deteles all shaders from program in GPU
	 */
	void clear_shaders();

	/**
	 * @brief Links program
	 */
	void link();

	/**
	 * @brief Use program
	 * Equivalent to glUseProgram(p_id)
	 * @return
	 */
	void use();
	/**
	 * @brief Set the pmat attribute
	 * @param mat The ipmat input in shaders
	 */
	void set_pmat(const glm::mat4& mat);

	// DRAWING
	IDRAW_IMGUI_DRAW override;
	IDRAW_IMGUI_NAME override { return filename(); }
	IDRAW_IMGUI_TYPE_NAME override{return "Program";};

	// FILE
	FILE_SUPPOSED_EXT override { return PROGRAM_EXT; }
	static bool supported(const std::string& ext);

	// SERIALIZING
	template <class Archive>
	void serialize(Archive& ar) {
		ar(FILE_SERIALIZE);
		ar(COMPONENT_SERIALIZE);
		
		// Saving/loading shaders
		bool s_empty = shaders.empty();
		std::cout << "Saving Shaders:" << std::endl;
		for (auto& s : shaders) std::cout << "  " << s[0] << std::endl;
		ar(CEREAL_NVP(shaders));
		if (s_empty)
			add_shaders();

		ar(CEREAL_NVP(attributes));
	
		// Saving/loading textures
		std::vector<std::vector<uint>> t_ids;
		for (auto& t : textures) t_ids.push_back(t->my_ref());
		ar(cereal::make_nvp("textures", t_ids));
		if (textures.empty() && t_ids.size())
			add_textures(t_ids);

		link();
	}
};
CEREAL_REGISTER_TYPE(Program)

#endif	// program_h