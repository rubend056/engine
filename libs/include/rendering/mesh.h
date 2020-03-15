#ifndef mesh_h
#define mesh_h

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
	public:
    Vertex* vertices;
    Mesh(int n){
        vertices = new Vertex[n];
    }
    ~Mesh(){
        delete[] vertices;
    }
};



#endif // mesh_h