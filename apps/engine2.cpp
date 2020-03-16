#include "engine.h"

#include "menus/menus.h"
#include "assets.h"
#include "components/components.h"

#include "groups/gl.h"

#include <SDL.h>

#include <algorithm>

using namespace std;

// GLuint load_vertex(const GLfloat* vertex_buffer){
	
// 	return vbo;
// }

// static GLuint vba;
// static GLuint vbo;
// void testing_init(){
//     static const GLfloat g_vertex_buffer_data[] = {
//         -1.0f, -1.0f,
//         1.0f, -1.0f,
//         0.0f,  1.0f
//     };
    
//     glGenVertexArrays(1, &vba);
//     glEnableVertexAttribArray(vba);
    
//     // GLuint vbo = load_vertex(g_vertex_buffer_data);
    
// 	glGenBuffers(1, &vbo);
//     glBindBuffer(GL_ARRAY_BUFFER, vbo);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
//     glVertexAttribPointer(
//     0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//     2,                  // size
//     GL_FLOAT,           // type
//     GL_FALSE,           // normalized?
//     0,                  // stride
//     (void*)0            // array buffer offset
//     );
// }
// void testing_draw(){
//     glEnableVertexAttribArray(vba);
//     glBindBuffer(GL_ARRAY_BUFFER, vbo);
//     glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
//     glDisableVertexAttribArray(vba);
    
// }


ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 1.00f);
namespace engine
{
    void init(){
        selected.push_back(instantiate());
		assets::init();
        // testing_init();
    }

    void update(){
        menus::stats();
        menus::inspector(selected[0]);
        menus::files(project_path);
        menus::text_editor();
        
		assets::update();
    }
    void render(SDL_Window* window, ImGuiIO& io){
        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // testing_draw();
        // RENDER
        assets::programs[0]->use();
        auto pred = [](Mesh*&m) -> bool{return string(m->filename).compare("testmesh") == 0;};
        auto mesh = *find_if(assets::meshes.begin(), assets::meshes.end(), pred);
        // mesh->bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
	
	void exit(){
		assets::exit();
	}
} // namespace engine

