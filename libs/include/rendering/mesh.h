#ifndef mesh_h
#define mesh_h

#include "rendering_common.h"
#include "assimp/postprocess.h"

class Mesh : public File, public IDraw {
   private:
    // Can't modify this after constructor is called
    unsigned int vbo_id;
	
   public:
   	
    unsigned int import_flags=aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_SortByPType;
    // Can't modify the size after constructor is called
    // glm::vec3 *positions = nullptr;
    // glm::vec3 *normals = nullptr;
    // glm::vec2 *tex_cords = nullptr;
	
	// Every VAO represents a different mesh
	struct VAO{
		std::string name;
		unsigned int vao_id;
		unsigned int n_vertices;
		unsigned int draw_function=GL_TRIANGLES;
		bool positions=false, normals=false, tex_cords=false;
		void vao_bind(){glBindVertexArray(vao_id);}
		
		void vao_attrib_enable(uint32_t attribs) {
			uint32_t enabled = (positions ? _BV32(0) : 0) |
							(normals ? _BV32(1) : 0) |
							(tex_cords ? _BV32(2) : 0);
			for (int i = 0; i < 3; ++i)
				if (attribs & enabled & _BV32(i))
					glEnableVertexArrayAttrib(vao_id, i);
		};
		void vao_attrib_disable() {
			for (int i = 0; i < 3; ++i)
				glDisableVertexArrayAttrib(vao_id, i);
		};
		void gl_draw(){
			vao_bind();
			glDrawArrays(draw_function, 0, n_vertices);
		}
		VAO(){glGenVertexArrays(1, &vao_id);}
		~VAO(){glDeleteVertexArrays(1, &vao_id);}
	};
	std::vector<std::unique_ptr<VAO>> vaos;
	
    void vbo_bind(){glBindBuffer(GL_ARRAY_BUFFER, vbo_id);}
    
	bool loaded=false;
	static bool supported(const std::string& ext);
	void load();
	
	// friend class cereal::access;
	template<class Archive>
	void serialize(Archive& ar){
		ar(FILE_SERIALIZE);
		ar(CEREAL_NVP(import_flags));
		load();
	}
	
	IDRAW_IMGUI_NAME override{return filename().c_str();}
	Mesh(FILE_CONSTRUCT_PARAM):File(FILE_CONSTRUCT_VARS){
		// create_supposed_ext();
		glGenBuffers(1, &vbo_id);
		if(!rpath.empty())load();
	}
	// Mesh(){glGenBuffers(1, &vbo_id);}
	~Mesh(){glDeleteBuffers(1, &vbo_id);}
};
CEREAL_REGISTER_TYPE(Mesh)

#endif // mesh_h