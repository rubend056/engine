#ifndef mesh_h
#define mesh_h

#include "rendering_common.h"
#include "assimp/postprocess.h"

#include "component.h"

class Mesh : public File, public Parent, public IDraw {
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
	struct VAO: public Component{
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
		COMPONENT_MAX_NUM override{return 0;}
		IDRAW_IMGUI_NAME override{return std::string("VAO ") + name;}
		IDRAW_IMGUI_DRAW override{
			ImGui::Text("ID: %d", vao_id);
			ImGui::Text("Vertices: %d", n_vertices);
			ImGui::Separator();
			ImGui::Text("Positions: %s", positions?"true":"false");
			ImGui::Text("Normals: %s", normals?"true":"false");
			ImGui::Text("Tex_cords: %s", tex_cords?"true":"false");
		}
		VAO(){glGenVertexArrays(1, &vao_id);}
		virtual ~VAO(){glDeleteVertexArrays(1, &vao_id);}
		template<class Archive>
		void serialize(Archive& ar){
			ar(COMPONENT_SERIALIZE);
			ar(name);
		}
	};
	
	
    void vbo_bind(){glBindBuffer(GL_ARRAY_BUFFER, vbo_id);}
    
	bool loaded=false;
	static bool supported(const std::string& ext);
	void load();
	
	// friend class cereal::access;
	template<class Archive>
	void serialize(Archive& ar){
		ar(FILE_SERIALIZE);
		ar(cereal::base_class<Parent>(this));
		ar(CEREAL_NVP(import_flags));
		load();
	}
	
	IDRAW_IMGUI_NAME override{return filename().c_str();}
	IDRAW_IMGUI_DRAW override;
	Mesh(FILE_CONSTRUCT_PARAM):File(FILE_CONSTRUCT_VARS){
		// create_supposed_ext();
		glGenBuffers(1, &vbo_id);
		if(!rpath.empty())load();
	}
	// Mesh(){glGenBuffers(1, &vbo_id);}
	~Mesh(){glDeleteBuffers(1, &vbo_id);}
};
CEREAL_REGISTER_TYPE(Mesh)
CEREAL_REGISTER_TYPE_WITH_NAME(Mesh::VAO, "VAO")

#endif // mesh_h