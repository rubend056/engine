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

#define _BV32(x) ((uint32_t)1 << x)

class IFile {
   public:
    char *filename;
    IFile(const char *_f) {
        filename = new char[strlen(_f)];
        strcpy(filename, _f);
    }
    ~IFile() { delete[] filename; }
};

// struct Vertex {
//     glm::vec3 Position;
//     glm::vec3 Normal;
//     glm::vec2 TexCoords;
// };

class Mesh : public IFile {
   private:
    void vao_set_vertex_attrib_pointer();
    // Can't modify this after constructor is called
    unsigned int vbo;
    unsigned int vao;

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
                glEnableVertexArrayAttrib(vao, i);
    };
    void vao_attrib_disable() {
        for (int i = 0; i < 3; ++i)
            glDisableVertexArrayAttrib(vao, i);
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
    void gl_draw() { glDrawArrays(GL_TRIANGLE_FAN, 0, n_vertices); }
    FilledCircleMesh(const char *_f, float radius, unsigned int subdivisions) : Mesh(_f, subdivisions + 2) {
        positions[0] = glm::vec3(0);

        auto dtheta = glm::pi<float>() * 2 / subdivisions;
        for (int i = subdivisions; i >= 0; --i) {
            auto theta = dtheta * i;
            positions[i + 1] = glm::vec3(glm::cos(theta) * radius, glm::sin(theta) * radius, 0);
        }
    }
};

class Texture : public IFile {
   public:
    unsigned int t_id;
    Texture(const char *_f, const char *path) : IFile(_f) {
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
    }
};

enum SHADER_ENUM {
    NOTHING = 0,
    VERTEX,
    GEOMETRY,
    FRAGMENT
};

struct Program;  //Forward declaration

struct Shader : public IFile {
    unsigned int s_id;
    SHADER_ENUM type;
    int status;
    char *log = new char[512];

    std::vector<Program *> _programs;

    /**
	 * Will udpate code and recompile, relinking any programs that use the shader in the process
	 */
    void update(const char *src);
    /**
	 * @param type Must be one of:
	 * GL_COMPUTE_SHADER, GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, or GL_FRAGMENT_SHADER
	 */
    Shader(SHADER_ENUM type, const char *_f, const char *src = NULL);
    ~Shader();
};

struct Program : public IFile {
    unsigned int p_id;
    int link_status = 0;
    uint32_t attribs_enabled = 0;

    std::vector<Shader *> _shaders;

    /**
	 * Adds a shader to the program
	 * glAttachShader
	 */
    void attach_shader(Shader *shader);
    void detach_shader(unsigned int index);
    void clear_shaders();
    void link();

    // unsigned int pos_attrib=0;
    // void link_vertex(Mesh* mesh);
    /**
	 * glUseProgram
	 */
    void use();
    Program(const char *_f);
    ~Program();
};

#endif