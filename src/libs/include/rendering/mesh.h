/**
 * @file mesh.h
 * @author RubenD (rubendariopm14@gmail.com)
 * @brief Defines Mesh object
 * @version 0.1
 * 
 * A mesh implementation in our engine.
 * Contains:
 * - Initializes a single VBO (Vertex Buffer Object)
 * - Contains VAO's (Vertex Array Objects) (Basically different locations in VBO where object data (vertex, normals, texture coordinates) are stored)
 * 
 * As defined by Assimp a mesh can contain multiple objects which is why there are multiple VAO's inside a Mesh object
 * 
 * - A Vertex Buffer Object (VBO) is a memory buffer in the high speed memory of your video card designed to hold information about vertices.
 * - A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store the information for a complete rendered object.
 * - VAO's basically tell the GPU, hey look, out of this VBO that's active, this is exactly how I want u to pass the data to the Rendering Pipeline (shaders and what not)
 */

#ifndef mesh_h
#define mesh_h

#include "assimp/postprocess.h"
#include "file.h"
#include "gl.h"
#include "idraw.h"

class Mesh : public File, public Parent, public IDraw {
   private:
	// The id of the Vertex Buffer Object (VBO) in GPU
	unsigned int vbo_id;

   public:
	// assimp import flags
	unsigned int import_flags = aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_SortByPType;
	// Have we loaded the mesh?
	bool loaded = false;

	// CONSTRUCTORS
	// Mesh(FILE_CONSTRUCT_PARAM);
	// virtual ~Mesh();
	Mesh(FILE_CONSTRUCT_PARAM) : File(FILE_CONSTRUCT_VARS) {
		// create_supposed_ext();
		glGenBuffers(1, &vbo_id);
		if (!rpath.empty())
			load();
	}
	virtual ~Mesh() { glDeleteBuffers(1, &vbo_id); }

	// FUNCTIONS
	void vbo_bind() { glBindBuffer(GL_ARRAY_BUFFER, vbo_id); }
	static bool supported(std::string ext);
	void load() override;

	// DRAW
	IDRAW_IMGUI_NAME override { return filename(); }
	ITYPE override { return "Mesh"; };
	IDRAW_IMGUI_DRAW override;

	// SERIALIZATION
	template <class Archive>
	void serialize(Archive& ar) {
		ar(
			FILE_SERIALIZE,
			PARENT_SERIALIZE,
			CEREAL_NVP(import_flags));
		load();
	}
};
CEREAL_REGISTER_TYPE(Mesh)

#endif	// mesh_h