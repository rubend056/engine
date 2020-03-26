#ifndef GL_helper_h
#define GL_helper_h

#include <cstring>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <string>
#include <vector>


#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "groups/gl.h"

#include "SOIL.h"
#include "components/component.h"

#define _BV32(x) ((uint32_t)1 << x)
#include "serialize.h"

// struct Vertex {
//     glm::vec3 Position;
//     glm::vec3 Normal;
//     glm::vec2 TexCoords;
// };

class Mesh : public File {
   private:
    void vao_set_vertex_attrib_pointer();
    // Can't modify this after constructor is called
    unsigned int vbo_id;
    unsigned int vao_id;

   protected:
    unsigned int n_vertices;

   public:
    // Can't modify the size after constructor is called
    glm::vec3 *positions = nullptr;
    glm::vec3 *normals = nullptr;
    glm::vec2 *tex_cords = nullptr;

    void vbo_bind();
    void vao_bind();

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
    virtual void vbo_set_data();
    virtual void gl_draw();

    /**
		 * You have to then vbo_set_data, then link it to 
		 */
    Mesh(const char *_f, int n, bool _normals = false, bool _tex_cords = false);
    ~Mesh() { delete[] positions, normals, tex_cords; }
};

class FilledCircleMesh : public Mesh {
   public:
    void gl_draw() override { glDrawArrays(GL_TRIANGLE_FAN, 0, n_vertices); }
    FilledCircleMesh(const char *_f, float radius, unsigned int subdivisions) : Mesh(_f, subdivisions + 2) {
        positions[0] = glm::vec3(0);

        auto dtheta = glm::pi<float>() * 2 / subdivisions;
        for (int i = subdivisions; i >= 0; --i) {
            auto theta = dtheta * i;
            positions[i + 1] = glm::vec3(glm::cos(theta) * radius, glm::sin(theta) * radius, 0);
        }
    }
};

class Texture : public File {
   public:
    unsigned int t_id;
    Texture(const char *_f, const char *path) : File(_f) {
        int width, height;
        auto image = SOIL_load_image(
            path,
            &width,
            &height,
            0,
            SOIL_LOAD_RGB);

        if (image == 0) return;

        glGenTextures(1, &t_id);
        glBindTexture(GL_TEXTURE_2D, t_id);
		// glTextureStorage2D(GL_TEXTURE_2D, 1, GL_RGB, width, height);
		// glTextureSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
};

// enum SHADER_ENUM {
//     NOTHING = 0,
//     VERTEX,
//     GEOMETRY,
//     FRAGMENT
// };

class Program;  //Forward declaration

class Shader : public File {
public:
    unsigned int s_id;
    GLenum type;
    int status;
    char log[200];

    std::vector<Program *> _programs;

    /**
	 * Will udpate code and recompile, relinking any programs that use the shader in the process
	 */
    void update(const char *src);
    /**
	 * @param type Must be one of:
	 * GL_COMPUTE_SHADER, GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, or GL_FRAGMENT_SHADER
	 */
    Shader(GLenum type, const char *_f, const char *src = NULL);
    ~Shader();
};


struct Attribute{
	GLboolean uniform=false;
	GLint i;
	GLint location;
	GLint size;
	GLenum type=0;
	static const GLsizei max_name=20;
	GLchar name[max_name];
	GLsizei length;
	void* val=nullptr;
	
	// void create_delete(bool create);
	// Attribute(){}
	// virtual ~Attribute(){}
};
template <typename T>
struct AttributeVar:public Attribute{
	std::unique_ptr<T> _val = std::unique_ptr<T>(new T(0));
	AttributeVar(void*_last_val=nullptr){
		if(_last_val)memcpy(_val.get(), _last_val, sizeof(T));
		val = _val.get();
	}
};

class Program : public File, public Component {
public:
    unsigned int p_id;
    int link_status = 0;
    uint32_t attribs_enabled (){
		uint32_t t=0;
		for(auto&a:attributes)if(!a->uniform)t|=_BV32(a->location);
		return t;
	};

    std::vector<std::shared_ptr<Shader>> _shaders;
	std::vector<std::unique_ptr<Attribute>> attributes;

    /**
	 * Adds a shader to the program
	 * glAttachShader
	 */
    void attach_shader(std::shared_ptr<Shader> shader);
    void detach_shader(unsigned int index);
    void clear_shaders();
    void link();

    // unsigned int pos_attrib=0;
    // void link_vertex(Mesh* mesh);
    /**
	 * glUseProgram
	 */
    void use();
	
	COMPONENT_IMGUI_DRAW override;
	COMPONENT_IMGUI_NAME override{
		return (std::string("Program ") + filename).c_str();
	}
	COMPONENT_NAME {return "Program";}
    Program(const char *_f=nullptr):File(_f){p_id = glCreateProgram();};
    ~Program(){
		clear_shaders();
		glDeleteProgram(p_id);
	};
};

#endif